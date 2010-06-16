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
SDName: Boss_KelThuzud
SD%Complete: 80
SDComment:
SDCategory: Naxxramas
SDAuthor: ScrappyDoo (c) Andeeria /dev/FallenAngelX
EndScriptData */

/* To Do
Support dla spela  Summon Shadow Fissure
Support dla Spela Chain of Kheltuzad
Icecrown Guardians  hp level sprawdzic cale
Szybkosc Ruchu addow
*/

#include "precompiled.h"
#include "naxxramas.h"


enum
{
    //when shappiron dies. dialog between kel and lich king (in this order)
    SAY_SAPP_DIALOG1          = -1533084,
    SAY_SAPP_DIALOG2_LICH     = -1533085,
    SAY_SAPP_DIALOG3          = -1533086,
    SAY_SAPP_DIALOG4_LICH     = -1533087,
    SAY_SAPP_DIALOG5          = -1533088,

    //when cat dies
    SAY_CAT_DIED              = -1533089,

    //when each of the 4 wing bosses dies
    SAY_TAUNT1                = -1533090,
    SAY_TAUNT2                = -1533091,
    SAY_TAUNT3                = -1533092,
    SAY_TAUNT4                = -1533093,

    SAY_SUMMON_MINIONS        = -1533105,                   //start of phase 1

    SAY_AGGRO1                = -1533094,                   //start of phase 2
    SAY_AGGRO2                = -1533095,
    SAY_AGGRO3                = -1533096,

    SAY_SLAY1                 = -1533097,
    SAY_SLAY2                 = -1533098,

    SAY_DEATH                 = -1533099,

    SAY_CHAIN1                = -1533100,
    SAY_CHAIN2                = -1533101,
    SAY_FROST_BLAST           = -1533102,

    SAY_REQUEST_AID           = -1533103,                   //start of phase 3
    SAY_ANSWER_REQUEST        = -1533104,                   //lich king answer

    SAY_SPECIAL1_MANA_DET     = -1533106,
    SAY_SPECIAL3_MANA_DET     = -1533107,
    SAY_SPECIAL2_DISPELL      = -1533108,

    //spells to be casted
    SPELL_FROST_BOLT          = 28478,
    SPELL_FROST_BOLT_H        = 55802,
    SPELL_FROST_BOLT_NOVA     = 28479,
    SPELL_FROST_BOLT_NOVA_H   = 55807,

    SPELL_CHAINS_OF_KELTHUZAD = 28410, //28408, //dev //casted spell should be 28408. Also as of 303, heroic only
	SPELL_MANA_DETONATION     = 27819,
    SPELL_MANA_DETONATIONDMG  = 37158,
    SPELL_SHADOW_FISURE       = 27810,
    SPELL_ARCANE_FORM         = 48019,
    SPELL_FROST_BLAST         = 27808,

    //shadow fissure spells
    SPELL_VOIDBLAST           = 27812, //100k near aoe ;D

	CREATURE_ICECROWN_GUARDIAN = 16441, //2 or 4
	CREATURE_SOLDIER		   = 16427, //71
	CREATURE_ABONIAMTION	   = 16428, //8
	CREATURE_BANSHEE		   = 16429, //8
    CREATURE_SHADOW_FISSURE    = 16129
};

double fSpawnCoords[6][4] =
{
    {3783.272,-5062.697,143.711},
    {3730.291,-5027.239,143.956},
    {3683.868,-5057.281,143.183},
    {3759.355,-5174.128,143.802},
    {3700.724,-5185.123,143.928},
    {3665.121,-5138.679,143.183}
};

struct MANGOS_DLL_DECL boss_kelthuzadAI : public ScriptedAI
{
    boss_kelthuzadAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;
    bool m_bIsDetonate;
    bool m_bIsAnswer;

    uint32 m_uiAnswerTimer;
    uint32 m_uiDetonateTimer;
    uint32 m_uiFrostBoltTimer;
    uint32 m_uiFrostBoltNovaTimer;
    uint32 m_uiChainsOfKelthuzadTimer;
    uint32 m_uiManaDetonationTimer;
    uint32 m_uiShadowFisureTimer;
    uint32 m_uiFrostBlastTimer;
	uint32 m_uiPhaseTimer;
	uint8  m_uiPhase;
	uint32 m_uiDialogTimer;
	uint8  m_uiDialogCount;
	uint32 m_uiAddsTimer;
    uint64 m_uiManaTargetGUID;

    void Reset()
    {
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        m_bIsDetonate               = false;
        m_bIsAnswer                 = true;

        m_uiAnswerTimer             = 5000;
        m_uiDetonateTimer           = 5000;
        m_uiManaTargetGUID          = 0;
        m_uiFrostBoltTimer			= urand(1000, 10000);             
        m_uiFrostBoltNovaTimer		= 15000;                      
        m_uiChainsOfKelthuzadTimer	= urand(30000, 60000);      
        m_uiManaDetonationTimer		= 20000;                       
        m_uiShadowFisureTimer		= 25000;                       
        m_uiFrostBlastTimer			= urand(30000, 60000);  
		m_uiPhaseTimer				= 228000;	
		m_uiPhase					= 1;
		m_uiDialogTimer				= 5000;
		m_uiDialogCount				= 0;
		m_uiAddsTimer				= 1000;

        if (m_pInstance)
            m_pInstance->SetData(TYPE_KELTHUZAD, NOT_STARTED);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1)?SAY_SLAY1:SAY_SLAY2, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_KELTHUZAD, DONE);
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWithZone();
        DoScriptText(SAY_SUMMON_MINIONS, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_KELTHUZAD, IN_PROGRESS);
    }

    void UpdateAI(const uint32 uiDiff)
    {
		//Intro Dialog
        if (m_pInstance && m_pInstance->GetData(TYPE_SAPPHIRON) == DONE)
        { 
			if (m_uiDialogCount < 6)
			{
				if (m_uiDialogTimer < uiDiff)
				{
					switch(m_uiDialogCount)
					{
						case 0: DoScriptText(SAY_SAPP_DIALOG1, m_creature); break;
						case 1: DoScriptText(SAY_SAPP_DIALOG2_LICH, m_creature); break;
						case 2: DoScriptText(SAY_SAPP_DIALOG3, m_creature); break;
						case 3: DoScriptText(SAY_SAPP_DIALOG4_LICH, m_creature); break;
						case 4: DoScriptText(SAY_SAPP_DIALOG5, m_creature); break;
						default: break;
					}
					++m_uiDialogCount;
					m_uiDialogTimer = 10000;
				}else m_uiDialogTimer -= uiDiff;
			}
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_bIsDetonate && m_uiDetonateTimer < uiDiff)
        {
            Unit* pPlayer = Unit::GetUnit(*m_creature, m_uiManaTargetGUID);
            if (pPlayer && pPlayer->isAlive())
                pPlayer->CastSpell(pPlayer, SPELL_MANA_DETONATIONDMG, true);       
            m_bIsDetonate      = false;
            m_uiManaTargetGUID = 0;
        }else m_uiDetonateTimer -= uiDiff;

        if (m_uiPhase == 1)
        {
			m_creature->StopMoving();
			m_creature->GetMotionMaster()->Clear();
			m_creature->GetMotionMaster()->MoveIdle();

			if (m_uiAddsTimer < uiDiff)
			{
                uint32 ID = 0;
				for (uint8 i=0; i<11; ++i)
				{
					if (i == 1)
						ID = CREATURE_BANSHEE;
					if (i == 0)
						ID = CREATURE_ABONIAMTION;
                    if (i > 1)
						ID = CREATURE_SOLDIER;

                    uint8 m_uiRand = urand(0,5);

					Creature* pAdd = m_creature->SummonCreature(ID, fSpawnCoords[m_uiRand][0], fSpawnCoords[m_uiRand][1], fSpawnCoords[m_uiRand][2], 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
					if (pAdd)
						if (Unit* pPlayer = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
							pAdd->AI()->AttackStart(pPlayer);
				}
				m_uiAddsTimer = 15000; //25000
			}else m_uiAddsTimer -= uiDiff;

            if (m_uiPhaseTimer < uiDiff)
            {
                switch(urand(0, 2))
                {
                    case 0: DoScriptText(SAY_AGGRO1, m_creature); break;
                    case 1: DoScriptText(SAY_AGGRO2, m_creature); break;
                    case 2: DoScriptText(SAY_AGGRO3, m_creature); break;
                }
				m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
				m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                if (m_creature->getVictim())
                    m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                ++m_uiPhase;
            }else m_uiPhaseTimer -= uiDiff;
        }

        if (m_uiPhase > 1)
        {
            //At 45% health, Kel'Thuzad will call for help, summoning 2 (25 Player: 4) Guardians of Icecrown. The fight becomes a DPS race during this Phase.
			if (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() < 45 && m_uiPhase == 2)
			{
                DoScriptText(SAY_REQUEST_AID, m_creature);

                m_uiAnswerTimer = 5000;
                m_bIsAnswer     = true;
                
				for (uint8 i=0; i<(m_bIsRegularMode ? 2 : 4); ++i)
				{
					Creature* pGuardian = m_creature->SummonCreature(CREATURE_ICECROWN_GUARDIAN, m_creature->GetPositionX()+15, m_creature->GetPositionY()+15, m_creature->GetPositionZ(), m_creature->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
					if(pGuardian)
						if(Unit* pPlayer = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
							pGuardian->AI()->AttackStart(pPlayer);
				}
				++m_uiPhase;
			}

            if (m_bIsAnswer && m_uiAnswerTimer < uiDiff)
            {
                DoScriptText(SAY_ANSWER_REQUEST, m_creature);
                m_bIsAnswer = false;
            }else m_uiAnswerTimer -= uiDiff;

            //Check for Frost Bolt
            if (m_uiFrostBoltTimer < uiDiff)
            {
				if (Unit* pPlayer = m_creature->SelectAttackingTarget(ATTACKING_TARGET_TOPAGGRO, 0))
					m_creature->CastSpell(pPlayer, m_bIsRegularMode ? SPELL_FROST_BOLT : SPELL_FROST_BOLT_H, false);
                m_uiFrostBoltTimer = urand(1000, 60000);
            }else m_uiFrostBoltTimer -= uiDiff;

            //Check for Frost Bolt Nova
            if (m_uiFrostBoltNovaTimer < uiDiff)
            {
				if (m_creature->getVictim())
					m_creature->CastSpell(m_creature->getVictim(), m_bIsRegularMode ? SPELL_FROST_BOLT_NOVA : SPELL_FROST_BOLT_NOVA_H, false);
                m_uiFrostBoltNovaTimer = urand(20000,30000);
            }else m_uiFrostBoltNovaTimer -= uiDiff;

            //Check for Chains Of Kelthuzad
            if (!m_bIsRegularMode && m_uiChainsOfKelthuzadTimer < uiDiff)
            {
                if (urand(0, 1))
                    DoScriptText(SAY_CHAIN1, m_creature);
                else
                    DoScriptText(SAY_CHAIN2, m_creature);

                //3 razy
				//DoCastSpellIfCan(m_creature->getVictim(),SPELL_CHAINS_OF_KELTHUZAD);

                m_uiChainsOfKelthuzadTimer = 90000;
            }else m_uiChainsOfKelthuzadTimer -= uiDiff;

            //Check for Mana Detonation
            if (m_uiManaDetonationTimer < uiDiff)
			{
				if (urand(0, 1)) DoScriptText(SAY_SPECIAL1_MANA_DET, m_creature);
                    
				if (Unit* pPlayer = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    m_uiManaTargetGUID = pPlayer->GetGUID();
					m_creature->CastSpell(pPlayer, SPELL_MANA_DETONATION, false);
                    m_uiDetonateTimer = 5000;
                    m_bIsDetonate     = true;
                }

                m_uiManaDetonationTimer = 30000;
            }else m_uiManaDetonationTimer -= uiDiff;

            //Check for Shadow Fissure
            if (m_uiShadowFisureTimer < uiDiff)
            {
                if (urand(0, 1)) DoScriptText(SAY_SPECIAL3_MANA_DET, m_creature);

				if (Unit* pPlayer = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))

                    if (Creature* pFisuure = m_creature->SummonCreature(CREATURE_SHADOW_FISSURE, pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 7000))
                    {
                        pFisuure->AI()->AttackStart(pPlayer);    
                        pFisuure->SetInCombatWithZone();
                    }

                m_uiShadowFisureTimer = 25000;
            }else m_uiShadowFisureTimer -= uiDiff;

            //Check for Frost Blast
            if (m_uiFrostBlastTimer < uiDiff)
            {
                if (Unit* pPlayer = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
					m_creature->CastSpell(pPlayer, SPELL_FROST_BLAST, false); //need to check is this chain effect

                if (urand(0, 1))
                    DoScriptText(SAY_FROST_BLAST, m_creature);

                m_uiFrostBlastTimer = 45000;
            }else m_uiFrostBlastTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
    }
};

CreatureAI* GetAI_boss_kelthuzadAI(Creature* pCreature)
{
    return new boss_kelthuzadAI(pCreature);
}

struct MANGOS_DLL_DECL mob_shadowfissureAI : public Scripted_NoMovementAI
{
    mob_shadowfissureAI(Creature *c) : Scripted_NoMovementAI(c) { Reset(); }
 
    uint32 m_uiVoidBlastTimer;        

    void Reset() 
    { 
        DoCastSpellIfCan(m_creature, SPELL_ARCANE_FORM, true); // missing  original visual efect
        m_uiVoidBlastTimer  = 5000; 
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiVoidBlastTimer < uiDiff)
        {
            if (m_creature)
                m_creature->CastSpell(m_creature, SPELL_VOIDBLAST, true);
            m_uiVoidBlastTimer = 15000;
        }else m_uiVoidBlastTimer -= uiDiff;
    }
};

CreatureAI* GetAI_mob_shadowfissureAI(Creature* pCreature)
{
    return new mob_shadowfissureAI(pCreature);
}

void AddSC_boss_kelthuzad()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "boss_kelthuzad";
    NewScript->GetAI = &GetAI_boss_kelthuzadAI;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_shadowfissure";
    NewScript->GetAI = &GetAI_mob_shadowfissureAI;
    NewScript->RegisterSelf();
}
