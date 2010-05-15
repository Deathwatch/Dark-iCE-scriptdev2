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
SDName: Boss_Gluth
SD%Complete: 70
SDComment: 
SDCategory: Naxxramas
SDAuthor: ScrappyDoo (c) Andeeria
EndScriptData */

/*ToDo
Devour Zombie -> wybierz zywego zombie  zjedz-> 5%hp max regen
Sprawdzic Spele
*/

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    EMOTE_ZOMBIE      = -1533119,

    SPELL_MORTALWOUND = 25646,
    SPELL_DECIMATE    = 28374,
    SPELL_ENRAGE      = 28371,
    SPELL_ENRAGE_H    = 54427,
    SPELL_BERSERK     = 26662,

    NPC_ZOMBIE_CHOW   = 16360
};

double m_fAddPos[9][3] = 
{
    {3269.590, -3161.287, 297.423},
    {3277.797, -3170.352, 297.423},
    {3267.049, -3172.820, 297.423},
    {3252.157, -3132.135, 297.423},
    {3259.990, -3126.590, 297.423},
    {3259.815, -3137.576, 297.423},
    {3308.030, -3132.135, 297.423},
    {3303.046, -3180.682, 297.423},
    {3313.283, -3180.766, 297.423},
};

struct MANGOS_DLL_DECL boss_gluthAI : public ScriptedAI
{
    boss_gluthAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiMortalWoundTimer;
    uint32 m_uiDecimateTimer;
    uint32 m_uiEnrageTimer;
    uint32 m_uiSummonTimer;
    uint32 m_uiBerserkTimer;
    uint32 m_uiDevourTimer;

    void Reset()
    {
        m_uiDevourTimer         = urand(30000, 60000);
        m_uiMortalWoundTimer    = 8000;
        m_uiDecimateTimer       = (m_bIsRegularMode ? 60000 : 45000);
        m_uiEnrageTimer         = 15000;
        m_uiSummonTimer         = 10000;
        m_uiBerserkTimer        = MINUTE*8*IN_MILLISECONDS;

        if (m_pInstance)
            m_pInstance->SetData(TYPE_GLUTH, NOT_STARTED);
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_GLUTH, DONE);
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_GLUTH, IN_PROGRESS);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiMortalWoundTimer < uiDiff)
        {
            if (m_creature->getVictim())
                m_creature->CastSpell(m_creature->getVictim(), SPELL_MORTALWOUND, false);
            m_uiMortalWoundTimer = 10000;
        }else m_uiMortalWoundTimer -= uiDiff;

        if (m_uiDecimateTimer < uiDiff)
        {
            if (m_creature->getVictim())
                m_creature->CastSpell(m_creature->getVictim(), SPELL_DECIMATE, false);
            m_uiDecimateTimer = (m_bIsRegularMode ? 120000 : 90000);
        }else m_uiDecimateTimer -= uiDiff;

        if (m_uiEnrageTimer < uiDiff)
        {
            m_creature->CastSpell(m_creature, m_bIsRegularMode ? SPELL_ENRAGE : SPELL_ENRAGE_H, false);
            m_uiEnrageTimer = 20000;
        }else m_uiEnrageTimer -= uiDiff;

        if (m_uiDevourTimer < uiDiff)
        {
            DoScriptText(EMOTE_ZOMBIE, m_creature);
            m_uiDevourTimer = urand(30000, 60000);
        }else m_uiDevourTimer -= uiDiff;

        // Summon
        if (m_uiSummonTimer < uiDiff)
        {
            for (uint8 i=0; i<(m_bIsRegularMode ? 1 : 2); ++i)
            {
                uint8 k = urand(0, 8);
                if (Creature* pZombie = m_creature->SummonCreature(NPC_ZOMBIE_CHOW, m_fAddPos[k][0], m_fAddPos[k][1], m_fAddPos[k][2], 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 50000))
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        pZombie->AddThreat(pTarget);
                }
            }

            m_uiSummonTimer = 10000;
        }else m_uiSummonTimer -= uiDiff;

        // Berserk
        if (m_uiBerserkTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_BERSERK, true);
            m_uiBerserkTimer = MINUTE*5*IN_MILLISECONDS;
        }else m_uiBerserkTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_gluth(Creature* pCreature)
{
    return new boss_gluthAI(pCreature);
}

void AddSC_boss_gluth()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "boss_gluth";
    NewScript->GetAI = &GetAI_boss_gluth;
    NewScript->RegisterSelf();
}
