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
SDName: Boss Halion_n10 
SD%Complete: 10%
SDComment: Achievement and Events need to be implemented
SDCategory: Ruby Sanctum
EndScriptData */

#include "precompiled.h"
#include "ruby_sanctum.h"

enum
{
    //SAYS
	//Xerestrasza
	SAY_XERESTRASZA_YELL_1	                = -1666000, //17490 Help! I am trapped within this tree! I require aid!
	SAY_XERESTRASZA_YELL_2	                = -1666001, //17491 Thank you! I could have not held out for much longer. A terrible thing has happened here.
	SAY_XERESTRASZA_SAY_1	                = -1666002, //17492 We believed that the Sanctum was well fortified, but we were not prepareted for the nature of this assault.
	SAY_XERESTRASZA_SAY_2	                = -1666003, //17493 The Black Dragonkin materialized from thin air, and set upon us before we could react.
	SAY_XERESTRASZA_SAY_3	                = -1666004, //17494 We did not stand a chance. As my brethren perished around me, I managed to retreat hear and bar the entrance.
	SAY_XERESTRASZA_SAY_4	                = -1666005, //17495 They slaughtered us with cold efficiency, but the true focus of their interest seemed to be the eggs kept here in the sanctum.
	SAY_XERESTRASZA_SAY_5	                = -1666006, //17496 The commander of the forces on the ground here is a cruel brute named Zarithrian. But I fear there are greater powers at work.
	SAY_XERESTRASZA_SAY_6	                = -1666007, //17497 In their initial assault I caught a glimpse of their true leader, a fearsome full-grown Twilight Dragon.
	SAY_XERESTRASZA_SAY_7	                = -1666008, //17498 I know not the extent of their plans heroes, but I know this: they cannot be allowed to succeed!

	//Halion
	SAY_HALION_SPAWN                        = -1666100, //17499 Meddlesome insects, you're too late! The Ruby Sanctum is lost.
	SAY_HALION_AGGRO                        = -1666101, //17500 Your world teeters on the brink of annihilation. You will all bear witness to the coming of a new age of destruction!
    SAY_HALION_SLAY_1                       = -1666102, //17501 Another hero falls.
    SAY_HALION_SLAY_2                       = -1666103, //17502 Ha Ha Ha!
	SAY_HALION_DEATH                        = -1666104, //17503 Relish this victory mortals, for it will be your last. This world will burn with the Master's return!
	SAY_HALION_BERSERK                      = -1666105, //17504 Not good enough!
    SAY_HALION_SPECIAL_1	                = -1666106, //17505 The heavens burn!
	SAY_HALION_SPECIAL_2	                = -1666107, //17506 Beware the shadow!
	SAY_HALION_PHASE_2		                = -1666108, //17507 You will find only suffering within the realm of Twilight. Enter if you dare.
	SAY_HALION_PHASE_3	                    = -1666109, //17508 I am the light AND the darkness! Cower mortals before the Herald of Deathwing!
	
	//Zarithrian
	SAY_ZARITHRIAN_AGGRO                    = -1666200, //17512 Alexstrasza has chosen capable allies. A pity that I must end you!
	SAY_ZARITHRIAN_SLAY_1                   = -1666201, //17513 You thought you stood a chance?
    SAY_ZARITHRIAN_SLAY_2                   = -1666202, //17514 It's for the best.
	SAY_ZARITHRIAN_DEATH                    = -1666203, //17515 Halion! I'm...aah!
    SAY_ZARITHRIAN_SPECIAL_1	            = -1666204, //17516 Turn them to ash, minions!
	
	//Baltharus
	SAY_BALTHARUS_AGGRO                     = -1666300, //17520 Ah, the entertainment has arrived...
    SAY_BALTHARUS_SLAY_1                    = -1666301, //17521 Baltharus leaves no survivors!
    SAY_BALTHARUS_SLAY_2                    = -1666302, //17522 This world has enough heroes!
	SAY_BALTHARUS_DEATH                     = -1666303, //17523 I...didn't see that coming...
    SAY_BALTHARUS_SPECIAL_1				    = -1666304, //17524 Twice the pain and half the fun!
	SAY_BALTHARUS_YELL					    = -1666305, //17525 Your power wanes, ancient one! Soon, you will join your friends!

	//Savina
	SAY_SAVIANA_AGGRO                       = -1666400, //17528 You will suffer for this intrusion...
    SAY_SAVIANA_SLAY_1                      = -1666401, //17529 As it should be!
    SAY_SAVIANA_SLAY_2                      = -1666402, //17530 Halion will be pleased...
	SAY_SAVIANA_DEATH                       = -1666403, //17531 <screaming>
    SAY_SAVIANA_SPECIAL_1				    = -1666404, //17532 Burn in the master's flame!

    //Halion Spells ALL
    SPELL_BERSERK                           = 61632,    // Increases the caster's attack speed by 150% and all damage it deals by 500% for 5 min.
	SPELL_PORTALS							= 74809,	//Summon Twilight Portal
	SPELL_CORPOREALITY0						= 74826,    //the 0% normal Damage
	SPELL_CORPOREALITY1						= 74827,    //the +15% increases damage dealt and damage taken
	SPELL_CORPOREALITY2						= 74828,    //the +35% increases damage dealt and damage taken
	SPELL_CORPOREALITY3						= 74829,    //the +55% increases damage dealt and damage taken
	SPELL_CORPOREALITY4						= 74830,    //the +75% increases damage dealt and damage taken
	SPELL_CORPOREALITY5						= 74831,    //the +95% increases damage dealt and damage taken
	SPELL_CORPOREALITY6						= 74832,    //the -15% decreases damage dealt and damage taken
	SPELL_CORPOREALITY7						= 74833,    //the -35% decreases damage dealt and damage taken
	SPELL_CORPOREALITY8						= 74834,    //the -55% decreases damage dealt and damage taken
	SPELL_CORPOREALITY9						= 74835,    //the -75% decreases damage dealt and damage taken
	SPELL_CORPOREALITY10					= 74836,    //the -95% decreases damage dealt and damage taken
	SPELL_MARK_OF_CONSUMPTION				= 74795,	//Builds Marks of Combustion on an enemy target. When Combustion fades or is dispelled, it creates an explosion and a patch of fire proportional in size to the number of Combustion charges present at the time. 

	//10 man norm
	SPELL_METEORSTRIKE1						= 75950,	//Deals x to y Fire damage.
	SPELL_METEORSTRIKE1A					= 75947,	//Deals x to y Fire damage.
	SPELL_METEORSTRIKE1B					= 75877,	//Deals x to y Fire damage to enemies within 0 yards and spawns a Living Inferno.
	SPELL_CONSUMPTION1						= 75874,    //Deals 8750 to 11250 Shadow damage
	SPELL_COMBUSTION1						= 75883,    //Deals x to y Fire damage. 
	SPELL_SOUL_CONSUMPTION1					= 74799,    //Engulfs an enemy target in flame, dealing 4000 Shadow damage every 2.0 sec. Every time Soul Consumption deals damage, it generates a Mark of Consumption charge on the target. When Consumption fades or is dispelled, it unleashes a dark explosion proportional to the number of charges present at the time
	SPELL_DUSK_SHROUD1						= 75484,    //An aura of twilight energy emanates from the caster, causing nearby enemies to suffer 3500 Shadow damage every 2.0 sec.
	SPELL_DARK_BREATH1						= 75954,    //Inflicts x to y Shadow damage to enemies in a cone in front of the caster
	//10man hero / 25man norm 
	SPELL_METEORSTRIKE2						= 75951,	//Deals x to y Fire damage.
	SPELL_METEORSTRIKE2A					= 75948,	//Deals x to y Fire damage.
	SPELL_METEORSTRIKE2B					= 75878,	//Deals 75000 to 85000 Fire damage to enemies within 0 yards and spawns a Living Inferno.
	SPELL_CONSUMPTION2						= 75875,    //Deals 8750 to 11250 Shadow damage
	SPELL_COMBUSTION2						= 75883,    //Deals 7000 to 9000 Fire damage. 
	SPELL_SOUL_CONSUMPTION2					= 74792,    //Engulfs an enemy target in flame, dealing 4000 Shadow damage every 2.0 sec. Every time Soul Consumption deals damage, it generates a Mark of Consumption charge on the target. When Consumption fades or is dispelled, it unleashes a dark explosion proportional to the number of charges present at the time
	SPELL_DUSK_SHROUD2						= 75485,    //An aura of twilight energy emanates from the caster, causing nearby enemies to suffer 4500 Shadow damage every 2.0 sec.
	SPELL_DARK_BREATH2						= 75955,    //Inflicts x to y Shadow damage to enemies in a cone in front of the caster
	//25man hero
	SPELL_METEORSTRIKE3						= 75952,	//Deals 11310 to 12690 Fire damage.
	SPELL_METEORSTRIKE3A					= 75949,	//Deals 11310 to 12690 Fire damage.
	SPELL_METEORSTRIKE3B					= 75879,	//Deals x to y Fire damage to enemies within 0 yards and spawns a Living Inferno.
	SPELL_CONSUMPTION3						= 75876,    //Deals 8750 to 11250 Shadow damage
	SPELL_COMBUSTION3						= 75884,    //Deals x to y Fire damage. 
	SPELL_SOUL_CONSUMPTION3					= 74800,    //Engulfs an enemy target in flame, dealing 4000 Shadow damage every 2.0 sec. Every time Soul Consumption deals damage, it generates a Mark of Consumption charge on the target. When Consumption fades or is dispelled, it unleashes a dark explosion proportional to the number of charges present at the time
	SPELL_DUSK_SHROUD3						= 75486,    //An aura of twilight energy emanates from the caster, causing nearby enemies to suffer 6000 Shadow damage every 2.0 sec.
	SPELL_DARK_BREATH3						= 75956,    //Inflicts 39375 to 50625 Shadow damage to enemies in a cone in front of the caster		
    //Miniboses (Zarithrian, Baltharus, Saviana)
    SPELL_BLAZING_AURA                      = 75887,    //Surrounded by superheated air, dealing 6938 to 8062 Fire damage to all enemies within x yards, and increasing health and damage of all Living Embers within x yards by 20%
    SPELL_SCORCH							= 75419,    //Scorches an enemy for 11563 to 13437 Fire damage. 30 yds. 1 sec cast
    SPELL_SHOCKWAVE							= 75418,	//Sends a wave of force in front of the caster, causing 16200 to 19800 damage and stunning all enemy targets within 15 yards in a frontal cone for 2 sec
    SPELL_FLAME_BREATH						= 74404,	//Inflicts 33688 to 43312 Fire damage to enemies in a cone in front of the caster. 
    SPELL_LAVA_GOUT							= 74395,	//Inflicts 9897 to 11103 Fire damage to an enemy
	SPELL_BLAST_NOVA						= 74393,	//Inflicts 6563 to 8437 Fire damage to nearby enemies

	//Twilight Realm
	SPELL_ENTER_REALM						= 74807,	//or 74808 ??
	SPELL_EXIT_REALM						= 74812,

};

/*######
## Boss Halion_0
######*/

struct MANGOS_DLL_DECL boss_halion_0AI : public ScriptedAI
{
    boss_halion_0AI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance* m_pInstance;

	uint32 m_uiEnrageTimer;
    bool m_bIsHardEnraged;

    uint32 m_uiFlameBreathTimer;
    uint32 m_uiTailSweepTimer;
    uint32 m_uiCleaveTimer;
    uint32 m_uiLavaStrikeTimer;
	//portal
	int32 m_iPortalRespawnTime;

    void Reset()
    {
        uint32 uiSpellId = 49368;
		m_uiEnrageTimer = MINUTE*15*IN_MILLISECONDS;
        m_bIsHardEnraged = false;

        m_uiFlameBreathTimer = 20000;
        m_uiTailSweepTimer = 20000;
        m_uiCleaveTimer = 7000;
        m_uiLavaStrikeTimer = 15000;
		//portal
		m_iPortalRespawnTime = 30000;
		RemoveDebuff(uiSpellId);

    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_HALION_0_EVENT, NOT_STARTED);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_HALION_AGGRO,m_creature);

        m_creature->SetInCombatWithZone();

        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_HALION_0_EVENT, IN_PROGRESS);
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_HALION_DEATH,m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_HALION_0_EVENT, DONE);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 1))
        {
            case 0: DoScriptText(SAY_HALION_SLAY_1, m_creature); break;
            case 1: DoScriptText(SAY_HALION_SLAY_2, m_creature); break;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
		// hard enrage
        if (!m_bIsHardEnraged)
        {
            if (m_uiEnrageTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature, SPELL_BERSERK, CAST_TRIGGERED);
                m_bIsHardEnraged = true;
            }
            else
                m_uiEnrageTimer -= uiDiff;

		}
        if (m_uiFlameBreathTimer < uiDiff)
        {
            DoScriptText(SAY_HALION_SPECIAL_2, m_creature);
			DoCastSpellIfCan(m_creature->getVictim(), SPELL_DARK_BREATH);
            m_uiFlameBreathTimer = urand(25000, 35000);
        }
        else
            m_uiFlameBreathTimer -= uiDiff;

        // Tail Sweep
        if (m_uiTailSweepTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_TAIL_LASH);
            m_uiTailSweepTimer = urand(15000, 20000);
        }
        else
            m_uiTailSweepTimer -= uiDiff;

        // Cleave
        if (m_uiCleaveTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_CLEAVE);
            m_uiCleaveTimer = urand(7000, 10000);
        }       
		else            
			m_uiCleaveTimer -= uiDiff;

        // Lavas Strike
        if (m_uiLavaStrikeTimer < uiDiff)
        {
            DoScriptText(SAY_HALION_SPECIAL_1, m_creature);
			DoCastSpellIfCan(m_creature->getVictim(), SPELL_COMBUSTION);
            m_uiLavaStrikeTimer = urand(10000, 20000);
        }
        else
            m_uiLavaStrikeTimer -= uiDiff;

	

        DoMeleeAttackIfReady();
    }

    //Removes each drakes unique debuff from players
    void RemoveDebuff(uint32 uiSpellId)
    {
        Map* pMap = m_creature->GetMap();

        if (pMap && pMap->IsDungeon())
        {
            Map::PlayerList const &PlayerList = pMap->GetPlayers();

            if (PlayerList.isEmpty())
                return;

            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            {
                if (i->getSource()->isAlive() && i->getSource()->HasAura(uiSpellId))
                    i->getSource()->RemoveAurasDueToSpell(uiSpellId);
            }
        }
    }
};

CreatureAI* GetAI_boss_halion_0(Creature* pCreature)
{
    return new boss_halion_0AI(pCreature);
}
void AddSC_boss_halion()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_halion_0";
    newscript->GetAI = &GetAI_boss_halion_0;
    newscript->RegisterSelf();
}
