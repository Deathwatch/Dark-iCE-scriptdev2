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
SDName: Boss_Gothik
SD%Complete: 90
SDComment:  
SDCategory: Naxxramas
SDAuthor: ScrappyDoo (c) Andeeria /dev/FallenAngelX
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    SAY_SPEECH                = -1533040,
    SAY_KILL                  = -1533041,
    SAY_DEATH                 = -1533042,
    SAY_TELEPORT              = -1533043,

    //Gothik
    SPELL_HARVESTSOUL         = 28679,
    SPELL_SHADOWBOLT          = 29317,
    SPELL_SHADOWBOLT_H        = 56405,

    CREATURE_TRAINEE            = 16124,
    CREATURE_DEATCHKNIGHT       = 16125,
    CREATURE_RIDER              = 16126,
    CREATURE_SPEC_TRAINEE       = 16127,
    CREATURE_SPEC_DEATCHKNIGHT  = 16148,
    CREATURE_SPEC_RIDER         = 16150,
    CREATURE_SPEC_HORSE         = 16149
};

uint32 m_uiAddsList[3] = 
{
    CREATURE_TRAINEE, CREATURE_DEATCHKNIGHT, CREATURE_RIDER
};

uint32 m_uiUndeadList[3] = 
{
    CREATURE_SPEC_TRAINEE, CREATURE_SPEC_DEATCHKNIGHT, CREATURE_SPEC_RIDER
};

double  fAddsPos[2][3] = 
{
    {2692.4526, -3420.8408, 267.688},
    {2689.0485, -3300.6027, 267.683}
};

struct MANGOS_DLL_DECL boss_gothikAI : public ScriptedAI
{
    boss_gothikAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;
    bool m_bIsSecondPhase;
    bool m_bIsGateOpen;

    uint32 m_uiSwitchPhaseTimer;
    uint32 m_uiShadowBoltTimer;
    uint32 m_uiHavestSoulTimer;
    uint32 m_uiSpawnAddsTimer;
    uint8  m_uiPhaseSide;

    uint64 m_uiLiveAddsGUID[70][2];
    uint8  m_uiMaxAdds;

    void Reset()
    {
        //Phase1
        m_bIsGateOpen           = false;
        m_bIsSecondPhase        = false;
        m_uiShadowBoltTimer     = 5000;
        m_uiHavestSoulTimer     = 15000;
        m_uiPhaseSide           = 0;

        //Phase2
        m_uiSpawnAddsTimer      = 5000;
        m_uiSwitchPhaseTimer    = 274000;
        m_uiMaxAdds             = 0;

        if (m_pInstance)
            m_pInstance->SetData(TYPE_GOTHIK, NOT_STARTED);

        for (uint8 i=0; i<70; ++i)
        {
            m_uiLiveAddsGUID[i][1] = 0;
            m_uiLiveAddsGUID[i][2] = 0;
        }

        if (GameObject* pCombatDoor = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(GO_MILI_GOTH_COMBAT_GATE)))
                pCombatDoor->SetGoState(GO_STATE_ACTIVE);
    }

    void KilledUnit(Unit* Victim)
    {
        DoScriptText(SAY_KILL, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_GOTHIK, DONE);
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        DoScriptText(SAY_SPEECH, m_creature);

        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_GOTHIK, IN_PROGRESS);
            m_pInstance->SetData(TYPE_GOTHIK, SPECIAL);
        }
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (Unit* pPlayer = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
            pSummoned->AI()->AttackStart(pPlayer);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        m_creature->StopMoving();
        m_creature->GetMotionMaster()->Clear();
        m_creature->GetMotionMaster()->MoveIdle();

        //when sumoned add died we summon undead for replace 
        for (uint8 i=0; i<m_uiMaxAdds; ++i)
        {
            Unit* pLive = Unit::GetUnit(*m_creature, m_uiLiveAddsGUID[i][1]);
            if(pLive && !pLive->isAlive())
            {
                if (m_uiLiveAddsGUID[i][2] == 2)
                    m_creature->SummonCreature(CREATURE_SPEC_HORSE, fAddsPos[1][0], fAddsPos[1][1], fAddsPos[1][2], m_creature->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);

                Creature* pUndeadAdd = m_creature->SummonCreature(m_uiUndeadList[m_uiLiveAddsGUID[i][2]], fAddsPos[1][0], fAddsPos[1][1], fAddsPos[1][2], m_creature->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);

                m_uiLiveAddsGUID[i][1] = 0;
                m_uiLiveAddsGUID[i][2] = 0;
            }
        }

        if (m_bIsSecondPhase)
        {
            if (m_uiSwitchPhaseTimer < uiDiff)
            {
                if(m_uiPhaseSide == 0)
                    m_uiPhaseSide = 1;
                else m_uiPhaseSide = 0;

                m_creature->GetMap()->CreatureRelocation(m_creature, fAddsPos[m_uiPhaseSide][0], fAddsPos[m_uiPhaseSide][1], fAddsPos[m_uiPhaseSide][2], m_creature->GetOrientation());

                m_uiSwitchPhaseTimer = 15000;
            }else m_uiSwitchPhaseTimer -= uiDiff;

            if ((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 30 && !m_bIsGateOpen)
            {
                if (m_pInstance)
                    m_pInstance->SetData(TYPE_GOTHIK, SPECIAL);
                m_bIsGateOpen = true;
            }

            if (m_uiHavestSoulTimer < uiDiff)
            {
                if (m_creature->getVictim())
                    m_creature->CastSpell(m_creature->getVictim(), SPELL_HARVESTSOUL, false);
               
                m_uiHavestSoulTimer = 14000;
            }else m_uiHavestSoulTimer -= uiDiff;

            if (m_uiShadowBoltTimer < uiDiff)
            {
                if (m_creature->getVictim())
                    m_creature->CastSpell(m_creature->getVictim(), m_bIsRegularMode ? SPELL_SHADOWBOLT : SPELL_SHADOWBOLT_H, false);

                m_uiShadowBoltTimer = 1000;
            }else m_uiShadowBoltTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
        else
        {
            //spawn add and get guid for future checking
            if (m_uiSpawnAddsTimer < uiDiff)
            {
                if (m_uiMaxAdds > 68)
                    m_uiMaxAdds = 0;

                if (Creature* pLiveAdd = m_creature->SummonCreature(m_uiAddsList[2], fAddsPos[0][0], fAddsPos[0][1], fAddsPos[0][2], m_creature->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
                {
                    m_uiLiveAddsGUID[m_uiMaxAdds][1] = pLiveAdd->GetGUID();
                    m_uiLiveAddsGUID[m_uiMaxAdds][2] = 2;
                    ++m_uiMaxAdds;
                }

                for (uint8 i=0; i<4; ++i)
                    if (Creature* pLiveAdd = m_creature->SummonCreature(m_uiAddsList[0], fAddsPos[0][0], fAddsPos[0][1], fAddsPos[0][2], m_creature->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
                    {
                        m_uiLiveAddsGUID[m_uiMaxAdds][1] = pLiveAdd->GetGUID();
                        m_uiLiveAddsGUID[m_uiMaxAdds][2] = 0;
                        ++m_uiMaxAdds;
                    }

                for (uint8 i=0; i<2; ++i)
                    if (Creature* pLiveAdd = m_creature->SummonCreature(m_uiAddsList[1], fAddsPos[0][0], fAddsPos[0][1], fAddsPos[0][2], m_creature->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
                    {
                        m_uiLiveAddsGUID[m_uiMaxAdds][1] = pLiveAdd->GetGUID();
                        m_uiLiveAddsGUID[m_uiMaxAdds][2] = 1;
                        ++m_uiMaxAdds;
                    }

                m_uiSpawnAddsTimer = 30000;
            }else m_uiSpawnAddsTimer -= uiDiff;

            if (m_uiSwitchPhaseTimer < uiDiff)
            {
                DoScriptText(SAY_TELEPORT, m_creature);

                m_uiSwitchPhaseTimer = 15000;
                m_uiHavestSoulTimer  = 10000;
                m_uiShadowBoltTimer  = 5000;

                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                m_uiPhaseSide = 1;
                m_creature->GetMap()->CreatureRelocation(m_creature, fAddsPos[1][0], fAddsPos[1][1], fAddsPos[1][2], m_creature->GetOrientation());

                m_bIsSecondPhase = true;
                return;
            }else m_uiSwitchPhaseTimer -= uiDiff;
        } 
    }
};

CreatureAI* GetAI_boss_gothik(Creature* pCreature)
{
    return new boss_gothikAI(pCreature);
}

void AddSC_boss_gothik()
{
    Script* NewScript;

    NewScript = new Script;
    NewScript->Name = "boss_gothik";
    NewScript->GetAI = &GetAI_boss_gothik;
    NewScript->RegisterSelf();
}
