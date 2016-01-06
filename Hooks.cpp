/*
 * Copyright (C) 2010 - 2015 Eluna Lua Engine <http://emudevs.com/>
 * This program is free software licensed under GPL version 3
 * Please see the included DOCS/LICENSE.md for more information
 */

#include "Hooks.h"

const Hooks::HookContainer Hooks::eventsServer = {
    { "SERVER_EVENT_ON_NETWORK_START",                ENV_NONE }, // Not Implemented
    { "SERVER_EVENT_ON_NETWORK_STOP",                 ENV_NONE }, // Not Implemented
    { "SERVER_EVENT_ON_SOCKET_OPEN",                  ENV_NONE }, // Not Implemented
    { "SERVER_EVENT_ON_SOCKET_CLOSE",                 ENV_NONE }, // Not Implemented
    { "WORLD_EVENT_ON_OPEN_STATE_CHANGE",             ENV_NONE }, // (event, open) - Needs core support on Mangos
    { "WORLD_EVENT_ON_CONFIG_LOAD",                   ENV_NONE }, // (event, reload)
    { "WORLD_EVENT_ON_CHANNEL_MESSAGE",               ENV_NONE }, // (event, channel, table)
    { "WORLD_EVENT_ON_SHUTDOWN_INIT",                 ENV_NONE }, // (event, code, mask)
    { "WORLD_EVENT_ON_SHUTDOWN_CANCEL",               ENV_NONE }, // (event)
    { "WORLD_EVENT_ON_UPDATE",                        ENV_NONE }, // (event, diff)
    { "WORLD_EVENT_ON_STARTUP",                       ENV_NONE }, // (event)
    { "WORLD_EVENT_ON_SHUTDOWN",                      ENV_NONE }, // (event)
    { "ELUNA_EVENT_ON_LUA_STATE_CLOSE",               ENV_NONE }, // (event) - triggers just before shutting down eluna (on shutdown and restart)
    { "MAP_EVENT_ON_CREATE",                          ENV_NONE }, // (event, map)
    { "MAP_EVENT_ON_DESTROY",                         ENV_NONE }, // (event, map)
    { "MAP_EVENT_ON_GRID_LOAD",                       ENV_NONE }, // Not Implemented
    { "MAP_EVENT_ON_GRID_UNLOAD",                     ENV_NONE }, // Not Implemented
    { "MAP_EVENT_ON_PLAYER_ENTER",                    ENV_NONE }, // (event, map, player)
    { "MAP_EVENT_ON_PLAYER_LEAVE",                    ENV_NONE }, // (event, map, player)
    { "MAP_EVENT_ON_UPDATE",                          ENV_NONE }, // (event, map, diff)
    { "TRIGGER_EVENT_ON_TRIGGER",                     ENV_NONE }, // (event, player, triggerId) - Can return true
    { "WEATHER_EVENT_ON_CHANGE",                      ENV_NONE }, // (event, zoneId, state, grade)
    { "AUCTION_EVENT_ON_ADD",                         ENV_NONE }, // (event, auctionId, owner, item, expireTime, buyout, startBid, currentBid, bidderGUIDLow)
    { "AUCTION_EVENT_ON_REMOVE",                      ENV_NONE }, // (event, auctionId, owner, item, expireTime, buyout, startBid, currentBid, bidderGUIDLow)
    { "AUCTION_EVENT_ON_SUCCESSFUL",                  ENV_NONE }, // (event, auctionId, owner, item, expireTime, buyout, startBid, currentBid, bidderGUIDLow)
    { "AUCTION_EVENT_ON_EXPIRE",                      ENV_NONE }, // (event, auctionId, owner, item, expireTime, buyout, startBid, currentBid, bidderGUIDLow)
    { "ADDON_EVENT_ON_MESSAGE",                       ENV_NONE }, // (event, sender, type, prefix, msg, target) - target can be nil/whisper_target/guild/group/channel. Can return false
    { "WORLD_EVENT_ON_DELETE_CREATURE",               ENV_NONE }, // (event, creature)
    { "WORLD_EVENT_ON_DELETE_GAMEOBJECT",             ENV_NONE }, // (event, gameobject)
    { "ELUNA_EVENT_ON_LUA_STATE_OPEN",                ENV_NONE }, // (event) - triggers after all scripts are loaded
    { "PLAYER_EVENT_ON_CHARACTER_CREATE",             ENV_NONE }, // (event, player)
    { "PLAYER_EVENT_ON_CHARACTER_DELETE",             ENV_NONE }, // (event, guid)
    { "PLAYER_EVENT_ON_LOGIN",                        ENV_NONE }, // (event, player)
    { "PLAYER_EVENT_ON_LOGOUT",                       ENV_NONE }, // (event, player)
    { "PLAYER_EVENT_ON_SPELL_CAST",                   ENV_NONE }, // (event, player, spell, skipCheck)
    { "PLAYER_EVENT_ON_KILL_PLAYER",                  ENV_MAP },  // (event, killer, killed)
    { "PLAYER_EVENT_ON_KILL_CREATURE",                ENV_NONE }, // (event, killer, killed)
    { "PLAYER_EVENT_ON_KILLED_BY_CREATURE",           ENV_NONE }, // (event, killer, killed)
    { "PLAYER_EVENT_ON_DUEL_REQUEST",                 ENV_NONE }, // (event, target, challenger)
    { "PLAYER_EVENT_ON_DUEL_START",                   ENV_NONE }, // (event, player1, player2)
    { "PLAYER_EVENT_ON_DUEL_END",                     ENV_NONE }, // (event, winner, loser, type)
    { "PLAYER_EVENT_ON_GIVE_XP",                      ENV_NONE }, // (event, player, amount, victim) - Can return new XP amount
    { "PLAYER_EVENT_ON_LEVEL_CHANGE",                 ENV_NONE }, // (event, player, oldLevel)
    { "PLAYER_EVENT_ON_MONEY_CHANGE",                 ENV_NONE }, // (event, player, amount) - Can return new money amount
    { "PLAYER_EVENT_ON_REPUTATION_CHANGE",            ENV_NONE }, // (event, player, factionId, standing, incremental) - Can return new standing
    { "PLAYER_EVENT_ON_TALENTS_CHANGE",               ENV_NONE }, // (event, player, points)
    { "PLAYER_EVENT_ON_TALENTS_RESET",                ENV_NONE }, // (event, player, noCost)
    { "PLAYER_EVENT_ON_CHAT",                         ENV_NONE }, // (event, player, msg, Type, lang) - Can return false, newMessage
    { "PLAYER_EVENT_ON_WHISPER",                      ENV_NONE }, // (event, player, msg, Type, lang, receiver) - Can return false, newMessage
    { "PLAYER_EVENT_ON_GROUP_CHAT",                   ENV_NONE }, // (event, player, msg, Type, lang, group) - Can return false, newMessage
    { "PLAYER_EVENT_ON_GUILD_CHAT",                   ENV_NONE }, // (event, player, msg, Type, lang, guild) - Can return false, newMessage
    { "PLAYER_EVENT_ON_CHANNEL_CHAT",                 ENV_NONE }, // (event, player, msg, Type, lang, channel) - Can return false, newMessage
    { "PLAYER_EVENT_ON_EMOTE",                        ENV_NONE }, // (event, player, emote) - Not triggered on any known emote
    { "PLAYER_EVENT_ON_TEXT_EMOTE",                   ENV_NONE }, // (event, player, textEmote, emoteNum, guid)
    { "PLAYER_EVENT_ON_SAVE",                         ENV_NONE }, // (event, player)
    { "PLAYER_EVENT_ON_BIND_TO_INSTANCE",             ENV_NONE }, // (event, player, difficulty, mapid, permanent)
    { "PLAYER_EVENT_ON_UPDATE_ZONE",                  ENV_NONE }, // (event, player, newZone, newArea)
    { "PLAYER_EVENT_ON_MAP_CHANGE",                   ENV_NONE }, // (event, player)
    { "PLAYER_EVENT_ON_EQUIP",                        ENV_NONE }, // (event, player, item, bag, slot)
    { "PLAYER_EVENT_ON_FIRST_LOGIN",                  ENV_NONE }, // (event, player)
    { "PLAYER_EVENT_ON_CAN_USE_ITEM",                 ENV_NONE }, // (event, player, itemEntry) - Can return InventoryResult
    { "PLAYER_EVENT_ON_LOOT_ITEM",                    ENV_NONE }, // (event, player, item, count)
    { "PLAYER_EVENT_ON_ENTER_COMBAT",                 ENV_NONE }, // (event, player, enemy)
    { "PLAYER_EVENT_ON_LEAVE_COMBAT",                 ENV_NONE }, // (event, player)
    { "PLAYER_EVENT_ON_REPOP",                        ENV_NONE }, // (event, player)
    { "PLAYER_EVENT_ON_RESURRECT",                    ENV_NONE }, // (event, player)
    { "PLAYER_EVENT_ON_LOOT_MONEY",                   ENV_NONE }, // (event, player, amount)
    { "PLAYER_EVENT_ON_QUEST_ABANDON",                ENV_NONE }, // (event, player, questId)
    { "PLAYER_EVENT_ON_LEARN_TALENTS",                ENV_NONE }, // (event, player, talentId, talentRank, spellid)
    { "PLAYER_EVENT_ON_COMMAND",                      ENV_NONE }, // (event, player, command) - player is nil if command used from console. Can return false
    { "GUILD_EVENT_ON_ADD_MEMBER",                    ENV_NONE }, // (event, guild, player, rank)
    { "GUILD_EVENT_ON_REMOVE_MEMBER",                 ENV_NONE }, // (event, guild, player, isDisbanding)
    { "GUILD_EVENT_ON_MOTD_CHANGE",                   ENV_NONE }, // (event, guild, newMotd)
    { "GUILD_EVENT_ON_INFO_CHANGE",                   ENV_NONE }, // (event, guild, newInfo)
    { "GUILD_EVENT_ON_CREATE",                        ENV_NONE }, // (event, guild, leader, name)  // Not on TC
    { "GUILD_EVENT_ON_DISBAND",                       ENV_NONE }, // (event, guild)
    { "GUILD_EVENT_ON_MONEY_WITHDRAW",                ENV_NONE }, // (event, guild, player, amount, isRepair) - Can return new money amount
    { "GUILD_EVENT_ON_MONEY_DEPOSIT",                 ENV_NONE }, // (event, guild, player, amount) - Can return new money amount
    { "GUILD_EVENT_ON_ITEM_MOVE",                     ENV_NONE }, // (event, guild, player, item, isSrcBank, srcContainer, srcSlotId, isDestBank, destContainer, destSlotId)   // TODO
    { "GUILD_EVENT_ON_EVENT",                         ENV_NONE }, // (event, guild, eventType, plrGUIDLow1, plrGUIDLow2, newRank)  // TODO
    { "GUILD_EVENT_ON_BANK_EVENT",                    ENV_NONE }, // (event, guild, eventType, tabId, playerGUIDLow, itemOrMoney, itemStackCount, destTabId)
    { "GROUP_EVENT_ON_MEMBER_ADD",                    ENV_NONE }, // (event, group, guid)
    { "GROUP_EVENT_ON_MEMBER_INVITE",                 ENV_NONE }, // (event, group, guid)
    { "GROUP_EVENT_ON_MEMBER_REMOVE",                 ENV_NONE }, // (event, group, guid, method, kicker, reason)
    { "GROUP_EVENT_ON_LEADER_CHANGE",                 ENV_NONE }, // (event, group, newLeaderGuid, oldLeaderGuid)
    { "GROUP_EVENT_ON_DISBAND",                       ENV_NONE }, // (event, group)
    { "GROUP_EVENT_ON_CREATE",                        ENV_NONE }, // (event, group, leaderGuid, groupType)
    { "BG_EVENT_ON_START",                            ENV_NONE }, // (event, bg, bgId, instanceId) - Needs to be added to TC
    { "BG_EVENT_ON_END",                              ENV_NONE }, // (event, bg, bgId, instanceId, winner) - Needs to be added to TC
    { "BG_EVENT_ON_CREATE",                           ENV_NONE }, // (event, bg, bgId, instanceId) - Needs to be added to TC
    { "BG_EVENT_ON_PRE_DESTROY",                      ENV_NONE }, // (event, bg, bgId, instanceId) - Needs to be added to TC
    { "VEHICLE_EVENT_ON_INSTALL",                     ENV_NONE }, // (event, vehicle)
    { "VEHICLE_EVENT_ON_UNINSTALL",                   ENV_NONE }, // (event, vehicle)
    { "VEHICLE_EVENT_ON_INSTALL_ACCESSORY",           ENV_NONE }, // (event, vehicle, creature)
    { "VEHICLE_EVENT_ON_ADD_PASSENGER",               ENV_NONE }, // (event, vehicle, unit, seatId)
    { "VEHICLE_EVENT_ON_REMOVE_PASSENGER",            ENV_NONE }, // (event, vehicle, unit)
};

const Hooks::HookContainer Hooks::TypeSpecific<BINDTYPE_CREATURE>::events = {
};
const Hooks::HookContainer Hooks::TypeSpecific<BINDTYPE_GAMEOBJECT>::events = {
};
const Hooks::HookContainer Hooks::TypeSpecific<BINDTYPE_ITEM>::events = {
};
const Hooks::HookContainer Hooks::TypeSpecific<BINDTYPE_MAP>::events = {
};
const Hooks::HookContainer Hooks::TypeSpecific<BINDTYPE_PLAYERGOSSIP>::events = {
};

ElunaEnvironments Hooks::GetEventEnv(HookContainer const & hook_container, std::string const & event_id)
{
    auto it = hook_container.find(event_id);
    return it != hook_container.end() ? it->second : ENV_NONE;
}
