/*
* Copyright (C) 2010 - 2015 Eluna Lua Engine <http://emudevs.com/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#include "Hooks.h"
#include "LuaEngine.h"
#include "BindingMap.h"
#include "TableMgr.h"
#include "ElunaEventMgr.h"
#include "ElunaIncludes.h"
#include "ElunaTemplate.h"
#include "ElunaUtility.h"
#include "ElunaGameObjectAI.h"
#include "ElunaCreatureAI.h"

// Filesystem
#ifdef USING_BOOST
#include <boost/filesystem.hpp>
#else
#include <ace/ACE.h>
#include <ace/Dirent.h>
#include <ace/OS_NS_sys_stat.h>
#endif

extern "C"
{
// Base lua libraries
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

// Additional lua libraries
};

Eluna::ScriptList Eluna::lua_scripts;
Eluna::ScriptList Eluna::lua_extensions;
std::string Eluna::lua_folderpath;
std::string Eluna::lua_requirepath;
Eluna::InstanceHolder Eluna::instances;
Eluna* Eluna::GEluna(nullptr);
MsgQueue Eluna::msgque;
std::thread::id const Eluna::main_thread_id(std::this_thread::get_id());
std::atomic<bool> Eluna::reload(false);
std::atomic<bool> Eluna::initialized(false);

extern void RegisterFunctions(Eluna* E);

Eluna* Eluna::GetGEluna(const char* info)
{
    if (main_thread_id != std::this_thread::get_id())
    {
        if (info)
        {
            ELUNA_LOG_ERROR("[Eluna]: Race condition accessing GEluna. Report to devs with this message and details about what you were doing - Info: %s", info);
        }
        else
        {
            ELUNA_LOG_ERROR("[Eluna]: Race condition accessing GEluna. Report to devs with this message and details about what you were doing");
        }
        ASSERT_MAIN_THREAD();
    }
    return GEluna;
}

void Eluna::Initialize()
{
    ASSERT_MAIN_THREAD();
    ASSERT(!IsInitialized());

#ifdef TRINITY
    // For instance data the data column needs to be able to hold more than 255 characters (tinytext)
    // so we change it to TEXT automatically on startup
    CharacterDatabase.DirectExecute("ALTER TABLE `instance` CHANGE COLUMN `data` `data` TEXT NOT NULL");
#endif

    LoadScriptPaths();

    // Must be before creating GEluna
    // This is checked on Eluna creation
    initialized = true;

    // Create global eluna
    GEluna = new Eluna(nullptr);
    GEluna->RunScripts();
}

void Eluna::Uninitialize()
{
    ASSERT_MAIN_THREAD();
    ASSERT(IsInitialized());

    delete GEluna;
    GEluna = nullptr;

    initialized = false;

    lua_scripts.clear();
    lua_extensions.clear();
}

void Eluna::LoadScriptPaths()
{
    uint32 oldMSTime = ElunaUtil::GetCurrTime();

    lua_scripts.clear();
    lua_extensions.clear();

    lua_folderpath = eConfigMgr->GetStringDefault("Eluna.ScriptPath", "lua_scripts");
#if PLATFORM == PLATFORM_UNIX || PLATFORM == PLATFORM_APPLE
    if (lua_folderpath[0] == '~')
        if (const char* home = getenv("HOME"))
            lua_folderpath.replace(0, 1, home);
#endif
    ELUNA_LOG_INFO("[Eluna]: Searching scripts from `%s`", lua_folderpath.c_str());
    lua_requirepath.clear();
    GetScripts(lua_folderpath);
    // Erase last ;
    if (!lua_requirepath.empty())
        lua_requirepath.erase(lua_requirepath.end() - 1);

    ELUNA_LOG_DEBUG("[Eluna]: Loaded %u scripts in %u ms", uint32(lua_scripts.size() + lua_extensions.size()), ElunaUtil::GetTimeDiff(oldMSTime));
}

#ifdef TRINITY
class ElunaAIUpdateWorker
{
public:
    void Visit(std::unordered_map<ObjectGuid, Creature*>& creatureMap)
    {
        for (auto const& p : creatureMap)
            if (p.second->IsInWorld())
                p.second->AIM_Initialize();
    }

    template<class T>
    void Visit(std::unordered_map<ObjectGuid, T*>&) { }
};
#endif

void Eluna::__ReloadEluna()
{
    // Close lua
    CloseLua();

    // remove timed events
    delete eventMgr;
    eventMgr = nullptr;

    // remove saved tables
    delete tableMgr;
    tableMgr = nullptr;

    // Open new lua and libaraies
    OpenLua();

    // new timed event mgr
    eventMgr = new EventMgr(this);

    // new table mgr
    tableMgr = new TableMgr(this);

    // Run scripts from loaded paths
    RunScripts();
}

void Eluna::_ReloadEluna()
{
    Eluna::ASSERT_MAIN_THREAD();
    ASSERT(IsInitialized());

    eWorld->SendServerMessage(SERVER_MSG_STRING, "Reloading Eluna...");

    // Reload script paths
    LoadScriptPaths();

    for (auto& e : instances.GetInstances())
        e->__ReloadEluna();

#ifdef TRINITY
    // Re initialize creature AI restoring C++ AI or applying lua AI
    sMapMgr->DoForAllMaps([](Map* map)
    {
        ElunaAIUpdateWorker worker;
        TypeContainerVisitor<ElunaAIUpdateWorker, MapStoredObjectTypesContainer> visitor(worker);
        visitor.Visit(map->GetObjectsStore());
    });
#endif

    reload = false;
}

Eluna::Eluna(Map* map) :
event_level(0),
push_counter(0),
enabled(false),

current_thread_id(std::this_thread::get_id()),
eventMgr(nullptr),
owner(map),
env(map ? ENV_MAP : ENV_WORLD),
L(nullptr),

ServerEventBindings(nullptr),
EntryEventBindings(nullptr),
GuidEventBindings(nullptr)
{
    ASSERT(IsInitialized());

    OpenLua();

    // Replace this with map insert if making multithread version

    // Set event manager. Must be after setting sEluna
    // on multithread have a map of state pointers and here insert this pointer to the map and then save a pointer of that pointer to the EventMgr
    eventMgr = new EventMgr(this);

    instances.Add(this);
}

Eluna::~Eluna()
{
    ASSERT(IsInitialized());

    instances.Remove(this);

    CloseLua();

    delete eventMgr;
    eventMgr = nullptr;
}

void Eluna::CloseLua()
{
    OnLuaStateClose();

    DestroyBindStores();

    // Must close lua state after deleting stores and mgr
    if (L)
        lua_close(L);
    L = nullptr;
}

void Eluna::OpenLua()
{
    enabled = eConfigMgr->GetBoolDefault("Eluna.Enabled", true);
    if (!IsEnabled())
    {
        ELUNA_LOG_INFO("[Eluna]: Eluna is disabled in config");
        return;
    }

    L = luaL_newstate();

    lua_pushlightuserdata(L, this);
    lua_setglobal(L, ELUNA_STATE_PTR);

    CreateBindStores();

    // open base lua libraries
    // -- to make states cheaper to create, comment this out and use require("table") etc to get libs you use
    luaL_openlibs(L);

    // open additional lua libraries

    // Register methods and functions
    RegisterFunctions(this);

    // Create hidden table with weak values
    lua_newtable(L);
    lua_newtable(L);
    lua_pushstring(L, "v");
    lua_setfield(L, -2, "__mode");
    lua_setmetatable(L, -2);
    lua_setglobal(L, ELUNA_OBJECT_STORE);

    // Set lua require folder paths (scripts folder structure)
    lua_getglobal(L, "package");
    lua_pushstring(L, lua_requirepath.c_str());
    lua_setfield(L, -2, "path");
    lua_pushstring(L, ""); // erase cpath
    lua_setfield(L, -2, "cpath");
    lua_pop(L, 1);
}

void Eluna::CreateBindStores()
{
    DestroyBindStores();

    ServerEventBindings = new BindingMap< EventKey >(L);
    EntryEventBindings = new BindingMap< EntryKey >(L);
    GuidEventBindings = new BindingMap< GuidKey >(L);
}

void Eluna::DestroyBindStores()
{
    delete ServerEventBindings;
    delete EntryEventBindings;
    delete GuidEventBindings;

    ServerEventBindings = nullptr;
    EntryEventBindings = nullptr;
    GuidEventBindings = nullptr;
}

void Eluna::AddScriptPath(std::string filename, const std::string& fullpath)
{
    ELUNA_LOG_DEBUG("[Eluna]: AddScriptPath Checking file `%s`", fullpath.c_str());

    // split file name
    std::size_t extDot = filename.find_last_of('.');
    if (extDot == std::string::npos)
        return;
    std::string ext = filename.substr(extDot);
    filename = filename.substr(0, extDot);

    // check extension and add path to scripts to load
    if (ext != ".lua" && ext != ".dll" && ext != ".ext")
        return;
    bool extension = ext == ".ext";

    LuaScript script;
    script.fileext = ext;
    script.filename = filename;
    script.filepath = fullpath;
    script.modulepath = fullpath.substr(0, fullpath.length() - filename.length() - ext.length());
    if (extension)
        lua_extensions.push_back(script);
    else
        lua_scripts.push_back(script);
    ELUNA_LOG_DEBUG("[Eluna]: AddScriptPath add path `%s`", fullpath.c_str());
}

// Finds lua script files from given path (including subdirectories) and pushes them to scripts
void Eluna::GetScripts(std::string path)
{
    ELUNA_LOG_DEBUG("[Eluna]: GetScripts from path `%s`", path.c_str());

#ifdef USING_BOOST
    boost::filesystem::path someDir(path);
    boost::filesystem::directory_iterator end_iter;

    if (boost::filesystem::exists(someDir) && boost::filesystem::is_directory(someDir))
    {
        lua_requirepath +=
            path + "/?;" +
            path + "/?.lua;" +
            path + "/?.ext;" +
            path + "/?.dll;" +
            path + "/?.so;";

        for (boost::filesystem::directory_iterator dir_iter(someDir); dir_iter != end_iter; ++dir_iter)
        {
            std::string fullpath = dir_iter->path().generic_string();

            // Check if file is hidden
#ifdef WIN32
            DWORD dwAttrib = GetFileAttributes(fullpath.c_str());
            if (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_HIDDEN))
                continue;
#endif
#ifdef UNIX
            std::string name = dir_iter->path().filename().generic_string().c_str();
            if (name != ".." || name != "." || name[0] == '.')
                continue;
#endif

            // load subfolder
            if (boost::filesystem::is_directory(dir_iter->status()))
            {
                GetScripts(fullpath);
                continue;
            }

            if (boost::filesystem::is_regular_file(dir_iter->status()))
            {
                // was file, try add
                std::string filename = dir_iter->path().filename().generic_string();
                AddScriptPath(filename, fullpath);
            }
        }
    }
#else
    ACE_Dirent dir;
    if (dir.open(path.c_str()) == -1) // Error opening directory, return
        return;

    lua_requirepath +=
        path + "/?;" +
        path + "/?.lua;" +
        path + "/?.ext;" +
        path + "/?.dll;" +
        path + "/?.so;";

    ACE_DIRENT *directory = 0;
    while ((directory = dir.read()))
    {
        // Skip the ".." and "." files.
        if (ACE::isdotdir(directory->d_name))
            continue;

        std::string fullpath = path + "/" + directory->d_name;

        // Check if file is hidden
#ifdef WIN32
        DWORD dwAttrib = GetFileAttributes(fullpath.c_str());
        if (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_HIDDEN))
            continue;
#endif
#ifdef UNIX
        std::string name = directory->d_name.c_str();
        if (name != ".." || name != "." || name[0] == '.')
            continue;
#endif

        ACE_stat stat_buf;
        if (ACE_OS::lstat(fullpath.c_str(), &stat_buf) == -1)
            continue;

        // load subfolder
        if ((stat_buf.st_mode & S_IFMT) == (S_IFDIR))
        {
            GetScripts(fullpath);
            continue;
        }

        // was file, try add
        std::string filename = directory->d_name;
        AddScriptPath(filename, fullpath);
    }
#endif
}

static bool ScriptPathComparator(const LuaScript& first, const LuaScript& second)
{
    return first.filepath < second.filepath;
}

void Eluna::RunScripts()
{
    if (!IsEnabled())
        return;

    uint32 oldMSTime = ElunaUtil::GetCurrTime();
    uint32 count = 0;

    ScriptList scripts;
    lua_extensions.sort(ScriptPathComparator);
    lua_scripts.sort(ScriptPathComparator);
    scripts.insert(scripts.end(), lua_extensions.begin(), lua_extensions.end());
    scripts.insert(scripts.end(), lua_scripts.begin(), lua_scripts.end());

    std::unordered_map<std::string, std::string> loaded; // filename, path

    lua_getglobal(L, "package");
    // Stack: package
    luaL_getsubtable(L, -1, "loaded");
    // Stack: package, modules
    int modules = lua_gettop(L);
    for (ScriptList::const_iterator it = scripts.begin(); it != scripts.end(); ++it)
    {
        // Check that no duplicate names exist
        if (loaded.find(it->filename) != loaded.end())
        {
            ELUNA_LOG_ERROR("[Eluna]: Error loading `%s`. File with same name already loaded from `%s`, rename either file", it->filepath.c_str(), loaded[it->filename].c_str());
            continue;
        }
        loaded[it->filename] = it->filepath;

        lua_getfield(L, modules, it->filename.c_str());
        // Stack: package, modules, module
        if (!lua_isnoneornil(L, -1))
        {
            lua_pop(L, 1);
            ELUNA_LOG_DEBUG("[Eluna]: `%s` was already loaded or required", it->filepath.c_str());
            continue;
        }
        lua_pop(L, 1);
        // Stack: package, modules

        if (luaL_loadfile(L, it->filepath.c_str()))
        {
            // Stack: package, modules, errmsg
            ELUNA_LOG_ERROR("[Eluna]: Error loading `%s`", it->filepath.c_str());
            Report(L);
            // Stack: package, modules
            continue;
        }
        // Stack: package, modules, filefunc

        if (ExecuteCall(0, 1))
        {
            // Stack: package, modules, result
            if (lua_isnoneornil(L, -1) || (lua_isboolean(L, -1) && !lua_toboolean(L, -1)))
            {
                // if result evaluates to false, change it to true
                lua_pop(L, 1);
                Push(L, true);
            }
            lua_setfield(L, modules, it->filename.c_str());
            // Stack: package, modules

            // successfully loaded and ran file
            ELUNA_LOG_DEBUG("[Eluna]: Successfully loaded `%s`", it->filepath.c_str());
            ++count;
            continue;
        }
    }
    // Stack: package, modules
    lua_pop(L, 2);
    ELUNA_LOG_DEBUG("[Eluna]: Executed %u Lua scripts in %u ms", count, ElunaUtil::GetTimeDiff(oldMSTime));

    OnLuaStateOpen();
}

void Eluna::InvalidateObjects()
{
    lua_getglobal(L, ELUNA_OBJECT_STORE);
    ASSERT(lua_istable(L, -1));

    lua_pushnil(L);
    while (lua_next(L, -2))
    {
        if (ElunaObject* elunaObj = CHECKOBJ<ElunaObject>(L, -1, false))
            elunaObj->Invalidate();
        lua_pop(L, 1);
    }
    lua_pop(L, 1);
}

void Eluna::Report(lua_State* _L)
{
    const char* msg = lua_tostring(_L, -1);
    ELUNA_LOG_ERROR("%s", msg);
    lua_pop(_L, 1);
}

// Borrowed from http://stackoverflow.com/questions/12256455/print-stacktrace-from-c-code-with-embedded-lua
int Eluna::StackTrace(lua_State *_L)
{
    // Stack: errmsg
    if (!lua_isstring(_L, -1))  /* 'message' not a string? */
        return 1;  /* keep it intact */
    // Stack: errmsg, debug
    lua_getglobal(_L, "debug");
    if (!lua_istable(_L, -1))
    {
        lua_pop(_L, 1);
        return 1;
    }
    // Stack: errmsg, debug, traceback
    lua_getfield(_L, -1, "traceback");
    if (!lua_isfunction(_L, -1))
    {
        lua_pop(_L, 2);
        return 1;
    }
    lua_pushvalue(_L, -3);  /* pass error message */
    lua_pushinteger(_L, 1);  /* skip this function and traceback */
    // Stack: errmsg, debug, traceback, errmsg, 2
    lua_call(_L, 2, 1);  /* call debug.traceback */

    // dirty stack?
    // Stack: errmsg, debug, tracemsg
    return 1;
}

bool Eluna::ExecuteCall(int params, int res)
{
    int top = lua_gettop(L);
    int base = top - params;

    // Expected: function, [parameters]
    ASSERT(base > 0);

    // Check function type
    if (!lua_isfunction(L, base))
    {
        ELUNA_LOG_ERROR("[Eluna]: Cannot execute call: registered value is %s, not a function.", luaL_tolstring(L, base, nullptr));
        ASSERT(false); // stack probably corrupt
    }

    bool usetrace = eConfigMgr->GetBoolDefault("Eluna.TraceBack", false);
    if (usetrace)
    {
        lua_pushcfunction(L, &StackTrace);
        // Stack: function, [parameters], traceback
        lua_insert(L, base);
        // Stack: traceback, function, [parameters]
    }

    // Objects are invalidated when event_level hits 0
    ++event_level;
    int result = lua_pcall(L, params, res, usetrace ? base : 0);
    --event_level;

    if (usetrace)
    {
        // Stack: traceback, [results or errmsg]
        lua_remove(L, base);
    }
    // Stack: [results or errmsg]

    // lua_pcall returns 0 on success.
    // On error print the error and push nils for expected amount of returned values
    if (result)
    {
        // Stack: errmsg
        Report(L);

        // Force garbage collect
        lua_gc(L, LUA_GCCOLLECT, 0);

        // Push nils for expected amount of results
        for (int i = 0; i < res; ++i)
            lua_pushnil(L);
        // Stack: [nils]
        return false;
    }

    // Stack: [results]
    return true;
}

void Eluna::RemoveWorldObjectData(WorldObject* obj)
{
    worldObjectDataRefs.erase(obj->GetGUID());
    GetEventMgr()->Delete(obj->GetGUID());
}

void Eluna::Push(lua_State* luastate)
{
    lua_pushnil(luastate);
}
void Eluna::Push(lua_State* luastate, const uint64 l)
{
    Push(luastate, static_cast<int64>(l));
}
void Eluna::Push(lua_State* luastate, const int8 i)
{
    lua_pushinteger(luastate, i);
}
void Eluna::Push(lua_State* luastate, const uint8 i)
{
    lua_pushinteger(luastate, i);
}
void Eluna::Push(lua_State* luastate, const int16 i)
{
    lua_pushinteger(luastate, i);
}
void Eluna::Push(lua_State* luastate, const uint16 i)
{
    lua_pushinteger(luastate, i);
}
void Eluna::Push(lua_State* luastate, const int32 i)
{
    lua_pushinteger(luastate, i);
}
void Eluna::Push(lua_State* luastate, const uint32 i)
{
    lua_pushinteger(luastate, i);
}
void Eluna::Push(lua_State* luastate, const int64 i)
{
    lua_pushinteger(luastate, i);
}
void Eluna::Push(lua_State* luastate, const double d)
{
    lua_pushnumber(luastate, d);
}
void Eluna::Push(lua_State* luastate, const float f)
{
    lua_pushnumber(luastate, f);
}
void Eluna::Push(lua_State* luastate, const bool b)
{
    lua_pushboolean(luastate, b);
}
void Eluna::Push(lua_State* luastate, const std::string& str)
{
    lua_pushstring(luastate, str.c_str());
}
void Eluna::Push(lua_State* luastate, const char* str)
{
    lua_pushstring(luastate, str);
}
void Eluna::Push(lua_State* luastate, Pet const* pet)
{
    Push<Creature>(luastate, pet);
}
void Eluna::Push(lua_State* luastate, TempSummon const* summon)
{
    Push<Creature>(luastate, summon);
}
void Eluna::Push(lua_State* luastate, Unit const* unit)
{
    if (!unit)
    {
        Push(luastate);
        return;
    }
    switch (unit->GetTypeId())
    {
        case TYPEID_UNIT:
            Push(luastate, unit->ToCreature());
            break;
        case TYPEID_PLAYER:
            Push(luastate, unit->ToPlayer());
            break;
        default:
            ElunaTemplate<Unit>::Push(luastate, unit);
    }
}
void Eluna::Push(lua_State* luastate, WorldObject const* obj)
{
    if (!obj)
    {
        Push(luastate);
        return;
    }
    switch (obj->GetTypeId())
    {
        case TYPEID_UNIT:
            Push(luastate, obj->ToCreature());
            break;
        case TYPEID_PLAYER:
            Push(luastate, obj->ToPlayer());
            break;
        case TYPEID_GAMEOBJECT:
            Push(luastate, obj->ToGameObject());
            break;
        case TYPEID_CORPSE:
            Push(luastate, obj->ToCorpse());
            break;
        default:
            ElunaTemplate<WorldObject>::Push(luastate, obj);
    }
}
void Eluna::Push(lua_State* luastate, Object const* obj)
{
    if (!obj)
    {
        Push(luastate);
        return;
    }
    switch (obj->GetTypeId())
    {
        case TYPEID_UNIT:
            Push(luastate, obj->ToCreature());
            break;
        case TYPEID_PLAYER:
            Push(luastate, obj->ToPlayer());
            break;
        case TYPEID_GAMEOBJECT:
            Push(luastate, obj->ToGameObject());
            break;
        case TYPEID_CORPSE:
            Push(luastate, obj->ToCorpse());
            break;
        default:
            ElunaTemplate<Object>::Push(luastate, obj);
    }
}

template <typename T, typename U>
static bool CanTypeFitValue(const U value) {
    const intmax_t bot_t = intmax_t(std::numeric_limits<T>::min());
    const intmax_t bot_u = intmax_t(std::numeric_limits<U>::min());
    const uintmax_t top_t = uintmax_t(std::numeric_limits<T>::max());
    const uintmax_t top_u = uintmax_t(std::numeric_limits<U>::max());
    return !((bot_t > bot_u && value < static_cast<U> (bot_t)) || (top_t < top_u && value > static_cast<U> (top_t)));
}

template<typename T>
static T CheckIntegerRange(lua_State* luastate, int narg)
{
    static_assert(static_cast<uintmax_t>(std::numeric_limits<T>::max()) <= static_cast<uintmax_t>(std::numeric_limits<lua_Integer>::max()),
        "lua_Integer cannot hold given type T, it has too large max value.");
    static_assert(static_cast<intmax_t>(std::numeric_limits<T>::min()) >= static_cast<intmax_t>(std::numeric_limits<lua_Integer>::min()),
        "lua_Integer cannot hold given type T, it has too large minimum value");
    lua_Integer value = luaL_checkinteger(luastate, narg);

    if (value > static_cast<lua_Integer>(std::numeric_limits<T>::max()))
    {
        std::ostringstream ss;
        ss << "value must be less than or equal to " << static_cast<lua_Integer>(std::numeric_limits<T>::max());
        return luaL_argerror(luastate, narg, ss.str().c_str());
    }

    if (value < static_cast<lua_Integer>(std::numeric_limits<T>::min()))
    {
        std::ostringstream ss;
        ss << "value must be greater than or equal to " << static_cast<lua_Integer>(std::numeric_limits<T>::min());
        return luaL_argerror(luastate, narg, ss.str().c_str());
    }
    return static_cast<T>(value);
}

template<> bool Eluna::CHECKVAL<bool>(lua_State* luastate, int narg)
{
    return lua_toboolean(luastate, narg) != 0;
}
template<> float Eluna::CHECKVAL<float>(lua_State* luastate, int narg)
{
    return static_cast<float>(luaL_checknumber(luastate, narg));
}
template<> double Eluna::CHECKVAL<double>(lua_State* luastate, int narg)
{
    return luaL_checknumber(luastate, narg);
}
template<> int8 Eluna::CHECKVAL<int8>(lua_State* luastate, int narg)
{
    return CheckIntegerRange<int8>(luastate, narg);
}
template<> uint8 Eluna::CHECKVAL<uint8>(lua_State* luastate, int narg)
{
    return CheckIntegerRange<uint8>(luastate, narg);
}
template<> int16 Eluna::CHECKVAL<int16>(lua_State* luastate, int narg)
{
    return CheckIntegerRange<int16>(luastate, narg);
}
template<> uint16 Eluna::CHECKVAL<uint16>(lua_State* luastate, int narg)
{
    return CheckIntegerRange<uint16>(luastate, narg);
}
template<> int32 Eluna::CHECKVAL<int32>(lua_State* luastate, int narg)
{
    return CheckIntegerRange<int32>(luastate, narg);
}
template<> uint32 Eluna::CHECKVAL<uint32>(lua_State* luastate, int narg)
{
    return CheckIntegerRange<uint32>(luastate, narg);
}
template<> int64 Eluna::CHECKVAL<int64>(lua_State* luastate, int narg)
{
    return CheckIntegerRange<int64>(luastate, narg);
}
template<> uint64 Eluna::CHECKVAL<uint64>(lua_State* luastate, int narg)
{
    return static_cast<uint64>(CHECKVAL<int64>(luastate, narg));
}
template<> const char* Eluna::CHECKVAL<const char*>(lua_State* luastate, int narg)
{
    return luaL_checkstring(luastate, narg);
}
template<> std::string Eluna::CHECKVAL<std::string>(lua_State* luastate, int narg)
{
    return luaL_checkstring(luastate, narg);
}

template<> Object* Eluna::CHECKOBJ<Object>(lua_State* luastate, int narg, bool error)
{
    Object* obj = CHECKOBJ<WorldObject>(luastate, narg, false);
    if (!obj)
        obj = CHECKOBJ<Item>(luastate, narg, false);
    if (!obj)
        obj = ElunaTemplate<Object>::Check(luastate, narg, error);
    return obj;
}
template<> WorldObject* Eluna::CHECKOBJ<WorldObject>(lua_State* luastate, int narg, bool error)
{
    WorldObject* obj = CHECKOBJ<Unit>(luastate, narg, false);
    if (!obj)
        obj = CHECKOBJ<GameObject>(luastate, narg, false);
    if (!obj)
        obj = CHECKOBJ<Corpse>(luastate, narg, false);
    if (!obj)
        obj = ElunaTemplate<WorldObject>::Check(luastate, narg, error);
    return obj;
}
template<> Unit* Eluna::CHECKOBJ<Unit>(lua_State* luastate, int narg, bool error)
{
    Unit* obj = CHECKOBJ<Player>(luastate, narg, false);
    if (!obj)
        obj = CHECKOBJ<Creature>(luastate, narg, false);
    if (!obj)
        obj = ElunaTemplate<Unit>::Check(luastate, narg, error);
    return obj;
}

template<> ElunaObject* Eluna::CHECKOBJ<ElunaObject>(lua_State* luastate, int narg, bool error)
{
    return CHECKTYPE(luastate, narg, nullptr, error);
}

ElunaObject* Eluna::CHECKTYPE(lua_State* luastate, int narg, const char* tname, bool error)
{
    if (lua_islightuserdata(luastate, narg))
    {
        if (error)
            luaL_argerror(luastate, narg, "bad argument : userdata expected, got lightuserdata");
        return nullptr;
    }

    ElunaObject** ptrHold = static_cast<ElunaObject**>(lua_touserdata(luastate, narg));

    if (!ptrHold || (tname && (*ptrHold)->GetTypeName() != tname))
    {
        if (error)
        {
            char buff[256];
            snprintf(buff, 256, "bad argument : %s expected, got %s", tname ? tname : "ElunaObject", ptrHold ? (*ptrHold)->GetTypeName() : luaL_typename(luastate, narg));
            luaL_argerror(luastate, narg, buff);
        }
        return nullptr;
    }
    return *ptrHold;
}

/*
 * Cleans up the stack, effectively undoing all Push calls and the Setup call.
 */
void Eluna::CleanUpStack(int number_of_arguments)
{
    // Stack: event_id, [arguments]

    lua_pop(L, number_of_arguments + 1); // Add 1 because the caller doesn't know about `event_id`.
    // Stack: (empty)

    if (event_level == 0)
        InvalidateObjects();
}

/*
 * Call a single event handler that was put on the stack with `Setup` and removes it from the stack.
 *
 * The caller is responsible for keeping track of how many times this should be called.
 */
int Eluna::CallOneFunction(int number_of_functions, int number_of_arguments, int number_of_results)
{
    ++number_of_arguments; // Caller doesn't know about `event_id`.
    ASSERT(number_of_functions > 0 && number_of_arguments > 0 && number_of_results >= 0);
    // Stack: event_id, [arguments], [functions]

    int functions_top        = lua_gettop(L);
    int first_function_index = functions_top - number_of_functions + 1;
    int arguments_top        = first_function_index - 1;
    int first_argument_index = arguments_top - number_of_arguments + 1;

    // Copy the arguments from the bottom of the stack to the top.
    for (int argument_index = first_argument_index; argument_index <= arguments_top; ++argument_index)
    {
        lua_pushvalue(L, argument_index);
    }
    // Stack: event_id, [arguments], [functions], event_id, [arguments]

    ExecuteCall(number_of_arguments, number_of_results);
    --functions_top;
    // Stack: event_id, [arguments], [functions - 1], [results]

    return functions_top + 1; // Return the location of the first result (if any exist).
}

CreatureAI* Eluna::GetAI(Creature* creature)
{
    if (!IsEnabled())
        return nullptr;

    for (auto const & event_id : Hooks::TypeSpecific<BINDTYPE_CREATURE>::events)
    {
        auto entryKey = EntryKey(event_id.first, BINDTYPE_CREATURE, creature->GetEntry());
        auto guidKey = GuidKey(event_id.first, BINDTYPE_CREATURE, GUID_LOPART(creature->GET_GUID()));

        if (EntryEventBindings->HasBindingsFor(entryKey) || GuidEventBindings->HasBindingsFor(guidKey))
            return new ElunaCreatureAI(creature);
    }

    return nullptr;
}

GameObjectAI* Eluna::GetAI(GameObject* gameobject)
{
    if (!IsEnabled())
        return nullptr;

    for (auto const & event_id : Hooks::TypeSpecific<BINDTYPE_GAMEOBJECT>::events)
    {
        auto entryKey = EntryKey(event_id.first, BINDTYPE_GAMEOBJECT, gameobject->GetEntry());
        auto guidKey = GuidKey(event_id.first, BINDTYPE_GAMEOBJECT, GUID_LOPART(gameobject->GET_GUID()));

        if (EntryEventBindings->HasBindingsFor(entryKey) || GuidEventBindings->HasBindingsFor(guidKey))
            return new ElunaGameObjectAI(gameobject);
    }

    return nullptr;
}
