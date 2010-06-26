/* Copyright (C) 2010 /dev/rsa for ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: boss_halion
SD%Complete: 0%
SDComment: by /dev/rsa & notagain
SDCategory: ruby_sanctum
EndScriptData */

#include "precompiled.h"
#include "def_ruby_sanctum.h"

enum 
{
    //SPELLS
	//All
	SPELL_TWILIGHT_PRECISION        = 78243,	// Increases Halion's chance to hit by 5% and decreases all players' chance to dodge by 20%
	SPELL_TWILIGHT_DIVISION         = 75063,	// Phases the caster, allowing him to exist and act simultaneously in both the material and Twilight realms.
	SPELL_BERSERK			        = 26663,	// Increases the caster's attack and movement speeds by 150% and all damage it deals by 500% for 5 min.  Also grants immunity to Taunt effects.
	SPELL_CORPREALITY_EVEN			= 74826,	// Deals & receives normal damage
	SPELL_CORPREALITY_20I			= 74827,	// Damage dealt increased by 10% & Damage taken increased by 15%
	SPELL_CORPREALITY_40I			= 74828,	// Damage dealt increased by 30% & Damage taken increased by 50%
	SPELL_CORPREALITY_60I			= 74829,	// Damage dealt increased by 60% & Damage taken increased by 100%
	SPELL_CORPREALITY_80I			= 74830,	// Damage dealt increased by 100% & Damage taken increased by 200%
	SPELL_CORPREALITY_100I			= 74831,	// Damage dealt increased by 200% & Damage taken increased by 400%
	SPELL_CORPREALITY_20D			= 74832,	// Damage dealt reduced by 10% & Damage taken reduced by 15%
	SPELL_CORPREALITY_40D			= 74833,	// Damage dealt reduced by 30% & Damage taken reduced by 50%
	SPELL_CORPREALITY_60D			= 74834,	// Damage dealt reduced by 60% & Damage taken reduced by 100%
	SPELL_CORPREALITY_80D			= 74835,	// Damage dealt reduced by 100% & Damage taken reduced by 200%
	SPELL_CORPREALITY_100D			= 74836,	// Damage dealt reduced by 200% & Damage taken reduced by 400%
	SPELL_METEOR_STRIKE				= 75877,	// Inflicts 18,750 to 21,250 Fire damage to enemies within 12 yards of the targeted area. Takes about 5 seconds to land.
	SPELL_FIERY_COMBUSTION			= 74562,	// Inflicts 4,000 Fire damage every 2 seconds for 30 seconds to a random raider. Every time Fiery Combustion does damage, it applies a stackable Mark of Combustion.
	SPELL_SOUL_CONSUMPTION			= 74792,	// Inflicts 4,000 Shadow damage every 2 seconds for 30 seconds to a random raider. Every time Soul Consumption does damage, it applies a stackable Mark of Consumption.
	SPELL_TWILIGHT_CUTTER			= 77844,	// Inflicts 13,875 to 16,125 Shadow damage every second to players touched by the shadow beam
	//N10
	SPELL_FLAME_BREATH_0			= 74525,	// Inflicts 17,500 to 22,500 Fire damage to players in front of Halion
	SPELL_DARK_BREATH_0				= 74806,	// Inflicts 17,500 to 22,500 Shadow damage to players in front of Halion
	SPELL_DUSK_SHROUD_0				= 75484,	// Inflicts 3,000 Shadow damage every 2 seconds to everyone in the Twilight Realm
	//H10 & N25
	SPELL_FLAME_BREATH_1			= 74527,	// Inflicts 26,250 to 33,750 Fire damage to players in front of Halion
	SPELL_DARK_BREATH_1				= 74806,	// Inflicts 26,250 to 33,750 Shadow damage to players in front of Halion
	SPELL_DUSK_SHROUD_1				= 75485,	// Inflicts 4,500 Shadow damage every 2 seconds to everyone in the Twilight Realm
	//H25
	SPELL_FLAME_BREATH_2			= 74404,	// Inflicts 17,500 to 22,500 Fire damage to players in front of Halion
	SPELL_DARK_BREATH_2				= 75956,	// Inflicts 33,688 to 43,312 Shadow damage to players in front of Halion
	SPELL_DUSK_SHROUD_2				= 75486,	// Inflicts 6,000 Shadow damage every 2 seconds to everyone in the Twilight Realm
	
	//NPC
	NPC_LIVING_EMBER				= 40683,
	NPC_LIVING_INFERNO				= 40681,
	NPC_SHADOW_PULSAR_0				= 40083,
	NPC_SHADOW_PULSAR_1				= 40100,
	NPC_SHADOW_PULSAR_2				= 40468,
	NPC_SHADOW_PULSAR_3				= 40469,
		
	//SAYS
	SAY_HALION_SPAWN                = -1666100, //17499 Meddlesome insects, you're too late! The Ruby Sanctum is lost.
	SAY_HALION_AGGRO                = -1666101, //17500 Your world teeters on the brink of annihilation. You will all bear witness to the coming of a new age of destruction!
    SAY_HALION_SLAY_1               = -1666102, //17501 Another hero falls.
    SAY_HALION_SLAY_2               = -1666103, //17502 Ha Ha Ha!
	SAY_HALION_DEATH                = -1666104, //17503 Relish this victory mortals, for it will be your last. This world will burn with the Master's return!
	SAY_HALION_BERSERK              = -1666105, //17504 Not good enough!
    SAY_HALION_SPECIAL_1	        = -1666106, //17505 The heavens burn!
	SAY_HALION_SPECIAL_2	        = -1666107, //17506 Beware the shadow!
	SAY_HALION_PHASE_2		        = -1666108, //17507 You will find only suffering within the realm of Twilight. Enter if you dare.
	SAY_HALION_PHASE_3	            = -1666109, //17508 I am the light AND the darkness! Cower mortals before the Herald of Deathwing!
};

struct Waypoint
{
    float m_fX, m_fY, m_fZ;
};

Waypoint m_pulsar0[]=
{
    {0.0f, 0.0f, 0.0f}, //N
	{0.0f, 0.0f, 0.0f}, //NNE
	{0.0f, 0.0f, 0.0f}, //NE
	{0.0f, 0.0f, 0.0f}, //NEE
	{0.0f, 0.0f, 0.0f}, //E
	{0.0f, 0.0f, 0.0f}, //SEE
	{0.0f, 0.0f, 0.0f}, //SE
	{0.0f, 0.0f, 0.0f}, //SSE
	{0.0f, 0.0f, 0.0f}, //S
	{0.0f, 0.0f, 0.0f}, //SSW
	{0.0f, 0.0f, 0.0f}, //SW
	{0.0f, 0.0f, 0.0f}, //SWW
	{0.0f, 0.0f, 0.0f}, //W
	{0.0f, 0.0f, 0.0f}, //NWW
	{0.0f, 0.0f, 0.0f}, //NW
	{0.0f, 0.0f, 0.0f}, //NNW
};

Waypoint m_pulsar1[]=
{
    {0.0f, 0.0f, 0.0f}, //S
	{0.0f, 0.0f, 0.0f}, //SSW
	{0.0f, 0.0f, 0.0f}, //SW
	{0.0f, 0.0f, 0.0f}, //SWW
	{0.0f, 0.0f, 0.0f}, //W
	{0.0f, 0.0f, 0.0f}, //NWW
	{0.0f, 0.0f, 0.0f}, //NW
	{0.0f, 0.0f, 0.0f}, //NNW
	{0.0f, 0.0f, 0.0f}, //N
	{0.0f, 0.0f, 0.0f}, //NNE
	{0.0f, 0.0f, 0.0f}, //NE
	{0.0f, 0.0f, 0.0f}, //NEE
	{0.0f, 0.0f, 0.0f}, //E
	{0.0f, 0.0f, 0.0f}, //SEE
	{0.0f, 0.0f, 0.0f}, //SE
	{0.0f, 0.0f, 0.0f}, //SSE
};

/*######
## boss_halion_p (Physical version)
######*/
struct MANGOS_DLL_DECL boss_halion_pAI : public ScriptedAI
{
    boss_halion_pAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
    m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
    bsw = new BossSpellWorker(this);
    Reset();
    }

    ScriptedInstance* m_pInstance;
    uint8 p_phase;
	bool intro;
    BossSpellWorker* bsw;

    void Reset() {
        if(!m_pInstance) return;
        p_phase = 0;
    }

    void MoveInLineOfSight(Unit* pWho) 
    {
        if (intro) return;
        DoScriptText(-1666100,m_creature);
        intro = true;
    }

	void JustReachedHome()
    {
        if (!m_pInstance) return;
            m_pInstance->SetData(TYPE_HALION, FAIL);
            m_pInstance->SetData(DATA_HEALTH_HALION_P, m_creature->GetMaxHealth());
    }

    void JustDied(Unit* pKiller)
    {
        if (!m_pInstance) return;
            DoScriptText(-1666104,m_creature);
            if (Creature* pclone = (Creature*)Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_HALION_T)))
               if (!pclone->isAlive())
                         m_pInstance->SetData(TYPE_HALION, DONE);
                else m_pInstance->SetData(TYPE_HALION, SPECIAL);
        m_pInstance->SetData(DATA_HEALTH_HALION_P, 0);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch (urand(0,1)) {
        case 0:
               DoScriptText(-1631006,m_creature,pVictim);
               break;
        case 1:
               DoScriptText(-1631007,m_creature,pVictim);
               break;
        };
    }

    void Aggro(Unit* pWho)
    {
        if (!m_pInstance) return;
        m_creature->SetInCombatWithZone();
        m_pInstance->SetData(TYPE_HALION, IN_PROGRESS);     
        DoScriptText(-1666101,m_creature);
        m_pInstance->SetData(DATA_HEALTH_HALION_P, m_creature->GetMaxHealth());
	}

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (!m_pInstance) return;
        if (!m_creature || !m_creature->isAlive())
            return;

        if(pDoneBy->GetGUID() == m_creature->GetGUID()) return;

        m_pInstance->SetData(DATA_HEALTH_HALION_P, m_creature->GetHealth() >= uiDamage ? m_creature->GetHealth() - uiDamage : 0);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_pInstance) return;
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_creature->GetHealth() > m_pInstance->GetData(DATA_HEALTH_HALION_T) &&
                                      m_pInstance->GetData(DATA_HEALTH_HALION_T) != 0)
                m_creature->SetHealth(m_pInstance->GetData(DATA_HEALTH_HALION_T));

    if (m_creature->GetHealthPercent() < 100.0f && p_phase == 0)
	{
		m_pInstance->SetData(TYPE_HALION_LOCK, DONE);
		p_phase = 1;
	}
	
	if (m_creature->GetHealthPercent() < 75.0f && p_phase == 1)
	{
		DoScriptText(-1666108,m_creature);
		p_phase = 2;
	}

	if (m_creature->GetHealthPercent() < 50.0f && p_phase == 2)
	{
		DoScriptText(-1666109,m_creature);
		p_phase = 3;
	}

	switch (p_phase)
        {
         case 0: //SPAWNED
                break;
		 case 1: //PHASE 1 PHYSICAL REALM
            bsw->timedCast(SPELL_FLAME_BREATH_0, uiDiff);
			bsw->timedCast(SPELL_FIERY_COMBUSTION, uiDiff);
			/* Needs Script
			bsw->timedCast(SPELL_METEOR_STRIKE, uiDiff); 
			*/
                break;
         case 2: //DEPHASE HALION FORCE TO TWILIGHT REALM
             //setflag unattackable, unselectable, remove combat
			    break;
         case 3: //PHASE 3 BOTH REALMS
            bsw->timedCast(SPELL_FLAME_BREATH_0, uiDiff);
			bsw->timedCast(SPELL_FIERY_COMBUSTION, uiDiff);
			bsw->doCast(SPELL_TWILIGHT_DIVISION);
			/* Needs Script
			bsw->timedCast(SPELL_METEOR_STRIKE, uiDiff); 
			bsw->timedCast(SPELL_CORPREALITY_EVEN, uiDiff); 
			*/
                break;
          default:
                break;
         }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_halion_p(Creature* pCreature)
{
    return new boss_halion_pAI(pCreature);
}

/*######
## boss_halion_t (Twilight version)
######*/

struct MANGOS_DLL_DECL boss_halion_tAI : public ScriptedAI
{
    boss_halion_tAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
    m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
    bsw = new BossSpellWorker(this);
    Reset();
    }

    ScriptedInstance* m_pInstance;
	uint8 t_phase;
    BossSpellWorker* bsw;

    void Reset() 
    {
        if(!m_pInstance) return;
		t_phase = 0;
    }

    void JustReachedHome()
    {
        if (!m_pInstance) return;
            m_pInstance->SetData(TYPE_HALION, FAIL);
            m_pInstance->SetData(DATA_HEALTH_HALION_T, m_creature->GetMaxHealth());
            m_creature->ForcedDespawn();
    }

    void JustDied(Unit* pKiller)
    {
        if (!m_pInstance) return;
            DoScriptText(-1666104,m_creature);

		m_pInstance->SetData(DATA_HEALTH_HALION_T, 0);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch (urand(0,1)) {
        case 0:
               DoScriptText(-1631006,m_creature,pVictim);
               break;
        case 1:
               DoScriptText(-1631007,m_creature,pVictim);
               break;
        };
    }

    void Aggro(Unit* pWho)
    {
        if (!m_pInstance) return;
        m_creature->SetInCombatWithZone();
        m_pInstance->SetData(DATA_HEALTH_HALION_T, m_creature->GetMaxHealth());

		if (t_phase == 0)
			t_phase = 2;
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (!m_pInstance) return;
        if (!m_creature || !m_creature->isAlive())
            return;

        if(pDoneBy->GetGUID() == m_creature->GetGUID()) return;

        m_pInstance->SetData(DATA_HEALTH_HALION_T, m_creature->GetHealth() >= uiDamage ? m_creature->GetHealth() - uiDamage : 0);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_creature->GetHealth() > m_pInstance->GetData(DATA_HEALTH_HALION_P) &&
                                      m_pInstance->GetData(DATA_HEALTH_HALION_P) != 0)
                m_creature->SetHealth(m_pInstance->GetData(DATA_HEALTH_HALION_P));

	if (m_creature->GetHealthPercent() < 50.0f && t_phase == 2)
	{
		DoScriptText(-1666109,m_creature);
		t_phase = 3;
		
	}
	
	switch (t_phase)
        {
         case 0: //SPAWNED
                break;
		 case 1: //FAIL
             //HOW DID U GET HERE
			 break;
         case 2: //PHASE 2 TWILIGHT REALM
			 bsw->doCast(SPELL_TWILIGHT_DIVISION);
			 bsw->doCast(SPELL_DUSK_SHROUD_0);
			 bsw->timedCast(SPELL_DARK_BREATH_0, uiDiff);
			 bsw->timedCast(SPELL_SOUL_CONSUMPTION, uiDiff);
             break;
         case 3: //PHASE 3 BOTH REALMS
			 bsw->doCast(SPELL_TWILIGHT_DIVISION);
			 bsw->doCast(SPELL_DUSK_SHROUD_0);
			 bsw->timedCast(SPELL_DARK_BREATH_0, uiDiff);
			 bsw->timedCast(SPELL_SOUL_CONSUMPTION, uiDiff);
             break;
          default:
                break;
         }
		
		bsw->timedCast(SPELL_BERSERK, uiDiff);

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_halion_t(Creature* pCreature)
{
    return new boss_halion_tAI(pCreature);
}

void AddSC_boss_halion()
{
    Script *newscript;

	newscript = new Script;
    newscript->Name = "boss_halion_p";
    newscript->GetAI = &GetAI_boss_halion_p;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_halion_t";
    newscript->GetAI = &GetAI_boss_halion_t;
    newscript->RegisterSelf();
}
