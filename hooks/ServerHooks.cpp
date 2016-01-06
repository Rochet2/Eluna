
#include "Hooks.h"
#include "HookHelpers.h"
#include "LuaEngine.h"
#include "BindingMap.h"
#include "ElunaEventMgr.h"
#include "ElunaIncludes.h"
#include "ElunaTemplate.h"

#define START_HOOK(EVENT) \
    if (!IsEnabled())\
        return;\
    auto key = EventKey(EVENT);\
    if (!ServerEventBindings->HasBindingsFor(key))\
        return;\
    LOCK_ELUNA

#define START_HOOK_WITH_RETVAL(EVENT, RETVAL) \
    if (!IsEnabled())\
        return RETVAL;\
    auto key = EventKey<PlayerEvents>(EVENT);\
    if (!PlayerEventBindings->HasBindingsFor(key))\
        return RETVAL;\
    LOCK_ELUNA

void Eluna::OnLuaStateClose()
{
}

void Eluna::OnLuaStateOpen()
{
}

void Eluna::OnPVPKill(Player* killer, Player* killed)
{
    START_HOOK("PLAYER_EVENT_ON_KILL_PLAYER");
    Push(killer);
    Push(killed);
    CallAllFunctions(ServerEventBindings, key);
}
