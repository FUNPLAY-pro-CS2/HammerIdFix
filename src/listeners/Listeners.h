//
// Created by Michal Přikryl on 09.07.2025.
// Copyright (c) 2025 slynxcz. All rights reserved.
//
#pragma once
#include <eiface.h>
#include <entitysystem.h>
#include <sourcehook.h>
#include <iserver.h>

namespace IPlugin::Listeners {
    void InitListeners();

    void DestructListeners();

    class SourceHooks {
    public:
        void Hook_StartupServer(const GameSessionConfiguration_t& config, ISource2WorldSession*, const char*);
    };

    class CEntityListener : public IEntityListener
    {
        void OnEntityCreated(CEntityInstance* pEntity) override;
    };

    extern SourceHooks sourceHooks;
    extern CEntityListener entityListener;
}
