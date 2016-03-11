/*
* Copyright (C) 2010 - 2016 Eluna Lua Engine <http://emudevs.com/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#include "ElunaUtility.h"
#include "World.h"
#include "Object.h"
#include "Unit.h"
#include "GameObject.h"
#include "DBCStores.h"

uint32 ElunaUtil::GetCurrTime()
{
#ifndef TRINITY
    return WorldTimer::getMSTime();
#else
    return getMSTime();
#endif
}

uint32 ElunaUtil::GetTimeDiff(uint32 oldMSTime)
{
#ifndef TRINITY
    return WorldTimer::getMSTimeDiff(oldMSTime, GetCurrTime());
#else
    return GetMSTimeDiffToNow(oldMSTime);
#endif
}

ElunaUtil::ObjectGUIDCheck::ObjectGUIDCheck(ObjectGuid guid) : _guid(guid)
{
}

bool ElunaUtil::ObjectGUIDCheck::operator()(WorldObject* object)
{
    return object->GET_GUID() == _guid;
}

ElunaUtil::ObjectDistanceOrderPred::ObjectDistanceOrderPred(WorldObject const* pRefObj, bool ascending) : m_refObj(pRefObj), m_ascending(ascending)
{
}
bool ElunaUtil::ObjectDistanceOrderPred::operator()(WorldObject const* pLeft, WorldObject const* pRight) const
{
    return m_ascending ? m_refObj->GetDistanceOrder(pLeft, pRight) : !m_refObj->GetDistanceOrder(pLeft, pRight);
}

ElunaUtil::WorldObjectInRangeCheck::WorldObjectInRangeCheck(bool nearest, WorldObject const* obj, float range,
    uint16 typeMask, uint32 entry, uint32 hostile, uint32 dead) :
    i_obj(obj), i_obj_unit(nullptr), i_obj_fact(nullptr), i_hostile(hostile), i_entry(entry), i_range(range), i_typeMask(typeMask), i_dead(dead), i_nearest(nearest)
{
    i_obj_unit = i_obj->ToUnit();
    if (!i_obj_unit)
        if (GameObject const* go = i_obj->ToGameObject())
            i_obj_unit = go->GetOwner();
    if (!i_obj_unit)
        i_obj_fact = sFactionTemplateStore.LookupEntry(14);
}
WorldObject const& ElunaUtil::WorldObjectInRangeCheck::GetFocusObject() const
{
    return *i_obj;
}
bool ElunaUtil::WorldObjectInRangeCheck::operator()(WorldObject* u)
{
    if (i_typeMask && !u->isType(TypeMask(i_typeMask)))
        return false;
    if (i_entry && u->GetEntry() != i_entry)
        return false;
    if (i_obj->GET_GUID() == u->GET_GUID())
        return false;
    if (!i_obj->IsWithinDistInMap(u, i_range))
        return false;
    Unit const* target = u->ToUnit();
    if (!target)
        if (GameObject const* go = u->ToGameObject())
            target = go->GetOwner();
    if (target)
    {
#ifdef CMANGOS
        if (i_dead && (i_dead == 1) != target->isAlive())
            return false;
#else
        if (i_dead && (i_dead == 1) != target->IsAlive())
            return false;
#endif
        if (i_hostile)
        {
            if (!i_obj_unit)
            {
                if (i_obj_fact)
                {
#ifdef TRINITY
                    if ((i_obj_fact->IsHostileTo(*target->GetFactionTemplateEntry())) != (i_hostile == 1))
                        return false;
#else
                    if ((i_obj_fact->IsHostileTo(*target->getFactionTemplateEntry())) != (i_hostile == 1))
                        return false;
#endif
                }
                else if (i_hostile == 1)
                    return false;
            }
            else if ((i_hostile == 1) != i_obj_unit->IsHostileTo(target))
                return false;
        }
    }
    if (i_nearest)
        i_range = i_obj->GetDistance(u);
    return true;
}
