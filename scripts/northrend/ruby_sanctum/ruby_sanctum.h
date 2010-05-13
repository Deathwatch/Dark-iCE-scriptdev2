/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_RUBY_SANCTUM_H
#define DEF_RUBY_SANCTUM_H

enum
{
    MAX_ENCOUNTER           = 1,

	//10man norm
	TYPE_DEAD0_EVENT		= 0, //check if mobs are dead to spawn halion
	TYPE_HALION0_EVENT		= 1,
	
	DATA_HALION0            = 10,
    DATA_ZARITHRIAN0        = 11,
    DATA_BALTHARUS0         = 12,
    DATA_SAVIANA0           = 13,

	NPC_HALION0				= 39863,
	NPC_ZARITHRIAN0			= 39746,
	NPC_BALTHARUS0			= 39751,
	NPC_SAVIANA0			= 39747,
	
	/*
	//25man norm
	NPC_ZARITHRIAN_1		= 39805,
	NPC_BALTHARUS_1			= 39899,
	NPC_SAVIANA_1			= 39823,
	NPC_HALION_1			= 39864,
	//10man hero
	NPC_ZARITHRIAN_2		= 39806,
	NPC_BALTHARUS_2			= 39920,
	NPC_SAVIANA_2			= 39824,
	NPC_HALION_2			= 39944,
	//25man hero
	NPC_ZARITHRIAN_3		= 39808,
	NPC_BALTHARUS_3			= 39922,
	NPC_SAVIANA_3			= 39829,
	NPC_HALION_3			= 39945,
    */
	//SPELL BASED
	NPC_SOUL_CONSUMPTION_1	= 40135, //1 stack
	NPC_SOUL_CONSUMPTION_2	= 40673, //2 stacks
	NPC_SOUL_CONSUMPTION_3	= 40674, //3 stacks
	NPC_SOUL_CONSUMPTION_4	= 40675, //4 stacks
	NPC_LIVING_INFERNO_10	= 40681, //spawned by Meteor Strike 10 man
	NPC_LIVING_INFERNO_25	= 40682, //spawned by Meteor Strike	25 man
	NPC_LIVING_EMBER_10		= 40683, //embers for minibosses
	NPC_LIVING_EMBER_25		= 40684, //embers for minibosses
	GO_TWILIGHT_PORTAL_ENT  = 202794, //blizzlike
	GO_TWILIGHT_PORTAL_EXT  = 202795, //blizzlike

	//SCRIPT BASED
	GO_TREE					= 202222,
	NPC_XERESTRASZA			= 40429,
	NPC_HALION_CONTROLLER	= 40146,
};

#endif
