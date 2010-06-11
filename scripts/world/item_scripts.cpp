/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Item_Scripts
SD%Complete: 100
SDComment: Items for a range of different items. See content below (in script)
SDCategory: Items
EndScriptData */

/* ContentData
item_arcane_charges                 Prevent use if player is not flying (cannot cast while on ground)
item_nether_wraith_beacon(i31742)   Summons creatures for quest Becoming a Spellfire Tailor (q10832)
item_flying_machine(i34060,i34061)  Engineering crafted flying machines
item_gor_dreks_ointment(i30175)     Protecting Our Own(q10488)
EndContentData */

#include "precompiled.h"
#include "Spell.h"

/*#####
# item_arcane_charges
#####*/

enum
{
    SPELL_ARCANE_CHARGES    = 45072
};

bool ItemUse_item_arcane_charges(Player* pPlayer, Item* pItem, const SpellCastTargets &pTargets)
{
    if (pPlayer->isInFlight())
        return false;

    pPlayer->SendEquipError(EQUIP_ERR_NONE, pItem, NULL);

    if (const SpellEntry* pSpellInfo = GetSpellStore()->LookupEntry(SPELL_ARCANE_CHARGES))
        Spell::SendCastResult(pPlayer, pSpellInfo, 1, SPELL_FAILED_NOT_ON_GROUND);

    return true;
}

/*#####
# item_nether_wraith_beacon
#####*/

bool ItemUse_item_nether_wraith_beacon(Player* pPlayer, Item* pItem, const SpellCastTargets &pTargets)
{
    if (pPlayer->GetQuestStatus(10832) == QUEST_STATUS_INCOMPLETE)
    {
        pPlayer->SummonCreature(22408,pPlayer->GetPositionX() ,pPlayer->GetPositionY()+20, pPlayer->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,180000);
        pPlayer->SummonCreature(22408,pPlayer->GetPositionX() ,pPlayer->GetPositionY()-20, pPlayer->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,180000);
    }
    return false;
}

/*#####
# item_flying_machine
#####*/

bool ItemUse_item_flying_machine(Player* pPlayer, Item* pItem, const SpellCastTargets &pTargets)
{
    uint32 itemId = pItem->GetEntry();

    if (itemId == 34060)
        if (pPlayer->GetBaseSkillValue(SKILL_RIDING) >= 225)
            return false;

    if (itemId == 34061)
        if (pPlayer->GetBaseSkillValue(SKILL_RIDING) == 300)
            return false;

    debug_log("SD2: Player attempt to use item %u, but did not meet riding requirement",itemId);
    pPlayer->SendEquipError(EQUIP_ERR_CANT_EQUIP_SKILL, pItem, NULL);
    return true;
}

/*#####
# item_gor_dreks_ointment
#####*/

enum
{
    NPC_TH_DIRE_WOLF        = 20748,
    SPELL_GORDREKS_OINTMENT = 32578
};

bool ItemUse_item_gor_dreks_ointment(Player* pPlayer, Item* pItem, const SpellCastTargets &pTargets)
{
    if (pTargets.getUnitTarget() && pTargets.getUnitTarget()->GetTypeId() == TYPEID_UNIT && pTargets.getUnitTarget()->HasAura(SPELL_GORDREKS_OINTMENT))
    {
        pPlayer->SendEquipError(EQUIP_ERR_NONE, pItem, NULL);

        if (const SpellEntry* pSpellInfo = GetSpellStore()->LookupEntry(SPELL_GORDREKS_OINTMENT))
            Spell::SendCastResult(pPlayer, pSpellInfo, 1, SPELL_FAILED_TARGET_AURASTATE);

        return true;
    }

    return false;
}

/*#####
# item_petrov_cluster_bombs
#####*/

enum
{
    SPELL_PETROV_BOMB           = 42406,
    AREA_ID_SHATTERED_STRAITS   = 4064,
    ZONE_ID_HOWLING             = 495
};

bool ItemUse_item_petrov_cluster_bombs(Player* pPlayer, Item* pItem, const SpellCastTargets &pTargets)
{
    if (pPlayer->GetZoneId() != ZONE_ID_HOWLING)
        return false;

    if (!pPlayer->GetTransport() || pPlayer->GetAreaId() != AREA_ID_SHATTERED_STRAITS)
    {
        pPlayer->SendEquipError(EQUIP_ERR_NONE, pItem, NULL);

        if (const SpellEntry* pSpellInfo = GetSpellStore()->LookupEntry(SPELL_PETROV_BOMB))
            Spell::SendCastResult(pPlayer, pSpellInfo, 1, SPELL_FAILED_NOT_HERE);

        return true;
    }

    return false;
}

/*#####
# item_horn_of_the_frostbrood
#####*/

enum
{
    QUEST_AN_END_TO_ALL_THINGS  = 12779,
	NPC_FROSTBROOD_VANQUISHER   = 28670,
	SPELL_FROST_MIST            = 29292,
};

#define POS_X                1625.0f
#define POS_Y                -5835.0f
#define POS_Z                180.0f

bool ItemUse_item_horn_of_the_frostbrood(Player* pPlayer, Item* pItem, const SpellCastTargets &pTargets)
{
    if (pPlayer->GetQuestStatus(QUEST_AN_END_TO_ALL_THINGS) != QUEST_STATUS_INCOMPLETE)
	    return false;
	
	if (Creature* pTemp = pPlayer->SummonCreature(NPC_FROSTBROOD_VANQUISHER, pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ() + 15.0f, pPlayer->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN, 20000)) 
    {
	    pTemp->CastSpell(pTemp, SPELL_FROST_MIST, true);
		pTemp->CastSpell(pPlayer, SPELL_FROST_MIST, true);
		pTemp->SetSpeedRate(MOVE_FLIGHT, 6.0f, true);
		pTemp->GetMotionMaster()->MovePoint(0, POS_X, POS_Y, POS_Z);
		pPlayer->SetQuestStatus(QUEST_AN_END_TO_ALL_THINGS, QUEST_STATUS_COMPLETE);
		pPlayer->CompleteQuest(QUEST_AN_END_TO_ALL_THINGS);
		
		return true;
	}
}	

/*#####
# item_jungle_punch_sample
#####*/

enum
{
    SPELL_OFFER_JUNGLE_PUNCH = 51962
};

bool ItemUse_item_jungle_punch_sample(Player* pPlayer, Item* pItem, const SpellCastTargets &pTargets)
{
   Unit* pTarget = Unit::GetUnit(*pPlayer, pPlayer->GetTargetGUID());
   if (pTarget && pTarget->GetTypeId() == TYPEID_UNIT)
   {
      pPlayer->CastSpell(pTarget, SPELL_OFFER_JUNGLE_PUNCH, false);
      return true;
   }
   else
   {
      pPlayer->SendEquipError(EQUIP_ERR_NONE, pItem, NULL);
      return true;
   }  
}

void AddSC_item_scripts()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "item_arcane_charges";
    newscript->pItemUse = &ItemUse_item_arcane_charges;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "item_nether_wraith_beacon";
    newscript->pItemUse = &ItemUse_item_nether_wraith_beacon;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "item_flying_machine";
    newscript->pItemUse = &ItemUse_item_flying_machine;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "item_gor_dreks_ointment";
    newscript->pItemUse = &ItemUse_item_gor_dreks_ointment;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "item_petrov_cluster_bombs";
    newscript->pItemUse = &ItemUse_item_petrov_cluster_bombs;
    newscript->RegisterSelf();
	
	newscript = new Script;
	newscript->Name = "item_jungle_punch_sample";
	newscript->pItemUse = &ItemUse_item_horn_of_the_frostbrood;
	newscript->RegisterSelf();
	
	newscript = new Script;
	newscript->Name = "item_horn_of_the_frostbrood";
	newscript->pItemUse = &ItemUse_item_horn_of_the_frostbrood;
	newscript->RegisterSelf();
}
