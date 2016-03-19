/*
* Copyright (C) 2010 - 2016 Eluna Lua Engine <http://emudevs.com/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#include "ElunaTemplate.h"

void RegisterTemplates(Eluna* E)
{
    RegisterTypeAura(E);
    RegisterTypeCreature(E);
    RegisterTypeElunaBase(E);
    RegisterTypeElunaQuery(E);
    RegisterTypeGameObject(E);
    RegisterTypeGlobal(E);
    RegisterTypeGroup(E);
    RegisterTypeGuild(E);
    RegisterTypeItem(E);
    RegisterTypeMap(E);
    RegisterTypeObject(E);
    RegisterTypePlayer(E);
    RegisterTypeQuest(E);
    RegisterTypeSpell(E);
    RegisterTypeUnit(E);
#ifndef CLASSIC
#ifndef TBC
    RegisterTypeVehicle(E);
#endif
#endif
    RegisterTypeWorldObject(E);
    RegisterTypeWorldPacket(E);
    RegisterTypeDynamicObject(E);
    RegisterTypeCorpse(E);
}
