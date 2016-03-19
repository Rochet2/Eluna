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
namespace LuaDynamicObject
{
};

ElunaFunction DynamicObjectMethods[] =
{
    { ENV_NONE, nullptr, nullptr },
};

ELUNA_TYPE(DynamicObject, false, DynamicObjectMethods, "WorldObject")

void RegisterTypeDynamicObject(Eluna* E)
{
    ElunaTemplate<DynamicObject>::RegisterTypeForState(E);
}
