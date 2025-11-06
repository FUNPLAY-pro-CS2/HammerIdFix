#include "HammerIdFix.h"
#include "path.h"
#include "Shared.h"

#include <entitysystem.h>
#include <iserver.h>
#include <steam/steam_gameserver.h>

#include "game_system.h"
#include "schemasystem/schemasystem.h"
#include "dynlibutils/module.h"
#include "schema/CCSPlayerController.h"
#include "schema/cgameresourceserviceserver.h"
#include "schema/plat.h"
#include "schema/serversideclient.h"
#include <filesystem>
#include <cstdio>

PLUGIN_EXPOSE(HammerIdFix, HammerIdFix::g_HammerIdFix);

#define VERSION_STRING SEMVER " @ " GITHUB_SHA
#define BUILD_TIMESTAMP __DATE__ " " __TIME__

CGameEntitySystem* GameEntitySystem()
{
#ifdef WIN32
    static int offset = 88;
#else
    static int offset = 80;
#endif
    return *reinterpret_cast<CGameEntitySystem**>((uintptr_t)(g_pGameResourceServiceServer) + offset);
}

class GameSessionConfiguration_t
{
};

namespace HammerIdFix
{
    HammerIdFix g_HammerIdFix;
    CEntityListener g_EntityListener;

    SH_DECL_HOOK3_void(INetworkServerService, StartupServer, SH_NOATTRIB, 0, const GameSessionConfiguration_t&,
                       ISource2WorldSession*, const char*);

    bool HammerIdFix::Load(PluginId id, ISmmAPI* ismm, char* error, size_t maxlen, bool late)
    {
        PLUGIN_SAVEVARS();

        GET_V_IFACE_CURRENT(GetEngineFactory, shared::g_pCVar, ICvar, CVAR_INTERFACE_VERSION);
        GET_V_IFACE_ANY(GetServerFactory, shared::g_pSource2Server, ISource2Server, SOURCE2SERVER_INTERFACE_VERSION);
        GET_V_IFACE_ANY(GetServerFactory, shared::g_pServer, IServerGameDLL, INTERFACEVERSION_SERVERGAMEDLL);
        GET_V_IFACE_CURRENT(GetEngineFactory, shared::g_pEngine, IVEngineServer, INTERFACEVERSION_VENGINESERVER);
        GET_V_IFACE_ANY(GetEngineFactory, shared::g_pSchemaSystem, CSchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
        GET_V_IFACE_ANY(GetServerFactory, shared::g_pGameEntities, ISource2GameEntities,
                        SOURCE2GAMEENTITIES_INTERFACE_VERSION);
        GET_V_IFACE_ANY(GetServerFactory, shared::g_pGameClients, IServerGameClients,
                        SOURCE2GAMECLIENTS_INTERFACE_VERSION);
        GET_V_IFACE_CURRENT(GetEngineFactory, g_pGameResourceServiceServer, IGameResourceService,
                            GAMERESOURCESERVICESERVER_INTERFACE_VERSION);
        GET_V_IFACE_ANY(GetEngineFactory, shared::g_pNetworkServerService, INetworkServerService,
                        NETWORKSERVERSERVICE_INTERFACE_VERSION);

        g_pCVar = shared::g_pCVar;
        g_pSource2GameEntities = shared::g_pGameEntities;
        shared::g_pGameResourceServiceServer = (CGameResourceService*)g_pGameResourceServiceServer;

        shared::modules::Initialize();
        shared::modules::engine = shared::modules::GetModuleByName(MODULE_PREFIX "engine2" MODULE_EXT);
        shared::modules::tier0 = shared::modules::GetModuleByName(MODULE_PREFIX "tier0" MODULE_EXT);
        shared::modules::server = shared::modules::GetModuleByName(MODULE_PREFIX "server" MODULE_EXT);
        shared::modules::schemasystem = shared::modules::GetModuleByName(MODULE_PREFIX "schemasystem" MODULE_EXT);
        shared::modules::vscript = shared::modules::GetModuleByName(MODULE_PREFIX "vscript" MODULE_EXT);

        if (!shared::g_pGameResourceServiceServer)
            return false;

        if (!InitGameSystems())
            return false;

        g_SMAPI->AddListener(this, this);

        g_pCVar = shared::g_pCVar;
        ConVar_Register(FCVAR_RELEASE | FCVAR_CLIENT_CAN_EXECUTE | FCVAR_GAMEDLL);

        SH_ADD_HOOK(INetworkServerService, StartupServer, shared::g_pNetworkServerService,
                    SH_MEMBER(this, &HammerIdFix::Hook_StartupServer), true);

        Msg("[HammerIdFix] <<< Load() success!\n");
        return true;
    }

    bool HammerIdFix::Unload(char* error, size_t maxlen)
    {
        SH_REMOVE_HOOK(INetworkServerService, StartupServer, shared::g_pNetworkServerService,
                       SH_MEMBER(this, &HammerIdFix::Hook_StartupServer), true);

        shared::g_pEntitySystem->RemoveListenerEntity(&g_EntityListener);

        return true;
    }

    bool done = false;

    void HammerIdFix::Hook_StartupServer(const GameSessionConfiguration_t& config, ISource2WorldSession*, const char*)
    {
        if (!done)
        {
            done = true;

            shared::g_pEntitySystem = GameEntitySystem();
            shared::g_pEntitySystem->AddListenerEntity(&g_EntityListener);
        }
    }

    // !!
    // Fix taken from CS2Fixes https://github.com/Source2ZE/CS2Fixes/blob/a9601616571ff74a1460ef4c85d05b143e4d3c55/src/entitylistener.cpp#L34
    // !!
    void Patch_GetHammerUniqueId(CEntityInstance* pEntity)
    {
#ifdef WIN32
        static int offset = 118;
#else
        static int offset = 117;
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

    const char* HammerIdFix::GetLicense() { return "GPLv3"; }
    const char* HammerIdFix::GetVersion() { return VERSION_STRING; }
    const char* HammerIdFix::GetDate() { return BUILD_TIMESTAMP; }
    const char* HammerIdFix::GetName() { return "HammerIdFix"; }
    const char* HammerIdFix::GetAuthor() { return "Slynx"; }
    const char* HammerIdFix::GetDescription() { return "Fixes m_sUniqueHammerID SCHEMA_FIELD"; }
    const char* HammerIdFix::GetLogTag() { return "HammerIdFix"; }
    const char* HammerIdFix::GetURL() { return "https://slynxdev.cz"; }
}
