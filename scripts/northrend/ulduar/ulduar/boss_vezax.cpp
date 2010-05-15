/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: General Vezax
SD%Complete: 0
SDComment: PH.
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "def_ulduar.h"

#define SP_AURA_OF_DESPAIR      62692
#define SP_SHADOW_CRASH         62660
#define H_SP_SHADOW_CRASH
#define SP_MARK_OF_FACELESS
#define SP_SEARING_FLAMES
#define SP_SURGE_OF_DARKNESS
#define SP_BERSERK              26662
#define SP_AURA_OF_DESPAIR      62692

struct MANGOS_DLL_DECL boss_vezaxAI : public ScriptedAI
{
    boss_vezaxAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool Regular;
    ScriptedInstance *pInstance;

    uint32 EnrageTimer;
    /*uint32 CrashTimer;
    uint32 MarkTimer;
    uint32 FlamesTimer;
    uint32 SurgeTimer;*/

    void Reset()
    {
        EnrageTimer = 600000; //10 minutes
        //FlamesTimer = 10000 + rand()%10000;
        if(pInstance) pInstance->SetData(TYPE_VEZAX, NOT_STARTED);
    }

    void Aggro(Unit *who) 
    {
        if(pInstance) pInstance->SetData(TYPE_VEZAX, IN_PROGRESS);
        DoCast(NULL, SP_AURA_OF_DESPAIR, true);
    }

    void JustDied(Unit *killer)
    {
        if(pInstance) pInstance->SetData(TYPE_VEZAX, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        /*if(FlamesTimer < diff)
        {
            DoCast(m_creature->getVictim(), SP_SEARING_FLAMES);
            FlamesTimer = 10000 + rand()%10000;
        }
        else FlamesTimer -= diff; */

        if(EnrageTimer < diff)
        {
            DoCast(m_creature, SP_BERSERK);
            EnrageTimer = 30000;
        }
        else EnrageTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_vezax(Creature* pCreature)
{
    return new boss_vezaxAI(pCreature);
}

void AddSC_boss_vezax()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_vezax";
    newscript->GetAI = &GetAI_boss_vezax;
    newscript->RegisterSelf();
}
