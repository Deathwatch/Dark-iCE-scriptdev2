/*
Comment: Gregoryho external event handler
Obsahuje: On PvP kill, On Zone change
*/

#include "precompiled.h"
#include "outdoor_pvp.h"

void external_handler_OnPvpKill(Player *killer, Player *killed)
{
	if(!killer || !killed) return;
	//OPVP.HandlePvpKill(killer,killed);
	debug_log("Script Library: Handle OnPvP Kill, Player %s killed player %s",killer->GetName(),killed->GetName());
}

void external_handler_OnZoneChange(Player *pPlayer, AreaTableEntry const *zone)
{
	if(!pPlayer) return;
	//OPVP.HandleZoneChange(pPlayer,zone);
	debug_log("Script Library: Player %s changed zone to %u",pPlayer->GetName(),zone->ID);
}
