/* Copyright (C) 2009 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_Chromaggus
SD%Complete: 95
SDComment: Chromatic Mutation disabled due to lack of core support
SDCategory: Blackwing Lair
EndScriptData */

#include "precompiled.h"
#include "blackwing_lair.h"

enum
{
    EMOTE_GENERIC_FRENZY_KILL   = -1000001,
    EMOTE_SHIMMER               = -1469003,

    //These spells are actually called elemental shield
    //What they do is decrease all damage by 75% then they increase
    //One school of damage by 1100%
    SPELL_FIRE_VURNALBILTY      = 22277,
    SPELL_FROST_VURNALBILTY     = 22278,
    SPELL_SHADOW_VURNALBILTY    = 22279,
    SPELL_NATURE_VURNALBILTY    = 22280,
    SPELL_ARCANE_VURNALBILTY    = 22281,

    SPELL_INCINERATE            = 23308,                    //Incinerate 23308,23309
    SPELL_TIMELAPSE             = 23310,                    //Time lapse 23310, 23311(old threat mod that was removed in 2.01)
    SPELL_CORROSIVEACID         = 23313,                    //Corrosive Acid 23313, 23314
    SPELL_IGNITEFLESH           = 23315,                    //Ignite Flesh 23315,23316
    SPELL_FROSTBURN             = 23187,                    //Frost burn 23187, 23189

    //Brood Affliction 23173 - Scripted Spell that cycles through all targets within 100 yards and has a chance to cast one of the afflictions on them
    //Since Scripted spells arn't coded I'll just write a function that does the same thing
    SPELL_BROODAF_BLUE          = 23153,                    //Blue affliction 23153
    SPELL_BROODAF_BLACK         = 23154,                    //Black affliction 23154
    SPELL_BROODAF_RED           = 23155,                    //Red affliction 23155 (23168 on death)
    SPELL_BROODAF_BRONZE        = 23170,                    //Bronze Affliction  23170
    SPELL_BROODAF_GREEN         = 23169,                    //Brood Affliction Green 23169

    SPELL_CHROMATIC_MUT_1       = 23174,                    //Spell cast on player if they get all 5 debuffs

    SPELL_FRENZY                = 28371,                    //The frenzy spell may be wrong
    SPELL_ENRAGE                = 28747
};

struct MANGOS_DLL_DECL boss_chromaggusAI : public ScriptedAI
{
    boss_chromaggusAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        //Select the 2 breaths that we are going to use until despawned
        //5 possiblities for the first breath, 4 for the second, 20 total possiblites
        //This way we don't end up casting 2 of the same breath
        //TL TL would be stupid
        srand(time(NULL));
        switch(urand(0, 19))
        {
            //B1 - Incin
            case 0:
                Breath1_Spell = SPELL_INCINERATE;
                Breath2_Spell = SPELL_TIMELAPSE;
                break;
            case 1:
                Breath1_Spell = SPELL_INCINERATE;
                Breath2_Spell = SPELL_CORROSIVEACID;
                break;
            case 2:
                Breath1_Spell = SPELL_INCINERATE;
                Breath2_Spell = SPELL_IGNITEFLESH;
                break;
            case 3:
                Breath1_Spell = SPELL_INCINERATE;
                Breath2_Spell = SPELL_FROSTBURN;
                break;

                //B1 - TL
            case 4:
                Breath1_Spell = SPELL_TIMELAPSE;
                Breath2_Spell = SPELL_INCINERATE;
                break;
            case 5:
                Breath1_Spell = SPELL_TIMELAPSE;
                Breath2_Spell = SPELL_CORROSIVEACID;
                break;
            case 6:
                Breath1_Spell = SPELL_TIMELAPSE;
                Breath2_Spell = SPELL_IGNITEFLESH;
                break;
            case 7:
                Breath1_Spell = SPELL_TIMELAPSE;
                Breath2_Spell = SPELL_FROSTBURN;
                break;

                //B1 - Acid
            case 8:
                Breath1_Spell = SPELL_CORROSIVEACID;
                Breath2_Spell = SPELL_INCINERATE;
                break;
            case 9:
                Breath1_Spell = SPELL_CORROSIVEACID;
                Breath2_Spell = SPELL_TIMELAPSE;
                break;
            case 10:
                Breath1_Spell = SPELL_CORROSIVEACID;
                Breath2_Spell = SPELL_IGNITEFLESH;
                break;
            case 11:
                Breath1_Spell = SPELL_CORROSIVEACID;
                Breath2_Spell = SPELL_FROSTBURN;
                break;

                //B1 - Ignite
            case 12:
                Breath1_Spell = SPELL_IGNITEFLESH;
                Breath2_Spell = SPELL_INCINERATE;
                break;
            case 13:
                Breath1_Spell = SPELL_IGNITEFLESH;
                Breath2_Spell = SPELL_CORROSIVEACID;
                break;
            case 14:
                Breath1_Spell = SPELL_IGNITEFLESH;
                Breath2_Spell = SPELL_TIMELAPSE;
                break;
            case 15:
                Breath1_Spell = SPELL_IGNITEFLESH;
                Breath2_Spell = SPELL_FROSTBURN;
                break;

                //B1 - Frost
            case 16:
                Breath1_Spell = SPELL_FROSTBURN;
                Breath2_Spell = SPELL_INCINERATE;
                break;
            case 17:
                Breath1_Spell = SPELL_FROSTBURN;
                Breath2_Spell = SPELL_TIMELAPSE;
                break;
            case 18:
                Breath1_Spell = SPELL_FROSTBURN;
                Breath2_Spell = SPELL_CORROSIVEACID;
                break;
            case 19:
                Breath1_Spell = SPELL_FROSTBURN;
                Breath2_Spell = SPELL_IGNITEFLESH;
                break;
        };

        EnterEvadeMode();
    }

    ScriptedInstance* m_pInstance;

    uint32 Breath1_Spell;
    uint32 Breath2_Spell;
    uint32 CurrentVurln_Spell;

    uint32 Shimmer_Timer;
    uint32 Breath1_Timer;
    uint32 Breath2_Timer;
    uint32 Affliction_Timer;
    uint32 Frenzy_Timer;
    bool Enraged;

    void Reset()
    {
        CurrentVurln_Spell = 0;                             //We use this to store our last vurlnability spell so we can remove it later

        Shimmer_Timer = 0;                                  //Time till we change vurlnerabilites
        Breath1_Timer = 30000;                              //First breath is 30 seconds
        Breath2_Timer = 60000;                              //Second is 1 minute so that we can alternate
        Affliction_Timer = 10000;                           //This is special - 5 seconds means that we cast this on 1 pPlayer every 5 sconds
        Frenzy_Timer = 15000;

        Enraged = false;
    }

    void JustDied(Unit*)
    {
	    m_pInstance->SetData(TYPE_CHROMAGGUS,DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //Shimmer_Timer Timer
        if (Shimmer_Timer < diff)
        {
            //Remove old vurlnability spell
            if (CurrentVurln_Spell)
                m_creature->RemoveAurasDueToSpell(CurrentVurln_Spell);

            //Cast new random vurlnabilty on self
            uint32 spell;
            switch(urand(0, 4))
            {
                case 0: spell = SPELL_FIRE_VURNALBILTY; break;
                case 1: spell = SPELL_FROST_VURNALBILTY; break;
                case 2: spell = SPELL_SHADOW_VURNALBILTY; break;
                case 3: spell = SPELL_NATURE_VURNALBILTY; break;
                case 4: spell = SPELL_ARCANE_VURNALBILTY; break;
            }

            if (DoCastSpellIfCan(m_creature, spell) == CAST_OK)
            {
                CurrentVurln_Spell = spell;

                DoScriptText(EMOTE_SHIMMER, m_creature);
                Shimmer_Timer = 45000;
            }
        }else Shimmer_Timer -= diff;

        //Breath1_Timer
        if (Breath1_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),Breath1_Spell);
            Breath1_Timer = 60000;
        }else Breath1_Timer -= diff;

        //Breath2_Timer
        if (Breath2_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),Breath2_Spell);
            Breath2_Timer = 60000;
        }else Breath2_Timer -= diff;

        //Affliction_Timer
        if (Affliction_Timer < diff)
        {
            uint32 SpellAfflict = 0;

            switch(urand(0, 4))
            {
                case 0: SpellAfflict = SPELL_BROODAF_BLUE; break;
                case 1: SpellAfflict = SPELL_BROODAF_BLACK; break;
                case 2: SpellAfflict = SPELL_BROODAF_RED; break;
                case 3: SpellAfflict = SPELL_BROODAF_BRONZE; break;
                case 4: SpellAfflict = SPELL_BROODAF_GREEN; break;
            }

            ThreatList const& tList = m_creature->getThreatManager().getThreatList();
            for (ThreatList::const_iterator i = tList.begin();i != tList.end(); ++i)
            {
                Unit* pUnit = NULL;
                pUnit = Unit::GetUnit((*m_creature), (*i)->getUnitGuid());

                if (pUnit)
                {
                    //Cast affliction
                    DoCastSpellIfCan(pUnit, SpellAfflict, true);

                    //Chromatic mutation if target is effected by all afflictions
                    if (pUnit->HasAura(SPELL_BROODAF_BLUE, EFFECT_INDEX_0)
                        && pUnit->HasAura(SPELL_BROODAF_BLACK, EFFECT_INDEX_0)
                        && pUnit->HasAura(SPELL_BROODAF_RED, EFFECT_INDEX_0)
                        && pUnit->HasAura(SPELL_BROODAF_BRONZE, EFFECT_INDEX_0)
                        && pUnit->HasAura(SPELL_BROODAF_GREEN, EFFECT_INDEX_0))
                    {
                        //target->RemoveAllAuras();
                        //DoCastSpellIfCan(target,SPELL_CHROMATIC_MUT_1);

                        //Chromatic mutation is causing issues
                        //Assuming it is caused by a lack of core support for Charm
                        //So instead we instant kill our target

                        //WORKAROUND
                        if (pUnit->GetTypeId() == TYPEID_PLAYER)
                            pUnit->CastSpell(pUnit, 5, false);
                    }
                }
            }

            Affliction_Timer = 10000;
        }else Affliction_Timer -= diff;

        //Frenzy_Timer
        if (Frenzy_Timer < diff)
        {
            if (DoCastSpellIfCan(m_creature,SPELL_FRENZY) == CAST_OK)
            {
                DoScriptText(EMOTE_GENERIC_FRENZY_KILL, m_creature);
                Frenzy_Timer = urand(10000, 15000);
            }
        }else Frenzy_Timer -= diff;

        //Enrage if not already enraged and below 20%
        if (!Enraged && m_creature->GetHealthPercent() < 20.0f)
        {
            DoCastSpellIfCan(m_creature,SPELL_ENRAGE);
            Enraged = true;
        }

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_chromaggus(Creature* pCreature)
{
    return new boss_chromaggusAI(pCreature);
}

void AddSC_boss_chromaggus()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_chromaggus";
    newscript->GetAI = &GetAI_boss_chromaggus;
    newscript->RegisterSelf();
}
