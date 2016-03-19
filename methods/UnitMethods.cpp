/*
* Copyright (C) 2010 - 2016 Eluna Lua Engine <http://emudevs.com/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#include "LuaEngine.h"
#include "ElunaTemplate.h"

#include "ElunaIncludes.h"

/***
 * Inherits all methods from: [ElunaBase], [Object], [WorldObject]
 */
namespace LuaUnit
{
    /**
     * The [Unit] tries to attack a given target
     *
     * @param [Unit] who : [Unit] to attack
     * @param bool meleeAttack = false: attack with melee or not
     * @return didAttack : if the [Unit] did not attack
     */
    int Attack(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        Unit* who = Eluna::CHECKOBJ<Unit>(L, 2);
        bool meleeAttack = Eluna::CHECKVAL<bool>(L, 3, false);

        Eluna::Push(L, unit->Attack(who, meleeAttack));
        return 1;
    }

    /**
     * The [Unit] stops attacking its target
     *
     * @return bool isAttacking : if the [Unit] wasn't attacking already
     */
    int AttackStop(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        Eluna::Push(L, unit->AttackStop());
        return 1;
    }

    /**
     * Returns true if the [Unit] is standing.
     *
     * @return bool isStanding
     */
    int IsStandState(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        Eluna::Push(L, unit->IsStandState());
        return 1;
    }

    /**
     * Returns true if the [Unit] is mounted.
     *
     * @return bool isMounted
     */
    int IsMounted(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        Eluna::Push(L, unit->IsMounted());
        return 1;
    }

    /**
     * Returns true if the [Unit] is rooted.
     *
     * @return bool isRooted
     */
    int IsRooted(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
#ifdef TRINITY
        Eluna::Push(L, unit->isInRoots() || unit->HasUnitMovementFlag(MOVEMENTFLAG_ROOT));
#endif
#ifdef CMANGOS
        Eluna::Push(L, unit->isInRoots() || unit->IsRooted());
#endif
#ifdef MANGOS
        Eluna::Push(L, unit->IsInRoots() || unit->IsRooted());
#endif
        return 1;
    }

    /**
     * Returns true if the [Unit] has full health.
     *
     * @return bool hasFullHealth
     */
    int IsFullHealth(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        Eluna::Push(L, unit->IsFullHealth());
        return 1;
    }

    /**
     * Returns true if the [WorldObject] is within given radius of the [Unit].
     *
     * @param [WorldObject] obj
     * @param float radius
     * @return bool withinDist
     */
    int IsWithinDistInMap(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        WorldObject* obj = Eluna::CHECKOBJ<WorldObject>(L, 2);
        float radius = Eluna::CHECKVAL<float>(L, 3);

        Eluna::Push(L, unit->IsWithinDistInMap(obj, radius));
        return 1;
    }

    /**
     * Returns true if the [Unit] is in an accessible place for the given [Creature].
     *
     * @param [WorldObject] obj
     * @param float radius
     * @return bool isAccessible
     */
    int IsInAccessiblePlaceFor(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        Creature* creature = Eluna::CHECKOBJ<Creature>(L, 2);

#ifndef TRINITY
        Eluna::Push(L, unit->isInAccessablePlaceFor(creature));
#else
        Eluna::Push(L, unit->isInAccessiblePlaceFor(creature));
#endif
        return 1;
    }

    /**
     * Returns true if the [Unit] an auctioneer.
     *
     * @return bool isAuctioneer
     */
    int IsAuctioneer(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
#ifndef TRINITY
        Eluna::Push(L, unit->isAuctioner());
#else
        Eluna::Push(L, unit->IsAuctioner());
#endif
        return 1;
    }

    /**
     * Returns true if the [Unit] a guild master.
     *
     * @return bool isGuildMaster
     */
    int IsGuildMaster(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
#ifdef CMANGOS
        Eluna::Push(L, unit->isGuildMaster());
#else
        Eluna::Push(L, unit->IsGuildMaster());
#endif
        return 1;
    }

    /**
     * Returns true if the [Unit] an innkeeper.
     *
     * @return bool isInnkeeper
     */
    int IsInnkeeper(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
#ifdef CMANGOS
        Eluna::Push(L, unit->isInnkeeper());
#else
        Eluna::Push(L, unit->IsInnkeeper());
#endif
        return 1;
    }

    /**
     * Returns true if the [Unit] a trainer.
     *
     * @return bool isTrainer
     */
    int IsTrainer(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
#ifdef CMANGOS
        Eluna::Push(L, unit->isTrainer());
#else
        Eluna::Push(L, unit->IsTrainer());
#endif
        return 1;
    }

    /**
     * Returns true if the [Unit] is able to show a gossip window.
     *
     * @return bool hasGossip
     */
    int IsGossip(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
#ifdef CMANGOS
        Eluna::Push(L, unit->isGossip());
#else
        Eluna::Push(L, unit->IsGossip());
#endif
        return 1;
    }

    /**
     * Returns true if the [Unit] is a taxi master.
     *
     * @return bool isTaxi
     */
    int IsTaxi(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
#ifdef CMANGOS
        Eluna::Push(L, unit->isTaxi());
#else
        Eluna::Push(L, unit->IsTaxi());
#endif
        return 1;
    }

    /**
     * Returns true if the [Unit] is a spirit healer.
     *
     * @return bool isSpiritHealer
     */
    int IsSpiritHealer(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
#ifdef CMANGOS
        Eluna::Push(L, unit->isSpiritHealer());
#else
        Eluna::Push(L, unit->IsSpiritHealer());
#endif
        return 1;
    }

    /**
     * Returns true if the [Unit] is a spirit guide.
     *
     * @return bool isSpiritGuide
     */
    int IsSpiritGuide(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
#ifdef CMANGOS
        Eluna::Push(L, unit->isSpiritGuide());
#else
        Eluna::Push(L, unit->IsSpiritGuide());
#endif
        return 1;
    }

    /**
     * Returns true if the [Unit] is a tabard designer.
     *
     * @return bool isTabardDesigner
     */
    int IsTabardDesigner(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
#ifdef CMANGOS
        Eluna::Push(L, unit->isTabardDesigner());
#else
        Eluna::Push(L, unit->IsTabardDesigner());
#endif
        return 1;
    }

    /**
     * Returns true if the [Unit] provides services like vendor, training and auction.
     *
     * @return bool isTabardDesigner
     */
    int IsServiceProvider(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
#ifdef CMANGOS
        Eluna::Push(L, unit->isServiceProvider());
#else
        Eluna::Push(L, unit->IsServiceProvider());
#endif
        return 1;
    }

    /**
     * Returns true if the [Unit] is a spirit guide or spirit healer.
     *
     * @return bool isSpiritService
     */
    int IsSpiritService(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
#ifdef CMANGOS
        Eluna::Push(L, unit->isSpiritService());
#else
        Eluna::Push(L, unit->IsSpiritService());
#endif
        return 1;
    }

    /**
     * Returns true if the [Unit] is alive.
     *
     * @return bool isAlive
     */
    int IsAlive(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
#ifdef CMANGOS
        Eluna::Push(L, unit->isAlive());
#else
        Eluna::Push(L, unit->IsAlive());
#endif
        return 1;
    }

    /**
     * Returns true if the [Unit] is dead.
     *
     * @return bool isDead
     */
    int IsDead(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
#ifdef MANGOS
        Eluna::Push(L, unit->IsDead());
#else
        Eluna::Push(L, unit->isDead());
#endif
        return 1;
    }

    /**
     * Returns true if the [Unit] is dying.
     *
     * @return bool isDying
     */
    int IsDying(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
#ifdef MANGOS
        Eluna::Push(L, unit->IsDying());
#else
        Eluna::Push(L, unit->isDying());
#endif
        return 1;
    }

    /**
     * Returns true if the [Unit] is a banker.
     *
     * @return bool isBanker
     */
    int IsBanker(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
#ifdef CMANGOS
        Eluna::Push(L, unit->isBanker());
#else
        Eluna::Push(L, unit->IsBanker());
#endif
        return 1;
    }

    /**
     * Returns true if the [Unit] is a vendor.
     *
     * @return bool isVendor
     */
    int IsVendor(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
#ifdef CMANGOS
        Eluna::Push(L, unit->isVendor());
#else
        Eluna::Push(L, unit->IsVendor());
#endif
        return 1;
    }

    /**
     * Returns true if the [Unit] is a battle master.
     *
     * @return bool isBattleMaster
     */
    int IsBattleMaster(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
#ifdef CMANGOS
        Eluna::Push(L, unit->isBattleMaster());
#else
        Eluna::Push(L, unit->IsBattleMaster());
#endif
        return 1;
    }

    /**
     * Returns true if the [Unit] is a charmed.
     *
     * @return bool isCharmed
     */
    int IsCharmed(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
#ifdef CMANGOS
        Eluna::Push(L, unit->isCharmed());
#else
        Eluna::Push(L, unit->IsCharmed());
#endif
        return 1;
    }

    /**
     * Returns true if the [Unit] is an armorer and can repair equipment.
     *
     * @return bool isArmorer
     */
    int IsArmorer(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
#ifdef CMANGOS
        Eluna::Push(L, unit->isArmorer());
#else
        Eluna::Push(L, unit->IsArmorer());
#endif
        return 1;
    }

    /**
     * Returns true if the [Unit] is attacking a player.
     *
     * @return bool isAttackingPlayer
     */
    int IsAttackingPlayer(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        Eluna::Push(L, unit->isAttackingPlayer());
        return 1;
    }

    /**
     * Returns true if the [Unit] flagged for PvP.
     *
     * @return bool isPvP
     */
    int IsPvPFlagged(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        Eluna::Push(L, unit->IsPvP());
        return 1;
    }

#ifndef CLASSIC
    /**
     * Returns true if the [Unit] is on a [Vehicle].
     *
     * @return bool isOnVehicle
     */
    int IsOnVehicle(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
#ifndef TRINITY
        Eluna::Push(L, unit->IsBoarded());
#else
        Eluna::Push(L, unit->GetVehicle());
#endif
        return 1;
    }
#endif
    
    /**
     * Returns true if the [Unit] is in combat.
     *
     * @return bool inCombat
     */
    int IsInCombat(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
#ifdef CMANGOS
        Eluna::Push(L, unit->isInCombat());
#else
        Eluna::Push(L, unit->IsInCombat());
#endif
        return 1;
    }
    
    /**
     * Returns true if the [Unit] is under water.
     *
     * @return bool underWater
     */
    int IsUnderWater(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        Eluna::Push(L, unit->IsUnderWater());
        return 1;
    }
    
    /**
     * Returns true if the [Unit] is in water.
     *
     * @return bool inWater
     */
    int IsInWater(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        Eluna::Push(L, unit->IsInWater());
        return 1;
    }
    
    /**
     * Returns true if the [Unit] is not moving.
     *
     * @return bool notMoving
     */
    int IsStopped(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        Eluna::Push(L, unit->IsStopped());
        return 1;
    }
    
    /**
     * Returns true if the [Unit] is a quest giver.
     *
     * @return bool questGiver
     */
    int IsQuestGiver(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
#ifdef CMANGOS
        Eluna::Push(L, unit->isQuestGiver());
#else
        Eluna::Push(L, unit->IsQuestGiver());
#endif
        return 1;
    }
    
    /**
     * Returns true if the [Unit]'s health is below the given percentage.
     *
     * @param int32 healthpct : percentage in integer from
     * @return bool isBelow
     */
    int HealthBelowPct(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        Eluna::Push(L, unit->HealthBelowPct(Eluna::CHECKVAL<int32>(L, 2)));
        return 1;
    }
    
    /**
     * Returns true if the [Unit]'s health is above the given percentage.
     *
     * @param int32 healthpct : percentage in integer from
     * @return bool isAbove
     */
    int HealthAbovePct(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        Eluna::Push(L, unit->HealthAbovePct(Eluna::CHECKVAL<int32>(L, 2)));
        return 1;
    }
    
    /**
     * Returns true if the [Unit] has an aura from the given spell entry.
     *
     * @param uint32 spell : entry of the aura spell
     * @return bool hasAura
     */
    int HasAura(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        uint32 spell = Eluna::CHECKVAL<uint32>(L, 2);

        Eluna::Push(L, unit->HasAura(spell));
        return 1;
    }

    /**
     * Returns true if the [Unit] is casting a spell
     *
     * @return bool isCasting
     */
    int IsCasting(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
#ifdef TRINITY
        Eluna::Push(L, unit->HasUnitState(UNIT_STATE_CASTING));
#else
        Eluna::Push(L, unit->IsNonMeleeSpellCasted(false));
#endif
        return 1;
    }

    /**
     * Returns true if the [Unit] has the given unit state.
     *
     * @param [UnitState] state : an unit state
     * @return bool hasState
     */
    int HasUnitState(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        uint32 state = Eluna::CHECKVAL<uint32>(L, 2);
#ifndef TRINITY
        Eluna::Push(L, unit->hasUnitState(state));
#else
        Eluna::Push(L, unit->HasUnitState(state));
#endif
        return 1;
    }

    /*int IsVisible(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        Eluna::Push(L, unit->IsVisible());
        return 1;
    }*/

    /*int IsMoving(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        Eluna::Push(L, unit->isMoving());
        return 1;
    }*/

    /*int IsFlying(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        Eluna::Push(L, unit->IsFlying());
        return 1;
    }*/
    
    /**
     * Returns the [Unit]'s owner.
     *
     * @return [Unit] owner
     */
    int GetOwner(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        Eluna::Push(L, unit->GetOwner());
        return 1;
    }
    
    /**
     * Returns the [Unit]'s owner's GUID.
     *
     * @return uint64 ownerGUID
     */
    int GetOwnerGUID(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
#ifndef TRINITY
        Eluna::Push(L, unit->GetOwnerGuid());
#else
        Eluna::Push(L, unit->GetOwnerGUID());
#endif
        return 1;
    }
    
    /**
     * Returns the [Unit]'s mount's modelID.
     *
     * @return uint32 mountId : displayId of the mount
     */
    int GetMountId(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        Eluna::Push(L, unit->GetMountID());
        return 1;
    }
    
    /**
     * Returns the [Unit]'s creator's GUID.
     *
     * @return uint64 creatorGUID
     */
    int GetCreatorGUID(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
#ifndef TRINITY
        Eluna::Push(L, unit->GetCreatorGuid());
#else
        Eluna::Push(L, unit->GetCreatorGUID());
#endif
        return 1;
    }
    
    /**
     * Returns the [Unit]'s charmer's GUID.
     *
     * @return uint64 charmerGUID
     */
    int GetCharmerGUID(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
#ifndef TRINITY
        Eluna::Push(L, unit->GetCharmerGuid());
#else
        Eluna::Push(L, unit->GetCharmerGUID());
#endif
        return 1;
    }
    
    /**
     * Returns the GUID of the [Unit]'s charmed entity.
     *
     * @return uint64 charmedGUID
     */
    int GetCharmGUID(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
#ifndef TRINITY
        Eluna::Push(L, unit->GetCharmGuid());
#else
        Eluna::Push(L, unit->GetCharmGUID());
#endif
        return 1;
    }
    
    /**
     * Returns the GUID of the [Unit]'s pet.
     *
     * @return uint64 petGUID
     */
    int GetPetGUID(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
#ifndef TRINITY
        Eluna::Push(L, unit->GetPetGuid());
#else
        Eluna::Push(L, unit->GetPetGUID());
#endif
        return 1;
    }
    
    /**
     * Returns the GUID of the [Unit]'s charmer or owner.
     *
     * @return uint64 controllerGUID
     */
    int GetControllerGUID(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
#ifndef TRINITY
        Eluna::Push(L, unit->GetCharmerOrOwnerGuid());
#else
        Eluna::Push(L, unit->GetCharmerOrOwnerGUID());
#endif
        return 1;
    }
    
    /**
     * Returns the GUID of the [Unit]'s charmer or owner or its own GUID.
     *
     * @return uint64 controllerGUID
     */
    int GetControllerGUIDS(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
#ifndef TRINITY
        Eluna::Push(L, unit->GetCharmerOrOwnerOrOwnGuid());
#else
        Eluna::Push(L, unit->GetCharmerOrOwnerOrOwnGUID());
#endif
        return 1;
    }

    /**
     * Returns [Unit]'s specified stat
     *
     * @param uint32 statType
     * @return float stat
     */
    int GetStat(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        uint32 stat = Eluna::CHECKVAL<uint32>(L, 2);

        if (stat >= MAX_STATS)
            return 1;

        Eluna::Push(L, unit->GetStat((Stats)stat));
        return 1;
    }

    /**
     * Returns the [Unit]'s base spell power
     *
     * @param uint32 spellSchool
     * @return uint32 spellPower
     */
    int GetBaseSpellPower(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        uint32 spellschool = Eluna::CHECKVAL<uint32>(L, 2);

        if (spellschool >= MAX_SPELL_SCHOOL)
            return 1;

        Eluna::Push(L, unit->GetUInt32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_POS + spellschool));
        return 1;
    }
    
    /**
     * Returns the [Unit]'s current victim target or nil.
     *
     * @return [Unit] victim
     */
    int GetVictim(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
#ifndef TRINITY
        Eluna::Push(L, unit->getVictim());
#else
        Eluna::Push(L, unit->GetVictim());
#endif
        return 1;
    }

    /**
     * Returns the currently casted [Spell] of given type or nil.
     *
     * <pre>
     * enum CurrentSpellTypes
     * {
     *     CURRENT_MELEE_SPELL             = 0,
     *     CURRENT_GENERIC_SPELL           = 1,
     *     CURRENT_CHANNELED_SPELL         = 2,
     *     CURRENT_AUTOREPEAT_SPELL        = 3
     * };
     * </pre>
     *
     * @param [CurrentSpellTypes] spellType
     * @return [Spell] castedSpell
     */
    int GetCurrentSpell(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        uint32 type = Eluna::CHECKVAL<uint32>(L, 2);
        if (type >= CURRENT_MAX_SPELL)
            return luaL_argerror(L, 2, "valid CurrentSpellTypes expected");

        Eluna::Push(L, unit->GetCurrentSpell(type));
        return 1;
    }
    
    /**
     * Returns the [Unit]'s current stand state.
     *
     * @return uint8 standState
     */
    int GetStandState(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        Eluna::Push(L, unit->getStandState());
        return 1;
    }
    
    /**
     * Returns the [Unit]'s current display ID.
     *
     * @return uint32 displayId
     */
    int GetDisplayId(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        Eluna::Push(L, unit->GetDisplayId());
        return 1;
    }
    
    /**
     * Returns the [Unit]'s native/original display ID.
     *
     * @return uint32 displayId
     */
    int GetNativeDisplayId(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        Eluna::Push(L, unit->GetNativeDisplayId());
        return 1;
    }
    
    /**
     * Returns the [Unit]'s level.
     *
     * @return uint8 level
     */
    int GetLevel(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        Eluna::Push(L, unit->getLevel());
        return 1;
    }
    
    /**
     * Returns the [Unit]'s health amount.
     *
     * @return uint32 healthAmount
     */
    int GetHealth(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        Eluna::Push(L, unit->GetHealth());
        return 1;
    }

    Powers PowerSelectorHelper(lua_State* L, Unit* unit, int powerType = -1)
    {
#ifdef TRINITY
        if (powerType == -1)
            return unit->getPowerType();
#else
        if (powerType == -1)
            return unit->GetPowerType();
#endif

        if (powerType < 0 || powerType >= int(MAX_POWERS))
            luaL_argerror(L, 2, "valid Powers expected");

        return (Powers)powerType;
    }

    /**
     * Returns the [Unit]'s power amount for given power type.
     *
     *     enum Powers
     *     {
     *         POWER_MANA        = 0,
     *         POWER_RAGE        = 1,
     *         POWER_FOCUS       = 2,
     *         POWER_ENERGY      = 3,
     *         POWER_HAPPINESS   = 4,
     *         POWER_RUNE        = 5,
     *         POWER_RUNIC_POWER = 6,
     *         MAX_POWERS        = 7,
     *         POWER_ALL         = 127,         // default for class?
     *         POWER_HEALTH      = 0xFFFFFFFE   // (-2 as signed value)
     *     };
     *
     * @param int type = -1 : a valid power type from [Powers] or -1 for the [Unit]'s current power type
     * @return uint32 powerAmount
     */
    int GetPower(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        int type = Eluna::CHECKVAL<int>(L, 2, -1);
        Powers power = PowerSelectorHelper(L, unit, type);

        Eluna::Push(L, unit->GetPower(power));
        return 1;
    }

    /**
     * Returns the [Unit]'s max power amount for given power type.
     *
     *     enum Powers
     *     {
     *         POWER_MANA        = 0,
     *         POWER_RAGE        = 1,
     *         POWER_FOCUS       = 2,
     *         POWER_ENERGY      = 3,
     *         POWER_HAPPINESS   = 4,
     *         POWER_RUNE        = 5,
     *         POWER_RUNIC_POWER = 6,
     *         MAX_POWERS        = 7,
     *         POWER_ALL         = 127,         // default for class?
     *         POWER_HEALTH      = 0xFFFFFFFE   // (-2 as signed value)
     *     };
     *
     * @param int type = -1 : a valid power type from [Powers] or -1 for the [Unit]'s current power type
     * @return uint32 maxPowerAmount
     */
    int GetMaxPower(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        int type = Eluna::CHECKVAL<int>(L, 2, -1);
        Powers power = PowerSelectorHelper(L, unit, type);

        Eluna::Push(L, unit->GetMaxPower(power));
        return 1;
    }

    /**
     * Returns the [Unit]'s power percent for given power type.
     *
     *     enum Powers
     *     {
     *         POWER_MANA        = 0,
     *         POWER_RAGE        = 1,
     *         POWER_FOCUS       = 2,
     *         POWER_ENERGY      = 3,
     *         POWER_HAPPINESS   = 4,
     *         POWER_RUNE        = 5,
     *         POWER_RUNIC_POWER = 6,
     *         MAX_POWERS        = 7,
     *         POWER_ALL         = 127,         // default for class?
     *         POWER_HEALTH      = 0xFFFFFFFE   // (-2 as signed value)
     *     };
     *
     * @param int type = -1 : a valid power type from [Powers] or -1 for the [Unit]'s current power type
     * @return float powerPct
     */
    int GetPowerPct(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        int type = Eluna::CHECKVAL<int>(L, 2, -1);
        Powers power = PowerSelectorHelper(L, unit, type);

#if (!defined(TRINITY) && defined(WOTLK))
        float percent = ((float)unit->GetPower(power) / (float)unit->GetMaxPower(power)) * 100.0f;
#else
        float percent = ((float)unit->GetPower(power) / (float)unit->GetMaxPower(power)) * 100.0f;
#endif
        Eluna::Push(L, percent);
        return 1;
    }

    /**
     * Returns the [Unit]'s current power type.
     *
     *     enum Powers
     *     {
     *         POWER_MANA        = 0,
     *         POWER_RAGE        = 1,
     *         POWER_FOCUS       = 2,
     *         POWER_ENERGY      = 3,
     *         POWER_HAPPINESS   = 4,
     *         POWER_RUNE        = 5,
     *         POWER_RUNIC_POWER = 6,
     *         MAX_POWERS        = 7,
     *         POWER_ALL         = 127,         // default for class?
     *         POWER_HEALTH      = 0xFFFFFFFE   // (-2 as signed value)
     *     };
     *
     * @return [Powers] powerType
     */
    int GetPowerType(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
#ifdef TRINITY
        Eluna::Push(L, unit->getPowerType());
#else
        Eluna::Push(L, unit->GetPowerType());
#endif
        return 1;
    }

    /**
     * Returns the [Unit]'s max health.
     *
     * @return uint32 maxHealth
     */
    int GetMaxHealth(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        Eluna::Push(L, unit->GetMaxHealth());
        return 1;
    }

    /**
     * Returns the [Unit]'s health percent.
     *
     * @return float healthPct
     */
    int GetHealthPct(lua_State* L) 
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
#ifndef TRINITY
        Eluna::Push(L, unit->GetHealthPercent());
#else
        Eluna::Push(L, unit->GetHealthPct());
#endif
        return 1;
    }

    /**
     * Returns the [Unit]'s gender.
     *
     * @return uint8 gender : 0 for male, 1 for female and 2 for none
     */
    int GetGender(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        Eluna::Push(L, unit->getGender());
        return 1;
    }

    /**
     * Returns the [Unit]'s race ID.
     *
     * @return [Races] race
     */
    int GetRace(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        Eluna::Push(L, unit->getRace());
        return 1;
    }

    /**
     * Returns the [Unit]'s class ID.
     *
     * @return [Classes] class
     */
    int GetClass(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        Eluna::Push(L, unit->getClass());
        return 1;
    }

    /**
     * Returns the [Unit]'s creature type ID like wolf or humanoid.
     *
     * @return uint32 creatureType
     */
    int GetCreatureType(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        Eluna::Push(L, unit->GetCreatureType());
        return 1;
    }

    /**
     * Returns the [Unit]'s class' name in given or default locale or nil.
     *
     * <pre>
     * enum LocaleConstant
     * {
     *     LOCALE_enUS = 0,
     *     LOCALE_koKR = 1,
     *     LOCALE_frFR = 2,
     *     LOCALE_deDE = 3,
     *     LOCALE_zhCN = 4,
     *     LOCALE_zhTW = 5,
     *     LOCALE_esES = 6,
     *     LOCALE_esMX = 7,
     *     LOCALE_ruRU = 8
     * };
     * </pre>
     *
     * @param [LocaleConstant] locale = DEFAULT_LOCALE
     * @return string className : class name or nil
     */
    int GetClassAsString(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        uint8 locale = Eluna::CHECKVAL<uint8>(L, 2, DEFAULT_LOCALE);
        if (locale >= TOTAL_LOCALES)
            return luaL_argerror(L, 2, "valid LocaleConstant expected");

        const ChrClassesEntry* entry = sChrClassesStore.LookupEntry(unit->getClass());
        if (!entry)
            return 1;

        Eluna::Push(L, entry->name[locale]);
        return 1;
    }

    /**
     * Returns the [Unit]'s race's name in given or default locale or nil.
     *
     * <pre>
     * enum LocaleConstant
     * {
     *     LOCALE_enUS = 0,
     *     LOCALE_koKR = 1,
     *     LOCALE_frFR = 2,
     *     LOCALE_deDE = 3,
     *     LOCALE_zhCN = 4,
     *     LOCALE_zhTW = 5,
     *     LOCALE_esES = 6,
     *     LOCALE_esMX = 7,
     *     LOCALE_ruRU = 8
     * };
     * </pre>
     *
     * @param [LocaleConstant] locale = DEFAULT_LOCALE : locale to return the race name in
     * @return string raceName : race name or nil
     */
    int GetRaceAsString(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        uint8 locale = Eluna::CHECKVAL<uint8>(L, 2, DEFAULT_LOCALE);
        if (locale >= TOTAL_LOCALES)
            return luaL_argerror(L, 2, "valid LocaleConstant expected");

        const ChrRacesEntry* entry = sChrRacesStore.LookupEntry(unit->getRace());
        if (!entry)
            return 1;

        Eluna::Push(L, entry->name[locale]);
        return 1;
    }

    /**
     * Returns the [Unit]'s faction ID.
     *
     * @return uint32 faction
     */
    int GetFaction(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        Eluna::Push(L, unit->getFaction());
        return 1;
    }

    /**
     * Returns the [Aura] of the given spell entry on the [Unit] or nil.
     *
     * @param uint32 spellID : entry of the aura spell
     * @return [Aura] aura : aura object or nil
     */
    int GetAura(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        uint32 spellID = Eluna::CHECKVAL<uint32>(L, 2);
#ifndef TRINITY
        Eluna::Push(L, unit->GetAura(spellID, EFFECT_INDEX_0));
#else
        Eluna::Push(L, unit->GetAura(spellID));
#endif
        return 1;
    }

    /**
     * Returns the [Unit]'s combat timer
     *
     * @return uint32 combatTimer
     */
    int GetCombatTime(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        Eluna::Push(L, unit->GetCombatTimer());
        return 1;
    }

    /**
     * Returns a table containing friendly [Unit]'s within given range of the [Unit].
     *
     * @param float range = 533.333 : search radius
     * @return table friendyUnits : table filled with friendly units
     */
    int GetFriendlyUnitsInRange(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        float range = Eluna::CHECKVAL<float>(L, 2, SIZE_OF_GRIDS);

        std::list<Unit*> list;
#ifndef TRINITY
        MaNGOS::AnyFriendlyUnitInObjectRangeCheck checker(unit, range);
        MaNGOS::UnitListSearcher<MaNGOS::AnyFriendlyUnitInObjectRangeCheck> searcher(list, checker);
        Cell::VisitGridObjects(unit, searcher, range);
#else
        Trinity::AnyFriendlyUnitInObjectRangeCheck checker(unit, unit, range);
        Trinity::UnitListSearcher<Trinity::AnyFriendlyUnitInObjectRangeCheck> searcher(unit, list, checker);
        unit->VisitNearbyObject(range, searcher);
#endif
        ElunaUtil::ObjectGUIDCheck guidCheck(unit->GET_GUID());
        list.remove_if(guidCheck);

        lua_newtable(L);
        int tbl = lua_gettop(L);
        uint32 i = 0;

        for (std::list<Unit*>::const_iterator it = list.begin(); it != list.end(); ++it)
        {
            Eluna::Push(L, *it);
            lua_rawseti(L, tbl, ++i);
        }

        lua_settop(L, tbl);
        return 1;
    }

    /**
     * Returns a table containing unfriendly [Unit]'s within given range of the [Unit].
     *
     * @param float range = 533.333 : search radius
     * @return table unfriendyUnits : table filled with unfriendly units
     */
    int GetUnfriendlyUnitsInRange(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        float range = Eluna::CHECKVAL<float>(L, 2, SIZE_OF_GRIDS);

        std::list<Unit*> list;
#ifndef TRINITY
        MaNGOS::AnyUnfriendlyUnitInObjectRangeCheck checker(unit, range);
        MaNGOS::UnitListSearcher<MaNGOS::AnyUnfriendlyUnitInObjectRangeCheck> searcher(list, checker);
        Cell::VisitGridObjects(unit, searcher, range);
#else
        Trinity::AnyUnfriendlyUnitInObjectRangeCheck checker(unit, unit, range);
        Trinity::UnitListSearcher<Trinity::AnyUnfriendlyUnitInObjectRangeCheck> searcher(unit, list, checker);
        unit->VisitNearbyObject(range, searcher);
#endif
        ElunaUtil::ObjectGUIDCheck guidCheck(unit->GET_GUID());
        list.remove_if(guidCheck);

        lua_createtable(L, list.size(), 0);
        int tbl = lua_gettop(L);
        uint32 i = 0;

        for (std::list<Unit*>::const_iterator it = list.begin(); it != list.end(); ++it)
        {
            Eluna::Push(L, *it);
            lua_rawseti(L, tbl, ++i);
        }

        lua_settop(L, tbl);
        return 1;
    }

#if (!defined(TBC) && !defined(CLASSIC))
    /**
     * Returns [Unit]'s [Vehicle] methods
     *
     * @return [Vehicle] vehicle
     */
    int GetVehicleKit(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
#ifndef TRINITY
        Eluna::Push(L, unit->GetVehicleInfo());
#else
        Eluna::Push(L, unit->GetVehicleKit());
#endif
        return 1;
    }

    /*
    int GetVehicle(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
    Eluna::Push(L, unit->GetVehicle());
    return 1;
    }
    */

    /**
     * Returns the Critter Guid
     *
     * @return uint64 critterGuid
     */
    int GetCritterGUID(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
#ifndef TRINITY
        Eluna::Push(L, unit->GetCritterGuid());
#else
        Eluna::Push(L, unit->GetCritterGUID());
#endif
        return 1;
    }
#endif

    /**
     * Returns the [Unit]'s speed of given [UnitMoveType].
     *
     * <pre>
     * enum UnitMoveType
     * {
     *     MOVE_WALK           = 0,
     *     MOVE_RUN            = 1,
     *     MOVE_RUN_BACK       = 2,
     *     MOVE_SWIM           = 3,
     *     MOVE_SWIM_BACK      = 4,
     *     MOVE_TURN_RATE      = 5,
     *     MOVE_FLIGHT         = 6,
     *     MOVE_FLIGHT_BACK    = 7,
     *     MOVE_PITCH_RATE     = 8
     * };
     * </pre>
     *
     * @param [UnitMoveType] type
     * @return float speed
     */
    int GetSpeed(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        uint32 type = Eluna::CHECKVAL<uint32>(L, 2);
        if (type >= MAX_MOVE_TYPE)
            return luaL_argerror(L, 2, "valid UnitMoveType expected");

#ifndef TRINITY
        Eluna::Push(L, unit->GetSpeedRate((UnitMoveType)type));
#else
        Eluna::Push(L, unit->GetSpeed((UnitMoveType)type));
#endif
        return 1;
    }

    /**
     * Returns the current movement type for this [Unit].
     *
     * <pre>
     * enum MovementGeneratorType
     * {
     *     IDLE_MOTION_TYPE                = 0,
     *     RANDOM_MOTION_TYPE              = 1,
     *     WAYPOINT_MOTION_TYPE            = 2,
     *     MAX_DB_MOTION_TYPE              = 3,
     *     ANIMAL_RANDOM_MOTION_TYPE       = 3, // TC
     * 
     *     CONFUSED_MOTION_TYPE            = 4,
     *     CHASE_MOTION_TYPE               = 5,
     *     HOME_MOTION_TYPE                = 6,
     *     FLIGHT_MOTION_TYPE              = 7,
     *     POINT_MOTION_TYPE               = 8,
     *     FLEEING_MOTION_TYPE             = 9,
     *     DISTRACT_MOTION_TYPE            = 10,
     *     ASSISTANCE_MOTION_TYPE          = 11,
     *     ASSISTANCE_DISTRACT_MOTION_TYPE = 12,          
     *     TIMED_FLEEING_MOTION_TYPE       = 13,
     *     FOLLOW_MOTION_TYPE              = 14,
     *     EFFECT_MOTION_TYPE              = 15, // mangos
     *     ROTATE_MOTION_TYPE              = 15, // TC
     *     EFFECT_MOTION_TYPE              = 16, // TC
     *     NULL_MOTION_TYPE                = 17, // TC
     * };
     * </pre>
     *
     * @return [MovementGeneratorType] movementType
     */
    int GetMovementType(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        Eluna::Push(L, unit->GetMotionMaster()->GetCurrentMovementGeneratorType());
        return 1;
    }

    /**
     * Sets the [Unit]'s owner GUID to given GUID.
     *
     * @param uint64 guid : new owner guid
     */
    int SetOwnerGUID(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        uint64 guid = Eluna::CHECKVAL<uint64>(L, 2);

#ifndef TRINITY
        unit->SetOwnerGuid(ObjectGuid(guid));
#else
        unit->SetOwnerGUID(ObjectGuid(guid));
#endif
        return 0;
    }

    /**
     * Sets the [Unit]'s PvP on or off.
     *
     * @param bool apply = true : true if set on, false if off
     */
    int SetPvP(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        bool apply = Eluna::CHECKVAL<bool>(L, 2, true);

        unit->SetPvP(apply);
        return 0;
    }

    /**
     * Sets the [Unit]'s sheath state.
     *
     *     enum SheathState
     *     {
     *         SHEATH_STATE_UNARMED  = 0, // non prepared weapon
     *         SHEATH_STATE_MELEE    = 1, // prepared melee weapon
     *         SHEATH_STATE_RANGED   = 2  // prepared ranged weapon
     *     };
     *
     * @param [SheathState] sheathState : valid SheathState
     */
    int SetSheath(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        uint32 sheathed = Eluna::CHECKVAL<uint32>(L, 2);
        if (sheathed >= MAX_SHEATH_STATE)
            return luaL_argerror(L, 2, "valid SheathState expected");

        unit->SetSheath((SheathState)sheathed);
        return 0;
    }

    /**
     * Sets the [Unit]'s name internally.
     *
     * @param string name : new name
     */
    int SetName(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        const char* name = Eluna::CHECKVAL<const char*>(L, 2);
        if (std::string(name).length() > 0)
            unit->SetName(name);
        return 0;
    }

    /**
     * Sets the [Unit]'s speed of given [UnitMoveType] to given rate.
     * If forced, packets sent to clients forcing the visual change.
     *
     * <pre>
     * enum UnitMoveType
     * {
     *     MOVE_WALK           = 0,
     *     MOVE_RUN            = 1,
     *     MOVE_RUN_BACK       = 2,
     *     MOVE_SWIM           = 3,
     *     MOVE_SWIM_BACK      = 4,
     *     MOVE_TURN_RATE      = 5,
     *     MOVE_FLIGHT         = 6,
     *     MOVE_FLIGHT_BACK    = 7,
     *     MOVE_PITCH_RATE     = 8
     * };
     * </pre>
     *
     * @param [UnitMoveType] type
     * @param float rate
     * @param bool forced = false
     */
    int SetSpeed(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        uint32 type = Eluna::CHECKVAL<uint32>(L, 2);
        float rate = Eluna::CHECKVAL<float>(L, 3);
        bool forced = Eluna::CHECKVAL<bool>(L, 4, false);
        if (type >= MAX_MOVE_TYPE)
            return luaL_argerror(L, 2, "valid UnitMoveType expected");
#ifndef TRINITY
        unit->SetSpeedRate((UnitMoveType)type, rate, forced);
#else
        unit->SetSpeed((UnitMoveType)type, rate, forced);
#endif
        return 0;
    }

    /**
     * Sets the [Unit]'s faction.
     *
     * @param uint32 faction : new faction ID
     */
    int SetFaction(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        uint32 factionId = Eluna::CHECKVAL<uint32>(L, 2);
        unit->setFaction(factionId);
        return 0;
    }

    /**
     * Sets the [Unit]'s level.
     *
     * @param uint8 level : new level
     */
    int SetLevel(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        uint8 newlevel = Eluna::CHECKVAL<uint8>(L, 2);

        if (newlevel < 1)
            return 0;

        if (newlevel > STRONG_MAX_LEVEL)
            newlevel = STRONG_MAX_LEVEL;

        if (Player* player = unit->ToPlayer())
        {
            player->GiveLevel(newlevel);
            player->InitTalentForLevel();
            player->SetUInt32Value(PLAYER_XP, 0);
        }
        else
            unit->SetLevel(newlevel);

        return 0;
    }

    /**
     * Sets the [Unit]'s health.
     *
     * @param uint32 health : new health
     */
    int SetHealth(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        uint32 amt = Eluna::CHECKVAL<uint32>(L, 2);
        unit->SetHealth(amt);
        return 0;
    }

    /**
     * Sets the [Unit]'s max health.
     *
     * @param uint32 maxHealth : new max health
     */
    int SetMaxHealth(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        uint32 amt = Eluna::CHECKVAL<uint32>(L, 2);
        unit->SetMaxHealth(amt);
        return 0;
    }

    /**
     * Sets the [Unit]'s power amount for the given power type.
     *
     *     enum Powers
     *     {
     *         POWER_MANA        = 0,
     *         POWER_RAGE        = 1,
     *         POWER_FOCUS       = 2,
     *         POWER_ENERGY      = 3,
     *         POWER_HAPPINESS   = 4,
     *         POWER_RUNE        = 5,
     *         POWER_RUNIC_POWER = 6,
     *         MAX_POWERS        = 7,
     *         POWER_ALL         = 127,         // default for class?
     *         POWER_HEALTH      = 0xFFFFFFFE   // (-2 as signed value)
     *     };
     *
     * @param int type = -1 : a valid power type from [Powers] or -1 for the [Unit]'s current power type
     * @param uint32 power : new power amount
     */
    int SetPower(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        int type = Eluna::CHECKVAL<int>(L, 2, -1);
        uint32 amt = Eluna::CHECKVAL<uint32>(L, 3);
        Powers power = PowerSelectorHelper(L, unit, type);

        unit->SetPower(power, amt);
        return 0;
    }

    /**
     * Sets the [Unit]'s max power amount for the given power type.
     *
     *     enum Powers
     *     {
     *         POWER_MANA        = 0,
     *         POWER_RAGE        = 1,
     *         POWER_FOCUS       = 2,
     *         POWER_ENERGY      = 3,
     *         POWER_HAPPINESS   = 4,
     *         POWER_RUNE        = 5,
     *         POWER_RUNIC_POWER = 6,
     *         MAX_POWERS        = 7,
     *         POWER_ALL         = 127,         // default for class?
     *         POWER_HEALTH      = 0xFFFFFFFE   // (-2 as signed value)
     *     };
     *
     * @param int type = -1 : a valid power type from [Powers] or -1 for the [Unit]'s current power type
     * @param uint32 maxPower : new max power amount
     */
    int SetMaxPower(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        int type = Eluna::CHECKVAL<int>(L, 2, -1);
        uint32 amt = Eluna::CHECKVAL<uint32>(L, 3);
        Powers power = PowerSelectorHelper(L, unit, type);

        unit->SetMaxPower(power, amt);
        return 0;
    }

    /**
     * Sets the [Unit]'s power type.
     *
     *     enum Powers
     *     {
     *         POWER_MANA        = 0,
     *         POWER_RAGE        = 1,
     *         POWER_FOCUS       = 2,
     *         POWER_ENERGY      = 3,
     *         POWER_HAPPINESS   = 4,
     *         POWER_RUNE        = 5,
     *         POWER_RUNIC_POWER = 6,
     *         MAX_POWERS        = 7,
     *         POWER_ALL         = 127,         // default for class?
     *         POWER_HEALTH      = 0xFFFFFFFE   // (-2 as signed value)
     *     };
     *
     * @param [Powers] type : a valid power type
     */
    int SetPowerType(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        uint32 type = Eluna::CHECKVAL<uint32>(L, 2);
        if (type >= int(MAX_POWERS))
            return luaL_argerror(L, 2, "valid Powers expected");

#ifdef TRINITY
        unit->setPowerType((Powers)type);
#else
        unit->SetPowerType((Powers)type);
#endif
        return 0;
    }

    /**
     * Sets the [Unit]'s modelID.
     *
     * @param uint32 displayId
     */
    int SetDisplayId(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        uint32 model = Eluna::CHECKVAL<uint32>(L, 2);
        unit->SetDisplayId(model);
        return 0;
    }

    /**
     * Sets the [Unit]'s native/default modelID.
     *
     * @param uint32 displayId
     */
    int SetNativeDisplayId(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        uint32 model = Eluna::CHECKVAL<uint32>(L, 2);
        unit->SetNativeDisplayId(model);
        return 0;
    }

    /**
     * Sets the [Unit]'s facing/orientation.
     *
     * @param uint32 orientation
     */
    int SetFacing(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        float o = Eluna::CHECKVAL<float>(L, 2);
        unit->SetFacingTo(o);
        return 0;
    }

    /**
     * Sets the [Unit] to face the given [WorldObject]'s direction.
     *
     * @param [WorldObject] target
     */
    int SetFacingToObject(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        WorldObject* obj = Eluna::CHECKOBJ<WorldObject>(L, 2);
        unit->SetFacingToObject(obj);
        return 0;
    }

    /**
     * Sets creator GUID
     *
     * @param uint64 guid
     */
    int SetCreatorGUID(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        uint64 guid = Eluna::CHECKVAL<uint64>(L, 2);
#ifndef TRINITY
        unit->SetCreatorGuid(ObjectGuid(guid));
#else
        unit->SetCreatorGUID(ObjectGuid(guid));
#endif
        return 0;
    }

    /**
     * Sets charmer GUID
     *
     * @param uint64 guid
     */
    int SetCharmerGUID(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        uint64 guid = Eluna::CHECKVAL<uint64>(L, 2);
#ifndef TRINITY
        unit->SetCharmerGuid(ObjectGuid(guid));
#else
        unit->SetCharmerGUID(ObjectGuid(guid));
#endif
        return 0;
    }

    /**
     * Sets pet GUID
     *
     * @param uint64 guid
     */
    int SetPetGUID(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        uint64 guid = Eluna::CHECKVAL<uint64>(L, 2);
#ifndef TRINITY
        unit->SetPetGuid(ObjectGuid(guid));
#else
        unit->SetPetGUID(ObjectGuid(guid));
#endif
        return 0;
    }

    /**
     * Toggles (Sets) [Unit]'s water walking
     *
     * @param bool enable = true
     */
    int SetWaterWalk(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        bool enable = Eluna::CHECKVAL<bool>(L, 2, true);
#ifndef TRINITY
        unit->SetWaterWalk(enable);
#else
        unit->SetWaterWalking(enable);
#endif
        return 0;
    }

    /**
     * Sets the [Unit]'s stand state
     *
     * @param uint8 state : stand state
     */
    int SetStandState(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        uint8 state = Eluna::CHECKVAL<uint8>(L, 2);
        unit->SetStandState(state);
        return 0;
    }

#if (!defined(TBC) && !defined(CLASSIC))
    /**
     * Sets the [Unit]'s FFA flag on or off.
     *
     * @param bool apply = true
     */
    int SetFFA(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        bool apply = Eluna::CHECKVAL<bool>(L, 2, true);

#ifndef TRINITY
        unit->SetFFAPvP(apply);
#else
        if (apply)
        {
            unit->SetByteFlag(UNIT_FIELD_BYTES_2, 1, UNIT_BYTE2_FLAG_FFA_PVP);
            for (Unit::ControlList::iterator itr = unit->m_Controlled.begin(); itr != unit->m_Controlled.end(); ++itr)
                (*itr)->SetByteValue(UNIT_FIELD_BYTES_2, 1, UNIT_BYTE2_FLAG_FFA_PVP);
        }
        else
        {
            unit->RemoveByteFlag(UNIT_FIELD_BYTES_2, 1, UNIT_BYTE2_FLAG_FFA_PVP);
            for (Unit::ControlList::iterator itr = unit->m_Controlled.begin(); itr != unit->m_Controlled.end(); ++itr)
                (*itr)->RemoveByteFlag(UNIT_FIELD_BYTES_2, 1, UNIT_BYTE2_FLAG_FFA_PVP);
        }
#endif
        return 0;
    }

    /**
     * Sets the [Unit]'s sanctuary flag on or off.
     *
     * @param bool apply = true
     */
    int SetSanctuary(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        bool apply = Eluna::CHECKVAL<bool>(L, 2, true);

        if (apply)
        {
            unit->SetByteFlag(UNIT_FIELD_BYTES_2, 1, UNIT_BYTE2_FLAG_SANCTUARY);
            unit->CombatStop();
            unit->CombatStopWithPets();
        }
        else
            unit->RemoveByteFlag(UNIT_FIELD_BYTES_2, 1, UNIT_BYTE2_FLAG_SANCTUARY);

        return 0;
    }

    /**
     * Sets the [Unit]'s phase mask.
     *
     * @param uint32 phaseMask
     * @param bool update = true : update visibility to nearby objects
     */
    int SetPhaseMask(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        uint32 phaseMask = Eluna::CHECKVAL<uint32>(L, 2);
        bool update = Eluna::CHECKVAL<bool>(L, 3, true);
        unit->SetPhaseMask(phaseMask, update);
        return 0;
    }

    int SetCritterGUID(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        uint64 guid = Eluna::CHECKVAL<uint64>(L, 2);
#ifndef TRINITY
        unit->SetCritterGuid(ObjectGuid(guid));
#else
        unit->SetCritterGUID(ObjectGuid(guid));
#endif
        return 0;
    }
#endif

    /*int SetStunned(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
    bool apply = Eluna::CHECKVAL<bool>(L, 2, true);
    unit->SetControlled(apply, UNIT_STATE_STUNNED);
    return 0;
    }*/

    /**
     * Roots the [Unit] to the ground, if 'false' specified, unroots the [Unit].
     *
     * @param bool apply = true
     */
    int SetRooted(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        bool apply = Eluna::CHECKVAL<bool>(L, 2, true);
#ifndef TRINITY
        unit->SetRoot(apply);
#else
        unit->SetControlled(apply, UNIT_STATE_ROOT);
#endif
        return 0;
    }

    /**
     * Confuses the [Unit], if 'false' specified, the [Unit] is no longer confused.
     *
     * @param bool apply = true
     */
    int SetConfused(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        bool apply = Eluna::CHECKVAL<bool>(L, 2, true);
#ifndef TRINITY
        unit->SetConfused(apply);
#else
        unit->SetControlled(apply, UNIT_STATE_CONFUSED);
#endif
        return 0;
    }

    /**
     * Fears the [Unit], if 'false' specified, the [Unit] is no longer feared.
     *
     * @param bool apply = true
     */
    int SetFeared(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        bool apply = Eluna::CHECKVAL<bool>(L, 2, true);
#ifndef TRINITY
        unit->SetFeared(apply);
#else
        unit->SetControlled(apply, UNIT_STATE_FLEEING);
#endif
        return 0;
    }

    /**
     * Makes the unit walk if enable is true and otherwise run.
     *
     * @param bool enable = true
     */
    int SetWalk(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        bool enable = Eluna::CHECKVAL<bool>(L, 2, true);
        unit->SetWalk(enable);
        return 0;
    }

    /*int SetCanFly(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        bool apply = Eluna::CHECKVAL<bool>(L, 2, true);
        unit->SetCanFly(apply);
        return 0;
    }*/

    /*int SetVisible(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        bool x = Eluna::CHECKVAL<bool>(L, 2, true);
        unit->SetVisible(x);
        return 0;
    }*/

    /**
     * Clears the [Unit]'s threat list.
     */
    int ClearThreatList(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
#ifdef MANGOS
        unit->GetThreatManager().clearReferences();
#else
        unit->getThreatManager().clearReferences();
#endif
        return 0;
    }
    
    /**
     * Mounts the [Unit] on the given displayID/modelID.
     *
     * @param uint32 displayId
     */
    int Mount(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        uint32 displayId = Eluna::CHECKVAL<uint32>(L, 2);

        unit->Mount(displayId);
        return 0;
    }
    
    /**
     * Dismounts the [Unit].
     */
    int Dismount(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        if (unit->IsMounted())
        {
#ifndef TRINITY
            unit->Unmount();
            unit->RemoveSpellsCausingAura(SPELL_AURA_MOUNTED);
#else
            unit->Dismount();
            unit->RemoveAurasByType(SPELL_AURA_MOUNTED);
#endif
        }

        return 0;
    }
    
    /**
     * Makes the [Unit] perform the given emote.
     *
     * @param uint32 emoteId
     */
    int Emote(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        unit->HandleEmoteCommand(Eluna::CHECKVAL<uint32>(L, 2));
        return 0;
    }

    /**
     * Makes the [Unit] perform the given emote continuously.
     *
     * @param uint32 emoteId
     */
    int EmoteState(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        uint32 emoteId = Eluna::CHECKVAL<uint32>(L, 2);

        unit->SetUInt32Value(UNIT_NPC_EMOTESTATE, emoteId);
        return 0;
    }

    /**
     * Returns calculated percentage from Health
     *
     * @return int32 percentage
     */
    int CountPctFromCurHealth(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        Eluna::Push(L, unit->CountPctFromCurHealth(Eluna::CHECKVAL<int32>(L, 2)));
        return 1;
    }

    /**
     * Returns calculated percentage from Max Health
     *
     * @return int32 percentage
     */
    int CountPctFromMaxHealth(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        Eluna::Push(L, unit->CountPctFromMaxHealth(Eluna::CHECKVAL<int32>(L, 2)));
        return 1;
    }

    /**
     * Sends chat message to [Player]
     *
     * @param uint8 type : chat, whisper, etc
     * @param uint32 lang : language to speak
     * @param string msg
     * @param [Player] target
     */
    int SendChatMessageToPlayer(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        uint8 type = Eluna::CHECKVAL<uint8>(L, 2);
        uint32 lang = Eluna::CHECKVAL<uint32>(L, 3);
        std::string msg = Eluna::CHECKVAL<std::string>(L, 4);
        Player* target = Eluna::CHECKOBJ<Player>(L, 5);

        if (type >= MAX_CHAT_MSG_TYPE)
            return luaL_argerror(L, 2, "valid ChatMsg expected");
        if (lang >= LANGUAGES_COUNT)
            return luaL_argerror(L, 3, "valid Language expected");

        WorldPacket data;
#ifdef TRINITY
        ChatHandler::BuildChatPacket(data, ChatMsg(type), Language(lang), unit, target, msg);
#else
        ChatHandler::BuildChatPacket(data, ChatMsg(type), msg.c_str(), Language(lang), 0, unit->GET_GUID(), unit->GetName(), target->GET_GUID(), target->GetName());
#endif
        target->GetSession()->SendPacket(&data);
        return 0;
    }

    /*static void PrepareMove(Unit* unit)
    {
        unit->GetMotionMaster()->MovementExpired(); // Chase
        unit->StopMoving(); // Some
        unit->GetMotionMaster()->Clear(); // all
    }*/

    /**
     * Stops the [Unit]'s movement
     */
    int MoveStop(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        unit->StopMoving();
        return 0;
    }

    /**
     * The [Unit]'s movement expires and clears movement
     *
     * @param bool reset = true : cleans movement
     */
    int MoveExpire(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        bool reset = Eluna::CHECKVAL<bool>(L, 2, true);
        unit->GetMotionMaster()->MovementExpired(reset);
        return 0;
    }

    /**
     * Clears the [Unit]'s movement
     *
     * @param bool reset = true : clean movement
     */
    int MoveClear(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        bool reset = Eluna::CHECKVAL<bool>(L, 2, true);
        unit->GetMotionMaster()->Clear(reset);
        return 0;
    }

    /**
     * The [Unit] will be idle
     */
    int MoveIdle(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        unit->GetMotionMaster()->MoveIdle();
        return 0;
    }

    /**
     * The [Unit] will move at random
     *
     * @param float radius : limit on how far the [Unit] will move at random
     */
    int MoveRandom(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        float radius = Eluna::CHECKVAL<float>(L, 2);
        float x, y, z;
        unit->GetPosition(x, y, z);
#ifndef TRINITY
        unit->GetMotionMaster()->MoveRandomAroundPoint(x, y, z, radius);
#else
        unit->GetMotionMaster()->MoveRandom(radius);
#endif
        return 0;
    }

    /**
     * The [Unit] will move to its set home location
     */
    int MoveHome(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        unit->GetMotionMaster()->MoveTargetedHome();
        return 0;
    }

    /**
     * The [Unit] will follow the target
     *
     * @param [Unit] target : target to follow
     * @param float dist = 0 : distance to start following
     * @param float angle = 0
     */
    int MoveFollow(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        Unit* target = Eluna::CHECKOBJ<Unit>(L, 2);
        float dist = Eluna::CHECKVAL<float>(L, 3, 0.0f);
        float angle = Eluna::CHECKVAL<float>(L, 4, 0.0f);
        unit->GetMotionMaster()->MoveFollow(target, dist, angle);
        return 0;
    }

    /**
     * The [Unit] will chase the target
     *
     * @param [Unit] target : target to chase
     * @param float dist = 0 : distance start chasing
     * @param float angle = 0
     */
    int MoveChase(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        Unit* target = Eluna::CHECKOBJ<Unit>(L, 2);
        float dist = Eluna::CHECKVAL<float>(L, 3, 0.0f);
        float angle = Eluna::CHECKVAL<float>(L, 4, 0.0f);
        unit->GetMotionMaster()->MoveChase(target, dist, angle);
        return 0;
    }

    /**
     * The [Unit] will move confused
     */
    int MoveConfused(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        unit->GetMotionMaster()->MoveConfused();
        return 0;
    }

    /**
     * The [Unit] will flee
     *
     * @param [Unit] target
     * @param uint32 time = 0 : flee delay
     */
    int MoveFleeing(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        Unit* target = Eluna::CHECKOBJ<Unit>(L, 2);
        uint32 time = Eluna::CHECKVAL<uint32>(L, 3, 0);
        unit->GetMotionMaster()->MoveFleeing(target, time);
        return 0;
    }

    /**
     * The [Unit] will move to the coordinates
     *
     * @param uint32 id : unique waypoint Id
     * @param float x
     * @param float y
     * @param float z
     * @param bool genPath = true : if true, generates path
     */
    int MoveTo(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        uint32 id = Eluna::CHECKVAL<uint32>(L, 2);
        float x = Eluna::CHECKVAL<float>(L, 3);
        float y = Eluna::CHECKVAL<float>(L, 4);
        float z = Eluna::CHECKVAL<float>(L, 5);
        bool genPath = Eluna::CHECKVAL<bool>(L, 6, true);
        unit->GetMotionMaster()->MovePoint(id, x, y, z, genPath);
        return 0;
    }

#if (!defined(TBC) && !defined(CLASSIC))
    /**
     * Makes the [Unit] jump to the coordinates
     *
     * @param float x
     * @param float y
     * @param float z
     * @param float zSpeed : start velocity
     * @param float maxHeight : maximum height
     * @param uint32 id = 0 : unique movement Id
     */
    int MoveJump(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        float x = Eluna::CHECKVAL<float>(L, 2);
        float y = Eluna::CHECKVAL<float>(L, 3);
        float z = Eluna::CHECKVAL<float>(L, 4);
        float zSpeed = Eluna::CHECKVAL<float>(L, 5);
        float maxHeight = Eluna::CHECKVAL<float>(L, 6);
        uint32 id = Eluna::CHECKVAL<uint32>(L, 7, 0);

        Position pos(x, y, z);
        unit->GetMotionMaster()->MoveJump(pos, zSpeed, maxHeight, id);
        return 0;
    }
#endif

    /**
     * The [Unit] will whisper the message to a [Player]
     *
     * @param string msg : message for the [Unit] to emote
     * @param uint32 lang : language for the [Unit] to speak
     * @param [Player] receiver : specific [Unit] to receive the message
     * @param bool bossWhisper = false : is a boss whisper
     */
    int SendUnitWhisper(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        const char* msg = Eluna::CHECKVAL<const char*>(L, 2);
        uint32 lang = Eluna::CHECKVAL<uint32>(L, 3);
        Player* receiver = Eluna::CHECKOBJ<Player>(L, 4);
        bool bossWhisper = Eluna::CHECKVAL<bool>(L, 5, false);
        if (std::string(msg).length() > 0)
#ifdef TRINITY
            unit->Whisper(msg, (Language)lang, receiver, bossWhisper);
#else
            unit->MonsterWhisper(msg, receiver, bossWhisper);
#endif
        return 0;
    }

    /**
     * The [Unit] will emote the message
     *
     * @param string msg : message for the [Unit] to emote
     * @param [Unit] receiver = nil : specific [Unit] to receive the message
     * @param bool bossEmote = false : is a boss emote
     */
    int SendUnitEmote(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        const char* msg = Eluna::CHECKVAL<const char*>(L, 2);
        Unit* receiver = Eluna::CHECKOBJ<Unit>(L, 3, false);
        bool bossEmote = Eluna::CHECKVAL<bool>(L, 4, false);
        if (std::string(msg).length() > 0)
#ifdef TRINITY
            unit->TextEmote(msg, receiver, bossEmote);
#else
            unit->MonsterTextEmote(msg, receiver, bossEmote);
#endif
        return 0;
    }

    /**
     * The [Unit] will say the message
     *
     * @param string msg : message for the [Unit] to say
     * @param uint32 language : language for the [Unit] to speak
     */
    int SendUnitSay(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        const char* msg = Eluna::CHECKVAL<const char*>(L, 2);
        uint32 language = Eluna::CHECKVAL<uint32>(L, 3);
        if (std::string(msg).length() > 0)
#ifdef TRINITY
            unit->Say(msg, (Language)language, unit);
#else
            unit->MonsterSay(msg, language, unit);
#endif
        return 0;
    }

    /**
     * The [Unit] will yell the message
     *
     * @param string msg : message for the [Unit] to yell
     * @param uint32 language : language for the [Unit] to speak
     */
    int SendUnitYell(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        const char* msg = Eluna::CHECKVAL<const char*>(L, 2);
        uint32 language = Eluna::CHECKVAL<uint32>(L, 3);
        if (std::string(msg).length() > 0)
#ifdef TRINITY
            unit->Yell(msg, (Language)language, unit);
#else
            unit->MonsterYell(msg, language, unit);
#endif
        return 0;
    }

    /**
     * Unmorphs the [Unit] setting it's display ID back to the native display ID.
     */
    int DeMorph(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        unit->DeMorph();
        return 0;
    }

    /**
     * Makes the [Unit] cast the spell on the target.
     *
     * @param [Unit] target = nil : can be self or another unit
     * @param uint32 spell : entry of a spell
     * @param bool triggered = false : if true the spell is instant and has no cost
     */
    int CastSpell(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        Unit* target = Eluna::CHECKOBJ<Unit>(L, 2, NULL);
        uint32 spell = Eluna::CHECKVAL<uint32>(L, 3);
        bool triggered = Eluna::CHECKVAL<bool>(L, 4, false);
        SpellEntry const* spellEntry = sSpellStore.LookupEntry(spell);
        if (!spellEntry)
            return 0;

        unit->CastSpell(target, spell, triggered);
        return 0;
    }

    /**
     * Casts the [Spell] at target [Unit] with custom basepoints or casters.
     * See also [Unit:CastSpell].
     *
     * @param [Unit] target = nil
     * @param uint32 spell
     * @param bool triggered = false
     * @param int32 bp0 = nil : custom basepoints for [Spell] effect 1. If nil, no change is made
     * @param int32 bp1 = nil : custom basepoints for [Spell] effect 2. If nil, no change is made
     * @param int32 bp2 = nil : custom basepoints for [Spell] effect 3. If nil, no change is made
     * @param [Item] castItem = nil
     * @param uint64 originalCaster = 0
     */
    int CastCustomSpell(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        Unit* target = Eluna::CHECKOBJ<Unit>(L, 2, NULL);
        uint32 spell = Eluna::CHECKVAL<uint32>(L, 3);
        bool triggered = Eluna::CHECKVAL<bool>(L, 4, false);
        bool has_bp0 = !lua_isnoneornil(L, 5);
        int32 bp0 = Eluna::CHECKVAL<int32>(L, 5, 0);
        bool has_bp1 = !lua_isnoneornil(L, 6);
        int32 bp1 = Eluna::CHECKVAL<int32>(L, 6, 0);
        bool has_bp2 = !lua_isnoneornil(L, 7);
        int32 bp2 = Eluna::CHECKVAL<int32>(L, 7, 0);
        Item* castItem = Eluna::CHECKOBJ<Item>(L, 8, false);
        uint64 originalCaster = Eluna::CHECKVAL<uint64>(L, 9, 0);

        unit->CastCustomSpell(target, spell, has_bp0 ? &bp0 : nullptr, has_bp1 ? &bp1 : nullptr, has_bp2 ? &bp2 : nullptr, triggered, castItem, nullptr, ObjectGuid(originalCaster));
        return 0;
    }
    
    /**
     * Makes the [Unit] cast the spell to the given coordinates, used for area effect spells.
     *
     * @param float x
     * @param float y
     * @param float z
     * @param uint32 spell : entry of a spell
     * @param bool triggered = false : if true the spell is instant and has no cost
     */
    int CastSpellAoF(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        float _x = Eluna::CHECKVAL<float>(L, 2);
        float _y = Eluna::CHECKVAL<float>(L, 3);
        float _z = Eluna::CHECKVAL<float>(L, 4);
        uint32 spell = Eluna::CHECKVAL<uint32>(L, 5);
        bool triggered = Eluna::CHECKVAL<bool>(L, 6, true);
        unit->CastSpell(_x, _y, _z, spell, triggered);
        return 0;
    }

    /**
     * Clears the [Unit]'s combat
     */
    int ClearInCombat(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        unit->ClearInCombat();
        return 0;
    }

    /**
     * Stops the [Unit]'s current spell cast
     *
     * @param uint32 spell = 0 : entry of a spell
     */
    int StopSpellCast(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        uint32 spellId = Eluna::CHECKVAL<uint32>(L, 2, 0);
        unit->CastStop(spellId);
        return 0;
    }

    /**
     * Interrupts [Unit]'s spell state, casting, etc.
     *
     * if spell is not interruptible, it will return
     *
     * @param int32 spellType : type of spell to interrupt
     * @param bool delayed = true : skips if the spell is delayed
     */
    int InterruptSpell(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        int spellType = Eluna::CHECKVAL<int>(L, 2);
        bool delayed = Eluna::CHECKVAL<bool>(L, 3, true);
        switch (spellType)
        {
            case 0:
                spellType = CURRENT_MELEE_SPELL;
                break;
            case 1:
                spellType = CURRENT_GENERIC_SPELL;
                break;
            case 2:
                spellType = CURRENT_CHANNELED_SPELL;
                break;
            case 3:
                spellType = CURRENT_AUTOREPEAT_SPELL;
                break;
            default:
                return luaL_argerror(L, 2, "valid CurrentSpellTypes expected");
        }

        unit->InterruptSpell((CurrentSpellTypes)spellType, delayed);
        return 0;
    }
    
    /**
     * Adds the [Aura] of the given spell entry on the given target from the [Unit].
     *
     * @param uint32 spell : entry of a spell
     * @param [Unit] target : aura will be applied on the target
     * @return [Aura] aura
     */
    int AddAura(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        uint32 spellId = Eluna::CHECKVAL<uint32>(L, 2);
        Unit* target = Eluna::CHECKOBJ<Unit>(L, 3);
        SpellEntry const* spellInfo = sSpellStore.LookupEntry(spellId);
        if (!spellInfo)
            return 1;

#ifndef TRINITY
        if (!IsSpellAppliesAura(spellInfo) && !IsSpellHaveEffect(spellInfo, SPELL_EFFECT_PERSISTENT_AREA_AURA))
            return 1;

        SpellAuraHolder* holder = CreateSpellAuraHolder(spellInfo, target, unit);

        for (uint32 i = 0; i < MAX_EFFECT_INDEX; ++i)
        {
            uint8 eff = spellInfo->Effect[i];
            if (eff >= TOTAL_SPELL_EFFECTS)
                continue;
            if (IsAreaAuraEffect(eff) ||
                eff == SPELL_EFFECT_APPLY_AURA ||
                eff == SPELL_EFFECT_PERSISTENT_AREA_AURA)
            {
                Aura* aur = CreateAura(spellInfo, SpellEffIndex(i), nullptr, holder, target);
                holder->AddAura(aur, SpellEffIndex(i));
            }
        }
        Eluna::Push(L, target->AddSpellAuraHolder(holder));
#else
        Eluna::Push(L, unit->AddAura(spellId, target));
#endif
        return 1;
    }
    
    /**
     * Removes [Aura] of the given spell entry from the [Unit].
     *
     * @param uint32 spell : entry of a spell
     */
    int RemoveAura(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        uint32 spellId = Eluna::CHECKVAL<uint32>(L, 2);
        unit->RemoveAurasDueToSpell(spellId);
        return 0;
    }
    
    /**
     * Removes all [Aura]'s from the [Unit].
     *
     *     Note: talents and racials are also auras, use with caution
     */
    int RemoveAllAuras(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        unit->RemoveAllAuras();
        return 0;
    }
    
    /**
     * Adds the given unit state for the [Unit].
     *
     * @param [UnitState] state
     */
    int AddUnitState(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        uint32 state = Eluna::CHECKVAL<uint32>(L, 2);

#ifndef TRINITY
        unit->addUnitState(state);
#else
        unit->AddUnitState(state);
#endif
        return 0;
    }
    
    /**
     * Removes the given unit state from the [Unit].
     *
     * @param [UnitState] state
     */
    int ClearUnitState(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        uint32 state = Eluna::CHECKVAL<uint32>(L, 2);

#ifndef TRINITY
        unit->clearUnitState(state);
#else
        unit->ClearUnitState(state);
#endif
        return 0;
    }
    
    /**
     * Makes the [Unit] teleport to given coordinates within same map.
     *
     * @param float x
     * @param float y
     * @param float z
     * @param float o : orientation
     */
    int NearTeleport(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        float x = Eluna::CHECKVAL<float>(L, 2);
        float y = Eluna::CHECKVAL<float>(L, 3);
        float z = Eluna::CHECKVAL<float>(L, 4);
        float o = Eluna::CHECKVAL<float>(L, 5);

        unit->NearTeleportTo(x, y, z, o);
        return 0;
    }

    /**
     * Makes the [Unit] damage the target [Unit]
     *
     * <pre>
     * enum SpellSchools
     * {
     *     SPELL_SCHOOL_NORMAL  = 0,
     *     SPELL_SCHOOL_HOLY    = 1,
     *     SPELL_SCHOOL_FIRE    = 2,
     *     SPELL_SCHOOL_NATURE  = 3,
     *     SPELL_SCHOOL_FROST   = 4,
     *     SPELL_SCHOOL_SHADOW  = 5,
     *     SPELL_SCHOOL_ARCANE  = 6,
     *     MAX_SPELL_SCHOOL     = 7
     * };
     * </pre>
     *
     * @param [Unit] target : [Unit] to damage
     * @param uint32 damage : amount to damage
     * @param bool durabilityloss = true : if false, the damage does not do durability damage
     * @param [SpellSchools] school = MAX_SPELL_SCHOOL : school the damage is done in or MAX_SPELL_SCHOOL for direct damage
     * @param uint32 spell = 0 : spell that inflicts the damage
     */
    int DealDamage(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        Unit* target = Eluna::CHECKOBJ<Unit>(L, 2);
        uint32 damage = Eluna::CHECKVAL<uint32>(L, 3);
        bool durabilityloss = Eluna::CHECKVAL<bool>(L, 4, true);
        uint32 school = Eluna::CHECKVAL<uint32>(L, 5, MAX_SPELL_SCHOOL);
        uint32 spell = Eluna::CHECKVAL<uint32>(L, 6, 0);
        if (school > MAX_SPELL_SCHOOL)
            return luaL_argerror(L, 6, "valid SpellSchool expected");

        // flat melee damage without resistence/etc reduction
        if (school == MAX_SPELL_SCHOOL)
        {
            unit->DealDamage(target, damage, nullptr, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, nullptr, durabilityloss);
#ifdef TRINITY
            unit->SendAttackStateUpdate(HITINFO_AFFECTS_VICTIM, target, 1, SPELL_SCHOOL_MASK_NORMAL, damage, 0, 0, VICTIMSTATE_HIT, 0);
#else
            unit->SendAttackStateUpdate(HITINFO_NORMALSWING2, target, SPELL_SCHOOL_MASK_NORMAL, damage, 0, 0, VICTIMSTATE_NORMAL, 0);
#endif
            return 0;
        }

        SpellSchoolMask schoolmask = SpellSchoolMask(1 << school);

#ifdef TRINITY
        if (Unit::IsDamageReducedByArmor(schoolmask))
            damage = unit->CalcArmorReducedDamage(target, damage, nullptr, BASE_ATTACK);
#else
        if (schoolmask & SPELL_SCHOOL_MASK_NORMAL)
            damage = unit->CalcArmorReducedDamage(target, damage);
#endif

        // melee damage by specific school
        if (!spell)
        {
            uint32 absorb = 0;
            uint32 resist = 0;
#ifdef TRINITY
            unit->CalcAbsorbResist(target, schoolmask, SPELL_DIRECT_DAMAGE, damage, &absorb, &resist);
#else
            target->CalculateDamageAbsorbAndResist(unit, schoolmask, SPELL_DIRECT_DAMAGE, damage, &absorb, &resist);
#endif

            if (damage <= absorb + resist)
                damage = 0;
            else
                damage -= absorb + resist;

            unit->DealDamageMods(target, damage, &absorb);
            unit->DealDamage(target, damage, nullptr, DIRECT_DAMAGE, schoolmask, nullptr, false);
#ifdef TRINITY
            unit->SendAttackStateUpdate(HITINFO_AFFECTS_VICTIM, target, 1, schoolmask, damage, absorb, resist, VICTIMSTATE_HIT, 0);
#else
            unit->SendAttackStateUpdate(HITINFO_NORMALSWING2, target, schoolmask, damage, absorb, resist, VICTIMSTATE_NORMAL, 0);
#endif
            return 0;
        }

        // non-melee damage
        unit->SpellNonMeleeDamageLog(target, spell, damage);
        return 0;
    }

    /**
     * Makes the [Unit] heal the target [Unit] with given spell
     *
     * @param [Unit] target : [Unit] to heal
     * @param uint32 spell : spell that causes the healing
     * @param uint32 amount : amount to heal
     * @param bool critical = false : if true, heal is logged as critical
     */
    int DealHeal(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        Unit* target = Eluna::CHECKOBJ<Unit>(L, 2);
        uint32 spell = Eluna::CHECKVAL<uint32>(L, 3);
        uint32 amount = Eluna::CHECKVAL<uint32>(L, 4);
        bool critical = Eluna::CHECKVAL<bool>(L, 5, false);

#ifndef TRINITY
        if (const SpellInfo* info = sSpellStore.LookupEntry(spell))
            unit->DealHeal(target, amount, info, critical);
#else
        if (const SpellInfo* info = sSpellMgr->GetSpellInfo(spell))
            unit->HealBySpell(target, info, amount, critical);
#endif
        return 0;
    }

    /**
     * Makes the [Unit] kill the target [Unit]
     *
     * @param [Unit] target : [Unit] to kill
     * @param bool durLoss = true : when true, the target's items suffer durability loss
     */
    int Kill(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        Unit* target = Eluna::CHECKOBJ<Unit>(L, 2);
        bool durLoss = Eluna::CHECKVAL<bool>(L, 3, true);

#ifndef TRINITY
        unit->DealDamage(target, target->GetHealth(), nullptr, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, nullptr, durLoss);
#else
        unit->Kill(target, durLoss);
#endif
        return 0;
    }

    /**
     * Adds threat to the [Unit] from the victim.
     *
     * <pre>
     * enum SpellSchoolMask
     * {
     *     SPELL_SCHOOL_MASK_NONE    = 0,
     *     SPELL_SCHOOL_MASK_NORMAL  = 1,
     *     SPELL_SCHOOL_MASK_HOLY    = 2,
     *     SPELL_SCHOOL_MASK_FIRE    = 4,
     *     SPELL_SCHOOL_MASK_NATURE  = 8,
     *     SPELL_SCHOOL_MASK_FROST   = 16,
     *     SPELL_SCHOOL_MASK_SHADOW  = 32,
     *     SPELL_SCHOOL_MASK_ARCANE  = 64,
     * }
     * </pre>
     *
     * @param [Unit] victim : [Unit] that caused the threat
     * @param float threat : threat amount
     * @param [SpellSchoolMask] schoolMask = 0 : [SpellSchoolMask] of the threat causer
     * @param uint32 spell = 0 : spell entry used for threat
     */
    int AddThreat(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        Unit* victim = Eluna::CHECKOBJ<Unit>(L, 2);
        float threat = Eluna::CHECKVAL<float>(L, 3, true);
        uint32 schoolMask = Eluna::CHECKVAL<uint32>(L, 4, 0);
        uint32 spell = Eluna::CHECKVAL<uint32>(L, 5, 0);

        if (schoolMask > SPELL_SCHOOL_MASK_ALL)
        {
            return luaL_argerror(L, 4, "valid SpellSchoolMask expected");
        }

#ifdef TRINITY
        unit->AddThreat(victim, threat, (SpellSchoolMask)schoolMask, spell ? sSpellMgr->GetSpellInfo(spell) : nullptr);
#else
        unit->AddThreat(victim, threat, false, (SpellSchoolMask)schoolMask, spell ? sSpellStore.LookupEntry(spell) : nullptr);
#endif
        return 0;
    }

    /*int RestoreDisplayId(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        unit->RestoreDisplayId();
        return 0;
    }*/

    /*int RestoreFaction(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        unit->RestoreFaction();
        return 0;
    }*/

    /*int RemoveBindSightAuras(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        unit->RemoveBindSightAuras();
        return 0;
    }*/

    /*int RemoveCharmAuras(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
        unit->RemoveCharmAuras();
        return 0;
    }*/

    /*int DisableMelee(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
    bool apply = Eluna::CHECKVAL<bool>(L, 2, true);

    if (apply)
    unit->AddUnitState(UNIT_STATE_CANNOT_AUTOATTACK);
    else
    unit->ClearUnitState(UNIT_STATE_CANNOT_AUTOATTACK);
    return 0;
    }*/

    /*int SummonGuardian(lua_State* L)
    {
        Unit* unit = Eluna::CHECKOBJ<Unit>(L, 1);
    uint32 entry = Eluna::CHECKVAL<uint32>(L, 2);
    float x = Eluna::CHECKVAL<float>(L, 3);
    float y = Eluna::CHECKVAL<float>(L, 4);
    float z = Eluna::CHECKVAL<float>(L, 5);
    float o = Eluna::CHECKVAL<float>(L, 6);
    uint32 desp = Eluna::CHECKVAL<uint32>(L, 7, 0);

    SummonPropertiesEntry const* properties = sSummonPropertiesStore.LookupEntry(61);
    if (!properties)
    return 1;
    Position pos;
    pos.Relocate(x,y,z,o);
    TempSummon* summon = unit->GetMap()->SummonCreature(entry, pos, properties, desp, unit);

    if (!summon)
    return 1;

    if (summon->HasUnitTypeMask(UNIT_MASK_GUARDIAN))
    ((Guardian*)summon)->InitStatsForLevel(unit->getLevel());

    if (properties && properties->Category == SUMMON_CATEGORY_ALLY)
    summon->setFaction(unit->getFaction());
    if (summon->GetEntry() == 27893)
    {
    if (uint32 weapon = unit->GetUInt32Value(PLAYER_VISIBLE_ITEM_16_ENTRYID))
    {
    summon->SetDisplayId(11686);
    summon->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, weapon);
    }
    else
    summon->SetDisplayId(1126);
    }
    summon->AI()->EnterEvadeMode();

    Eluna::Push(L, summon);
    return 1;
    }*/
};

ElunaFunction UnitMethods[] =
{
    { ENV_BOTH, "AddAura", &LuaUnit::AddAura },
    { ENV_BOTH, "AddThreat", &LuaUnit::AddThreat },
    { ENV_BOTH, "AddUnitState", &LuaUnit::AddUnitState },
    { ENV_BOTH, "Attack", &LuaUnit::Attack },
    { ENV_BOTH, "AttackStop", &LuaUnit::AttackStop },
    { ENV_BOTH, "CastCustomSpell", &LuaUnit::CastCustomSpell },
    { ENV_BOTH, "CastSpell", &LuaUnit::CastSpell },
    { ENV_BOTH, "CastSpellAoF", &LuaUnit::CastSpellAoF },
    { ENV_BOTH, "ClearInCombat", &LuaUnit::ClearInCombat },
    { ENV_BOTH, "ClearThreatList", &LuaUnit::ClearThreatList },
    { ENV_BOTH, "ClearUnitState", &LuaUnit::ClearUnitState },
    { ENV_BOTH, "CountPctFromCurHealth", &LuaUnit::CountPctFromCurHealth },
    { ENV_BOTH, "CountPctFromMaxHealth", &LuaUnit::CountPctFromMaxHealth },
    { ENV_BOTH, "DealDamage", &LuaUnit::DealDamage },
    { ENV_BOTH, "DealHeal", &LuaUnit::DealHeal },
    { ENV_BOTH, "DeMorph", &LuaUnit::DeMorph },
    { ENV_BOTH, "Dismount", &LuaUnit::Dismount },
    { ENV_BOTH, "Emote", &LuaUnit::Emote },
    { ENV_BOTH, "EmoteState", &LuaUnit::EmoteState },
    { ENV_BOTH, "GetAura", &LuaUnit::GetAura },
    { ENV_BOTH, "GetBaseSpellPower", &LuaUnit::GetBaseSpellPower },
    { ENV_BOTH, "GetCharmerGUID", &LuaUnit::GetCharmerGUID },
    { ENV_BOTH, "GetCharmGUID", &LuaUnit::GetCharmGUID },
    { ENV_BOTH, "GetClass", &LuaUnit::GetClass },
    { ENV_BOTH, "GetClassAsString", &LuaUnit::GetClassAsString },
    { ENV_BOTH, "GetCombatTime", &LuaUnit::GetCombatTime },
    { ENV_BOTH, "GetControllerGUID", &LuaUnit::GetControllerGUID },
    { ENV_BOTH, "GetControllerGUIDS", &LuaUnit::GetControllerGUIDS },
    { ENV_BOTH, "GetCreatorGUID", &LuaUnit::GetCreatorGUID },
    { ENV_BOTH, "GetCreatureType", &LuaUnit::GetCreatureType },
    { ENV_BOTH, "GetCurrentSpell", &LuaUnit::GetCurrentSpell },
    { ENV_BOTH, "GetDisplayId", &LuaUnit::GetDisplayId },
    { ENV_BOTH, "GetFaction", &LuaUnit::GetFaction },
    { ENV_BOTH, "GetFriendlyUnitsInRange", &LuaUnit::GetFriendlyUnitsInRange },
    { ENV_BOTH, "GetGender", &LuaUnit::GetGender },
    { ENV_BOTH, "GetHealth", &LuaUnit::GetHealth },
    { ENV_BOTH, "GetHealthPct", &LuaUnit::GetHealthPct },
    { ENV_BOTH, "GetLevel", &LuaUnit::GetLevel },
    { ENV_BOTH, "GetMaxHealth", &LuaUnit::GetMaxHealth },
    { ENV_BOTH, "GetMaxPower", &LuaUnit::GetMaxPower },
    { ENV_BOTH, "GetMinionGUID", &LuaUnit::GetPetGUID },
    { ENV_BOTH, "GetMountId", &LuaUnit::GetMountId },
    { ENV_BOTH, "GetMovementType", &LuaUnit::GetMovementType },
    { ENV_BOTH, "GetNativeDisplayId", &LuaUnit::GetNativeDisplayId },
    { ENV_BOTH, "GetOwner", &LuaUnit::GetOwner },
    { ENV_BOTH, "GetOwnerGUID", &LuaUnit::GetOwnerGUID },
    { ENV_BOTH, "GetPetGUID", &LuaUnit::GetPetGUID },
    { ENV_BOTH, "GetPower", &LuaUnit::GetPower },
    { ENV_BOTH, "GetPowerPct", &LuaUnit::GetPowerPct },
    { ENV_BOTH, "GetPowerType", &LuaUnit::GetPowerType },
    { ENV_BOTH, "GetRace", &LuaUnit::GetRace },
    { ENV_BOTH, "GetRaceAsString", &LuaUnit::GetRaceAsString },
    { ENV_BOTH, "GetSpeed", &LuaUnit::GetSpeed },
    { ENV_BOTH, "GetStandState", &LuaUnit::GetStandState },
    { ENV_BOTH, "GetStat", &LuaUnit::GetStat },
    { ENV_BOTH, "GetUnfriendlyUnitsInRange", &LuaUnit::GetUnfriendlyUnitsInRange },
    { ENV_BOTH, "GetVictim", &LuaUnit::GetVictim },
    { ENV_BOTH, "HasAura", &LuaUnit::HasAura },
    { ENV_BOTH, "HasUnitState", &LuaUnit::HasUnitState },
    { ENV_BOTH, "HealthAbovePct", &LuaUnit::HealthAbovePct },
    { ENV_BOTH, "HealthBelowPct", &LuaUnit::HealthBelowPct },
    { ENV_BOTH, "InterruptSpell", &LuaUnit::InterruptSpell },
    { ENV_BOTH, "IsAlive", &LuaUnit::IsAlive },
    { ENV_BOTH, "IsArmorer", &LuaUnit::IsArmorer },
    { ENV_BOTH, "IsAttackingPlayer", &LuaUnit::IsAttackingPlayer },
    { ENV_BOTH, "IsAuctioneer", &LuaUnit::IsAuctioneer },
    { ENV_BOTH, "IsBanker", &LuaUnit::IsBanker },
    { ENV_BOTH, "IsBattleMaster", &LuaUnit::IsBattleMaster },
    { ENV_BOTH, "IsCasting", &LuaUnit::IsCasting },
    { ENV_BOTH, "IsCharmed", &LuaUnit::IsCharmed },
    { ENV_BOTH, "IsDead", &LuaUnit::IsDead },
    { ENV_BOTH, "IsDying", &LuaUnit::IsDying },
    { ENV_BOTH, "IsFullHealth", &LuaUnit::IsFullHealth },
    { ENV_BOTH, "IsGossip", &LuaUnit::IsGossip },
    { ENV_BOTH, "IsGuildMaster", &LuaUnit::IsGuildMaster },
    { ENV_BOTH, "IsInAccessiblePlaceFor", &LuaUnit::IsInAccessiblePlaceFor },
    { ENV_BOTH, "IsInCombat", &LuaUnit::IsInCombat },
    { ENV_BOTH, "IsInnkeeper", &LuaUnit::IsInnkeeper },
    { ENV_BOTH, "IsInWater", &LuaUnit::IsInWater },
    { ENV_BOTH, "IsMounted", &LuaUnit::IsMounted },
    { ENV_BOTH, "IsPvPFlagged", &LuaUnit::IsPvPFlagged },
    { ENV_BOTH, "IsQuestGiver", &LuaUnit::IsQuestGiver },
    { ENV_BOTH, "IsRooted", &LuaUnit::IsRooted },
    { ENV_BOTH, "IsServiceProvider", &LuaUnit::IsServiceProvider },
    { ENV_BOTH, "IsSpiritGuide", &LuaUnit::IsSpiritGuide },
    { ENV_BOTH, "IsSpiritHealer", &LuaUnit::IsSpiritHealer },
    { ENV_BOTH, "IsSpiritService", &LuaUnit::IsSpiritService },
    { ENV_BOTH, "IsStandState", &LuaUnit::IsStandState },
    { ENV_BOTH, "IsStopped", &LuaUnit::IsStopped },
    { ENV_BOTH, "IsTabardDesigner", &LuaUnit::IsTabardDesigner },
    { ENV_BOTH, "IsTaxi", &LuaUnit::IsTaxi },
    { ENV_BOTH, "IsTrainer", &LuaUnit::IsTrainer },
    { ENV_BOTH, "IsUnderWater", &LuaUnit::IsUnderWater },
    { ENV_BOTH, "IsVendor", &LuaUnit::IsVendor },
    { ENV_BOTH, "IsWithinDistInMap", &LuaUnit::IsWithinDistInMap },
    { ENV_BOTH, "Kill", &LuaUnit::Kill },
    { ENV_BOTH, "Mount", &LuaUnit::Mount },
    { ENV_BOTH, "MoveChase", &LuaUnit::MoveChase },
    { ENV_BOTH, "MoveClear", &LuaUnit::MoveClear },
    { ENV_BOTH, "MoveConfused", &LuaUnit::MoveConfused },
    { ENV_BOTH, "MoveExpire", &LuaUnit::MoveExpire },
    { ENV_BOTH, "MoveFleeing", &LuaUnit::MoveFleeing },
    { ENV_BOTH, "MoveFollow", &LuaUnit::MoveFollow },
    { ENV_BOTH, "MoveHome", &LuaUnit::MoveHome },
    { ENV_BOTH, "MoveIdle", &LuaUnit::MoveIdle },
    { ENV_BOTH, "MoveRandom", &LuaUnit::MoveRandom },
    { ENV_BOTH, "MoveStop", &LuaUnit::MoveStop },
    { ENV_BOTH, "MoveTo", &LuaUnit::MoveTo },
    { ENV_BOTH, "NearTeleport", &LuaUnit::NearTeleport },
    { ENV_BOTH, "RemoveAllAuras", &LuaUnit::RemoveAllAuras },
    { ENV_BOTH, "RemoveAura", &LuaUnit::RemoveAura },
    { ENV_BOTH, "SendChatMessageToPlayer", &LuaUnit::SendChatMessageToPlayer },
    { ENV_BOTH, "SendUnitEmote", &LuaUnit::SendUnitEmote },
    { ENV_BOTH, "SendUnitSay", &LuaUnit::SendUnitSay },
    { ENV_BOTH, "SendUnitWhisper", &LuaUnit::SendUnitWhisper },
    { ENV_BOTH, "SendUnitYell", &LuaUnit::SendUnitYell },
    { ENV_BOTH, "SetCharmerGUID", &LuaUnit::SetCharmerGUID },
    { ENV_BOTH, "SetConfused", &LuaUnit::SetConfused },
    { ENV_BOTH, "SetCreatorGUID", &LuaUnit::SetCreatorGUID },
    { ENV_BOTH, "SetDisplayId", &LuaUnit::SetDisplayId },
    { ENV_BOTH, "SetFacing", &LuaUnit::SetFacing },
    { ENV_BOTH, "SetFacingToObject", &LuaUnit::SetFacingToObject },
    { ENV_BOTH, "SetFaction", &LuaUnit::SetFaction },
    { ENV_BOTH, "SetFeared", &LuaUnit::SetFeared },
    { ENV_BOTH, "SetHealth", &LuaUnit::SetHealth },
    { ENV_BOTH, "SetLevel", &LuaUnit::SetLevel },
    { ENV_BOTH, "SetMaxHealth", &LuaUnit::SetMaxHealth },
    { ENV_BOTH, "SetMaxPower", &LuaUnit::SetMaxPower },
    { ENV_BOTH, "SetMinionGUID", &LuaUnit::SetPetGUID },
    { ENV_BOTH, "SetName", &LuaUnit::SetName },
    { ENV_BOTH, "SetNativeDisplayId", &LuaUnit::SetNativeDisplayId },
    { ENV_BOTH, "SetOwnerGUID", &LuaUnit::SetOwnerGUID },
    { ENV_BOTH, "SetPetGUID", &LuaUnit::SetPetGUID },
    { ENV_BOTH, "SetPower", &LuaUnit::SetPower },
    { ENV_BOTH, "SetPowerType", &LuaUnit::SetPowerType },
    { ENV_BOTH, "SetPvP", &LuaUnit::SetPvP },
    { ENV_BOTH, "SetRooted", &LuaUnit::SetRooted },
    { ENV_BOTH, "SetSheath", &LuaUnit::SetSheath },
    { ENV_BOTH, "SetSpeed", &LuaUnit::SetSpeed },
    { ENV_BOTH, "SetStandState", &LuaUnit::SetStandState },
    { ENV_BOTH, "SetWaterWalk", &LuaUnit::SetWaterWalk },
    { ENV_BOTH, "StopSpellCast", &LuaUnit::StopSpellCast },
#if (!defined(TBC) && !defined(CLASSIC))
    { ENV_BOTH, "GetCritterGUID", &LuaUnit::GetCritterGUID },
    { ENV_BOTH, "GetVehicleKit", &LuaUnit::GetVehicleKit },
    { ENV_BOTH, "MoveJump", &LuaUnit::MoveJump },
    { ENV_BOTH, "SetCritterGUID", &LuaUnit::SetCritterGUID },
    { ENV_BOTH, "SetFFA", &LuaUnit::SetFFA },
    { ENV_BOTH, "SetPhaseMask", &LuaUnit::SetPhaseMask },
    { ENV_BOTH, "SetSanctuary", &LuaUnit::SetSanctuary },
#endif
#ifndef CLASSIC
    { ENV_BOTH, "IsOnVehicle", &LuaUnit::IsOnVehicle },
#endif

    { ENV_NONE, nullptr, nullptr },
};

ELUNA_TYPE(Unit, false, UnitMethods, "WorldObject")

void RegisterTypeUnit(Eluna* E)
{
    ElunaTemplate<Unit>::RegisterTypeForState(E);
}