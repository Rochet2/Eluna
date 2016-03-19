/*
* Copyright (C) 2010 - 2016 Eluna Lua Engine <http://emudevs.com/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#ifndef _ELUNA_TEMPLATE_H
#define _ELUNA_TEMPLATE_H

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};
#include "LuaEngine.h"
#include "ElunaUtility.h"
#include "SharedDefines.h"
#include "Hooks.h"

struct ElunaFunction
{
    ElunaEnvironments env;
    const char* name;
    int(*mfunc)(lua_State*);

    static int FunctionCaller(lua_State* L)
    {
        ElunaFunction* l = static_cast<ElunaFunction*>(lua_touserdata(L, lua_upvalueindex(1)));
        if (Eluna::GetEluna(L)->current_thread_id != std::this_thread::get_id())
        {
            ELUNA_LOG_ERROR("[Eluna]: Race condition using function. Report to devs with this message and details about what you were doing - Function name: %s, env: %i", l->name, l->env);
        }
        int top = lua_gettop(L);
        int expected = l->mfunc(L);
        int args = lua_gettop(L) - top;
        if (args < 0 || args > expected)
        {
            ELUNA_LOG_ERROR("[Eluna]: %s returned unexpected amount of arguments %i out of %i. Report to devs", l->name, args, expected);
        }
        if (args < expected)
            lua_settop(L, top+expected);
        return expected;
    }

    static void SetGlobalFunctions(Eluna* E, ElunaFunction* functions)
    {
        ASSERT(E && E->L);
        ASSERT(functions);

        lua_pushglobaltable(E->L);

        for (ElunaFunction* function = functions; function && function->name; ++function)
        {
            if (!(function->env & E->env))
                continue;
            // allow using null function pointer to indicate that the field should be cleared
            if (!function->mfunc)
            {
                lua_pushstring(E->L, function->name);
                lua_pushnil(E->L);
                lua_rawset(E->L, -3);
                continue;
            }
            lua_pushstring(E->L, function->name);
            lua_pushlightuserdata(E->L, static_cast<void*>(function));
            lua_pushcclosure(E->L, ElunaFunction::FunctionCaller, 1);
            lua_rawset(E->L, -3);
        }

        lua_pop(E->L, 1);
    }
};

class ElunaObject
{
public:
    template<typename T>
    ElunaObject(T* obj, bool manageMemory);

    ~ElunaObject()
    {
    }

    // Get wrapped object pointer
    void* GetObj() const { return object; }
    // Returns if Eluna should manage the memory of wrapped object
    bool IsMemorymanaged() const { return managememory; }
    // Returns if the wrapped object is valid to be accessed
    bool IsValid() const { return isvalid; }
    // Returns pointer to the wrapped object's type name
    const char* const GetTypeName() const { return type_name_ptr; }

    // Sets the wrapped object to valid or invalid
    void SetValid(bool valid)
    {
        ASSERT(!valid || (valid && object));
        isvalid = valid;
    }
    // Invalidates the pointer if it should be invalidated
    void Invalidate()
    {
        if (!IsMemorymanaged())
            isvalid = false;
    }

private:
    bool isvalid;
    const bool managememory;
    void* const object;
    const char* const type_name_ptr;
};

template<typename T>
class ElunaTemplate
{
public:
    static const char* const type_name;
    static const bool manageMemory;
    static ElunaFunction* const methods;
    static const std::string inherited;

    /*
     * RegisterTypeForState registers the the type to the method system.
     * Registering basically allows the methods to be called if the type
     * is used as a metatable. It also allows inheritance of the type.
     * All used types MUST be registered and only ONCE per lua instance.
     * CreateMetatable can be called only once all types are registered.
     */
    static void RegisterTypeForState(Eluna* E)
    {
        ASSERT(E);
        ASSERT(type_name);
        ASSERT(E->classCreators.find(type_name) == E->classCreators.end());
        E->classCreators[type_name] = CreateMetatable;
        // Do not yet call CreateMetatable.
        // All types must be registered before
        // doing that due to inheritance.
    }

    /*
     * CreateMetatable creates the metatable for this specifc type.
     * The inherited metatables are also created.
     * The metatables are not created if they already exist.
     * Note that this function should be called only after all types have been
     * registered by calling RegisterTypeForState on each type respectively.
     */
    static void CreateMetatable(Eluna* E)
    {
        ASSERT(E && E->L);
        lua_State* L = E->L;

        // Make sure that the class is actually registered
        ASSERT(E->classCreators.find(type_name) != E->classCreators.end());

        // check that metatable isn't already there
        lua_pushstring(L, type_name);
        lua_rawget(L, LUA_REGISTRYINDEX);
        bool exists = !lua_isnoneornil(L, -1);
        lua_pop(L, 1);
        if (exists)
            return;

        // create methodtable for userdata of this type
        lua_newtable(L);
        int methodtable = lua_gettop(L);

        // create metatable for userdata of this type
        luaL_newmetatable(L, type_name);
        int metatable = lua_gettop(L);

        // garbage collecting
        lua_pushcfunction(L, &__gc);
        lua_setfield(L, metatable, "__gc");

        // pointer equality
        lua_pushcfunction(L, &__eq);
        lua_setfield(L, metatable, "__eq");

        // tostring
        lua_pushcfunction(L, &__tostring);
        lua_setfield(L, metatable, "__tostring");

        // make methods accessible through metatable
        lua_pushvalue(L, methodtable);
        lua_setfield(L, metatable, "__index");

        // hide metatable for safety reasons
        // for example __gc metamethod could be overwritten by a user
        lua_pushstring(L, "metatable access not allowed");
        lua_setfield(L, metatable, "__metatable");

        // add normal methods to methodtable
        for (ElunaFunction* method = methods; method && method->name; ++method)
        {
            if (!(method->env & E->env))
                continue;
            // allow using null function pointer to indicate that the field should be cleared
            if (!method->mfunc)
            {
                lua_pushstring(L, method->name);
                lua_pushnil(L);
                lua_rawset(L, methodtable);
                continue;
            }
            lua_pushstring(L, method->name);
            lua_pushlightuserdata(L, static_cast<void*>(method));
            lua_pushcclosure(L, ElunaFunction::FunctionCaller, 1);
            lua_rawset(L, methodtable);
        }

        // check that inherit exists
        auto it = E->classCreators.find(inherited);
        if (it != E->classCreators.end())
        {
            // create inherited metatable if needed
            it->second(E);

            // check that metatable is there
            lua_pushstring(L, inherited.c_str());
            lua_rawget(L, LUA_REGISTRYINDEX);
            bool exists = !lua_isnoneornil(L, -1);
            if (exists)
            {
                // add the inherited metatable as metatable
                // for the methodtable
                lua_setmetatable(L, methodtable);
            }
            else
            {
                lua_pop(L, 1);
                ELUNA_LOG_ERROR("%s cannot inherit %s, could not create/find metatable", type_name, inherited.c_str());
            }
        }
        else if (!inherited.empty())
        {
            ELUNA_LOG_ERROR("%s cannot inherit %s, no metatable creator for inherited", type_name, inherited.c_str());
        }

        // pop methodtable and metatable
        lua_pop(L, 2);
    }

    /*
     * Wraps the obj to ElunaObject and pushes it to the lua stack.
     * Creates the metatable for the type pushed if it does not exist.
     * Sets the type's metatable as a metatable for the pushed object.
     * The object is wrapped and stored unless it already exists in lua.
     * Only one wrapped instance of an object exists in lua.
     */
    // Use default memory management
    static int Push(lua_State* L, T* obj)
    {
        // use default memory management
        return Push(L, obj, manageMemory);
    }
    // Use selected memory management
    static int Push(lua_State* L, T* obj, bool managememory)
    {
        if (!obj)
        {
            lua_pushnil(L);
            return 1;
        }

        void* obj_voidptr = static_cast<void*>(obj);

        lua_pushstring(L, ELUNA_OBJECT_STORE);
        lua_rawget(L, LUA_REGISTRYINDEX);
        ASSERT(lua_istable(L, -1));
        lua_pushlightuserdata(L, obj_voidptr);
        lua_rawget(L, -2);
        if (ElunaObject* elunaObj = Eluna::CHECKTYPE(L, -1, type_name, false))
        {
            // set userdata valid
            elunaObj->SetValid(true);

            // remove userdata_table, leave userdata
            lua_remove(L, -2);
            return 1;
        }
        lua_pop(L, 1);
        // left userdata_table in stack

        // Create new userdata
        ElunaObject** ptrHold = static_cast<ElunaObject**>(lua_newuserdata(L, sizeof(ElunaObject*)));
        if (!ptrHold)
        {
            ELUNA_LOG_ERROR("%s could not create new userdata", type_name);
            lua_pop(L, 2);
            lua_pushnil(L);
            return 1;
        }
        *ptrHold = new ElunaObject(obj, managememory);

        // Set metatable for it
        lua_pushstring(L, type_name);
        lua_rawget(L, LUA_REGISTRYINDEX);
        if (!lua_istable(L, -1))
        {
            lua_pop(L, 1);
            ELUNA_LOG_DEBUG("%s missing metatable, attempt creating it", type_name);
            CreateMetatable(Eluna::GetEluna(L));

            lua_pushstring(L, type_name);
            lua_rawget(L, LUA_REGISTRYINDEX);
            if (!lua_istable(L, -1))
            {
                ELUNA_LOG_ERROR("%s missing metatable and creation attempt failed, possibly forgot to register the type", type_name);
                lua_pop(L, 3);
                lua_pushnil(L);
                ASSERT(false);
                return 1;
            }
        }
        lua_setmetatable(L, -2);

        lua_pushlightuserdata(L, obj_voidptr);
        lua_pushvalue(L, -2);
        lua_rawset(L, -4);
        lua_remove(L, -2);
        return 1;
    }

    /*
     * Returns an object of the type from the given index if can.
     * Otherwise returns nullptr. The object in stack must be ElunaObject that is valid.
     * If error parameter is true then an error is raised if the object is not valid.
     */
    static T* Check(lua_State* L, int narg, bool error = true)
    {
        ASSERT(type_name);

        ElunaObject* elunaObj = Eluna::CHECKTYPE(L, narg, type_name, error);
        if (!elunaObj)
            return NULL;

        if (!elunaObj->IsValid())
        {
            char buff[256];
            snprintf(buff, 256, "%s expected, got pointer to nonexisting (invalidated) object (%s). Check your code.", type_name, luaL_typename(L, narg));
            if (error)
            {
                luaL_argerror(L, narg, buff);
            }
            else
            {
                ELUNA_LOG_ERROR("%s", buff);
            }
            return nullptr;
        }
        return static_cast<T*>(elunaObj->GetObj());
    }

    /**
     * Deletes the ElunaObject and the wrapped object if it should be managed by Eluna
     *
     * The method tries to get the EluaObject wrapper and delete it.
     * If no ElunaObjet is available, then nothing is done.
     * Otherwise if memory is managed for this type, the object is destroyed as well
     */
    static int __gc(lua_State* L)
    {
        // Get object pointer (and check type, no error)
        // Note that this may be used as a metamethod for another table
        // and not only as metamethod for userdata.
        ElunaObject* obj = Eluna::CHECKOBJ<ElunaObject>(L, 1, false);
        if (obj && obj->IsMemorymanaged())
            delete static_cast<T*>(obj->GetObj());
        delete obj;
        return 0;
    }

    /**
     * Returns the the class name along with the internal pointer value in a string.
     *
     * The class name and internal pointer value should be in similar format
     * as the return value of tostring(t) on a table t.
     * This is a metamethod that is triggered when tostring is used with an object.
     *
     *     Player: 4F37EBD0
     *
     * @return string to_string
     */
    static int __tostring(lua_State* L)
    {
        ElunaObject* obj = Eluna::CHECKOBJ<ElunaObject>(L, 1);
        lua_pushfstring(L, "%s: %p", obj->GetTypeName(), obj->GetObj());
        return 1;
    }

    /**
     * Returns true if the objects are have the same internal pointer value.
     *
     * Note that if both pointers are null, true is returned.
     * This is a metamethod that is triggered when compairson is used with two objects.
     *
     * @return bool equal
     */
    static int __eq(lua_State* L)
    {
        ElunaObject* a = Eluna::CHECKOBJ<ElunaObject>(L, 1, false);
        ElunaObject* b = Eluna::CHECKOBJ<ElunaObject>(L, 2, false);
        Eluna::Push(L, a == b);
        return 1;
    }
};

template<typename T>
ElunaObject::ElunaObject(T* obj, bool manageMemory) : isvalid(false), managememory(manageMemory), object(obj), type_name_ptr(ElunaTemplate<T>::type_name)
{
    ASSERT(type_name_ptr);
    SetValid(true);
}

#if (!defined(TBC) && !defined(CLASSIC))
// fix compile error about accessing vehicle destructor
template<> int ElunaTemplate<Vehicle>::__gc(lua_State* L)
{
    ASSERT(!manageMemory);

    // Get object pointer (and check type, no error)
    // Note that this may be used as a metamethod for another table
    // and not only as metamethod for userdata.
    ElunaObject* obj = Eluna::CHECKOBJ<ElunaObject>(L, 1, false);
    delete obj;
    return 0;
}
#endif

/*
 * ELUNA_TYPE is a macro that is used to define a type for Eluna.
 * All types MUST use this and only ONCE.
 * As an example: ELUNA_TYPE(Aura, false, AuraMethods, "ElunaBase")
 * TYPE is the class we define, MANAGE_MEMORY defines whether by default
 * Eluna should destroy the pushed object when it is no longer referenced from lua.
 * METHODS is either nullptr or an array of ElunaFunction that define all methods of the type.
 * INHERITED is an inherited class. This could for example be "Aura" to inherit the Aura type's methods.
 * It is assumed that if nothing is inherited, INHERITED is set to "ElunaBase" to inherit the base methods.
 */
#define ELUNA_TYPE(TYPE, MANAGE_MEMORY, METHODS, INHERITED) \
template<> const char* const ElunaTemplate<TYPE>::type_name = #TYPE; \
template<> const bool ElunaTemplate<TYPE>::manageMemory = MANAGE_MEMORY; \
template<> ElunaFunction* const ElunaTemplate<TYPE>::methods = METHODS; \
template<> const std::string ElunaTemplate<TYPE>::inherited = INHERITED; \
template class ElunaTemplate<TYPE>;

void RegisterTypeAura(Eluna* E);
void RegisterTypeCreature(Eluna* E);
void RegisterTypeElunaBase(Eluna* E);
void RegisterTypeElunaQuery(Eluna* E);
void RegisterTypeGameObject(Eluna* E);
void RegisterTypeGlobal(Eluna* E);
void RegisterTypeGroup(Eluna* E);
void RegisterTypeGuild(Eluna* E);
void RegisterTypeItem(Eluna* E);
void RegisterTypeMap(Eluna* E);
void RegisterTypeObject(Eluna* E);
void RegisterTypePlayer(Eluna* E);
void RegisterTypeQuest(Eluna* E);
void RegisterTypeSpell(Eluna* E);
void RegisterTypeUnit(Eluna* E);
#ifndef CLASSIC
#ifndef TBC
void RegisterTypeVehicle(Eluna* E);
#endif
#endif
void RegisterTypeWorldObject(Eluna* E);
void RegisterTypeWorldPacket(Eluna* E);
void RegisterTypeDynamicObject(Eluna* E);
void RegisterTypeCorpse(Eluna* E);

void RegisterTemplates(Eluna* E);

#endif
