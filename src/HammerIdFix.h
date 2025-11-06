#ifndef _INCLUDE_METAMOD_SOURCE_STUB_PLUGIN_H_
#define _INCLUDE_METAMOD_SOURCE_STUB_PLUGIN_H_

#include <entitysystem.h>
#include <ISmmPlugin.h>

namespace HammerIdFix
{
    class HammerIdFix : public ISmmPlugin, public IMetamodListener
    {
    protected: // SourceHooks.
        void Hook_StartupServer(const GameSessionConfiguration_t& config, ISource2WorldSession*, const char*);
        void Hook_ClientDisconnect(CPlayerSlot slot, ENetworkDisconnectionReason reason, const char* pszName, uint64 xuid, const char* pszNetworkID);
    public:
        bool Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late) override;
        bool Unload(char *error, size_t maxlen) override;
        const char *GetAuthor() override;
        const char *GetName() override;
        const char *GetDescription() override;
        const char *GetURL() override;
        const char *GetLicense() override;
        const char *GetVersion() override;
        const char *GetDate() override;
        const char *GetLogTag() override;
    };

    class CEntityListener : public IEntityListener
    {
        void OnEntityCreated(CEntityInstance* pEntity) override;
    };

    extern HammerIdFix g_HammerIdFix;
}

#endif //_INCLUDE_METAMOD_SOURCE_STUB_PLUGIN_H_

PLUGIN_GLOBALVARS();
