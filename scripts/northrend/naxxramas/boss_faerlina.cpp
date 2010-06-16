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
SDName: Boss_Faerlina
SD%Complete: 90
SDComment:
SDCategory: Naxxramas
SDAuthor: ScrappyDoo (c) Andeeria /dev/FallenAngelX
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    SAY_GREET                 = -1533009,
    SAY_AGGRO1                = -1533010,
    SAY_AGGRO2                = -1533011,
    SAY_AGGRO3                = -1533012,
    SAY_AGGRO4                = -1533013,
    SAY_SLAY1                 = -1533014,
    SAY_SLAY2                 = -1533015,
    SAY_DEATH                 = -1533016,

    //SOUND_RANDOM_AGGRO        = 8955,                              //soundId containing the 4 aggro sounds, we not using this

    SPELL_POSIONBOLT_VOLLEY   = 28796,
    H_SPELL_POSIONBOLT_VOLLEY = 54098,
    SPELL_ENRAGE              = 28798,
    H_SPELL_ENRAGE            = 54100,
    
    SPELL_WIDOWS_EMBRANCE     = 28732,

    SPELL_RAINOFFIRE          = 28794,                       //Not sure if targeted AoEs work if casted directly upon a pPlayer
    SPELL_RAINOFFIRE1         = 39024,
    //43440,   
    SPELL_RAINOFFIRE_H        = 58936,

    CREATURE_WORSHIPER        = 16506,
    CREATURE_FOLLOWER         = 16505,
};

struct MANGOS_DLL_DECL boss_faerlinaAI : public ScriptedAI
{
    boss_faerlinaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        m_bHasTaunted = false;
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint64 m_uiWorshiperGUID[4];
    uint32 m_uiPoisonBoltVolleyTimer;
    uint32 m_uiRainOfFireTimer;
    uint32 m_uiEnrageTimer;
    bool   m_bHasTaunted;

    void Reset()
    {
        m_uiPoisonBoltVolleyTimer = 5000;
        m_uiRainOfFireTimer = 10000;
        m_uiEnrageTimer = 35000;

        for(uint8 i=0; i<4; ++i)
            m_uiWorshiperGUID[i] = 0;

        if (m_pInstance)
            m_pInstance->SetData(TYPE_FAERLINA, NOT_STARTED);
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWithZone();

        SpawnAdds();

        switch(urand(0, 3))
        {
            case 0: DoScriptText(SAY_AGGRO1, m_creature); break;
            case 1: DoScriptText(SAY_AGGRO2, m_creature); break;
            case 2: DoScriptText(SAY_AGGRO3, m_creature); break;
            case 3: DoScriptText(SAY_AGGRO4, m_creature); break;
        }

        if (m_pInstance)
            m_pInstance->SetData(TYPE_FAERLINA, IN_PROGRESS);
    }

    void SpawnAdds()
    {
        for(uint8 i=0; i<(m_bIsRegularMode ? 4 : 6); ++i)
        {
            uint32 ID = CREATURE_FOLLOWER;
            if(i<4)
                ID = CREATURE_WORSHIPER;

            Creature* Guard = m_creature->SummonCreature(ID, m_creature->GetPositionX()+urand(2,6), m_creature->GetPositionY()+urand(2,6), m_creature->GetPositionZ(), m_creature->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
            if(Guard)
            {
                if(Unit* pPlayer = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                    Guard->AI()->AttackStart(pPlayer);
                if(i<4)
                    m_uiWorshiperGUID[i] = Guard->GetGUID();
            }
        }
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (!m_bHasTaunted && m_creature->IsWithinDistInMap(pWho, 60.0f))
        {
            DoScriptText(SAY_GREET, m_creature);
            m_bHasTaunted = true;
        }

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1)?SAY_SLAY1:SAY_SLAY2, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_FAERLINA, DONE);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        for(uint8 i=0; i<4; ++i)
        {
            Unit* Worshiper = Unit::GetUnit(*m_creature, m_uiWorshiperGUID[i]);
            if(Worshiper && !Worshiper->isAlive())
            {
                if(m_creature->HasAura(SPELL_ENRAGE))
                    m_creature->RemoveAurasDueToSpell(SPELL_ENRAGE);

                m_uiWorshiperGUID[i] = 0;
            }
        }

        // Poison Bolt Volley
        if (m_uiPoisonBoltVolleyTimer < uiDiff)
        {
            if(m_creature->getVictim())
                m_creature->CastSpell(m_creature->getVictim(), m_bIsRegularMode ? SPELL_POSIONBOLT_VOLLEY : H_SPELL_POSIONBOLT_VOLLEY, true);
            m_uiPoisonBoltVolleyTimer = urand(7000,12000);
        }
        else
            m_uiPoisonBoltVolleyTimer -= uiDiff;

        // Rain Of Fire
        if (m_uiRainOfFireTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                m_creature->CastSpell(pTarget, m_bIsRegularMode ? SPELL_RAINOFFIRE1 : SPELL_RAINOFFIRE_H, true);

            m_uiRainOfFireTimer = 10000;
        }
        else
            m_uiRainOfFireTimer -= uiDiff;

        //Enrage_Timer
        if (m_uiEnrageTimer < uiDiff)
        {
            if(m_creature)
                DoCastSpellIfCan(m_creature, SPELL_ENRAGE);
            m_uiEnrageTimer = urand(30000,50000);
        }
        else 
            m_uiEnrageTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_faerlina(Creature* pCreature)
{
    return new boss_faerlinaAI(pCreature);
}

void AddSC_boss_faerlina()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "boss_faerlina";
    NewScript->GetAI = &GetAI_boss_faerlina;
    NewScript->RegisterSelf();
}
