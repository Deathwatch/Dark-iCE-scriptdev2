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
SDName: Boss_Four_Horsemen
SD%Complete: 70
SDComment: Lady Blaumeux, Thane Korthazz, Sir Zeliek, Baron Rivendare
SDCategory: Naxxramas
SDAuthor: ScrappyDoo (c) Andeeria
EndScriptData */

/* ToDo
Agro Event 
need hack for bosses marks
*/

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    //all horsemen
    SPELL_SHIELDWALL        = 29061,
    SPELL_BESERK            = 26662,

    //lady blaumeux
    SAY_BLAU_AGGRO          = -1533044,
    SAY_BLAU_TAUNT1         = -1533045,
    SAY_BLAU_TAUNT2         = -1533046,
    SAY_BLAU_TAUNT3         = -1533047,
    SAY_BLAU_SPECIAL        = -1533048,
    SAY_BLAU_SLAY           = -1533049,
    SAY_BLAU_DEATH          = -1533050,

    SPELL_MARK_OF_BLAUMEUX  = 28833,
    SPELL_UNYILDING_PAIN    = 57381,
    SPELL_VOIDZONE          = 28863,
    SPELL_VOIDZONE_H        = 57463,
    SPELL_SHADOW_BOLT       = 57374,
    SPELL_SHADOW_BOLT_H     = 57464,

    //baron rivendare
    SAY_RIVE_AGGRO1         = -1533065,
    SAY_RIVE_AGGRO2         = -1533066,
    SAY_RIVE_AGGRO3         = -1533067,
    SAY_RIVE_SLAY1          = -1533068,
    SAY_RIVE_SLAY2          = -1533069,
    SAY_RIVE_SPECIAL        = -1533070,
    SAY_RIVE_TAUNT1         = -1533071,
    SAY_RIVE_TAUNT2         = -1533072,
    SAY_RIVE_TAUNT3         = -1533073,
    SAY_RIVE_DEATH          = -1533074,

    SPELL_MARK_OF_RIVENDARE = 28834,
    SPELL_UNHOLY_SHADOW     = 28882,
    SPELL_UNHOLY_SHADOW_H   = 57369,

    //thane korthazz
    SAY_KORT_AGGRO          = -1533051,
    SAY_KORT_TAUNT1         = -1533052,
    SAY_KORT_TAUNT2         = -1533053,
    SAY_KORT_TAUNT3         = -1533054,
    SAY_KORT_SPECIAL        = -1533055,
    SAY_KORT_SLAY           = -1533056,
    SAY_KORT_DEATH          = -1533057,

    SPELL_MARK_OF_KORTHAZZ  = 28832,
    SPELL_METEOR            = 26558,                       
    SPELL_METEOR_H          = 57467,

    //sir zeliek
    SAY_ZELI_AGGRO          = -1533058,
    SAY_ZELI_TAUNT1         = -1533059,
    SAY_ZELI_TAUNT2         = -1533060,
    SAY_ZELI_TAUNT3         = -1533061,
    SAY_ZELI_SPECIAL        = -1533062,
    SAY_ZELI_SLAY           = -1533063,
    SAY_ZELI_DEATH          = -1533064,

    SPELL_MARK_OF_ZELIEK    = 28835,
    SPELL_HOLY_WRATH        = 28883,
    SPELL_HOLY_WRATH_H      = 57466,
    SPELL_HOLY_BOLT         = 57376,
    SPELL_HOLY_BOLT_H       = 57465,
    SPELL_CONDEMNATION      = 57377,

    // horseman spirits
    NPC_SPIRIT_OF_BLAUMEUX    = 16776,
    NPC_SPIRIT_OF_RIVENDARE   = 0,                          //creature entry not known yet
    NPC_SPIRIT_OF_KORTHAZZ    = 16778,
    NPC_SPIRIT_OF_ZELIREK     = 16777
};

//rivandare //korthazz //blaumax //zeliek
float fAgrroPos[4][4] = 
{
    {2589.1127, -2954.6237, 241.316, 3.363},
    {2525.5441, -3029.0300, 241.337, 1.483},
    {2518.6086, -2899.8730, 241.275, 4.616},
    {2467.6564, -2951.2202, 241.276, 0.069}
};

struct MANGOS_DLL_DECL boss_lady_blaumeuxAI : public ScriptedAI
{
    boss_lady_blaumeuxAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool   m_bIsRegularMode;
    bool   m_bIsShieldWall1;
    bool   m_bIsShieldWall2;
    bool   m_bIsEnrage;
    uint32 m_uiEnrageTimer;
    uint32 m_uiPainTimer;
    uint32 m_uiShadowBoltTimer;
    uint32 m_uiMarkTimer;
    uint32 m_uiVoidZoneTimer;

    void Reset()
    {
        m_bIsShieldWall1    = true;
        m_bIsShieldWall2    = true;
        m_bIsEnrage         = true;
        m_uiPainTimer       = 2000;
        m_uiEnrageTimer     = 1000;
        m_uiShadowBoltTimer = 3000;
        m_uiMarkTimer       = 20000;                            
        m_uiVoidZoneTimer   = 12000;                             
    }

    void Aggro(Unit *who)
    {
        m_creature->GetMap()->CreatureRelocation(m_creature, fAgrroPos[2][0], fAgrroPos[2][1], fAgrroPos[2][2], fAgrroPos[2][3]);
        DoScriptText(SAY_BLAU_AGGRO, m_creature);
    }

    void KilledUnit(Unit* Victim)
    {
        DoScriptText(SAY_BLAU_SLAY, m_creature);
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_BLAU_DEATH, m_creature);
    }

    void DamageDeal(Unit* pDoneTo, uint32& uiDamage) 
    {
        if (pDoneTo->HasAura(SPELL_MARK_OF_BLAUMEUX))
            uiDamage *= 1.5;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        m_creature->StopMoving();
        m_creature->GetMotionMaster()->Clear();
        m_creature->GetMotionMaster()->MoveIdle();

        //Enrage if anyone isnt in 45 yard range
        if (m_uiEnrageTimer < uiDiff)
        {
            m_bIsEnrage = true;
            std::list<HostileReference *> t_list = m_creature->getThreatManager().getThreatList();
            for(std::list<HostileReference *>::iterator itr = t_list.begin(); itr!= t_list.end(); ++itr)
            {
                Unit *TargetedPlayer = Unit::GetUnit(*m_creature, (*itr)->getUnitGuid());  
                if(TargetedPlayer && TargetedPlayer->GetTypeId() == TYPEID_PLAYER && m_creature->IsWithinDistInMap(TargetedPlayer, 20))
                    m_bIsEnrage = false;
            }
            m_uiEnrageTimer = 1000;
        }else m_uiEnrageTimer -= uiDiff;

        if (m_bIsEnrage && m_uiPainTimer < uiDiff)
        {
            m_creature->CastSpell(m_creature, SPELL_UNYILDING_PAIN, true);
            m_uiPainTimer = 2000;
        } else m_uiPainTimer -= uiDiff;

        // Mark of Blaumeux
        if (m_uiMarkTimer < uiDiff)
        {
            if (m_creature->getVictim())
                m_creature->CastSpell(m_creature->getVictim(), SPELL_MARK_OF_BLAUMEUX, false);
            m_uiMarkTimer = 12000;
        }else m_uiMarkTimer -= uiDiff;

        // Shield Wall - All 4 horsemen will shield wall at 50% hp and 20% hp for 20 seconds
        if (m_bIsShieldWall1 && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 50)
        {
            DoCast(m_creature,SPELL_SHIELDWALL);
            m_bIsShieldWall1 = false;
        }

        if (m_bIsShieldWall2 && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 20)
        {
            DoCast(m_creature,SPELL_SHIELDWALL);
            m_bIsShieldWall2 = false;
        }

        // Void Zone
        if (m_uiVoidZoneTimer < uiDiff)
        {
            if (m_creature->getVictim())
                m_creature->CastSpell(m_creature->getVictim(), m_bIsRegularMode ? SPELL_VOIDZONE : SPELL_VOIDZONE_H, false);
            m_uiVoidZoneTimer = 12000;
        }else m_uiVoidZoneTimer -= uiDiff;

        // Shadow Bolt
        if (m_uiShadowBoltTimer < uiDiff)
        {
            if (m_creature->getVictim())
                m_creature->CastSpell(m_creature->getVictim(), m_bIsRegularMode ? SPELL_SHADOW_BOLT : SPELL_SHADOW_BOLT_H, false);
            m_uiShadowBoltTimer = 2000;
        }else m_uiShadowBoltTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_lady_blaumeux(Creature* pCreature)
{
    return new boss_lady_blaumeuxAI(pCreature);
}

struct MANGOS_DLL_DECL boss_rivendare_naxxAI : public ScriptedAI
{
    boss_rivendare_naxxAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool   m_bIsRegularMode;
    bool   m_bIsShieldWall1;
    bool   m_bIsShieldWall2;
    uint32 m_uiMarkTimer;
    uint32 m_uiUnholyShadow;

    void Reset()
    {
        m_bIsShieldWall1 = true;
        m_bIsShieldWall2 = true;
        m_uiMarkTimer    = 20000;
        m_uiUnholyShadow = 15000;
    }

    void Aggro(Unit *who)
    {
        m_creature->GetMap()->CreatureRelocation(m_creature, fAgrroPos[0][0], fAgrroPos[0][1], fAgrroPos[0][2], fAgrroPos[0][3]);
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_RIVE_AGGRO1, m_creature); break;
            case 1: DoScriptText(SAY_RIVE_AGGRO2, m_creature); break;
            case 2: DoScriptText(SAY_RIVE_AGGRO3, m_creature); break;
        }
    }

    void KilledUnit(Unit* Victim)
    {
        DoScriptText(urand(0, 1) ? SAY_RIVE_SLAY1 : SAY_RIVE_SLAY2, m_creature);
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_RIVE_DEATH, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Mark of Rivendare
        if (m_uiMarkTimer < uiDiff)
        {
            if (m_creature->getVictim())
                m_creature->CastSpell(m_creature->getVictim(), SPELL_MARK_OF_RIVENDARE, false);
            m_uiMarkTimer = 12000;
        }else m_uiMarkTimer -= uiDiff;

        // Shield Wall - All 4 horsemen will shield wall at 50% hp and 20% hp for 20 seconds
        if (m_bIsShieldWall1 && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 50)
        {
            DoCast(m_creature,SPELL_SHIELDWALL);
            m_bIsShieldWall1 = false;
        }

        if (m_bIsShieldWall2 && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 20)
        {
            DoCast(m_creature,SPELL_SHIELDWALL);
            m_bIsShieldWall2 = false;
        }

        // Unholy Shadow
        if (m_uiUnholyShadow < uiDiff)
        {
            if (Unit* pPlayer = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 0))
                m_creature->CastSpell(pPlayer, m_bIsRegularMode ? SPELL_UNHOLY_SHADOW : SPELL_UNHOLY_SHADOW_H, false);
            m_uiUnholyShadow = 12000;
        }else m_uiUnholyShadow -= uiDiff;
 
        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_rivendare_naxx(Creature* pCreature)
{
    return new boss_rivendare_naxxAI(pCreature);
}

struct MANGOS_DLL_DECL boss_thane_korthazzAI : public ScriptedAI
{
    boss_thane_korthazzAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool   m_bIsRegularMode;
    bool   m_bIsShieldWall1;
    bool   m_bIsShieldWall2;
    uint32 m_uiMarkTimer;
    uint32 m_uiMeteorTimer;

    void Reset()
    {
        m_bIsShieldWall1 = true;
        m_bIsShieldWall2 = true;
        m_uiMarkTimer    = 20000;                         
        m_uiMeteorTimer  = 15000;                            
    }

    void Aggro(Unit *who)
    {
        m_creature->GetMap()->CreatureRelocation(m_creature, fAgrroPos[1][0], fAgrroPos[1][1], fAgrroPos[1][2], fAgrroPos[1][3]);
        DoScriptText(SAY_KORT_AGGRO, m_creature);
    }

    void KilledUnit(Unit* Victim)
    {
        DoScriptText(SAY_KORT_SLAY, m_creature);
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_KORT_DEATH, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Mark of Korthazz
        if (m_uiMarkTimer < uiDiff)
        {
            if (m_creature->getVictim())
                m_creature->CastSpell(m_creature->getVictim(), SPELL_MARK_OF_KORTHAZZ, false);
            m_uiMarkTimer = 12000;
        }else m_uiMarkTimer -= uiDiff;

        // Shield Wall - All 4 horsemen will shield wall at 50% hp and 20% hp for 20 seconds
        if (m_bIsShieldWall1 && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 50)
        {
            DoCast(m_creature,SPELL_SHIELDWALL);
            m_bIsShieldWall1 = false;
        }

        if (m_bIsShieldWall2 && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 20)
        {
            DoCast(m_creature,SPELL_SHIELDWALL);
            m_bIsShieldWall2 = false;
        }

        // Meteor
        if (m_uiMeteorTimer < uiDiff)
        {
            if (m_creature->getVictim())
                m_creature->CastSpell(m_creature->getVictim(),m_bIsRegularMode ?  SPELL_METEOR : SPELL_METEOR_H, false);
            m_uiMeteorTimer = 15000;
        }else m_uiMeteorTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_thane_korthazz(Creature* pCreature)
{
    return new boss_thane_korthazzAI(pCreature);
}

struct MANGOS_DLL_DECL boss_sir_zeliekAI : public ScriptedAI
{
    boss_sir_zeliekAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool   m_bIsRegularMode;
    bool   m_bIsShieldWall1;
    bool   m_bIsShieldWall2;
    bool   m_bIsEnrage;
    uint32 m_uiEnrageTimer;
    uint32 m_uiCondemnationTimer;
    uint32 m_uiMarkTimer;
    uint32 m_uiHolyWrathTimer;
    uint32 m_uiHolyBoltTimer;

    void Reset()
    {
        m_bIsEnrage        = true;
        m_bIsShieldWall1   = true;
        m_bIsShieldWall2   = true;
        m_uiEnrageTimer    = 1000;
        m_uiHolyBoltTimer  = 3000;
        m_uiMarkTimer      = 20000;                      
        m_uiHolyWrathTimer = 12000;     
        m_uiCondemnationTimer = 2000;
    }

    void Aggro(Unit *who)
    {
        m_creature->GetMap()->CreatureRelocation(m_creature, fAgrroPos[3][0], fAgrroPos[3][1], fAgrroPos[3][2], fAgrroPos[3][3]);
        DoScriptText(SAY_ZELI_AGGRO, m_creature);
    }

    void KilledUnit(Unit* Victim)
    {
        DoScriptText(SAY_ZELI_SLAY, m_creature);
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_ZELI_DEATH, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        m_creature->StopMoving();
        m_creature->GetMotionMaster()->Clear();
        m_creature->GetMotionMaster()->MoveIdle();

        //Enrage if anyone isnt in 45 yard range
        if (m_uiEnrageTimer < uiDiff)
        {
            m_bIsEnrage = true;
            std::list<HostileReference *> t_list = m_creature->getThreatManager().getThreatList();
            for(std::list<HostileReference *>::iterator itr = t_list.begin(); itr!= t_list.end(); ++itr)
            {
                Unit *TargetedPlayer = Unit::GetUnit(*m_creature, (*itr)->getUnitGuid());  
                if(TargetedPlayer && TargetedPlayer->GetTypeId() == TYPEID_PLAYER && m_creature->IsWithinDistInMap(TargetedPlayer, 20))
                    m_bIsEnrage = false;
            }
            m_uiEnrageTimer = 1000;
        }else m_uiEnrageTimer -= uiDiff;

        if (m_bIsEnrage && m_uiCondemnationTimer < uiDiff)
        {
            m_creature->CastSpell(m_creature, SPELL_CONDEMNATION, true);
            m_uiCondemnationTimer = 2000;
        } else m_uiCondemnationTimer -= uiDiff;

        // Mark of Zeliek
        if (m_uiMarkTimer < uiDiff)
        {
            if (m_creature->getVictim())
                m_creature->CastSpell(m_creature->getVictim(), SPELL_MARK_OF_ZELIEK, false);
            m_uiMarkTimer = 12000;
        }else m_uiMarkTimer -= uiDiff;

        // Shield Wall - All 4 horsemen will shield wall at 50% hp and 20% hp for 20 seconds
        if (m_bIsShieldWall1 && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 50)
        {
            DoCast(m_creature,SPELL_SHIELDWALL);
            m_bIsShieldWall1 = false;
        }

        if (m_bIsShieldWall2 && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 20)
        {
            DoCast(m_creature,SPELL_SHIELDWALL);
            m_bIsShieldWall2 = false;
        }

        // Holy Wrath
        if (m_uiHolyWrathTimer < uiDiff)
        {
            if (Unit* pPlayer = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                m_creature->CastSpell(pPlayer, m_bIsRegularMode ? SPELL_HOLY_WRATH : SPELL_HOLY_WRATH_H, true);
            m_uiHolyWrathTimer = 12000;
        }else m_uiHolyWrathTimer -= uiDiff;

        // Shadow Bolt
        if (m_uiHolyBoltTimer < uiDiff)
        {
            if (m_creature->getVictim())
                m_creature->CastSpell(m_creature->getVictim(), m_bIsRegularMode ? SPELL_HOLY_BOLT : SPELL_HOLY_BOLT_H, false);
            m_uiHolyBoltTimer = 3000;
        }else m_uiHolyBoltTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_sir_zeliek(Creature* pCreature)
{
    return new boss_sir_zeliekAI(pCreature);
}

void AddSC_boss_four_horsemen()
{
    Script* NewScript;

    NewScript = new Script;
    NewScript->Name = "boss_lady_blaumeux";
    NewScript->GetAI = &GetAI_boss_lady_blaumeux;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "boss_rivendare_naxx";
    NewScript->GetAI = &GetAI_boss_rivendare_naxx;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "boss_thane_korthazz";
    NewScript->GetAI = &GetAI_boss_thane_korthazz;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "boss_sir_zeliek";
    NewScript->GetAI = &GetAI_boss_sir_zeliek;
    NewScript->RegisterSelf();
}
