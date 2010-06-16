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
SDName: Boss_Razuvious
SD%Complete: 90%
SDComment: Hardmode Playable
SDCategory: Naxxramas
SDAuthor: ScrappyDoo (c) Andeeria /dev/FallenAngelX
EndScriptData */

/* ToDo
- Mind Control Effect -> Control Orbs script
*/

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    SAY_AGGRO1               = -1533120,
    SAY_AGGRO2               = -1533121,
    SAY_AGGRO3               = -1533122,
    SAY_SLAY1                = -1533123,
    SAY_SLAY2                = -1533124,
    SAY_COMMAND1             = -1533125,
    SAY_COMMAND2             = -1533126,
    SAY_COMMAND3             = -1533127,
    SAY_COMMAND4             = -1533128,
    SAY_DEATH                = -1533129,

    //Razovius Spells
    SPELL_UNBALANCING_STRIKE = 26613,
    SPELL_DISRUPTING_SHOUT   = 55543,
    SPELL_DISRUPTING_SHOUT_H = 29107,
    SPELL_JAGGED_KNIFE       = 55550,

    //Understudies Deatch Knights Spells
    SPELL_BLOOD_STRIKE       = 61696,
    SPELL_TAUNT              = 29060,
    SPELL_BONEBARRIER        = 29061,
    SPELL_HOPELESS           = 29125
};

struct MANGOS_DLL_DECL boss_razuviousAI : public ScriptedAI
{
    boss_razuviousAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiUnbalancingStrikeTimer;
    uint32 m_uiDisruptingShoutTimer;
    uint32 m_uiJaggedKnifeTimer;
    uint32 m_uiCommandSoundTimer;

    void Reset()
    {
        m_uiUnbalancingStrikeTimer = 30000;                 // 30 seconds
        m_uiDisruptingShoutTimer   = urand(14000,16000);    // 15 seconds
        m_uiJaggedKnifeTimer       = urand(9000,11000);
        m_uiCommandSoundTimer      = 40000;                 // 40 seconds

        if (m_pInstance)
            m_pInstance->SetData(TYPE_RAZUVIOUS, NOT_STARTED);
    }

    void KilledUnit(Unit* Victim)
    {
        if (urand(0, 3))
            return;

        switch(urand(0, 1))
        {
            case 0: DoScriptText(SAY_SLAY1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY2, m_creature); break;
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_RAZUVIOUS, DONE);
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
            m_pInstance->SetData(TYPE_RAZUVIOUS, IN_PROGRESS);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Unbalancing Strike
        if (m_uiUnbalancingStrikeTimer < uiDiff)
        {
            if(m_creature->getVictim())
                m_creature->CastSpell(m_creature->getVictim(), SPELL_UNBALANCING_STRIKE, false);
            m_uiUnbalancingStrikeTimer = 30000;

            //test
            if (m_pInstance) 
                m_pInstance->SetData(TYPE_UNDERSTUDIES, SPECIAL);
        }
        else 
            m_uiUnbalancingStrikeTimer -= uiDiff;

        // Disrupting Shout
        if (m_uiDisruptingShoutTimer < uiDiff)
        {
            if(m_creature->getVictim())
                m_creature->CastSpell(m_creature->getVictim(), m_bIsRegularMode ? SPELL_DISRUPTING_SHOUT : SPELL_DISRUPTING_SHOUT_H, false);
            m_uiDisruptingShoutTimer = urand(14000,16000);
        }
        else 
            m_uiDisruptingShoutTimer -= uiDiff;

        // Jagged Knife
        if (m_uiJaggedKnifeTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                m_creature->CastSpell(pTarget, SPELL_JAGGED_KNIFE, false);
            m_uiJaggedKnifeTimer = urand(9000,11000);
        }
        else 
            m_uiJaggedKnifeTimer -= uiDiff;

        // Random say
        if (m_uiCommandSoundTimer < uiDiff)
        {
            //test
            if (m_pInstance) 
                m_pInstance->SetData(TYPE_UNDERSTUDIES, IN_PROGRESS);

            switch(urand(0, 3))
            {
                case 0: DoScriptText(SAY_COMMAND1, m_creature); break;
                case 1: DoScriptText(SAY_COMMAND2, m_creature); break;
                case 2: DoScriptText(SAY_COMMAND3, m_creature); break;
                case 3: DoScriptText(SAY_COMMAND4, m_creature); break;
            }

            m_uiCommandSoundTimer = 40000;
        }
        else
            m_uiCommandSoundTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_understudiesAI : public ScriptedAI
{
    mob_understudiesAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;
    bool m_bIsBloodStrikeReady;
    bool m_bIsBoneReady;
    bool m_bIsEnraged;
    bool m_bIsMindControled;

    uint32 m_uiBloodStrikeTimer;
    uint32 m_uiTauntTimer;
    uint32 m_uiBoneTimer;

    void Reset()
    {
        m_bIsMindControled      = true;  //tests
        m_bIsBloodStrikeReady   = false;
        m_bIsBoneReady          = false;
        m_bIsEnraged            = false;

        m_uiBloodStrikeTimer    = 4000;
        m_uiTauntTimer          = 20000;
        m_uiBoneTimer           = 30000;
    }

    /*
    void ONSpellHit()
    {
        //mc
        setfacton 35?
        set timer wyjscia 15 sekund ?
        m_bIsMindControled = false;
    }
    */

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!m_bIsEnraged)
            if (Unit* pRazovius = Unit::GetUnit(*m_creature, m_pInstance->GetData64(TYPE_RAZUVIOUS)))
                if (!pRazovius->isAlive())
                {
                    m_creature->CastSpell(m_creature, SPELL_HOPELESS, false);
                    m_bIsEnraged = true;
                }

        if (m_bIsBloodStrikeReady)
            if ((m_pInstance && m_pInstance->GetData(TYPE_UNDERSTUDIES) == IN_PROGRESS) || !m_bIsMindControled)
            {
                //m_uiMindControlTimer = 30000;
                if(m_creature->getVictim())
                    m_creature->CastSpell(m_creature->getVictim(), SPELL_BLOOD_STRIKE, false);
                m_pInstance->SetData(TYPE_UNDERSTUDIES, NOT_STARTED); 
                m_bIsBloodStrikeReady = false;
            }

        if (m_bIsBoneReady)
            if ((m_pInstance && m_pInstance->GetData(TYPE_UNDERSTUDIES) == SPECIAL) || !m_bIsMindControled)
            {
                //m_uiMindControlTimer = 30000;
                if(m_creature)
                    m_creature->CastSpell(m_creature, SPELL_BONEBARRIER, false);
                m_pInstance->SetData(TYPE_UNDERSTUDIES, NOT_STARTED); 
                m_bIsBoneReady = false;
            }

        if (m_uiBloodStrikeTimer < uiDiff)
        {
            if(m_creature->getVictim())
                m_creature->CastSpell(m_creature->getVictim(), SPELL_BLOOD_STRIKE, false);
            m_bIsBloodStrikeReady = true;
            m_uiBloodStrikeTimer = 4000;
        }
        else 
            m_uiBloodStrikeTimer -= uiDiff;

        if (m_uiBoneTimer < uiDiff)
        {
            if(m_creature)
                m_creature->CastSpell(m_creature, SPELL_BONEBARRIER, false);
            m_bIsBoneReady = true;
            m_uiBoneTimer = 30000;
        }
        else 
            m_uiBoneTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};


CreatureAI* GetAI_boss_razuvious(Creature* pCreature)
{
    return new boss_razuviousAI(pCreature);
}

CreatureAI* GetAI_mob_understudies(Creature* pCreature)
{
    return new mob_understudiesAI(pCreature);
}

void AddSC_boss_razuvious()
{
    Script* NewScript;

    NewScript = new Script;
    NewScript->Name = "mob_understudies";
    NewScript->GetAI = &GetAI_mob_understudies;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "boss_razuvious";
    NewScript->GetAI = &GetAI_boss_razuvious;
    NewScript->RegisterSelf();
}
