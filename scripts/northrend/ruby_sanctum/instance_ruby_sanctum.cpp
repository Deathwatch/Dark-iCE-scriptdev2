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
SDName: Instance_Ruby_Sanctum
SD%Complete: 10%
SDComment: Working on it lol
SDCategory: Ruby Sanctum
EndScriptData */

#include "precompiled.h"
#include "ruby_sanctum.h"

/* Ruby Sanctum encounters:
0 - Halion
*/

struct MANGOS_DLL_DECL instance_ruby_sanctum : public ScriptedInstance
{
    instance_ruby_sanctum(Map* pMap) : ScriptedInstance(pMap) {Initialize();};

    uint32 m_auiEncounter[MAX_ENCOUNTER];
    uint64 m_uiHalion0GUID;
    uint64 m_uiZarithrian0GUID;
    uint64 m_uiBaltharus0GUID;
    uint64 m_uiSaviana0GUID;

    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

        m_uiHalion0GUID		= 0;
        m_uiZarithrian0GUID  = 0;
        m_uiBaltharus0GUID   = 0;
        m_uiSaviana0GUID		= 0;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case NPC_HALION0:
                m_uiHalion0GUID = pCreature->GetGUID();
                break;
            //three dragons below set to active state once created.
            //we must expect bigger raid to encounter main boss, and then three dragons must be active due to grid differences
            case NPC_ZARITHRIAN0:
                m_uiZarithrian0GUID = pCreature->GetGUID();
                pCreature->SetActiveObjectState(true);
                break;
            case NPC_BALTHARUS0:
                m_uiBaltharus0GUID = pCreature->GetGUID();
                pCreature->SetActiveObjectState(true);
                break;
            case NPC_SAVIANA0:
                m_uiSaviana0GUID = pCreature->GetGUID();
                pCreature->SetActiveObjectState(true);
                break;
        }
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        if (uiType == TYPE_DEAD0_EVENT)
            m_auiEncounter[0] = uiData;
		if (uiType == TYPE_HALION0_EVENT)
            m_auiEncounter[0] = uiData;
    }

    uint32 GetData(uint32 uiType)
    {
        if (uiType == TYPE_DEAD0_EVENT)
            return m_auiEncounter[0];
		if (uiType == TYPE_HALION0_EVENT)
            return m_auiEncounter[0];

        return 0;
    }

    uint64 GetData64(uint32 uiData)
    {
        switch(uiData)
        {
            case DATA_HALION0:
                return m_uiHalion0GUID;
            case DATA_ZARITHRIAN0:
                return m_uiZarithrian0GUID;
            case DATA_BALTHARUS0:
                return m_uiBaltharus0GUID;
            case DATA_SAVIANA0:
                return m_uiSaviana0GUID;
        }
        return 0;
    }
};

InstanceData* GetInstanceData_instance_ruby_sanctum(Map* pMap)
{
    return new instance_ruby_sanctum(pMap);
}

void AddSC_instance_ruby_sanctum()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_ruby_sanctum";
    newscript->GetInstanceData = GetInstanceData_instance_ruby_sanctum;
    newscript->RegisterSelf();
}