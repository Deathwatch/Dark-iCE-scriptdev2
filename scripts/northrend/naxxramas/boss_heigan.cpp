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
SDName: Boss_Heigan
SD%Complete: 0
SDComment: 
SDCategory: Naxxramas
SDAuthor: ScrappyDoo (c) Andeeria /dev/FallenAngelX
EndScriptData */

/*
event  = pojawiaja sie  na ziemi animacje  ktore  po chwili beda zadawaly w huj dmg  raid musi biegac  na  okolo "dance"
w drugiej fazie  jest szybciej :D
*/

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    SAY_AGGRO1      = -1533109,
    SAY_AGGRO2      = -1533110,
    SAY_AGGRO3      = -1533111,
    SAY_SLAY        = -1533112,
    SAY_TAUNT1      = -1533113,
    SAY_TAUNT2      = -1533114,
    SAY_TAUNT3      = -1533115,
    SAY_TAUNT4      = -1533116,
    SAY_TAUNT5      = -1533117,
    SAY_DEATH       = -1533118,

    SPELL_DISTRUPTION   = 29310, // 300% cast speed 20yard
    SPELL_FEVER         = 29998, // -50%hp and 3k 4,5k dmg co 3 sekundy przez 21 sekund  20yard
    SPELL_FEVER_H       = 55011, // hero
    SPELL_PLAG_CLOUD    = 30122, // phase2 7.5k every 1 second 20yard

    //Spell used by floor peices to cause damage to players
    SPELL_ERUPTION  = 29371,

    //Spells by boss
    SPELL_WILT      = 23772,
};

struct MANGOS_DLL_DECL boss_heiganAI : public ScriptedAI
{
    boss_heiganAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;
    bool m_bIsPhaseOne;

    uint32 m_uiChangePhaseTimer;
    uint32 m_uiDistruptionTimer;
    uint32 m_uiFeverTimer;
    uint32 m_uiEruptionTimer;
    uint8  m_uiEruptionCount;

    void Reset()
    {
        m_uiEruptionCount    = 0;
        m_uiChangePhaseTimer = 90000;
        m_uiDistruptionTimer = 10000;
        m_uiFeverTimer       = 15000;
        m_uiEruptionTimer    = urand(7000,9000);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_HEIGAN, NOT_STARTED);
    }

    void Aggro(Unit* pWho)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_AGGRO1, m_creature); break;
            case 1: DoScriptText(SAY_AGGRO2, m_creature); break;
            case 2: DoScriptText(SAY_AGGRO3, m_creature); break;
        }

        if (m_pInstance)
            m_pInstance->SetData(TYPE_HEIGAN, IN_PROGRESS);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(SAY_SLAY, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_HEIGAN, DONE);
    }

    void Eruption()
    {
        switch(m_uiEruptionCount)
        {
            case 0:
                //aktywuj w  odpowiednich meisjcach chmure
                break;
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
                break;
        }
        if(m_uiEruptionCount == 6)
            m_uiEruptionCount = 0;
        else
            ++m_uiEruptionCount; 
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(!m_bIsPhaseOne)
        {
            m_creature->StopMoving();
            m_creature->GetMotionMaster()->Clear();
            m_creature->GetMotionMaster()->MoveIdle();

            if(m_uiChangePhaseTimer < uiDiff)
            {
                if(m_creature->getVictim())
                    m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                
                m_uiEruptionTimer = urand(7000,9000);
                m_bIsPhaseOne = true;
                m_uiChangePhaseTimer = 900000;
            }else m_uiChangePhaseTimer -= uiDiff;

            // Eruption
            if (m_uiEruptionTimer < uiDiff)
            {
                Eruption();
                m_uiEruptionTimer = urand(2000,3000);
            }else m_uiEruptionTimer -= uiDiff;
        }
        else
        {
            if(m_uiChangePhaseTimer < uiDiff)
            {
                //m_creature->GetMap()->CreatureRelocation(m_creature, fBalcony[0], fBalcony[1], fBalcony[2], fBalcony[3]);
 
                if(m_creature->getVictim())
                    m_creature->CastSpell(m_creature->getVictim(), SPELL_PLAG_CLOUD, false);
                m_uiEruptionTimer = urand(2000,3000);
                m_bIsPhaseOne = false;
                m_uiChangePhaseTimer = 45000;
                return;
            }else m_uiChangePhaseTimer -= uiDiff;

            if (m_uiDistruptionTimer < uiDiff)
            {
                //if(Unit* pPlayer = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                if(m_creature->getVictim())
                    m_creature->CastSpell(m_creature->getVictim(), SPELL_DISTRUPTION, false);

                m_uiDistruptionTimer = urand(15000,20000);
            }else m_uiDistruptionTimer -= uiDiff;

            if (m_uiFeverTimer < uiDiff)
            {
                if(m_creature->getVictim())
                    //m_creature->CastSpell(m_creature->getVictim(), m_bIsRegularMode ? SPELL_FEVER : SPELL_FEVER_H, true);
                m_uiFeverTimer = 16000;
            }else m_uiFeverTimer -= uiDiff;

            // Eruption
            if (m_uiEruptionTimer < uiDiff)
            {
                Eruption();
                m_uiEruptionTimer = urand(7000,9000);
            }else m_uiEruptionTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
    }
};

struct MANGOS_DLL_DECL mob_eruptiontriggerAI : public Scripted_NoMovementAI
{
    mob_eruptiontriggerAI(Creature *c) : Scripted_NoMovementAI(c) { Reset(); }
 
    uint32 m_uiEruptionTimer;

    void Reset()
    {
        m_uiEruptionTimer = 2000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiEruptionTimer < uiDiff)
        {
            if(m_creature->getVictim())
                m_creature->CastSpell(m_creature->getVictim(), SPELL_ERUPTION, false);
            m_uiEruptionTimer = 1000;
        }else m_uiEruptionTimer -= uiDiff;
    }
};

CreatureAI* GetAI_boss_heigan(Creature* pCreature)
{
    return new boss_heiganAI(pCreature);
}

CreatureAI* GetAI_mob_eruptiontrigger(Creature* pCreature)
{
    return new mob_eruptiontriggerAI(pCreature);
}

void AddSC_boss_heigan()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "boss_heigan";
    NewScript->GetAI = &GetAI_boss_heigan;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_eruptiontrigger";
    NewScript->GetAI = &GetAI_mob_eruptiontrigger;
    NewScript->RegisterSelf();
}
