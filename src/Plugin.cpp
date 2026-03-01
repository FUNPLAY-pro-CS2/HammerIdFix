#include "Plugin.h"
#include "shared.h"
#include "dynlibutils/module.h"
#include <entitysystem.h>
#include "igameevents.h"
#include <iserver.h>
#include "schemasystem/schemasystem.h"
#include <filesystem>
#include <cstdio>
#include <fstream>
#include <regex>
#include <listeners/Listeners.h>

#include "utils/path.h"
#include "utils/log.h"
#include "utils/tasks.h"

#define VERSION_STRING SEMVER " @ " GITHUB_SHA
#define BUILD_TIMESTAMP __DATE__ " " __TIME__

PLUGIN_EXPOSE(RayTrace, IPlugin::g_iPlugin);

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

namespace IPlugin
{
    IPlugin g_iPlugin;

    bool IPlugin::Load(PluginId id, ISmmAPI* ismm, char* error, size_t maxlen, bool late)
    {
        PLUGIN_SAVEVARS();

        GET_V_IFACE_CURRENT(GetEngineFactory, shared::g_pCVar, ICvar, CVAR_INTERFACE_VERSION);
        GET_V_IFACE_ANY(GetServerFactory, shared::g_pSource2Server, ISource2Server, SOURCE2SERVER_INTERFACE_VERSION);
        GET_V_IFACE_ANY(GetServerFactory, shared::g_pServer, IServerGameDLL, INTERFACEVERSION_SERVERGAMEDLL);
        GET_V_IFACE_CURRENT(GetEngineFactory, shared::g_pEngine, IVEngineServer, INTERFACEVERSION_VENGINESERVER);
        GET_V_IFACE_ANY(GetEngineFactory, shared::g_pSchemaSystem, CSchemaSystem, SCHEMASYSTEM_INTERFACE_VERSION);
        GET_V_IFACE_ANY(GetEngineFactory, shared::g_pGameEventSystem, IGameEventSystem,
                        GAMEEVENTSYSTEM_INTERFACE_VERSION);
        GET_V_IFACE_ANY(GetServerFactory, shared::g_pGameEntities, ISource2GameEntities,
                        SOURCE2GAMEENTITIES_INTERFACE_VERSION);
        GET_V_IFACE_ANY(GetServerFactory, shared::g_pGameClients, IServerGameClients,
                        SOURCE2GAMECLIENTS_INTERFACE_VERSION);
        GET_V_IFACE_CURRENT(GetEngineFactory, g_pGameResourceServiceServer, IGameResourceService,
                            GAMERESOURCESERVICESERVER_INTERFACE_VERSION);
        GET_V_IFACE_ANY(GetEngineFactory, shared::g_pNetworkMessages, INetworkMessages,
                        NETWORKMESSAGES_INTERFACE_VERSION);
        GET_V_IFACE_ANY(GetEngineFactory, shared::g_pNetworkServerService, INetworkServerService,
                        NETWORKSERVERSERVICE_INTERFACE_VERSION);

        g_pCVar = shared::g_pCVar;
        g_pSource2GameEntities = shared::g_pGameEntities;

        Log::Init();
        Tasks::Init();

        g_SMAPI->AddListener(this, this);
        Listeners::InitListeners();

        g_pCVar = shared::g_pCVar;
        ConVar_Register(FCVAR_RELEASE | FCVAR_CLIENT_CAN_EXECUTE | FCVAR_GAMEDLL);

        if (late)
        {
            shared::g_pEntitySystem = GameEntitySystem();
            shared::g_pEntitySystem->AddListenerEntity(&Listeners::entityListener);
            shared::g_bDetoursLoaded = true;
        }

        FP_INFO("<<< Load() success! >>>");
        return true;
    }

    bool IPlugin::Unload(char* error, size_t maxlen)
    {
        shared::g_bDetoursLoaded = false;

        Listeners::DestructListeners();
        Tasks::Shutdown();

        FP_INFO("<<< Unload() success! >>>");

        Log::Close();

        return true;
    }

    const char* IPlugin::GetAuthor() { return "Slynx, contributors"; }
    const char* IPlugin::GetName() { return "RayTrace"; }
    const char* IPlugin::GetDescription() { return "RayTrace Metamod plugin for CS2 servers."; }
    const char* IPlugin::GetURL() { return "https://slynxdev.cz"; }
    const char* IPlugin::GetLicense() { return "GPLv3"; }
    const char* IPlugin::GetVersion() { return VERSION_STRING; }
    const char* IPlugin::GetDate() { return BUILD_TIMESTAMP; }
    const char* IPlugin::GetLogTag() { return "RayTrace"; }
}
