/* Copyright (C) 2009 - 2010 Project Dark-iCE <http://projectdarkice.clanice.com/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: custom_npc_professionmaster
SD%Complete: 100
SDComment: Profession Master. Allows profession skills increasement.
SDCategory: NPC
SDAuthor: Darkiss @ Hellscream Network <http://www.hellscream-wow.com/>
EndScriptData */

#include "precompiled.h"
#include "../../../../shared/Config/Config.h"
#include "../../darkice_config.h"

bool GossipHello_custom_npc_professionmaster(Player* pPlayer, Creature* pCreature)
{
	Config DARKICEConfig;
	if(!DARKICEConfig.SetSource(_DARKICE_CONFIG,true))
		error_log("DARKICE: Unable to open configuration file");

	bool ProfessionsEnabled = DARKICEConfig.GetBoolDefault("ProfessionMaster.ProfessionsEnabled", true);
	bool SecondarySkillsEnabled = DARKICEConfig.GetBoolDefault("ProfessionMaster.SecondarySkillsEnabled", true);

    if (pPlayer->GetTeam() == ALLIANCE)
	{
	    if(ProfessionsEnabled)
		pPlayer->ADD_GOSSIP_ITEM( 7, "Professions ->"        , GOSSIP_SENDER_MAIN, 1000);
	    if(SecondarySkillsEnabled)
		pPlayer->ADD_GOSSIP_ITEM( 7, "Secondary Skills ->"   , GOSSIP_SENDER_MAIN, 2000);
	}
	else
	{
	    if(ProfessionsEnabled)
		pPlayer->ADD_GOSSIP_ITEM( 7, "Professions ->"        , GOSSIP_SENDER_MAIN, 1000);
	    if(SecondarySkillsEnabled)
		pPlayer->ADD_GOSSIP_ITEM( 7, "Secondary Skills ->"   , GOSSIP_SENDER_MAIN, 2000);
	}

    pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());

return true;
}

void SendDefaultMenu_custom_npc_professionmaster(Player* pPlayer, Creature* pCreature, uint32 uiAction)
{

// Not allow in combat
if (pPlayer->isInCombat())
{
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->MonsterSay("You are in combat!", LANG_UNIVERSAL, NULL);
	return;
}

	Config DARKICEConfig;
	if(!DARKICEConfig.SetSource(_DARKICE_CONFIG,true))
		error_log("DARKICE: Unable to open configuration file");

	bool ProfessionsEnabled = DARKICEConfig.GetBoolDefault("ProfessionMaster.ProfessionsEnabled", true);
	bool SecondarySkillsEnabled = DARKICEConfig.GetBoolDefault("ProfessionMaster.SecondarySkillsEnabled", true);

//Mony Check
if (pPlayer->GetMoney() < (DARKICEConfig.GetFloatDefault("ProfessionMaster.SkillPointCost",0)))
{
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->MonsterWhisper("You don't have enough money.", pPlayer->GetGUID());
	return;
}

switch(uiAction)
{

case 1000: //Profession
            pPlayer->ADD_GOSSIP_ITEM( 5, "Alchemy."              , GOSSIP_SENDER_MAIN, 1001);
            pPlayer->ADD_GOSSIP_ITEM( 5, "Blacksmithing."        , GOSSIP_SENDER_MAIN, 1002);
            pPlayer->ADD_GOSSIP_ITEM( 5, "Enchanting."           , GOSSIP_SENDER_MAIN, 1003);
            pPlayer->ADD_GOSSIP_ITEM( 5, "Engineering."          , GOSSIP_SENDER_MAIN, 1004);
            pPlayer->ADD_GOSSIP_ITEM( 5, "Herbalism."            , GOSSIP_SENDER_MAIN, 1005);
            pPlayer->ADD_GOSSIP_ITEM( 5, "Inscription."          , GOSSIP_SENDER_MAIN, 1006);
            pPlayer->ADD_GOSSIP_ITEM( 5, "Jewelcrafting."        , GOSSIP_SENDER_MAIN, 1007);
            pPlayer->ADD_GOSSIP_ITEM( 5, "Leatherworking."       , GOSSIP_SENDER_MAIN, 1008);
            pPlayer->ADD_GOSSIP_ITEM( 5, "Mining."               , GOSSIP_SENDER_MAIN, 1009);
            pPlayer->ADD_GOSSIP_ITEM( 5, "Skinning."             , GOSSIP_SENDER_MAIN, 1010);
            pPlayer->ADD_GOSSIP_ITEM( 5, "Tailoring."            , GOSSIP_SENDER_MAIN, 1011);
            pPlayer->ADD_GOSSIP_ITEM( 7, "<- Main Menu"          , GOSSIP_SENDER_MAIN, 3000);

	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 2000: //Secondary Skills
            pPlayer->ADD_GOSSIP_ITEM( 5, "Cooking."              , GOSSIP_SENDER_MAIN, 2001);
            pPlayer->ADD_GOSSIP_ITEM( 5, "First Aid."            , GOSSIP_SENDER_MAIN, 2002);
            pPlayer->ADD_GOSSIP_ITEM( 5, "Fishing."              , GOSSIP_SENDER_MAIN, 2003);
          //pPlayer->ADD_GOSSIP_ITEM( 5, "Riding."               , GOSSIP_SENDER_MAIN, 2004);
			pPlayer->ADD_GOSSIP_ITEM( 5, "Archaeology."          , GOSSIP_SENDER_MAIN, 2005);
            pPlayer->ADD_GOSSIP_ITEM( 7, "<- Main Menu"          , GOSSIP_SENDER_MAIN, 3000);

	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;

case 3000:

    if (pPlayer->GetTeam() == ALLIANCE)
	{
	    if(ProfessionsEnabled)
		pPlayer->ADD_GOSSIP_ITEM( 7, "Professions ->"        , GOSSIP_SENDER_MAIN, 1000);
	    if(SecondarySkillsEnabled)
		pPlayer->ADD_GOSSIP_ITEM( 7, "Secondary Skills ->"   , GOSSIP_SENDER_MAIN, 2000);
	}
	else
	{
	    if(ProfessionsEnabled)
		pPlayer->ADD_GOSSIP_ITEM( 7, "Professions ->"        , GOSSIP_SENDER_MAIN, 1000);
	    if(SecondarySkillsEnabled)
		pPlayer->ADD_GOSSIP_ITEM( 7, "Secondary Skills ->"   , GOSSIP_SENDER_MAIN, 2000);
	}

	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
break;


case 1001: // Alchemy
	if(!pPlayer->UpdateSkill(171,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
		GossipHello_custom_npc_professionmaster(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(171,(DARKICEConfig.GetFloatDefault("ProfessionMaster.AddSkillPoints",0)));
	pPlayer->ModifyMoney(-(DARKICEConfig.GetFloatDefault("ProfessionMaster.SkillPointCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 1002: // Blacksmithing
	if(!pPlayer->UpdateSkill(164,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
		GossipHello_custom_npc_professionmaster(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(164,(DARKICEConfig.GetFloatDefault("ProfessionMaster.AddSkillPoints",0)));
	pPlayer->ModifyMoney(-(DARKICEConfig.GetFloatDefault("ProfessionMaster.SkillPointCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 1003: // Enchanting
	if(!pPlayer->UpdateSkill(333,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
		GossipHello_custom_npc_professionmaster(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(333,(DARKICEConfig.GetFloatDefault("ProfessionMaster.AddSkillPoints",0)));
	pPlayer->ModifyMoney(-(DARKICEConfig.GetFloatDefault("ProfessionMaster.SkillPointCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 1004: // Engineering
	if(!pPlayer->UpdateSkill(202,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
		GossipHello_custom_npc_professionmaster(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(202,(DARKICEConfig.GetFloatDefault("ProfessionMaster.AddSkillPoints",0)));
	pPlayer->ModifyMoney(-(DARKICEConfig.GetFloatDefault("ProfessionMaster.SkillPointCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 1005: // Herbalism
	if(!pPlayer->UpdateSkill(182,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
		GossipHello_custom_npc_professionmaster(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(182,(DARKICEConfig.GetFloatDefault("ProfessionMaster.AddSkillPoints",0)));
	pPlayer->ModifyMoney(-(DARKICEConfig.GetFloatDefault("ProfessionMaster.SkillPointCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 1006: // Inscription
	if(!pPlayer->UpdateSkill(773,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
		GossipHello_custom_npc_professionmaster(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(773,(DARKICEConfig.GetFloatDefault("ProfessionMaster.AddSkillPoints",0)));
	pPlayer->ModifyMoney(-(DARKICEConfig.GetFloatDefault("ProfessionMaster.SkillPointCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 1007: // Jewelcrafting
	if(!pPlayer->UpdateSkill(755,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
		GossipHello_custom_npc_professionmaster(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(755,(DARKICEConfig.GetFloatDefault("ProfessionMaster.AddSkillPoints",0)));
	pPlayer->ModifyMoney(-(DARKICEConfig.GetFloatDefault("ProfessionMaster.SkillPointCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 1008: // Leatherworking
	if(!pPlayer->UpdateSkill(165,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
		GossipHello_custom_npc_professionmaster(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(165,(DARKICEConfig.GetFloatDefault("ProfessionMaster.AddSkillPoints",0)));
	pPlayer->ModifyMoney(-(DARKICEConfig.GetFloatDefault("ProfessionMaster.SkillPointCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 1009: // Mining
	if(!pPlayer->UpdateSkill(186,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
		GossipHello_custom_npc_professionmaster(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(186,(DARKICEConfig.GetFloatDefault("ProfessionMaster.AddSkillPoints",0)));
	pPlayer->ModifyMoney(-(DARKICEConfig.GetFloatDefault("ProfessionMaster.SkillPointCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 1010: // Skinning
	if(!pPlayer->UpdateSkill(393,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
		GossipHello_custom_npc_professionmaster(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(393,(DARKICEConfig.GetFloatDefault("ProfessionMaster.AddSkillPoints",0)));
	pPlayer->ModifyMoney(-(DARKICEConfig.GetFloatDefault("ProfessionMaster.SkillPointCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 1011: // Tailoring
	if(!pPlayer->UpdateSkill(197,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
		GossipHello_custom_npc_professionmaster(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(197,(DARKICEConfig.GetFloatDefault("ProfessionMaster.AddSkillPoints",0)));
	pPlayer->ModifyMoney(-(DARKICEConfig.GetFloatDefault("ProfessionMaster.SkillPointCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

//////////////////////////////////////////////////Secondary Skills///////////////////////////////////////////////////////////////

case 2001: // Cooking
	if(!pPlayer->UpdateSkill(185,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
		GossipHello_custom_npc_professionmaster(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(185,(DARKICEConfig.GetFloatDefault("ProfessionMaster.AddSkillPoints",0)));
	pPlayer->ModifyMoney(-(DARKICEConfig.GetFloatDefault("ProfessionMaster.SkillPointCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 2002: // First Aid
	if(!pPlayer->UpdateSkill(129,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
		GossipHello_custom_npc_professionmaster(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(129,(DARKICEConfig.GetFloatDefault("ProfessionMaster.AddSkillPoints",0)));
	pPlayer->ModifyMoney(-(DARKICEConfig.GetFloatDefault("ProfessionMaster.SkillPointCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 2003: // Fishing
	if(!pPlayer->UpdateSkill(356,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
		GossipHello_custom_npc_professionmaster(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(356,(DARKICEConfig.GetFloatDefault("ProfessionMaster.AddSkillPoints",0)));
	pPlayer->ModifyMoney(-(DARKICEConfig.GetFloatDefault("ProfessionMaster.SkillPointCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

case 2005: // Archaeology
	if(!pPlayer->UpdateSkill(794,0))
      {
		pCreature->MonsterWhisper("You don't have this skill or you allready have the maximum skill value $C.", pPlayer->GetGUID());
		GossipHello_custom_npc_professionmaster(pPlayer, pCreature);
		return;
	}

	pPlayer->UpdateSkill(794,(DARKICEConfig.GetFloatDefault("ProfessionMaster.AddSkillPoints",0)));
	pPlayer->ModifyMoney(-(DARKICEConfig.GetFloatDefault("ProfessionMaster.SkillPointCost",0)));
	pPlayer->CLOSE_GOSSIP_MENU();
break;

 pPlayer->CLOSE_GOSSIP_MENU();

} // end of switch
} //end of function



bool GossipSelect_custom_npc_professionmaster(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
	// Main menu
	if (uiSender == GOSSIP_SENDER_MAIN)
	SendDefaultMenu_custom_npc_professionmaster(pPlayer, pCreature, uiAction);

return true;
}

void AddSC_custom_npc_professionmaster()
{
    Script *newscript;

	newscript                   = new Script;
    newscript->Name             = "custom_npc_professionmaster";
    newscript->pGossipHello     = &GossipHello_custom_npc_professionmaster;
    newscript->pGossipSelect    = &GossipSelect_custom_npc_professionmaster;
    newscript->RegisterSelf();
}