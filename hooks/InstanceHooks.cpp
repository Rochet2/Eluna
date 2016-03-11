/*
* Copyright (C) 2010 - 2015 Eluna Lua Engine <http://emudevs.com/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#include "Hooks.h"
#include "HookHelpers.h"
#include "LuaEngine.h"
#include "BindingMap.h"
#include "ElunaIncludes.h"
#include "ElunaTemplate.h"


void Eluna::OnInitialize(ElunaInstanceAI* ai)
{
}

void Eluna::OnLoad(ElunaInstanceAI* ai)
{
}

void Eluna::OnUpdateInstance(ElunaInstanceAI* ai, uint32 diff)
{
}

void Eluna::OnPlayerEnterInstance(ElunaInstanceAI* ai, Player* player)
{
}

void Eluna::OnCreatureCreate(ElunaInstanceAI* ai, Creature* creature)
{
}

void Eluna::OnGameObjectCreate(ElunaInstanceAI* ai, GameObject* gameobject)
{
}

bool Eluna::OnCheckEncounterInProgress(ElunaInstanceAI* ai)
{
    return false;
}