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
SDName: Freya
SD%Complete: 90
SDComment: TODO: Brightleaf, 3wave ress,
SDAuthor: Killerfrca
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"
enum
{
    //freya yells
    SAY_AGGRO							= -1700152,
    SAY_AGGRO_HARD                      = -1700153,
    SAY_WAVE_1                          = -1700154,
    SAY_WAVE_3                          = -1700155,
    SAY_WAVE_10                         = -1700156,
    SAY_SLAY_1                          = -1700157,
    SAY_SLAY_2                          = -1700158,
    SAY_BERSERK                         = -1700159,
    SAY_DEATH                           = -1700160,
    SAY_YOGGSARON                       = -1700161,
    
    //stonebark yells
    SAY_STONE_AGGRO                     = -1700166,
    SAY_STONE_SLAY_1                    = -1700167,
    SAY_STONE_SLAY_2                    = -1700168,
    SAY_STONE_DEATH                     = -1700169,

    //ironbranch yells
    SAY_IRON_AGGRO                      = -1700170,
    SAY_IRON_SLAY_1                     = -1700171,
    SAY_IRON_SLAY_2                     = -1700172,
    SAY_IRON_DEATH                      = -1700173,

    //brightleaf yells
    SAY_BRIGHT_AGGRO                     = -1700162,
    SAY_BRIGHT_SLAY_1                    = -1700163,
    SAY_BRIGHT_SLAY_2                    = -1700164,
    SAY_BRIGHT_DEATH                     = -1700165,
    
    //freya
    //general abilities
    SPELL_SUNBEAM                       = 62623,
    H_SPELL_SUNBEAM                     = 62872,
    SPELL_ATTUNED_TO_NATURE             = 62519,
    SPELL_TOUCH_OF_EONAR                = 62528,
    H_SPELL_TOUCH_OF_EONAR              = 62892,
    SPELL_LIFEBINDER_GIFT               = 62584,
    H_SPELL_LIFEBINDER_GIFT             = 62619,
    SPELL_NATURE_BOMB                   = 64587,
    H_SPELL_NATURE_BOMB                 = 64650,
    SPELL_BERSERK                       = 47008,

    SPELL_PHEROMONES_LG                 = 62619,
    SPELL_POTENT_PHEROMONES             = 62541,

    SPELL_SUMMON_ALLIES_OF_NATURE       = 62678, //better do that in sd2
    SPELL_SUMMON_WAVE_10                = 62688,
    SPELL_SUMMON_WAVE_3                 = 62686,
    SPELL_SUMMON_WAVE_1                 = 62685,
    SPELL_LIFEBINDERS_GIFT_SUMMON       = 62869,
    SPELL_NATURE_BOMB_SUMMON            = 64606,
    SPELL_SPORE_SUMMON_NE               = 62591,
    SPELL_SPORE_SUMMON_SE               = 62592,
    SPELL_SPORE_SUMMON_SW               = 62593,
    SPELL_SPORE_SUMMON_NW               = 62582,

    //abilities with Elder Brightleaf
    SPELL_UNSTABLE_ENERGY_FREYA         = 62451,
    H_SPELL_UNSTABLE_ENERGY_FREYA       = 62865,
    SPELL_BRIGHTLEAFS_ESSENCE           = 62385,
    SPELL_EFFECT_BRIGHTLEAF             = 63294,

    //abilities with Elder Ironbranch
    SPELL_IRON_ROOTS_FREYA              = 62283,
    H_SPELL_IRON_ROOTS_FREYA            = 62930,
    SPELL_IRONBRANCHS_ESSENCE           = 62387,
    SPELL_EFFECT_IRONBRANCH             = 63292,
    
    SPELL_STRENGTHENED_IRON_ROOTS_SUMM  = 63601, //better way to do that through SummonCreature for better control with summoned creature
    
    //abilities with Elder Stonebark 
    SPELL_GROUND_TREMOR_FREYA           = 62437,
    H_SPELL_GROUND_TREMOR_FREYA         = 62859,
    SPELL_STONEBARKS_ESSENCE            = 62386,
    SPELL_EFFECT_STONEBARK              = 63295,

    //elders
    SPELL_DRAINED_OF_POWER              = 62467,
    
    //Elder Brightleaf
    SPELL_BRIGHTLEAFS_FLUX              = 62262,
    SPELL_BRIGHTLEAFS_FLUXP             = 62251,
    SPELL_BRIGHTLEAFS_FLUXM             = 62252,
    SPELL_SOLAR_FLARE                   = 62240,
    H_SPELL_SOLAR_FLARE                 = 62920,
    SPELL_UNSTABLE_SUN_BEAM             = 62243,
    SPELL_UNSTABLE_ENERGY               = 62217,
    H_SPELL_UNSTABLE_ENERGY             = 62922,
    SPELL_PHOTOSYNTHESIS                = 62209,

    //Elder Ironbranch
    SPELL_IMPALE                        = 62310,
    H_SPELL_IMPALE                      = 62928,
    SPELL_IRON_ROOTS                    = 62438,
    H_SPELL_IRON_ROOTS                  = 62861,
    SPELL_THORN_SWARM                   = 62285,
    H_SPELL_THORN_SWARM                 = 62931,
    
    SPELL_IRON_ROOTS_SUMM               = 65160, // same as strengthened roots

    //Elder Stonebark
    SPELL_FIST_OF_STONE                 = 62344,
    //SPELL_BROKEN_BONES                = 62356, probably unused
    SPELL_GROUND_TREMOR                 = 62325,
    H_SPELL_GROUND_TREMOR               = 62932,
    SPELL_PETRIFIED_BARK                = 62337,
    H_SPELL_PETRIFIED_BARK              = 62933,
    SPELL_PETRIFIED_BARK_DMG            = 62379,

    //waves
    SPELL_DETONATE                      = 62598,
    H_SPELL_DETONATE                    = 62937,
    SPELL_FLAME_LASH                    = 62608,

    SPELL_CONSERVATORS_GRIP             = 62532,
    SPELL_NATURES_FURY                  = 62589,
    H_SPELL_NATURES_FURY                = 63571,

    SPELL_TIDAL_WAVE                    = 62653,
    H_SPELL_TIDAL_WAVE                  = 62935,

    SPELL_STORMBOLT                     = 62649,
    H_SPELL_STORMBOLT                   = 62938,
    SPELL_LIGHTNING_LASH                = 62648,
    H_SPELL_LIGHTNING_LASH              = 62939,
    SPELL_HARDENED_BARK                 = 62664,
    H_SPELL_HARDENED_BARK               = 64191,

    SPELL_LIFEBINDERS_VISUAL            = 62579,
    SPELL_LIFEBINDER_GROW               = 44833,

    SPELL_SUMMON_CHEST_1                = 62950,
    SPELL_SUMMON_CHEST_2                = 62952,
    SPELL_SUMMON_CHEST_3                = 62953,
    SPELL_SUMMON_CHEST_4                = 62954,
    //SPELL_SUMMON_CHEST_5                = 62955,       four chests is enough :)
    //SPELL_SUMMON_CHEST_6                = 62956,
    //SPELL_SUMMON_CHEST_7                = 62957,
    //SPELL_SUMMON_CHEST_8                = 62958,

    SPELL_HEALTHY_SPORE_VISUAL          = 62538,
    SPELL_NATURE_BOMB_VISUAL            = 64604,

    NPC_EONARS_GIFT                     = 33228,
    NPC_HEALTHY_SPORE                   = 33215,
    NPC_IRON_ROOTS                      = 33088,
    NPC_STRENGTHENED_IRON_ROOTS         = 33168,
    NPC_NATURE_BOMB                     = 34129,
    
    GO_NATURE_BOMB                      = 194902,

    NPC_ELDER_BRIGHTLEAF                = 32915,
    NPC_ELDER_IRONBRANCH                = 32913,
    NPC_ELDER_STONEBARK                 = 32914,

    NPC_WAVE_1                          = 33203,
    NPC_WAVE_3_WATER                    = 33202,
    NPC_WAVE_3_SNAPLASHER               = 32916,
    NPC_WAVE_3_STORM                    = 32919,
    NPC_WAVE_10                         = 32918,

    NPC_SUN_BEAM                        = 33170,
    NPC_UNSTABLE_SUN_BEAM               = 33050,

    ACHI_KNOCK_1_NORM                   = 3177,
    ACHI_KNOCK_2_NORM                   = 3178,
    ACHI_KNOCK_3_NORM                   = 3179,
    ACHI_KNOCK_1_HC                     = 3185,
    ACHI_KNOCK_2_HC                     = 3186,
    ACHI_KNOCK_3_HC                     = 3187,

    WAVE_COUNT                          = 6,
};

const struct OriginalOrder { uint32 entry, yell; }
InitialOrder[WAVE_COUNT] = 
{
    {SPELL_SUMMON_WAVE_10, SAY_WAVE_10},
    {SPELL_SUMMON_WAVE_10, SAY_WAVE_10},
    {SPELL_SUMMON_WAVE_3, SAY_WAVE_3},
    {SPELL_SUMMON_WAVE_3, SAY_WAVE_3},
    {SPELL_SUMMON_WAVE_1, SAY_WAVE_1},
    {SPELL_SUMMON_WAVE_1, SAY_WAVE_1},
};


Creature* GetFreya(Creature* pCreature, ScriptedInstance* instance){return (Creature*)Unit::GetUnit((*pCreature), instance->GetData64(DATA_FREYA));}

///////////////////
/// iron roots  ///
///////////////////
struct MANGOS_DLL_DECL mob_iron_rootsAI : public ScriptedAI
{
    mob_iron_rootsAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }
    
    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint64 m_uiVictimGUID;

    void Reset()
    {
        m_uiVictimGUID = 0;
    }
    
    void SetVictim(uint64 victim)
    {
        if (victim)
        {
            m_uiVictimGUID = victim;
            if (Unit* pVictim = Unit::GetUnit((*m_creature), m_uiVictimGUID))
            {
                pVictim->CastSpell(pVictim, m_bIsRegularMode ? SPELL_IRON_ROOTS : H_SPELL_IRON_ROOTS, true);
                pVictim->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            }
        }
    }

    void JustDied(Unit* Killer)
    {
        if (Unit* pVictim = Unit::GetUnit((*m_creature), m_uiVictimGUID))
            pVictim->RemoveAurasDueToSpell(m_bIsRegularMode ? SPELL_IRON_ROOTS : H_SPELL_IRON_ROOTS);
    }

    void AttackStart(Unit* pWho){ }
    void MoveInLineOfSight(Unit* pWho) { }
    void UpdateAI(const uint32 uiDiff) { }
};

//////////////////////////////
/// strenghtend iron roots ///
//////////////////////////////
struct MANGOS_DLL_DECL mob_str_iron_rootsAI : public ScriptedAI
{
    mob_str_iron_rootsAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }
    
    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint64 m_uiVictimGUID;

    void Reset()
    {
        m_uiVictimGUID = 0;
    }
    
    void SetVictim(uint64 victim)
    {
        if (victim)
        {
            m_uiVictimGUID = victim;
            if (Unit* pVictim = Unit::GetUnit((*m_creature), m_uiVictimGUID))
                pVictim->CastSpell(pVictim, m_bIsRegularMode ? SPELL_IRON_ROOTS_FREYA : H_SPELL_IRON_ROOTS_FREYA, true);
        }
    }

    void JustDied(Unit* Killer)
    {
        if (Unit* pVictim = Unit::GetUnit((*m_creature), m_uiVictimGUID))
            pVictim->RemoveAurasDueToSpell(m_bIsRegularMode ? SPELL_IRON_ROOTS_FREYA : H_SPELL_IRON_ROOTS_FREYA);
    }

    void AttackStart(Unit* pWho){ }
    void MoveInLineOfSight(Unit* pWho) { }
    void UpdateAI(const uint32 uiDiff) { }
};

///////////////
//// Freya ////
///////////////
struct MANGOS_DLL_DECL boss_freyaAI : public ScriptedAI
{
    boss_freyaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    Creature* pWater;
    Creature* pSnap;
    Creature* pStorm;

    uint32 SpellOrder[WAVE_COUNT];
    uint32 YellOrder[WAVE_COUNT];

    uint32 Wave_Timer;
    uint32 LifebinderGift_Timer;
    uint32 Sunbeam_Timer;
    uint32 Wave_Count;
    uint32 Berserk_Timer;
    uint32 ChangeFaction_Timer;
    uint32 EndPhaseDespawn_Timer;
    uint32 GroundTremor_Timer;
    uint32 IronRoots_Timer;
    uint32 SunBeams_Timer;
    uint32 NatureBomb_Timer;
    uint32 ThreeWaveDeath_Timer;

    bool EndPhase;
    bool HardMode;
    bool Knock1;
    bool Knock2;
    bool FactionChanged;

    bool BrightleafAlive;
    bool StonebarkAlive;
    bool IronbranchAlive;

    bool ThreeAddPhase;

    void Reset()
    {
        Wave_Timer = urand(9,12)*IN_MILLISECONDS;
        LifebinderGift_Timer = urand(19,23)*IN_MILLISECONDS;
        Sunbeam_Timer = urand(44,49)*IN_MILLISECONDS;
        Berserk_Timer = 10*MINUTE*IN_MILLISECONDS;
        ChangeFaction_Timer = 5*IN_MILLISECONDS;
        EndPhaseDespawn_Timer = 10*IN_MILLISECONDS;
        GroundTremor_Timer = urand(20,25)*IN_MILLISECONDS;
        IronRoots_Timer = urand(35,40)*IN_MILLISECONDS;
        SunBeams_Timer = urand(23,28)*IN_MILLISECONDS;
        NatureBomb_Timer = urand(15,20)*IN_MILLISECONDS;
        ThreeWaveDeath_Timer = 5*IN_MILLISECONDS;
        Wave_Count = 0;

        FactionChanged = false;
        HardMode = false;
        EndPhase = false;
        Knock1 = false;
        Knock2 = false;
        BrightleafAlive = true;
        StonebarkAlive = true;
        IronbranchAlive = true;

        pWater = 0;
        pSnap = 0;
        pStorm = 0;

        ThreeAddPhase = false;

        SetRandomOrder();
        SetCombatMovement(true);
        m_creature->setFaction(16);
    }

    void KilledUnit(Unit *victim)
    {
        DoScriptText(urand(0,1) ? SAY_SLAY_1 : SAY_SLAY_2, m_creature);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(m_bIsRegularMode ? SAY_AGGRO : SAY_AGGRO_HARD, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_FREYA, IN_PROGRESS);
        
        BuffOnAggro();
    }
    void AttackStart(Unit* pWho)
    {
        if (!pWho)
            return;

        if (EndPhase)
            return;
        
        if (m_creature->Attack(pWho, true))
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);

            if (IsCombatMovement())
                m_creature->GetMotionMaster()->MoveChase(pWho);
        }
    }
    
    void CompleteAchievement(uint16 entry)
    {
        //Map* pMap = m_creature->GetMap();
        //AchievementEntry const *Achievement = GetAchievementStore()->LookupEntry(entry);
        //if(Achievement && pMap)
        //{
        //    Map::PlayerList const &lPlayers = pMap->GetPlayers();
        //    if (!lPlayers.isEmpty())
        //    {
        //        for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
        //        {
        //            if (Player* pPlayer = itr->getSource())
        //                pPlayer->GetAchievementMgr().CompletedAchievement(Achievement);
        //        }
        //    }
        //}
    }
    void DamageTaken(Unit *done_by, uint32 &damage)
    {
        if (damage > m_creature->GetHealth())
        {
            damage = 0;
            m_creature->SetHealth(1);
            SetCombatMovement(false);
            m_creature->GetMotionMaster()->Clear(false);
            EndPhase = true;
        }
    }

    void SetRandomOrder()
    {
        for(int8 i = 0; i < WAVE_COUNT;i++)
        {
            SpellOrder[i] = 0;
            YellOrder[i] = 0;
        }

        for(int8 i = 0; i < WAVE_COUNT;)
        {
            int8 pos = urand(0,5);
            if(SpellOrder[pos] == 0 && YellOrder[pos] == 0)
            {
                SpellOrder[pos] = InitialOrder[i].entry;
                YellOrder[pos] = InitialOrder[i].yell;
                i++;
            }
        }
    }

    void JustReachedHome()
    {
        DespawnAllCreaturesWithEntry(NPC_NATURE_BOMB);
        DespawnAllCreaturesWithEntry(NPC_IRON_ROOTS);
        DespawnAllCreaturesWithEntry(NPC_STRENGTHENED_IRON_ROOTS);
        DespawnAllCreaturesWithEntry(NPC_WAVE_1);
        DespawnAllCreaturesWithEntry(NPC_WAVE_3_WATER);
        DespawnAllCreaturesWithEntry(NPC_WAVE_3_SNAPLASHER);
        DespawnAllCreaturesWithEntry(NPC_WAVE_3_STORM);
        DespawnAllCreaturesWithEntry(NPC_WAVE_10);
        DespawnAllCreaturesWithEntry(NPC_EONARS_GIFT);

        for(uint32 i = NPC_ELDER_IRONBRANCH; i <= NPC_ELDER_BRIGHTLEAF; i++)
            if(Creature* pTemp = GetClosestCreatureWithEntry(m_creature, i, 180.0f))
                pTemp->RemoveAllAuras();
    }


    void BuffOnAggro()
    {
        m_creature->CastSpell(m_creature, SPELL_ATTUNED_TO_NATURE, true);
        if(Aura *pAura = m_creature->GetAura(SPELL_ATTUNED_TO_NATURE, EFFECT_INDEX_0))
            pAura->SetStackAmount(150);
        m_creature->CastSpell(m_creature, m_bIsRegularMode ? SPELL_TOUCH_OF_EONAR : H_SPELL_TOUCH_OF_EONAR, true);

        if(Creature* pBright = GetClosestCreatureWithEntry(m_creature, NPC_ELDER_BRIGHTLEAF, 180.0f))
        {
            m_creature->CastSpell(m_creature, SPELL_BRIGHTLEAFS_ESSENCE, true);
            pBright->CastSpell(m_creature, SPELL_EFFECT_BRIGHTLEAF, true);
            pBright->CastSpell(pBright, SPELL_DRAINED_OF_POWER, true);
            BrightleafAlive = true;
        }else BrightleafAlive = false;

        if(Creature* pIron = GetClosestCreatureWithEntry(m_creature, NPC_ELDER_IRONBRANCH, 180.0f))
        {
            m_creature->CastSpell(m_creature, SPELL_IRONBRANCHS_ESSENCE, true);
            pIron->CastSpell(m_creature, SPELL_EFFECT_IRONBRANCH, true);
            pIron->CastSpell(pIron, SPELL_DRAINED_OF_POWER, true);
            IronbranchAlive = true;
        }else IronbranchAlive = false;

        if(Creature* pStone = GetClosestCreatureWithEntry(m_creature, NPC_ELDER_STONEBARK, 180.0f))
        {
            m_creature->CastSpell(m_creature, SPELL_STONEBARKS_ESSENCE, true);
            pStone->CastSpell(m_creature, SPELL_EFFECT_STONEBARK, true);
            pStone->CastSpell(pStone, SPELL_DRAINED_OF_POWER, true);
            StonebarkAlive = true;
        }else StonebarkAlive = false;

        if(BrightleafAlive && IronbranchAlive && StonebarkAlive)
            HardMode = true;
        if(BrightleafAlive || IronbranchAlive || StonebarkAlive)
            Knock1 = true;
        if((BrightleafAlive && IronbranchAlive) || 
            (BrightleafAlive && StonebarkAlive) || 
            (IronbranchAlive && StonebarkAlive))
            Knock2 = true;

        m_creature->SetHealth(m_creature->GetMaxHealth());
    }

    void DespawnAllCreaturesWithEntry(uint32 entry)
    {
        std::list<Creature*> m_pCreatures;
        GetCreatureListWithEntryInGrid(m_pCreatures, m_creature, entry, 150.0f);
 
        if (m_pCreatures.empty())
            return;
 
        for(std::list<Creature*>::iterator iter = m_pCreatures.begin(); iter != m_pCreatures.end(); ++iter)
            (*iter)->ForcedDespawn();
    }

    void UpdateAI(const uint32 diff)
    {
        if(EndPhase)
        {
            if(ChangeFaction_Timer < diff && !FactionChanged)
            {
                DoScriptText(SAY_DEATH, m_creature);
                m_creature->AttackStop();
                m_creature->RemoveAllAuras();
                m_creature->CastSpell(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0), m_bIsRegularMode ? (HardMode ? SPELL_SUMMON_CHEST_2 : SPELL_SUMMON_CHEST_1) : (HardMode ? SPELL_SUMMON_CHEST_4 : SPELL_SUMMON_CHEST_3), true);
                if(HardMode)
                    CompleteAchievement(m_bIsRegularMode ? ACHI_KNOCK_3_NORM : ACHI_KNOCK_3_HC);
                else if(Knock2)
                    CompleteAchievement(m_bIsRegularMode ? ACHI_KNOCK_2_NORM : ACHI_KNOCK_2_HC);
                else if(Knock1)
                    CompleteAchievement(m_bIsRegularMode ? ACHI_KNOCK_1_NORM : ACHI_KNOCK_1_HC);				
                m_creature->CombatStop(true);
                m_creature->setFaction(35);
                FactionChanged = true;
            }else ChangeFaction_Timer -= diff;

            if(EndPhaseDespawn_Timer < diff)
                m_creature->ForcedDespawn();
            else EndPhaseDespawn_Timer -= diff;
            return;
        }
        if(ThreeAddPhase)
        {
            if(!pWater->isAlive() || !pStorm->isAlive() || !pSnap->isAlive())
            {
                if(!pWater->isAlive() && !pStorm->isAlive() && !pSnap->isAlive())
                {
                    pWater->ForcedDespawn();
                    pStorm->ForcedDespawn();
                    pSnap->ForcedDespawn();
                    ThreeAddPhase = false;
                    return;
                }
                if(ThreeWaveDeath_Timer < diff)
                {
                    if(pWater && pWater->isDead())
                        pWater->Respawn();
                    if(pStorm && pStorm->isDead())
                        pStorm->Respawn();
                    if(pSnap && pSnap->isDead())
                        pSnap->Respawn();
                }else ThreeWaveDeath_Timer -= diff;
            
            }else ThreeWaveDeath_Timer = 5*IN_MILLISECONDS;
        
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(Wave_Timer < diff && Wave_Count < WAVE_COUNT)
        {
            uint32 spell = SpellOrder[Wave_Count];
            uint32 yell = YellOrder[Wave_Count];
            m_creature->CastSpell(m_creature, spell, true);
            DoScriptText(yell, m_creature);
            
            if(spell == SPELL_SUMMON_WAVE_3)
                ThreeAddPhase = true;
            
            Wave_Timer = MINUTE*IN_MILLISECONDS;
            Wave_Count++;
        }else Wave_Timer -= diff;

        if (LifebinderGift_Timer < diff)
        {
            DoCast(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0), SPELL_LIFEBINDERS_GIFT_SUMMON);
            LifebinderGift_Timer = urand(38, 42)*1000;
        }else LifebinderGift_Timer -= diff;

        if(Sunbeam_Timer < diff)
        {
            DoCast(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0), m_bIsRegularMode ? SPELL_SUNBEAM : H_SPELL_SUNBEAM);
            Sunbeam_Timer = urand(15,20)*IN_MILLISECONDS;		
        }else Sunbeam_Timer -= diff;

        if(StonebarkAlive)
        {
            if(GroundTremor_Timer < diff)
            {
                DoCast(m_creature, m_bIsRegularMode ? SPELL_GROUND_TREMOR_FREYA : H_SPELL_GROUND_TREMOR_FREYA);
                GroundTremor_Timer = urand(26,29)*IN_MILLISECONDS;
            }else GroundTremor_Timer -= diff;
        }
        if(IronbranchAlive)
        {
            if(IronRoots_Timer < diff)
            {
                int8 times = m_bIsRegularMode ? 1 : 2;
                for(int8 i = 0; i < times; )
                {
                    if(Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,1))
                    {
                        if(target->HasAura(SPELL_IRON_ROOTS_FREYA || H_SPELL_IRON_ROOTS_FREYA))
                            return;

                        float x = target->GetPositionX();
                        float y = target->GetPositionY();
                        float z = target->GetPositionZ();
                        if(Creature* pRoots = m_creature->SummonCreature(NPC_STRENGTHENED_IRON_ROOTS, x, y, z, 0, TEMPSUMMON_DEAD_DESPAWN, 0))
                            ((mob_str_iron_rootsAI*)pRoots->AI())->SetVictim(target->GetGUID());
                        DoTeleportPlayer(target, x, y, z, target->GetOrientation());
                        i++;
                    }else break;
                }
                IronRoots_Timer = urand(50,70)*IN_MILLISECONDS;
            }else IronRoots_Timer -= diff;
        }

        if(BrightleafAlive)
        {
            if(SunBeams_Timer < diff)
            {
                for(int8 i = 0; i < 3; i++)
                {
                    if(Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                    {
                        float x = target->GetPositionX();
                        float y = target->GetPositionY();
                        float z = target->GetPositionZ();
                        m_creature->SummonCreature(NPC_SUN_BEAM, x, y, z, 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 20000);
                    }
                }
                SunBeams_Timer = urand(29,33)*IN_MILLISECONDS;
            }else SunBeams_Timer -= diff;
        }

        //if(Wave_Count == WAVE_COUNT)
        //{
            if(NatureBomb_Timer < diff)
            {
                m_creature->CastSpell(m_creature, SPELL_NATURE_BOMB_VISUAL, true);
                m_creature->CastSpell(m_creature, SPELL_NATURE_BOMB_SUMMON, true);
                
                int8 count = urand(8,10);
                for(int8 i = 0; i < count; i++)
                {
                    float radius = 30* rand_norm_f();
                    float angle = 2.0f * M_PI_F * rand_norm_f();
                    float x = m_creature->GetPositionX() + cos(angle) * radius;
                    float y = m_creature->GetPositionY() + sin(angle) * radius;
                    float z = m_creature->GetMap()->GetHeight(x, y, MAX_HEIGHT);
                    m_creature->SummonCreature(NPC_NATURE_BOMB, x, y, z, 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 20000);
                    m_creature->SummonGameobject(GO_NATURE_BOMB, x, y, z, 0, 25000);
                }
                NatureBomb_Timer = urand(15,18)*IN_MILLISECONDS;
            }else NatureBomb_Timer -= diff;
        //}

        if(Berserk_Timer < diff)
        {
            DoCast(m_creature, SPELL_BERSERK);
            DoScriptText(SAY_BERSERK, m_creature);
            Berserk_Timer = 60*IN_MILLISECONDS;		
        }else Berserk_Timer -= diff;
        
        DoMeleeAttackIfReady();

    }

};

/////////////////////////////////////////////////
/// nature bomb / eonar's gift / healthy spore///
/////////////////////////////////////////////////
struct MANGOS_DLL_DECL mob_freya_groundAI : public ScriptedAI
{
     mob_freya_groundAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 NatureBomb_Timer;
    uint32 EonarsGift_Timer;
    uint32 NonSelectable_Timer;
    uint32 Grow_Timer;
    uint32 SunBeamDespawn_Timer;
    uint32 UnstableEnergy_Timer;
    uint32 HealthyGrow_Timer;
    float size;

    bool NpcNatureBomb;
    bool NpcEonarsGift;
    bool NpcHealthySpore;
    bool NpcSunBeamFreya;
    bool NpcSunBeamBright;

    bool Grow;

    GameObject* Bomb;

    void Reset()
    {
        NatureBomb_Timer = urand(9,11)*IN_MILLISECONDS;
        EonarsGift_Timer = urand(11,13)*IN_MILLISECONDS;
        NonSelectable_Timer = 5*IN_MILLISECONDS;
        UnstableEnergy_Timer = IN_MILLISECONDS;
        Grow_Timer = 0;
        SunBeamDespawn_Timer = urand(10,11)*IN_MILLISECONDS;
        Grow = true;
        HealthyGrow_Timer = urand(3,12)*IN_MILLISECONDS;
        NpcNatureBomb = false;
        NpcEonarsGift = false;
        NpcHealthySpore = false;
        NpcSunBeamFreya = false;
        NpcSunBeamBright = false;
        Bomb = NULL;
        switch(m_creature->GetEntry())
        {
            case NPC_NATURE_BOMB:
                NpcNatureBomb = true;
                size = 1;
                m_creature->CastSpell(m_creature, SPELL_LIFEBINDERS_VISUAL, true);
                Bomb = m_creature->SummonGameobject(GO_NATURE_BOMB, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ()+1, 0, 25000);
                break;
            case NPC_EONARS_GIFT:
                NpcEonarsGift = true;
                size = float(0.1);
                m_creature->CastSpell(m_creature, SPELL_LIFEBINDERS_VISUAL, true);
                break;
            case NPC_HEALTHY_SPORE:
                NpcHealthySpore = true; 
                m_creature->CastSpell(m_creature, SPELL_HEALTHY_SPORE_VISUAL, true);
                m_creature->CastSpell(m_creature, SPELL_POTENT_PHEROMONES, true);
                break;
            case NPC_SUN_BEAM:
                NpcSunBeamFreya = true;
                m_creature->CastSpell(m_creature, SPELL_LIFEBINDERS_VISUAL, true);
                m_creature->CastSpell(m_creature, m_bIsRegularMode ? SPELL_UNSTABLE_ENERGY_FREYA : H_SPELL_UNSTABLE_ENERGY_FREYA, true);
                break;

            case NPC_UNSTABLE_SUN_BEAM:
                NpcSunBeamBright = true; 
                m_creature->CastSpell(m_creature, SPELL_LIFEBINDERS_VISUAL, true);
                m_creature->CastSpell(m_creature, SPELL_PHOTOSYNTHESIS, true);
                break;
        }

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
           
    }
    void AttackStart(Unit* pWho){return;}

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->isAlive())
            return;

        if(NpcNatureBomb)
            if(NatureBomb_Timer < diff)
            {
                m_creature->CastSpell(m_creature, m_bIsRegularMode ? SPELL_NATURE_BOMB : H_SPELL_NATURE_BOMB, true);
                m_creature->ForcedDespawn();
                if(Bomb)
                    Bomb->Delete();
            }else NatureBomb_Timer -= diff;

        if(NpcEonarsGift)
        {
            if (Grow_Timer > 500 && size < 1.5)
            {
                size += float(Grow_Timer)/8000;
                m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X, size);
                Grow_Timer = 0;
            }else Grow_Timer += diff;

            if(EonarsGift_Timer < diff)
            {
                m_creature->CastSpell(GetFreya(m_creature, m_pInstance), m_bIsRegularMode ? SPELL_LIFEBINDER_GIFT : H_SPELL_LIFEBINDER_GIFT, true);
                EonarsGift_Timer = IN_MILLISECONDS;
            }else EonarsGift_Timer -= diff;

            if(NonSelectable_Timer < diff && m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE))
            {
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                m_creature->CastSpell(m_creature, SPELL_PHEROMONES_LG, true);
            }else NonSelectable_Timer -= diff;
        }
        if(NpcHealthySpore)
        {
            if(!Grow && size < 0.25)
                m_creature->ForcedDespawn();

            if(HealthyGrow_Timer < diff)
            {
                if(Grow)
                {
                    size = float(urand(150,225))/100;
                    Grow = false;
                }
                else
                    size = float(urand(1,300))/100;
                if(size < 1)
                    size = 0.1f;
                m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X, size);
                HealthyGrow_Timer = urand(3,5)*IN_MILLISECONDS;
            }else HealthyGrow_Timer -= diff;

        }
        if(NpcSunBeamBright)
        {
            if(UnstableEnergy_Timer < diff)
            {
                m_creature->CastSpell(m_creature, m_bIsRegularMode ? SPELL_UNSTABLE_ENERGY : H_SPELL_UNSTABLE_ENERGY, true);
                UnstableEnergy_Timer = IN_MILLISECONDS;
            }else UnstableEnergy_Timer -= diff;
        
        }
        if(NpcSunBeamFreya || NpcSunBeamBright)
        {
            if(SunBeamDespawn_Timer < diff)
                m_creature->ForcedDespawn();
            else SunBeamDespawn_Timer -= diff;
        }
    }
};

////////////////////////
/// Elder Brightleaf ///
////////////////////////
struct MANGOS_DLL_DECL boss_elder_brightleafAI : public ScriptedAI
{
    boss_elder_brightleafAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }
    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 BrightleafsFlux_Timer;
    uint32 SolarFlare_Timer;
    uint32 UnstableSunBeam_Timer;

    void Reset()
    {
        BrightleafsFlux_Timer = 2*IN_MILLISECONDS;
        UnstableSunBeam_Timer = 3*IN_MILLISECONDS;
        SolarFlare_Timer = urand(50,60)*IN_MILLISECONDS;
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_BRIGHT_AGGRO, m_creature);
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_BRIGHT_DEATH, m_creature);
    }

    void KilledUnit(Unit *victim)
    {
        DoScriptText(urand(0,1) ? SAY_BRIGHT_SLAY_1 : SAY_BRIGHT_SLAY_2, m_creature);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (BrightleafsFlux_Timer < diff)
        {
            DoCast(m_creature, SPELL_BRIGHTLEAFS_FLUX);
            BrightleafsFlux_Timer = 5*IN_MILLISECONDS;
        }else BrightleafsFlux_Timer -= diff;

        if (UnstableSunBeam_Timer < diff)
        {
            DoCast(m_creature, SPELL_UNSTABLE_SUN_BEAM);
            UnstableSunBeam_Timer = 5*IN_MILLISECONDS;
        }else UnstableSunBeam_Timer -= diff;

        if (SolarFlare_Timer < diff)
        {
            DoCast(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0), m_bIsRegularMode ? SPELL_SOLAR_FLARE : H_SPELL_SOLAR_FLARE);
            SolarFlare_Timer = urand(40,50)*IN_MILLISECONDS;
        }else SolarFlare_Timer -= diff;

         DoMeleeAttackIfReady();
    }
};

////////////////////////
/// Elder Ironbranch ///
////////////////////////
struct MANGOS_DLL_DECL boss_elder_ironbranchAI : public ScriptedAI
{
    boss_elder_ironbranchAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 IronRoots_Timer;
    uint32 ThronSwarm_Timer;
    uint32 Impale_Timer;
    
    void Reset()
    {
        IronRoots_Timer = urand(8,10)*IN_MILLISECONDS;
        ThronSwarm_Timer = urand(4,6)*IN_MILLISECONDS;
        Impale_Timer = urand(40,50)*IN_MILLISECONDS;;
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_IRON_AGGRO, m_creature);
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_IRON_DEATH, m_creature);
    }

    void KilledUnit(Unit *victim)
    {
        DoScriptText(urand(0,1) ? SAY_IRON_SLAY_1 : SAY_IRON_SLAY_2, m_creature);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (IronRoots_Timer < diff)
        {
            int8 times = m_bIsRegularMode ? 1 : 2;
            for(int8 i = 0; i < times; )
            {
                if(Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,1))
                {
                    if(target->HasAura(SPELL_IRON_ROOTS || H_SPELL_IRON_ROOTS))
                        return;

                    float x = target->GetPositionX();
                    float y = target->GetPositionY();
                    float z = target->GetPositionZ();
                    if(Creature* pRoots = m_creature->SummonCreature(NPC_IRON_ROOTS, x, y, z, 0, TEMPSUMMON_DEAD_DESPAWN, 0))
                        ((mob_iron_rootsAI*)pRoots->AI())->SetVictim(target->GetGUID());
                    DoTeleportPlayer(target, x, y, z, target->GetOrientation());
                    i++;
                }else break;
            }
            IronRoots_Timer = urand(23, 28)*IN_MILLISECONDS;
        }else IronRoots_Timer -= diff;

        if (ThronSwarm_Timer < diff)
        {
            if(Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,1))
                DoCast(target, m_bIsRegularMode ? SPELL_THORN_SWARM : H_SPELL_THORN_SWARM);
            ThronSwarm_Timer = urand(9,11)*IN_MILLISECONDS;
        }else ThronSwarm_Timer -= diff;

        if (Impale_Timer < diff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_IMPALE : H_SPELL_IMPALE);
            Impale_Timer = urand(50,60)*IN_MILLISECONDS;
        }else Impale_Timer -= diff;

         DoMeleeAttackIfReady();
    }
};

///////////////////////
/// Elder Stonebark ///
///////////////////////
struct MANGOS_DLL_DECL boss_elder_stonebarkAI : public ScriptedAI
{
    boss_elder_stonebarkAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }
    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 GroundTremor_Timer;
    uint32 FistOfStone_Timer;
    uint32 PetrifiedBark_Timer;
    void Reset()
    {
        GroundTremor_Timer = urand(7,10)*IN_MILLISECONDS;
        FistOfStone_Timer = urand(13,16)*IN_MILLISECONDS;
        PetrifiedBark_Timer = urand(30,40)*IN_MILLISECONDS;
    }
    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_STONE_AGGRO, m_creature);
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_STONE_DEATH, m_creature);
    }

    void KilledUnit(Unit *victim)
    {
        DoScriptText(urand(0,1) ? SAY_STONE_SLAY_1 : SAY_STONE_SLAY_2, m_creature);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(GroundTremor_Timer < diff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_GROUND_TREMOR : H_SPELL_GROUND_TREMOR);
            GroundTremor_Timer = urand(18,22)*IN_MILLISECONDS;
        }else GroundTremor_Timer -= diff;

        if(FistOfStone_Timer < diff)
        {
            DoCast(m_creature, SPELL_FIST_OF_STONE);
            FistOfStone_Timer = urand(45,55)*IN_MILLISECONDS;
        }else FistOfStone_Timer -= diff;

        if(PetrifiedBark_Timer < diff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_PETRIFIED_BARK : H_SPELL_PETRIFIED_BARK);
            PetrifiedBark_Timer = urand(30,40)*IN_MILLISECONDS;
        }else PetrifiedBark_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

///////////////////
/// spawned adds///
///////////////////
struct MANGOS_DLL_DECL mob_freya_spawnedAI : public ScriptedAI
{
    mob_freya_spawnedAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    bool AncientConservator;
    bool DetonatingLasher;
    bool AncientWaterSpirit;
    bool StormLasher;
    bool Snaplasher;

    uint32 DeathCountdown;
    uint32 TidalWave_Timer;
    uint32 Stormbolt_Timer;
    uint32 LightningLash_Timer;
    uint32 FlameLash_Timer;
    uint32 NaturesFury_Timer;
    uint32 Wave3_DeathCountdown;
    uint32 RespawnSpores_Timer;

    Creature* Freya;

    void Reset()
    {
        AttackStart(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0));
        AncientWaterSpirit = false;
        StormLasher = false;
        Snaplasher = false;
        AncientConservator = false;
        DetonatingLasher = false;
        DeathCountdown = 10*IN_MILLISECONDS;
        TidalWave_Timer = urand(2,4)*IN_MILLISECONDS;
        Stormbolt_Timer = IN_MILLISECONDS;
        LightningLash_Timer = urand(11,14)*IN_MILLISECONDS;        
        FlameLash_Timer = urand(5,10)*IN_MILLISECONDS;
        NaturesFury_Timer = urand(8,10)*IN_MILLISECONDS;
        RespawnSpores_Timer = 5*IN_MILLISECONDS;
        Freya = GetFreya(m_creature, m_pInstance);

        if(!Freya)
            return;

        switch(m_creature->GetEntry())
        {
            case NPC_WAVE_1:
                AncientConservator = true;
                m_creature->CastSpell(m_creature, SPELL_CONSERVATORS_GRIP, true);
                DoSpores(10);
                break;
            case NPC_WAVE_10:
                DetonatingLasher = true;
                break;
            case NPC_WAVE_3_WATER:
                AncientWaterSpirit = true;
                ((boss_freyaAI*)Freya->AI())->pWater = m_creature;
                break;
            case NPC_WAVE_3_SNAPLASHER:
                Snaplasher = true;
                ((boss_freyaAI*)Freya->AI())->pSnap = m_creature;
                m_creature->CastSpell(m_creature, m_bIsRegularMode ? SPELL_HARDENED_BARK : H_SPELL_HARDENED_BARK, true);
                break;
            case NPC_WAVE_3_STORM:
                StormLasher = true;
                ((boss_freyaAI*)Freya->AI())->pStorm = m_creature;
                break;
        }

    }
    void JustDied(Unit* Killer)
    {
        if(!Freya)
            return;

        if (AncientConservator)
            ReduceStack(30);
        if (DetonatingLasher)
            ReduceStack(2);
        if(AncientWaterSpirit || Snaplasher || StormLasher)
            ReduceStack(10);
    }

    void ReduceStack(uint8 count)
    {
        if(Aura *pAura = Freya->GetAura(SPELL_ATTUNED_TO_NATURE, EFFECT_INDEX_0))
        {
            if(count > pAura->GetStackAmount())
            {
                pAura->SetStackAmount(1);
                pAura->DropAuraCharge();
            }else pAura->SetStackAmount(pAura->GetStackAmount()-count);
        }
    }

    void DamageTaken(Unit *done_by, uint32 &damage)
    {
        if (DetonatingLasher && damage > m_creature->GetHealth())
            m_creature->CastSpell(m_creature, m_bIsRegularMode ? SPELL_DETONATE : H_SPELL_DETONATE, true);
    }

    void DoSpores(int8 times)
    {
        for(int8 i = 0; i < times; i++)
        {
            for(int8 itr = 0; i < 3; i++)
                m_creature->CastSpell(m_creature, SPELL_SPORE_SUMMON_NE+itr, true);
            m_creature->CastSpell(m_creature, SPELL_SPORE_SUMMON_NW, true);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_creature->isAlive())
            return;
        
        if(DetonatingLasher && FlameLash_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_FLAME_LASH);
            FlameLash_Timer = urand(5,10)*IN_MILLISECONDS;
        }else FlameLash_Timer -= diff;

        if(AncientConservator)
        {
            if(NaturesFury_Timer < diff)
            {
                DoCast(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0), m_bIsRegularMode ? SPELL_NATURES_FURY : H_SPELL_NATURES_FURY);
                NaturesFury_Timer = urand(5,6)*IN_MILLISECONDS;
            }else NaturesFury_Timer -= diff;

            if(RespawnSpores_Timer < diff)
            {
                DoSpores(3);
                RespawnSpores_Timer = 5*IN_MILLISECONDS;
            }else RespawnSpores_Timer -= diff;
        }

        if(AncientWaterSpirit && TidalWave_Timer < diff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_TIDAL_WAVE : H_SPELL_TIDAL_WAVE);
            TidalWave_Timer = urand(7,9)*IN_MILLISECONDS;
        }else TidalWave_Timer -= diff;

        if(StormLasher)
        {
            if (LightningLash_Timer < diff)
            {
                DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_LIGHTNING_LASH : H_SPELL_LIGHTNING_LASH);
                LightningLash_Timer = urand(11,14)*IN_MILLISECONDS;
            }else
            {
                LightningLash_Timer -= diff;
                if (Stormbolt_Timer <diff)
                {
                    DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_STORMBOLT : H_SPELL_STORMBOLT);
                    Stormbolt_Timer = 2*IN_MILLISECONDS;
                }else Stormbolt_Timer -= diff;
            }
        }

        DoMeleeAttackIfReady();
    }
};


CreatureAI* GetAI_boss_freya(Creature* pCreature)
{
    return new boss_freyaAI(pCreature);
}
CreatureAI* GetAI_mob_freya_ground(Creature* pCreature)
{
    return new mob_freya_groundAI(pCreature);
}
CreatureAI* GetAI_mob_iron_roots(Creature* pCreature)
{
    return new mob_iron_rootsAI(pCreature);
}
CreatureAI* GetAI_mob_str_iron_roots(Creature* pCreature)
{
    return new mob_str_iron_rootsAI(pCreature);
}
CreatureAI* GetAI_boss_elder_brightleaf(Creature* pCreature)
{
    return new boss_elder_brightleafAI(pCreature);
}
CreatureAI* GetAI_boss_elder_ironbranch(Creature* pCreature)
{
    return new boss_elder_ironbranchAI(pCreature);
}
CreatureAI* GetAI_boss_elder_stonebark(Creature* pCreature)
{
    return new boss_elder_stonebarkAI(pCreature);
}
CreatureAI* GetAI_mob_freya_spawned(Creature* pCreature)
{
    return new mob_freya_spawnedAI(pCreature);
}
void AddSC_boss_freya()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_freya";
    newscript->GetAI = &GetAI_boss_freya;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_freya_ground";
    newscript->GetAI = &GetAI_mob_freya_ground;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_iron_roots";
    newscript->GetAI = &GetAI_mob_iron_roots;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_iron_roots";
    newscript->GetAI = &GetAI_mob_iron_roots;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_elder_brightleaf";
    newscript->GetAI = &GetAI_boss_elder_brightleaf;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name = "boss_elder_ironbranch";
    newscript->GetAI = &GetAI_boss_elder_ironbranch;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name = "boss_elder_stonebark";
    newscript->GetAI = &GetAI_boss_elder_stonebark;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_freya_spawned";
    newscript->GetAI = &GetAI_mob_freya_spawned;
    newscript->RegisterSelf();

}