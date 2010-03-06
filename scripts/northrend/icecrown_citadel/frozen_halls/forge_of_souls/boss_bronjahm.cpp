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
SDName: boss_bronjahm
SD%Complete: 0%
SDComment:
SDCategory: The Forge of Souls
EndScriptData */

#include "precompiled.h"
#include "forge_of_souls.h"

enum {
    SPELL_MAGICS_BANE_N         = 68793,
    SPELL_MAGICS_BANE_H         = 69050,
    SPELL_CORRUPT_SOUL_VISUAL   = 68839,
    //SPELL_DRAW_CORRUPTED_SOUL   = 68846,
    SPELL_CONSUME_SOUL_N        = 68858,
    SPELL_CONSUME_SOUL_H        = 69047,
    SPELL_SHADOW_BOLT           = 70043,
    SPELL_TELEPORT              = 68988,
    SPELL_SOULSTORM_N           = 68872,
    SPELL_SOULSTORM_SINGLE_TICK = 69049,
    SPELL_SOULSTORM_CASTBAR     = 68912,
    SPELL_FEAR                  = 68950,

    SPELL_SOULSTORM_VISUAL_LEFT  = 68907,
    SPELL_SOULSTORM_VISUAL_BACK  = 68906,
    SPELL_SOULSTORM_VISUAL_RIGHT = 68905,

    SAY_AGGRO1       = -1730001,
    SAY_AGGRO2       = -1730002,
    SAY_CORRUPT_SOUL = -1730003,
    SAY_SOULSTORM    = -1730004,
    SAY_KILL1        = -1730005,
    SAY_KILL2        = -1730006,
    SAY_DEATH        = -1730007,

    NPC_CORRUPTED_SOUL_FRAGMENT = 36535
};

struct MANGOS_DLL_DECL boss_bronjahmAI : public ScriptedAI
{
    boss_bronjahmAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiMagicsBaneTimer;
    uint32 m_uiCorruptSoulTimer;
    //uint32 m_uiConsumeSoulTimer;
    uint32 m_uiShadowBoltTimer;
    uint32 m_uiFearTimer;
    uint32 m_uiSummonTimer;

    Unit* pCorrSoulTarget;
    Creature* pCorruptSoul;
    bool m_bPhase2;
    bool m_bHadCastCorrSoul;

    void Reset()
    {
        m_uiMagicsBaneTimer = 6500;
        m_uiCorruptSoulTimer = 45000;
        m_uiShadowBoltTimer = 10000;
        m_uiFearTimer = 10000;
        m_uiSummonTimer = 4000;

        m_bPhase2 = false;
        m_bHadCastCorrSoul = false;
        pCorruptSoul = 0;
        pCorrSoulTarget = 0;
    }

    void Aggro(Unit* pWho)
    {
        switch(urand(0, 1))
        {
            case 0: DoScriptText(SAY_AGGRO1, m_creature); break;
            case 1: DoScriptText(SAY_AGGRO2, m_creature); break;
        }

        if (m_pInstance)
            m_pInstance->SetData(TYPE_BRONJAHM, IN_PROGRESS);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 1))
        {
            case 0: DoScriptText(SAY_KILL1, m_creature); break;
            case 1: DoScriptText(SAY_KILL2, m_creature); break;
        }
    }
    
    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_BRONJAHM, DONE);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
        
        if (m_creature->GetHealthPercent() < 30.0f && !m_bPhase2)
            m_bPhase2 = true;

        if (!m_bPhase2)
        {
            pCorrSoulTarget = SelectUnit(SELECT_TARGET_RANDOM, 0);

            // first phase
            if (m_uiMagicsBaneTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_MAGICS_BANE_N : SPELL_MAGICS_BANE_H);
                m_uiMagicsBaneTimer = 6500;
            } else
                m_uiMagicsBaneTimer -= uiDiff;

            if (m_uiCorruptSoulTimer < uiDiff)
            {
                DoCastSpellIfCan(pCorrSoulTarget, SPELL_CORRUPT_SOUL_VISUAL);
                m_bHadCastCorrSoul = true;

                m_uiCorruptSoulTimer = 45000;
            } else
                m_uiCorruptSoulTimer -= uiDiff;

            if (m_uiSummonTimer < uiDiff && m_bHadCastCorrSoul)
            {
                pCorruptSoul = m_creature->SummonCreature(NPC_CORRUPTED_SOUL_FRAGMENT, pCorrSoulTarget->GetPositionX(), pCorrSoulTarget->GetPositionY(), pCorrSoulTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 30000);
                pCorruptSoul->GetMotionMaster()->Clear(false);
                pCorruptSoul->GetMotionMaster()->MoveChase(m_creature , 3.0f);
                m_bHadCastCorrSoul = false;

                m_uiSummonTimer = 4000;
            } else
                m_uiSummonTimer -= uiDiff;
            
            if (pCorruptSoul)
                CorruptedAI();

            if (m_uiShadowBoltTimer < uiDiff)
            {
                DoCastSpellIfCan(SelectUnit(SELECT_TARGET_BOTTOMAGGRO, 0), SPELL_SHADOW_BOLT);
                m_uiShadowBoltTimer = 10000;
            } else
                m_uiShadowBoltTimer -= uiDiff;
        }
        
        DoMeleeAttackIfReady();
    }
    // TODO write AI for Corrupted Soul
    void CorruptedAI()
    {
        if (float distanceToSoul = m_creature->GetDistance2d((WorldObject*)pCorruptSoul) <= 5.0f)
        {
            DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_CONSUME_SOUL_N : SPELL_CONSUME_SOUL_H);
            pCorruptSoul->RemoveFromWorld();
            pCorruptSoul = 0;
        }
    }
};

CreatureAI* GetAI_boss_bronjahm(Creature* pCreature)
{
    return new boss_bronjahmAI(pCreature);
}

void AddSC_boss_bronjahm()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "boss_bronjahm";
    NewScript->GetAI = &GetAI_boss_bronjahm;
    NewScript->RegisterSelf();
}