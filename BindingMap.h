/*
* Copyright (C) 2010 - 2016 Eluna Lua Engine <http://emudevs.com/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#ifndef _BINDING_MAP_H
#define _BINDING_MAP_H

#include <memory>
#include "Common.h"
#include "ObjectGuid.h"
#include <type_traits>
#include "LuaEngine.h"

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};

struct lua_State;

/*
* A set of bindings from keys of type `K` to Lua references.
*/
template<typename K>
class BindingMap
{
public:
    typedef uint64 BindGuid;
private:
    lua_State* L;
    BindGuid maxBindingID;

    struct Binding
    {
        BindGuid id;
        lua_State* L;
        uint32 remainingShots;
        int functionReference;

        Binding(lua_State* L, BindGuid id, int functionReference, uint32 remainingShots) :
            id(id),
            L(L),
            remainingShots(remainingShots),
            functionReference(functionReference)
        { }

        ~Binding()
        {
            luaL_unref(L, LUA_REGISTRYINDEX, functionReference);
        }
    };

    typedef std::vector< std::unique_ptr<Binding> > BindingList;

    std::unordered_map<K, BindingList> bindings;
    /*
    * This table is for fast removal of bindings by ID.
    *
    * Instead of having to look through (potentially) every BindingList to find
    *   the Binding with the right ID, this allows you to go directly to the
    *   BindingList that might have the Binding with that ID.
    *
    * However, you must be careful not to store pointers to BindingLists
    *   that no longer exist (see `void Clear(const K& key)` implementation).
    */
    std::unordered_map<BindGuid, BindingList*> id_lookup_table;

    static int cancelBinding(lua_State *L)
    {
        BindGuid bindid = Eluna::CHECKVAL<BindGuid>(L, lua_upvalueindex(1));
        BindingMap<K>* bindmap = static_cast<BindingMap<K>*>(lua_touserdata(L, lua_upvalueindex(2)));
        bindmap->Remove(bindid);
        return 0;
    }

public:
    BindingMap(lua_State* L) :
        L(L),
        maxBindingID(0)
    { }

    /*
    * Insert a new binding from `key` to `ref`, which lasts for `shots`-many pushes.
    * Pushes a cancel callback function to lua stack.
    *
    * If `shots` is 0, it will never automatically expire, but can still be
    *   removed with `Clear` or `Remove`.
    */
    BindGuid Insert(const K& key, int ref, uint32 shots)
    {
        BindGuid id = (++maxBindingID);
        ASSERT(id != 0);

        // save as a binding
        BindingList& list = bindings[key];
        list.push_back(std::unique_ptr<Binding>(new Binding(L, id, ref, shots)));
        id_lookup_table[id] = &list;

        // create cancel callback and push it to stack
        Eluna::Push(L, id);
        lua_pushlightuserdata(L, this);
        lua_pushcclosure(L, &cancelBinding, 2);
        return id;
    }

    /*
    * Clear all bindings for `key`.
    */
    void Clear(const K& key)
    {
        if (bindings.empty())
            return;

        auto iter = bindings.find(key);
        if (iter == bindings.end())
            return;

        BindingList& list = iter->second;

        // Remove all pointers to `list` from `id_lookup_table`.
        for (auto i = list.begin(); i != list.end(); ++i)
        {
            std::unique_ptr<Binding>& binding = *i;
            id_lookup_table.erase(binding->id);
        }

        bindings.erase(key);
    }

    /*
    * Clear all bindings for all keys.
    */
    void Clear()
    {
        if (bindings.empty())
            return;

        id_lookup_table.clear();
        bindings.clear();
    }

    /*
    * Remove a specific binding identified by `id`.
    *
    * If `id` in invalid, nothing is removed.
    */
    void Remove(BindGuid id)
    {
        auto iter = id_lookup_table.find(id);
        if (iter == id_lookup_table.end())
            return;

        BindingList* list = iter->second;
        auto i = list->begin();

        for (; i != list->end(); ++i)
        {
            std::unique_ptr<Binding>& binding = *i;
            if (binding->id == id)
                break;
        }

        if (i != list->end())
            list->erase(i);

        // Unconditionally erase the ID in the lookup table because
        //   it was either already invalid, or it's no longer valid.
        id_lookup_table.erase(id);
    }

    /*
    * Check whether `key` has any bindings.
    */
    bool HasBindingsFor(const K& key)
    {
        if (bindings.empty())
            return false;

        auto result = bindings.find(key);
        if (result == bindings.end())
            return false;

        BindingList& list = result->second;
        return !list.empty();
    }

    /*
    * Push all Lua references for `key` onto the stack.
    */
    void PushRefsFor(const K& key)
    {
        if (bindings.empty())
            return;

        auto result = bindings.find(key);
        if (result == bindings.end())
            return;

        BindingList& list = result->second;
        for (auto i = list.begin(); i != list.end();)
        {
            std::unique_ptr<Binding>& binding = (*i);
            auto i_prev = (i++);

            lua_rawgeti(L, LUA_REGISTRYINDEX, binding->functionReference);

            if (binding->remainingShots > 0)
            {
                binding->remainingShots -= 1;

                if (binding->remainingShots == 0)
                {
                    id_lookup_table.erase(binding->id);
                    list.erase(i_prev);
                }
            }
        }
    }
};

/*
* A `BindingMap` key type for an event (some global event)
*   (event specific - event happens).
*/
struct EventKey
{
    std::string event_id;

    EventKey(std::string const & event_id) :
        event_id(event_id)
    { }
};

/*
* A `BindingMap` key type for event ID/Object entry ID bindings
*   (entry specific - event happens for some entry (creature, gameobject .. map)).
*/
struct EntryKey
{
    std::string event_id;
    BindingType type_id;
    uint32 entry;

    EntryKey(std::string const & event_id, BindingType const & type_id, uint32 const & entry) :
        event_id(event_id),
        type_id(type_id),
        entry(entry)
    { }
};

/*
* A `BindingMap` key type for event ID/unique Object bindings
*   (guid specific - event happens for some guid (creature, gameobject ..)).
*/
struct GuidKey
{
#ifdef TRINITY
    typedef ObjectGuid::LowType GuidType;
#else
    typedef uint32 GuidType;
#endif

    std::string event_id;
    BindingType type_id;
    GuidType guid;

    GuidKey(std::string const & event_id, BindingType const & type_id, GuidType const & guid) :
        event_id(event_id),
        guid(guid)
    { }
};

class hash_helper
{
public:
    typedef std::size_t result_type;

    template <typename... T>
    static inline result_type hash(T const &... t)
    {
        result_type seed = 0;
        _hash_combine(seed, t...);
        return seed;
    }

    template <typename T, typename std::enable_if<std::is_enum<T>::value>::type* = nullptr>
    static inline result_type hash(T const & t)
    {
        return std::hash<typename std::underlying_type<T>::type>()(t);
    }

    template <typename T, typename std::enable_if<!std::is_enum<T>::value>::type* = nullptr>
    static inline result_type hash(T const & t)
    {
        return std::hash<T>()(t);
    }

private:
    template <typename T>
    static inline void _hash_combine(result_type& seed, T const & v)
    {
        // from http://www.boost.org/doc/libs/1_40_0/boost/functional/hash/hash.hpp
        seed ^= hash(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    template <typename H, typename... T>
    static inline void _hash_combine(result_type& seed, H const & h, T const &... t)
    {
        _hash_combine(seed, h);
        _hash_combine(seed, t...);
    }
};

/*
* Implementations of various std functions on the above key types,
*   so that they can be used within an unordered_map.
*/
namespace std
{
    template<>
    struct equal_to < EventKey >
    {
        bool operator()(EventKey const& lhs, EventKey const& rhs) const
        {
            return
                lhs.event_id == rhs.event_id;
        }
    };

    template<>
    struct equal_to < EntryKey >
    {
        bool operator()(EntryKey const& lhs, EntryKey const& rhs) const
        {
            return
                lhs.entry == rhs.entry &&
                lhs.event_id == rhs.event_id &&
                lhs.type_id == rhs.type_id;
        }
    };

    template<>
    struct equal_to < GuidKey >
    {
        bool operator()(GuidKey const& lhs, GuidKey const& rhs) const
        {
            return
                lhs.guid == rhs.guid &&
                lhs.event_id == rhs.event_id &&
                lhs.type_id == rhs.type_id;
        }
    };

    template<>
    struct hash < EventKey >
    {
        typedef EventKey argument_type;

        hash_helper::result_type operator()(argument_type const& k) const
        {
            return hash_helper::hash(k.event_id);
        }
    };

    template<>
    struct hash < EntryKey >
    {
        typedef EntryKey argument_type;

        hash_helper::result_type operator()(argument_type const& k) const
        {
            return hash_helper::hash(k.event_id, k.type_id, k.entry);
        }
    };

    template<>
    struct hash < GuidKey >
    {
        typedef GuidKey argument_type;

        hash_helper::result_type operator()(argument_type const& k) const
        {
            return hash_helper::hash(k.event_id, k.type_id, k.guid);
        }
    };
}

#endif // _BINDING_MAP_H
