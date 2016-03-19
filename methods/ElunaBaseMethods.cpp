/*
* Copyright (C) 2010 - 2016 Eluna Lua Engine <http://emudevs.com/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#include "LuaEngine.h"
#include "ElunaTemplate.h"

class ElunaBase
{
public:
};

/***
 * ElunaBase is inherited by all other classes
 *
 * Inherits all methods from: none
 */
namespace LuaElunaBase
{
    /**
     * Returns the the class name as string.
     *
     * Class name can be [Player], [Creature], [GameObject].. and so on.
     *
     * @return string classname
     */
    int GetObjectType(lua_State* L)
    {
        ElunaObject* obj = Eluna::CHECKOBJ<ElunaObject>(L, 1);
        Eluna::Push(L, obj->GetTypeName());
        return 1;
    }
};

ElunaFunction ElunaBaseMethods[] =
{
    { ENV_BOTH, "GetObjectType", &LuaElunaBase::GetObjectType },

    { ENV_NONE, nullptr, nullptr },
};

ELUNA_TYPE(ElunaBase, true, ElunaBaseMethods, "")

void RegisterTypeElunaBase(Eluna* E)
{
    ElunaTemplate<ElunaBase>::RegisterTypeForState(E);
}
