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
SDName: Boss_Grobbulus
SD%Complete: 90
SDComment:
SDCategory: Naxxramas
SDAuthor: ScrappyDoo (c) Andeeria
EndScriptData */

/* ToDo
Poison Cliud
Fallout Slime
*/

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    SPELL_BERSERK           = 26662,
    SPELL_POISON_CLOUD      = 28240,
    SPELL_SLIME_SPRAY       = 28157,
    SPELL_SLIME_SPRAY_H     = 54364,
    SPELL_MUTATING          = 28169,

    CREATURE_FOLLOUT_SLIME  = 16290,
};

struct MANGOS_DLL_DECL boss_grobbulusAI : public ScriptedAI
{
    boss_grobbulusAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool   m_bIsRegularMode;
    bool   m_bIsBerserk;

    uint32 m_uiBerserkTimer;
    uint32 m_uiSlimeSprayTimer;
    uint32 m_uiPoisonCloudTimer;
    uint32 m_uiMutatingTimer;
    uint8  m_uiMutatingCount;

    void Reset()
    {
        m_bIsBerserk         = false;
        m_uiMutatingCount    = 0;
        m_uiMutatingTimer    = 20000;
        m_uiBerserkTimer     = 720000;
        m_uiSlimeSprayTimer  = 10000;
        m_uiPoisonCloudTimer = 15000;                      
    }   

    void Aggro(Unit *who) {}

    void KilledUnit(Unit* Victim) {}

    void JustDied(Unit* Killer) {}

    void JustSummoned(Creature* pSumoned)
    {
        pSumoned->AI()->AttackStart(pSumoned->getVictim());
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!m_bIsBerserk && m_uiBerserkTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_BERSERK);
            m_bIsBerserk = true;
        }else m_uiBerserkTimer -= uiDiff;

        if (m_uiPoisonCloudTimer < uiDiff)
        {
            m_creature->CastSpell(m_creature, SPELL_POISON_CLOUD, true);
            m_uiPoisonCloudTimer = 15000;
        }else m_uiPoisonCloudTimer -= uiDiff;

        if (m_uiSlimeSprayTimer < uiDiff)
        {
            if(m_creature->getVictim())
            {
                m_creature->CastSpell(m_creature->getVictim(), m_bIsRegularMode ? SPELL_SLIME_SPRAY : SPELL_SLIME_SPRAY_H, false);
                m_creature->CastSpell(m_creature->getVictim(), SPELL_POISON_CLOUD, true);
            }
            m_uiSlimeSprayTimer = urand(10000,14000);
        }else m_uiSlimeSprayTimer -= uiDiff;

        if (m_uiMutatingTimer < uiDiff)
        {
            if (Unit* pPlayer = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                m_creature->CastSpell(pPlayer, SPELL_MUTATING, false);
            ++m_uiMutatingCount;
            m_uiMutatingTimer = m_uiMutatingTimer - m_uiMutatingCount*1000;
        }else m_uiMutatingTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_grobbulus(Creature* pCreature)
{
    return new boss_grobbulusAI(pCreature);
}

void AddSC_boss_grobbulus()
{
    Script* NewScript;

    NewScript = new Script;
    NewScript->Name = "boss_grobbulus";
    NewScript->GetAI = &GetAI_boss_grobbulus;
    NewScript->RegisterSelf();
}