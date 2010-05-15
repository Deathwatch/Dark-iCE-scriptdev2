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
SDName: Boss_Noth
SD%Complete: 90%
SDComment:
SDCategory: Naxxramas
SDAuthot: ScrappyDoo (c) Andeeria
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    SAY_AGGRO1                          = -1533075,
    SAY_AGGRO2                          = -1533076,
    SAY_AGGRO3                          = -1533077,
    SAY_SUMMON                          = -1533078,
    SAY_SLAY1                           = -1533079,
    SAY_SLAY2                           = -1533080,
    SAY_DEATH                           = -1533081,

    SPELL_BLINK                         = 29211,            //29208, 29209 and 29210 too
    SPELL_CRIPPLE                       = 29212,
    SPELL_CRIPPLE_H                     = 54814,
    SPELL_CURSE_PLAGUEBRINGER           = 29213,
    SPELL_CURSE_PLAGUEBRINGER_H         = 54835,
    SPELL_ENRAGE                        = 28131,

    SPELL_SUMMON_CHAMPION_AND_CONSTRUCT = 29240,
    SPELL_SUMMON_GUARDIAN_AND_CONSTRUCT = 29269,

    NPC_PLAGUED_WARRIOR                 = 16984,
    NPC_PLAGUED_CHAMPION                = 16983,
    NPC_PLAGUED_GUARDIAN                = 16981
};

// Teleport position of Noth on his balcony
double fBalcony[4] =
{
    2634.1916,
    -3529.829,
    274.035,
    0.0614
};

struct MANGOS_DLL_DECL boss_nothAI : public ScriptedAI
{
    boss_nothAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;
    bool m_bIsBalconyPhase;
    float fGroundPos;

    uint32 m_uiBalconyTimer;
    uint32 m_uiBlinkTimer;
    uint32 m_uiCurseTimer;
    uint32 m_uiSummonTimer;
    uint8  m_uiPhaseCount;

    void Reset()
    {
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_bIsBalconyPhase = false;
        m_uiBalconyTimer = 110000;
        m_uiBlinkTimer = 25000;
        m_uiCurseTimer = 4000;
        m_uiSummonTimer = 12000;
        m_uiPhaseCount = 0;

        if (m_pInstance)
            m_pInstance->SetData(TYPE_NOTH, NOT_STARTED);
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
            m_pInstance->SetData(TYPE_NOTH, IN_PROGRESS);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            pSummoned->AddThreat(pTarget);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1)?SAY_SLAY1:SAY_SLAY2, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_NOTH, DONE);
    }

    void SummonAdds()
    {
        uint32 ID = NPC_PLAGUED_CHAMPION;
        for(uint8 i=0; i<(m_bIsRegularMode ? 4 : 8); ++i)
        {
            if(m_uiPhaseCount == 1 && i<(m_bIsRegularMode ? 2 : 4))
                ID = NPC_PLAGUED_GUARDIAN;
            if(m_uiPhaseCount == 2)
                ID = NPC_PLAGUED_GUARDIAN;

            if (Creature* pUndead = m_creature->SummonCreature(ID, m_creature->GetPositionX()+urand(1,10), m_creature->GetPositionY()+urand(1,10), fGroundPos+1, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
                if (Unit* pPlayer = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    pUndead->AI()->AttackStart(pPlayer);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_uiPhaseCount == 3)
        {
            m_creature->CastSpell(m_creature, SPELL_ENRAGE, false);
            m_bIsBalconyPhase = false;
            m_uiBalconyTimer = 900000;
            ++m_uiPhaseCount;
        }

        if(m_bIsBalconyPhase)
        {
            m_creature->StopMoving();
            m_creature->GetMotionMaster()->Clear();
            m_creature->GetMotionMaster()->MoveIdle();

            if(m_uiBalconyTimer < uiDiff)
            {
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                ++m_uiPhaseCount;
                m_uiSummonTimer = 15000;
                m_uiCurseTimer = 10000;
                m_uiBlinkTimer = 5000;
                m_creature->GetMap()->CreatureRelocation(m_creature, fBalcony[0], fBalcony[1], fGroundPos, fBalcony[3]);
                if(m_creature->getVictim())
                    m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                m_bIsBalconyPhase = false;
                m_uiBalconyTimer = 110000;
            }else m_uiBalconyTimer -= uiDiff;
        }
        else
        {
            if(m_uiBalconyTimer < uiDiff)
            {
                fGroundPos = m_creature->GetPositionZ();
                SummonAdds();
                m_creature->GetMap()->CreatureRelocation(m_creature, fBalcony[0], fBalcony[1], fBalcony[2], fBalcony[3]);
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                m_bIsBalconyPhase = true;
                m_uiBalconyTimer = 70000;
            }else m_uiBalconyTimer -= uiDiff;

            // Blink
            if (m_uiBlinkTimer < uiDiff)
            {
                
                DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_CRIPPLE : SPELL_CRIPPLE_H);
                //DoCast(m_creature, SPELL_BLINK);
                m_creature->GetMap()->CreatureRelocation(m_creature, m_creature->GetPositionX()+urand(5,15), m_creature->GetPositionY()+urand(5,15), m_creature->GetPositionZ(), 0);
                if(Unit* pPlayer = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    m_creature->AI()->AttackStart(pPlayer);

                m_uiBlinkTimer = 20000;
            }else m_uiBlinkTimer -= uiDiff;

            // Curse
            if (m_uiCurseTimer < uiDiff)
            {
                for(uint8 i=0; i<(m_bIsRegularMode ? 3 : 10); ++i)
                    if(Unit* pPlayer = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        m_creature->CastSpell(pPlayer, m_bIsRegularMode ? SPELL_CURSE_PLAGUEBRINGER : SPELL_CURSE_PLAGUEBRINGER_H, true);
                m_uiCurseTimer = 11000;
            }else m_uiCurseTimer -= uiDiff;

            // Summon
            if (m_uiSummonTimer < uiDiff)
            {
                DoScriptText(SAY_SUMMON, m_creature);
                for(uint8 i=0; i<(m_bIsRegularMode ? 4 : 6); ++i)
                {
                    Creature* pUndead = m_creature->SummonCreature(NPC_PLAGUED_WARRIOR, m_creature->GetPositionX()+urand(1,10), m_creature->GetPositionY()+urand(1,10), m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                    if(Unit* pPlayer = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        pUndead->AI()->AttackStart(pPlayer);
                }
                m_uiSummonTimer = 21000;
            }else m_uiSummonTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
    }
};

CreatureAI* GetAI_boss_noth(Creature* pCreature)
{
    return new boss_nothAI(pCreature);
}

void AddSC_boss_noth()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "boss_noth";
    NewScript->GetAI = &GetAI_boss_noth;
    NewScript->RegisterSelf();
}
