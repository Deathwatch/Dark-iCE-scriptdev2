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
SDName: Boss_Maexxna
SD%Complete: 90
SDComment:
SDCategory: Naxxramas
SDAuthor: ScrappyDoo (c) Andeeria /dev/FallenAngelX
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    SPELL_WEBWRAP           = 28622,                        //Spell is normally used by the webtrap on the wall NOT by Maexxna

    SPELL_WEBSPRAY          = 29484,
    H_SPELL_WEBSPRAY        = 54125,
    SPELL_POISONSHOCK       = 28741,
    H_SPELL_POISONSHOCK     = 54122,
    SPELL_NECROTICPOISON    = 28776,
    H_SPELL_NECROTICPOISON  = 54121,
    SPELL_FRENZY            = 54123,
    H_SPELL_FRENZY          = 54124,

    //spellId invalid
    SPELL_SUMMON_SPIDERLING = 29434,
    CREATURE_SPIRDERLING    = 17055,
};

float fLoc[2][3] =
{
    (3546.796,-3869.082,296.450),
    (3531.271,-3847.424,299.450),
};

struct MANGOS_DLL_DECL mob_webwrapAI : public Scripted_NoMovementAI
{
    mob_webwrapAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature) {Reset();}

    uint64 m_uiVictimGUID;

    void Reset()
    {
        m_uiVictimGUID = 0;
    }

    void JustDied(Unit* pKiller)
    {
        if (m_uiVictimGUID)
        {
            if (Unit* pVictim = Unit::GetUnit((*m_creature), m_uiVictimGUID))
                pVictim->RemoveAurasDueToSpell(SPELL_WEBWRAP);
        }
    }

    void SetVictim(Unit* pVictim)
    {
        if (pVictim)
        {
            m_uiVictimGUID = pVictim->GetGUID();
            pVictim->CastSpell(pVictim, SPELL_WEBWRAP, true);
        }
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (uiDamage > m_creature->GetHealth())
        {
            if (m_uiVictimGUID)
            {
                if (Unit* pVictim = Unit::GetUnit((*m_creature), m_uiVictimGUID))
                    pVictim->RemoveAurasDueToSpell(SPELL_WEBWRAP);
            }
        }
    }

    void MoveInLineOfSight(Unit* pWho) { }
    void UpdateAI(const uint32 uiDiff) { }
};

struct MANGOS_DLL_DECL boss_maexxnaAI : public ScriptedAI
{
    boss_maexxnaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiWebWrapTimer;
    uint32 m_uiWebSprayTimer;
    uint32 m_uiPoisonShockTimer;
    uint32 m_uiNecroticPoisonTimer;
    uint32 m_uiSummonSpiderlingTimer;
    bool   m_bEnraged;

    void Reset()
    {
        m_uiWebWrapTimer = 20000;                           //20 sec init, 40 sec normal
        m_uiWebSprayTimer = 40000;                          //40 seconds
        m_uiPoisonShockTimer = 20000;                       //20 seconds
        m_uiNecroticPoisonTimer = 30000;                    //30 seconds
        m_uiSummonSpiderlingTimer = 30000;                  //30 sec init, 40 sec normal
        m_bEnraged = false;

        if (m_pInstance)
            m_pInstance->SetData(TYPE_MAEXXNA, NOT_STARTED);
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_MAEXXNA, IN_PROGRESS);
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_MAEXXNA, DONE);
    }

    void DoCastWebWrap()
    {
        for(uint8 i=0; i<(m_bIsRegularMode ? 2 : 4); ++i)
        {
            if(Unit* pPlayer = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
            {
                if (pPlayer && pPlayer->isAlive() && pPlayer->GetTypeId() == TYPEID_PLAYER)
                {
                    //DoTeleportPlayer(pPlayer, fLoc[i][0], fLoc[i][1], fLoc[i][2], pPlayer->GetOrientation());
                    if (Creature* pWrap = m_creature->SummonCreature(16486, pPlayer->GetPositionX(), pPlayer->GetPositionY(),pPlayer->GetPositionZ(), 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 120000))
                        ((mob_webwrapAI*)pWrap->AI())->SetVictim(pPlayer);
                }
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_bEnraged)
            if(m_bIsRegularMode ? !m_creature->HasAura(SPELL_FRENZY) : !m_creature->HasAura(H_SPELL_FRENZY))
                DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_FRENZY : H_SPELL_FRENZY);

        // Web Wrap
        if (m_uiWebWrapTimer < uiDiff)
        {
            DoCastWebWrap();
            m_uiWebWrapTimer = 30000;
        }
        else
            m_uiWebWrapTimer -= uiDiff;

        // Web Spray
        if (m_uiWebSprayTimer < uiDiff)
        {
            if(m_creature->getVictim())
                m_creature->CastSpell(m_creature->getVictim(), m_bIsRegularMode ? SPELL_WEBSPRAY : H_SPELL_WEBSPRAY, false);
            m_uiWebSprayTimer = 25000;
        }
        else
            m_uiWebSprayTimer -= uiDiff;

        // Poison Shock
        if (m_uiPoisonShockTimer < uiDiff)
        {
            if(m_creature->getVictim())
                m_creature->CastSpell(m_creature->getVictim(), m_bIsRegularMode ? SPELL_POISONSHOCK : H_SPELL_POISONSHOCK, false);
            m_uiPoisonShockTimer = 10000;
        }
        else
            m_uiPoisonShockTimer -= uiDiff;

        // Necrotic Poison
        if (m_uiNecroticPoisonTimer < uiDiff)
        {
            if(m_creature->getVictim())
                m_creature->CastSpell(m_creature->getVictim(), m_bIsRegularMode ? SPELL_NECROTICPOISON : H_SPELL_NECROTICPOISON, false);
            m_uiNecroticPoisonTimer = 19000;
        }
        else
            m_uiNecroticPoisonTimer -= uiDiff;

        // Summon Spiderling
        if (m_uiSummonSpiderlingTimer < uiDiff)
        {
            for(uint8 i=0; i<8; ++i)
            {
                Creature* pSpiderling = m_creature->SummonCreature(CREATURE_SPIRDERLING, m_creature->GetPositionX()+urand(2,6), m_creature->GetPositionY()+urand(2,6), m_creature->GetPositionZ(), m_creature->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                if(pSpiderling)
                {
                    if(Unit* pPlayer = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                        pSpiderling->AI()->AttackStart(pPlayer);
                }
            }

            /*
            if(m_creature)
                DoCastSpellIfCan(m_creature, SPELL_SUMMON_SPIDERLING);
                */
            m_uiSummonSpiderlingTimer = 31000;
        }
        else
            m_uiSummonSpiderlingTimer -= uiDiff;

        //Enrage if not already enraged and below 30%
        if (!m_bEnraged && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 30)
        {
            if(m_creature)
                DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_FRENZY : H_SPELL_FRENZY);
            m_bEnraged = true;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_webwrap(Creature* pCreature)
{
    return new mob_webwrapAI(pCreature);
}

CreatureAI* GetAI_boss_maexxna(Creature* pCreature)
{
    return new boss_maexxnaAI(pCreature);
}

void AddSC_boss_maexxna()
{
    Script* NewScript;

    NewScript = new Script;
    NewScript->Name = "boss_maexxna";
    NewScript->GetAI = &GetAI_boss_maexxna;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_webwrap";
    NewScript->GetAI = &GetAI_mob_webwrap;
    NewScript->RegisterSelf();
}
