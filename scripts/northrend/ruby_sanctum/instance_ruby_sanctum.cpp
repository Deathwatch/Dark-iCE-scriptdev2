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

struct MANGOS_DLL_DECL instance_ruby_sanctum : public ScriptedInstance
{
    instance_ruby_sanctum(Map *pMap) : ScriptedInstance(pMap) {Initialize();}

    uint32 m_auiEncounter[MAX_ENCOUNTER];
    std::string strSaveData;

    uint64 m_uiHalionGUID;
    uint64 m_uiZarithrianGUID;
    uint64 m_uiBaltharusGUID;
    uint64 m_uiSavianaGUID;

    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

        m_uiHalionGUID             = 0;
        m_uiZarithrianGUID         = 0;
        m_uiBaltharusGUID          = 0;
        m_uiSavianaGUID            = 0;
    }

    bool IsEncounterInProgress() const
    {
        for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
            if (m_auiEncounter[i] == IN_PROGRESS)
                return true;

        return false;
    }

    void OnCreatureCreate(Creature* pCreature)
    {
        switch (pCreature->GetEntry())
        {
            case 39863: m_uiHalionGUID      = pCreature->GetGUID(); break;
			case 39746: m_uiZarithrianGUID  = pCreature->GetGUID(); break;
			case 39751: m_uiBaltharusGUID   = pCreature->GetGUID(); break;
			case 39747: m_uiSavianaGUID     = pCreature->GetGUID(); break;
        }
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        if (uiData == DONE)
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;
            saveStream << m_auiEncounter[0];

            strSaveData = saveStream.str();

            SaveToDB();
            OUT_SAVE_INST_DATA_COMPLETE;
        }
    }

    const char* Save()
    {
        return strSaveData.c_str();
    }

    uint32 GetData(uint32 uiType)
    {
        switch (uiType)
        {
            case TYPE_HALION_EVENT:    return m_auiEncounter[0];
        }
        return 0;
    }

    uint64 GetData64(uint32 uiData)
    {
        switch (uiData)
        {
            case DATA_HALION:              return m_uiHalionGUID;
            case DATA_BALTHARUS:           return m_uiZarithrianGUID;
            case DATA_ZARITHRIAN:          return m_uiBaltharusGUID;
            case DATA_SAVIANA:             return m_uiSavianaGUID;
        }
        return 0;
    }

    void Load(const char* chrIn)
    {
        if (!chrIn)
        {
            OUT_LOAD_INST_DATA_FAIL;
            return;
        }

        OUT_LOAD_INST_DATA(chrIn);

        std::istringstream loadStream(chrIn);

        loadStream >> m_auiEncounter[0] >> m_auiEncounter[1];

        for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
            if (m_auiEncounter[i] == IN_PROGRESS)
                m_auiEncounter[i] = NOT_STARTED;

        OUT_LOAD_INST_DATA_COMPLETE;
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
    newscript->GetInstanceData = &GetInstanceData_instance_ruby_sanctum;
    newscript->RegisterSelf();
}
