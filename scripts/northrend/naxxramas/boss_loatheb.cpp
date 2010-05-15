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
SDName: Boss_Loatheb
SD%Complete: 90
SDComment:
SDCategory: Naxxramas
SDAuthor: ScrappyDoo (c) Andeeria
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    SPELL_BERSERK         = 26662,
    SPELL_DEATHBLOOM      = 29865,
    SPELL_BLOOM           = 55594, 
    SPELL_DEATHBLOOM_H    = 55053, 
    SPELL_BLOOM_H         = 55601,
    SPELL_DOOM            = 29204, 
    SPELL_DOOM_H          = 55052, 
    SPELL_NECROTIC_AURA   = 55593, 
    SPELL_FUNGAL_CREEP    = 29232, 

    SPELL_SUMMON_SPORES   = 29234,

    CREATURE_SPORE        = 16286 
};

struct MANGOS_DLL_DECL boss_loathebAI : public ScriptedAI
{
    boss_loathebAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;
    bool m_bIsBloom;
    bool m_bIsEnrage;

    uint32 m_uiEnrage2Timer;
    uint32 m_uiBloomTimer;
    uint32 m_uiNecroticAuraTimer;
    uint32 m_uiDeathBloomTimer;
    uint32 m_uiDoomTimer;
    uint32 m_uiEnrageTimer;
    uint32 m_uiSummonTimer;

    void Reset()
    {
        m_bIsBloom            = false;
        m_bIsEnrage          = false;
        m_uiBloomTimer        = 26000;
        m_uiNecroticAuraTimer = 10000;
        m_uiDeathBloomTimer  = 20000;
        m_uiDoomTimer         = 120000;
        m_uiEnrageTimer       = 300000;
        m_uiEnrage2Timer      = 720000;
        m_uiSummonTimer       = 8000;
        if (m_pInstance)
            m_pInstance->SetData(TYPE_LOATHEB, NOT_STARTED);
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_LOATHEB, IN_PROGRESS);
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_LOATHEB, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_LOATHEB, FAIL);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(!m_bIsEnrage && m_uiEnrage2Timer < uiDiff)
        {
            DoCast(m_creature, SPELL_BERSERK);
            m_bIsEnrage = true;
        }else m_uiEnrage2Timer -= uiDiff;

        if(m_bIsBloom && m_uiBloomTimer < uiDiff)
        {
            std::list<HostileReference *> t_list = m_creature->getThreatManager().getThreatList();
            for(std::list<HostileReference *>::iterator itr = t_list.begin(); itr!= t_list.end(); ++itr)
            {
                Unit *TargetedPlayer = Unit::GetUnit(*m_creature, (*itr)->getUnitGuid());
                if (TargetedPlayer && TargetedPlayer->GetTypeId() == TYPEID_PLAYER && TargetedPlayer->isAlive())
                    TargetedPlayer->CastSpell(TargetedPlayer, m_bIsRegularMode ? SPELL_BLOOM : SPELL_BLOOM_H, false);
            }
            m_uiBloomTimer = 20000;  
            m_bIsBloom = false;
        }else m_uiBloomTimer -= uiDiff;

        if(m_uiNecroticAuraTimer < uiDiff)
        {
            if(m_creature->getVictim())
                m_creature->CastSpell(m_creature->getVictim(), SPELL_NECROTIC_AURA, false);
            m_uiNecroticAuraTimer = 20000;
        }else m_uiNecroticAuraTimer -= uiDiff;

        if(m_uiDeathBloomTimer < uiDiff)
        {
            if(m_creature->getVictim())
                m_creature->CastSpell(m_creature->getVictim(), m_bIsRegularMode ? SPELL_DEATHBLOOM : SPELL_DEATHBLOOM_H, false);
            m_uiDeathBloomTimer = (m_bIsRegularMode ? 30000 : 20000);
            m_uiBloomTimer = 6000;
            m_bIsBloom = true;
        }else m_uiDeathBloomTimer -= uiDiff;

        if(m_uiDoomTimer < uiDiff)
        {
            if(m_creature->getVictim())
                m_creature->CastSpell(m_creature->getVictim(), m_bIsRegularMode ? SPELL_DOOM : SPELL_DOOM_H, false);
            m_uiDoomTimer = 30000;
            if(m_uiEnrageTimer < uiDiff)
                m_uiDoomTimer = 15000;
            else m_uiEnrageTimer -= uiDiff;
        }else m_uiDoomTimer -= uiDiff;

        if (m_uiSummonTimer < uiDiff)
        {
            for(uint8 i=0; i<3; ++i)
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                {
                    Creature* pSummonedSpores = m_creature->SummonCreature(CREATURE_SPORE, pTarget->GetPositionX()+1, pTarget->GetPositionY()+1, pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN,80000);
                    if (pSummonedSpores)
                    {
                            pSummonedSpores->AddThreat(pTarget);
                            pSummonedSpores->AI()->AttackStart(pTarget);
                    }
                }
            m_uiSummonTimer = 30000;
        }else m_uiSummonTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_fungalsporeAI : public ScriptedAI
{
    mob_fungalsporeAI(Creature *c) : ScriptedAI(c) { Reset(); }
 
    void Reset(){}

    void JustDied(Unit* pKiller)
    {
        if(!pKiller)
            return;

        m_creature->CastSpell(pKiller, SPELL_FUNGAL_CREEP, true);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_fungalspore(Creature* pCreature)
{
    return new mob_fungalsporeAI(pCreature);
}

CreatureAI* GetAI_boss_loatheb(Creature* pCreature)
{
    return new boss_loathebAI(pCreature);
}

void AddSC_boss_loatheb()
{
    Script* NewScript;

    NewScript = new Script;
    NewScript->Name = "mob_fungalspore";
    NewScript->GetAI = &GetAI_mob_fungalspore;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "boss_loatheb";
    NewScript->GetAI = &GetAI_boss_loatheb;
    NewScript->RegisterSelf();
}
