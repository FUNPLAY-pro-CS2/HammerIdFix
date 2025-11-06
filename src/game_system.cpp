//
// Created by Michal Přikryl on 21.09.2025.
// Copyright (c) 2025 slynxcz. All rights reserved.
//
#include "Shared.h"
#include "game_system.h"
#include <tier0/vprof.h>

#include "obfuscate.h"
#include "dynlibutils/module.h"

CBaseGameSystemFactory** CBaseGameSystemFactory::sm_pFirst = nullptr;

CGameSystem g_GameSystem;
IGameSystemFactory* CGameSystem::sm_Factory = nullptr;

IEntityResourceManifest* m_exportResourceManifest = nullptr;

// This mess is needed to get the pointer to sm_pFirst so we can insert game systems
bool InitGameSystems()
{
#ifdef _WIN32
    static constexpr const char sig_InitAllSystems_pFirst_Win_Data[] = {
        '4','8',' ','8','B',' ','1','D',' ','?',' ','?',' ','?',' ','?',' ',
        '4','8',' ','8','5',' ','D','B',' ','0','F',' ','8','4',' ',
        '?',' ','?',' ','?',' ','?',' ','B','D','\0'
    };
    static constexpr const char* sig_InitAllSystems_pFirst = sig_InitAllSystems_pFirst_Win_Data;
#else
    static constexpr const char sig_InitAllSystems_pFirst_Linux_Data[] = {
        '4','C',' ','8','B',' ','3','5',' ','?',' ','?',' ','?',' ','?',' ',
        '4','D',' ','8','5',' ','F','6',' ','7','5','\0'
    };
    static constexpr const char* sig_InitAllSystems_pFirst = sig_InitAllSystems_pFirst_Linux_Data;
#endif

    DynLibUtils::CModule libserver(HammerIdFix::shared::g_pSource2Server);
    auto result = libserver.FindPattern(sig_InitAllSystems_pFirst);
    if (!result)
    {
        Msg(OBF_LITERAL_SAFE("[HammerIdFix] <<< Failed to find IGameSystem_InitAllSystems_pFirst!\n").c_str());
        return false;
    }

    uint8_t* ptr = reinterpret_cast<uint8_t*>(result.GetPtr()) + 3;
    uint32 offset = *(uint32*)ptr;
    ptr += 4;

    CBaseGameSystemFactory::sm_pFirst = (CBaseGameSystemFactory**)(ptr + offset);
    CGameSystem::sm_Factory = new CGameSystemStaticFactory<CGameSystem>(OBF_LITERAL_SAFE("HammerIdFix_GameSystem").c_str(), &g_GameSystem);

    return true;
}

GS_EVENT_MEMBER(CGameSystem, BuildGameSessionManifest)
{
    IEntityResourceManifest* pResourceManifest = msg->m_pResourceManifest;

    m_exportResourceManifest = pResourceManifest;
}
