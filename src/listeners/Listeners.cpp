//
// Created by Michal Přikryl on 10.07.2025.
// Copyright (c) 2025 slynxcz. All rights reserved.
//
#include "Listeners.h"
#include "shared.h"
#include <dynlibutils/module.h>

#include "utils/plat.h"

class GameSessionConfiguration_t
{
};

namespace IPlugin::Listeners
{
    SourceHooks sourceHooks;
    CEntityListener entityListener;

    SH_DECL_HOOK3_void(INetworkServerService, StartupServer, SH_NOATTRIB, 0, const GameSessionConfiguration_t&,
                       ISource2WorldSession*, const char*);

    void InitListeners()
    {
        SH_ADD_HOOK(INetworkServerService, StartupServer, shared::g_pNetworkServerService,
                    SH_MEMBER(&sourceHooks, &SourceHooks::Hook_StartupServer), true);
    }

    void DestructListeners()
    {
        SH_REMOVE_HOOK(INetworkServerService, StartupServer, shared::g_pNetworkServerService,
                       SH_MEMBER(&sourceHooks, &SourceHooks::Hook_StartupServer), true);
        shared::g_pEntitySystem->RemoveListenerEntity(&entityListener);
    }

    void SourceHooks::Hook_StartupServer(const GameSessionConfiguration_t& config,
                                         ISource2WorldSession*, const char*)
    {
        if (!shared::g_bDetoursLoaded)
        {
            shared::g_pEntitySystem = GameEntitySystem();
            shared::g_pEntitySystem->AddListenerEntity(&entityListener);
            shared::g_bDetoursLoaded = true;
        }
    }

    static void Patch_GetHammerUniqueId(CEntityInstance* pEntity)
    {
#ifdef WIN32
        static int offset = 111;
#else
        static int offset = 110;
#endif
        void** vtable = *(void***)pEntity;

        // xor al, al -> mov al, 1
        // so it always returns true and allows hammerid to be copied into the schema prop
        Plat_WriteMemory(vtable[offset], (uint8_t*)"\xB0\x01", 2);
    }

    void CEntityListener::OnEntityCreated(CEntityInstance* pEntity)
    {
        if (!pEntity) return;

        ExecuteOnce(Patch_GetHammerUniqueId(pEntity));
    }
}
