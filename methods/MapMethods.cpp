/*
* Copyright (C) 2010 - 2016 Eluna Lua Engine <http://emudevs.com/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#include "LuaEngine.h"
#include "ElunaTemplate.h"

#include "ElunaIncludes.h"

/***
 * A game map, e.g. Azeroth, Eastern Kingdoms, the Molten Core, etc.
 *
 * Inherits all methods from: [ElunaBase]
 */
namespace LuaMap
{

#ifndef CLASSIC
    /**
     * Returns `true` if the [Map] is an arena [BattleGround], `false` otherwise.
     *
     * @return bool isArena
     */
    int IsArena(lua_State* L)
    {
        Map* map = Eluna::CHECKOBJ<Map>(L, 1);
        Eluna::Push(L, map->IsBattleArena());
        return 1;
    }
#endif

    /**
     * Returns `true` if the [Map] is a non-arena [BattleGround], `false` otherwise.
     *
     * @return bool isBattleGround
     */
    int IsBattleground(lua_State* L)
    {
        Map* map = Eluna::CHECKOBJ<Map>(L, 1);
#ifndef TRINITY
        Eluna::Push(L, map->IsBattleGround());
#else
        Eluna::Push(L, map->IsBattleground());
#endif
        return 1;
    }

    /**
     * Returns `true` if the [Map] is a dungeon, `false` otherwise.
     *
     * @return bool isDungeon
     */
    int IsDungeon(lua_State* L)
    {
        Map* map = Eluna::CHECKOBJ<Map>(L, 1);
        Eluna::Push(L, map->IsDungeon());
        return 1;
    }

    /**
     * Returns `true` if the [Map] has no [Player]s, `false` otherwise.
     *
     * @return bool isEmpty
     */
    int IsEmpty(lua_State* L)
    {
        Map* map = Eluna::CHECKOBJ<Map>(L, 1);
        Eluna::Push(L, map->isEmpty());
        return 1;
    }

#ifndef CLASSIC
    /**
     * Returns `true` if the [Map] is a heroic, `false` otherwise.
     *
     * @return bool isHeroic
     */
    int IsHeroic(lua_State* L)
    {
        Map* map = Eluna::CHECKOBJ<Map>(L, 1);
        Eluna::Push(L, map->IsHeroic());
        return 1;
    }
#endif

    /**
     * Returns `true` if the [Map] is a raid, `false` otherwise.
     *
     * @return bool isRaid
     */
    int IsRaid(lua_State* L)
    {
        Map* map = Eluna::CHECKOBJ<Map>(L, 1);
        Eluna::Push(L, map->IsRaid());
        return 1;
    }

    /**
     * Returns the name of the [Map].
     *
     * @return string mapName
     */
    int GetName(lua_State* L)
    {
        Map* map = Eluna::CHECKOBJ<Map>(L, 1);
        Eluna::Push(L, map->GetMapName());
        return 1;
    }

    /**
     * Returns the height of the [Map] at the given X and Y coordinates.
     *
     * In case of no height found nil is returned
     *
     * @param float x
     * @param float y
     * @return float z
     */
    int GetHeight(lua_State* L)
    {
        Map* map = Eluna::CHECKOBJ<Map>(L, 1);
        float x = Eluna::CHECKVAL<float>(L, 2);
        float y = Eluna::CHECKVAL<float>(L, 3);
#if (defined(TBC) || defined(CLASSIC))
        float z = map->GetHeight(x, y, MAX_HEIGHT);
#else
        uint32 phasemask = Eluna::CHECKVAL<uint32>(L, 4, 1);
        float z = map->GetHeight(phasemask, x, y, MAX_HEIGHT);
#endif
        if (z != INVALID_HEIGHT)
            Eluna::Push(L, z);
        return 1;
    }

    /**
     * Returns the difficulty of the [Map].
     *
     * Always returns 0 if the expansion is pre-TBC.
     *
     * @return int32 difficulty
     */
    int GetDifficulty(lua_State* L)
    {
        Map* map = Eluna::CHECKOBJ<Map>(L, 1);
#ifndef CLASSIC
        Eluna::Push(L, map->GetDifficulty());
#else
        Eluna::Push(L, (Difficulty)0);
#endif
        return 1;
    }

    /**
     * Returns the instance ID of the [Map].
     *
     * @return uint32 instanceId
     */
    int GetInstanceId(lua_State* L)
    {
        Map* map = Eluna::CHECKOBJ<Map>(L, 1);
        Eluna::Push(L, map->GetInstanceId());
        return 1;
    }

    /**
     * Returns the player count currently on the [Map] (excluding GMs).
     *
     * @return uint32 playerCount
     */
    int GetPlayerCount(lua_State* L)
    {
        Map* map = Eluna::CHECKOBJ<Map>(L, 1);
        Eluna::Push(L, map->GetPlayersCountExceptGMs());
        return 1;
    }

    /**
     * Returns the ID of the [Map].
     *
     * @return uint32 mapId
     */
    int GetMapId(lua_State* L)
    {
        Map* map = Eluna::CHECKOBJ<Map>(L, 1);
        Eluna::Push(L, map->GetId());
        return 1;
    }

    /**
     * Returns the area ID of the [Map] at the specified X, Y, and Z coordinates.
     *
     * @param float x
     * @param float y
     * @param float z
     * @return uint32 areaId
     */
    int GetAreaId(lua_State* L)
    {
        Map* map = Eluna::CHECKOBJ<Map>(L, 1);
        float x = Eluna::CHECKVAL<float>(L, 2);
        float y = Eluna::CHECKVAL<float>(L, 3);
        float z = Eluna::CHECKVAL<float>(L, 4);

#ifndef TRINITY
        Eluna::Push(L, map->GetTerrain()->GetAreaId(x, y, z));
#else
        Eluna::Push(L, map->GetAreaId(x, y, z));
#endif
        return 1;
    }

    /**
     * Returns a [WorldObject] by its GUID from the map if it is spawned.
     *
     * @param uint64 guid
     */
    int GetWorldObject(lua_State* L)
    {
        Map* map = Eluna::CHECKOBJ<Map>(L, 1);
        uint64 guid = Eluna::CHECKVAL<uint64>(L, 2);

#ifndef TRINITY
        Eluna::Push(L, map->GetWorldObject(ObjectGuid(guid)));
#else
        switch (GUID_HIPART(guid))
        {
            case HIGHGUID_PLAYER:
                Eluna::Push(L, eObjectAccessor()GetPlayer(map, ObjectGuid(guid)));
                break;
            case HIGHGUID_TRANSPORT:
            case HIGHGUID_MO_TRANSPORT:
            case HIGHGUID_GAMEOBJECT:
                Eluna::Push(L, map->GetGameObject(ObjectGuid(guid)));
                break;
            case HIGHGUID_VEHICLE:
            case HIGHGUID_UNIT:
                Eluna::Push(L, map->GetCreature(ObjectGuid(guid)));
                break;
            case HIGHGUID_PET:
                Eluna::Push(L, map->GetPet(ObjectGuid(guid)));
                break;
            case HIGHGUID_DYNAMICOBJECT:
                Eluna::Push(L, map->GetDynamicObject(ObjectGuid(guid)));
                break;
            case HIGHGUID_CORPSE:
                Eluna::Push(L, map->GetCorpse(ObjectGuid(guid)));
                break;
            default:
                break;
        }
#endif
        return 1;
    }

    /**
     * Sets the [Weather] type based on [WeatherType] and grade supplied.
     *
     *     enum WeatherType
     *     {
     *         WEATHER_TYPE_FINE       = 0,
     *         WEATHER_TYPE_RAIN       = 1,
     *         WEATHER_TYPE_SNOW       = 2,
     *         WEATHER_TYPE_STORM      = 3,
     *         WEATHER_TYPE_THUNDERS   = 86,
     *         WEATHER_TYPE_BLACKRAIN  = 90
     *     };
     *
     * @param uint32 zone : id of the zone to set the weather for
     * @param [WeatherType] type : the [WeatherType], see above available weather types
     * @param float grade : the intensity/grade of the [Weather], ranges from 0 to 1
     */
    int SetWeather(lua_State* L)
    {
        Map* map = Eluna::CHECKOBJ<Map>(L, 1);
        uint32 zoneId = Eluna::CHECKVAL<uint32>(L, 2);
        uint32 weatherType = Eluna::CHECKVAL<uint32>(L, 3);
        float grade = Eluna::CHECKVAL<float>(L, 4);

#if defined(TRINITY)
        Weather* weather = WeatherMgr::FindWeather(zoneId);
        if (!weather)
            weather = WeatherMgr::AddWeather(zoneId);
        if (weather)
            weather->SetWeather((WeatherType)weatherType, grade);
#else
        if (Weather::IsValidWeatherType(weatherType))
            map->SetWeather(zoneId, (WeatherType)weatherType, grade, false);
#endif
        return 0;
    }

    /**
     * Returns a table with current [Player]s on the map
     *
     *     enum TeamId
     *     {
     *         TEAM_ALLIANCE = 0,
     *         TEAM_HORDE = 1,
     *         TEAM_NEUTRAL = 2
     *     };
     *
     * @param [TeamId] team = TEAM_NEUTRAL : optional check team of the player. Neutral means either team
     * @return table players
     */
    int GetPlayers(lua_State* L)
    {
        Map* map = Eluna::CHECKOBJ<Map>(L, 1);
        uint32 team = Eluna::CHECKVAL<uint32>(L, 2, TEAM_NEUTRAL);

        Map::PlayerList const& players = map->GetPlayers();
        lua_createtable(L, team >= TEAM_NEUTRAL ? players.getSize() : players.getSize() / 3, 0);
        int tbl = lua_gettop(L);
        uint32 i = 0;

        for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
        {
#ifndef TRINITY
            Player* player = itr->getSource();
#else
            Player* player = itr->GetSource();
#endif
            if (!player || !player->GetSession())
                continue;
            if (team >= TEAM_NEUTRAL || player->GetTeamId() == team)
            {
                Eluna::Push(L, player);
                lua_rawseti(L, tbl, ++i);
            }
        }

        lua_settop(L, tbl);
        return 1;
    }
};

ElunaFunction MapMethods[] =
{
    { ENV_BOTH, "GetAreaId", &LuaMap::GetAreaId },
    { ENV_BOTH, "GetDifficulty", &LuaMap::GetDifficulty },
    { ENV_BOTH, "GetHeight", &LuaMap::GetHeight },
    { ENV_BOTH, "GetInstanceId", &LuaMap::GetInstanceId },
    { ENV_BOTH, "GetMapId", &LuaMap::GetMapId },
    { ENV_BOTH, "GetName", &LuaMap::GetName },
    { ENV_BOTH, "GetPlayerCount", &LuaMap::GetPlayerCount },
    { ENV_BOTH, "GetPlayers", &LuaMap::GetPlayers },
    { ENV_BOTH, "GetWorldObject", &LuaMap::GetWorldObject },
    { ENV_BOTH, "IsBattleground", &LuaMap::IsBattleground },
    { ENV_BOTH, "IsDungeon", &LuaMap::IsDungeon },
    { ENV_BOTH, "IsEmpty", &LuaMap::IsEmpty },
    { ENV_BOTH, "IsRaid", &LuaMap::IsRaid },
    { ENV_BOTH, "SetWeather", &LuaMap::SetWeather },
#ifndef CLASSIC
    { ENV_BOTH, "IsArena", &LuaMap::IsArena },
    { ENV_BOTH, "IsHeroic", &LuaMap::IsHeroic },
#endif                    

    { ENV_NONE, nullptr, nullptr },
};

ELUNA_TYPE(Map, false, MapMethods, "ElunaBase")

void RegisterTypeMap(Eluna* E)
{
    ElunaTemplate<Map>::RegisterTypeForState(E);
}