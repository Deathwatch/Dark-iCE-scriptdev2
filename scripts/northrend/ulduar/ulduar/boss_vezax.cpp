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
SD%Complete: 35%
SDComment: PH.
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

// General Vezax Aura
#define AURA_AURA_OF_DESPAIR       62692

// General Vezax Spells
#define SPELL_BERSERK              26662
#define SPELL_MARK_OF_THE_FACELESS 63276
#define SPELL_SARONITE_BARRIER     63364
#define SPELL_SEARING_FLAMES       62661
#define SPELL_SHADOW_CRASH         62660
#define SPELL_SURGE_OF_DARKNESS    62662

// General Vezax Sounds
#define SOUND_UR_Vezax_Aggro01     15542
#define SOUND_UR_Vezax_Attack      15550
#define SOUND_UR_Vezax_Berserk01   15547
#define SOUND_UR_Vezax_Death01     15546
#define SOUND_UR_Vezax_HardMode01  15548
#define SOUND_UR_Vezax_Kite01      15545
#define SOUND_UR_Vezax_Slay01      15543
#define SOUND_UR_Vezax_Slay02      15544
#define SOUND_UR_Vezax_Wound       15549
#define SOUND_UR_Vezax_WoundCrit   15551

// General Vezax Says
#define SAY_UR_Vezax_Aggro01  "Your destruction will herald a new age of suffering!"
#define SAY_UR_Vezax_Slay01   "You thought to stand before the legions of death and survive?"
#define SAY_UR_Vezax_Slay02   "Defiance... *laughs* A flaw of mortality!"
#define SAY_UR_Vezax_Spawns   "Behold now! Terror, absolute!"
#define SAY_UR_Vezax_Enrage01 "Your defeat was inevitable."
#define SAY_UR_Vezax_Enrage02 "The black blood of Yogg-Saron courses through me! I...AM...UNSTOPPABLE!"
#define SAY_UR_Vezax_Death01  "Ha-ha-ha! Oh, what horrors await..."

// Saronite Vapors
#define MOB_SAROBITE_VAPORS        33488

// Saronite Vapors Spells
#define SPELL_SAROBITE_VAPORS      63322

struct MANGOS_DLL_DECL boss_vezaxAI : public ScriptedAI
{
    boss_generalvezaxAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();

        Reset();
    }

    ScriptedInstance* m_pInstance;
	bool m_bIsRegularMode;

	uint32 Searing_Flames_Timer;
	uint32 Shadow_Crash_Timer;
	uint32 Mark_of_the_Faceless_Timer;
	uint32 Surge_of_Darkness_Timer;
	uint32 Saronite_Barrier_Timer;
	uint32 Saronite_Vapors_Timer;    //Spawn 8x the entire battle
	uint32 Saronite_Vapors_h_Timer;  //Spawn 6x the entire battle
	uint32 Berserk_Timer;
	bool berserk;

    void Reset()
    {
		Searing_Flames_Timer = 10000;
		Shadow_Crash_Timer = 15000;
		Mark_of_the_Faceless_Timer = 40000;
		Surge_of_Darkness_Timer = 30000;
		Saronite_Barrier_Timer = 35000;
		Saronite_Vapors_Timer = 20000;
		Saronite_Vapors_h_Timer = 20000;
		Berserk_Timer = 600000; //10 Minutes
		if (m_pInstance)
            m_pInstance->SetData(TYPE_VEZAX, NOT_STARTED);
    }

    void KilledUnit(Unit *victim)
    {
		switch(urand(0, 1))
                {
                    case 0: 
						DoPlaySoundToSet(m_creature, SOUND_UR_Vezax_Slay01); 
						m_creature->MonsterYell(SAY_UR_Vezax_Slay01, LANG_UNIVERSAL, 0);
						break;
                    case 1:
						DoPlaySoundToSet(m_creature, SOUND_UR_Vezax_Slay02);
						m_creature->MonsterYell(SAY_UR_Vezax_Slay02, LANG_UNIVERSAL, 0);
						break;
                }
    }

    void JustDied(Unit *victim)
    {
		if (m_pInstance)
            m_pInstance->SetData(TYPE_VEZAX, DONE);
		DoPlaySoundToSet(m_creature, SOUND_UR_Vezax_Death01);
		m_creature->MonsterYell(SAY_UR_Vezax_Death01, LANG_UNIVERSAL, 0);
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_VEZAX, IN_PROGRESS);
		DoCast(m_creature, AURA_AURA_OF_DESPAIR);
		DoPlaySoundToSet(m_creature, SOUND_UR_Vezax_Aggro01);
		m_creature->MonsterYell(SAY_UR_Vezax_Aggro01, LANG_UNIVERSAL, 0);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (Searing_Flames_Timer < diff )
        {
            if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                DoCast(m_creature->getVictim(), SPELL_SEARING_FLAMES);
            Searing_Flames_Timer = 25000;
        }else Searing_Flames_Timer -= diff;

		if (Shadow_Crash_Timer < diff )
        {
            if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                DoCast(m_creature->getVictim(), SPELL_SHADOW_CRASH);
            Shadow_Crash_Timer = 15000;
        }else Shadow_Crash_Timer -= diff;

		if (Mark_of_the_Faceless_Timer < diff )
        {
            if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                DoCast(m_creature->getVictim(), SPELL_MARK_OF_THE_FACELESS);
            Mark_of_the_Faceless_Timer = 40000;
        }else Mark_of_the_Faceless_Timer -= diff;

		if (Berserk_Timer < diff && !berserk)
        {
            m_creature->CastStop();
            DoCast(m_creature, SPELL_BERSERK);
            berserk = true;
        }else Berserk_Timer -= diff;

        DoMeleeAttackIfReady();

        EnterEvadeIfOutOfCombatArea(diff);

    }

};

CreatureAI* GetAI_boss_generalvezax(Creature* pCreature)
{
    return new boss_generalvezaxAI(pCreature);
}

struct MANGOS_DLL_DECL mob_saronite_vaporsrAI : public ScriptedAI
{
    mob_saronite_vaporsrAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    void Reset()
    {
    }

	void JustDied(Unit *victim)
    {
		DoCast(m_creature, SPELL_SAROBITE_VAPORS);
    }

    void UpdateAI(const uint32 diff)
    {
        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_saronite_vaporsr(Creature* pCreature)
{
    return new mob_saronite_vaporsrAI(pCreature);
}

void AddSC_boss_vezax()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_vezax";
    newscript->GetAI = &GetAI_boss_generalvezax;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_saronite_vaporsr";
    newscript->GetAI = &GetAI_mob_saronite_vaporsr;
    newscript->RegisterSelf();

}
/*UPDATE `creature_template` SET `ScriptName`='boss_vezax' WHERE (`entry`='33271') LIMIT 1
  UPDATE `creature_template` SET `ScriptName`='mob_saronite_vaporsr' WHERE (`entry`='33488') LIMIT 1*/
