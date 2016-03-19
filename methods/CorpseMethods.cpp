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
namespace LuaCorpse
{
};

ElunaFunction CorpseMethods[] =
{
    { ENV_NONE, nullptr, nullptr },
};

ELUNA_TYPE(Corpse, false, CorpseMethods, "WorldObject")

void RegisterTypeCorpse(Eluna* E)
{
    ElunaTemplate<Corpse>::RegisterTypeForState(E);
}
