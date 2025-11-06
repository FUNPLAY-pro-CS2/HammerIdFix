//
// Created by Michal Přikryl on 19.06.2025.
// Copyright (c) 2025 slynxcz. All rights reserved.
//
#pragma once

#include <eiface.h>
#include <string>
#include "Shared.h"

namespace HammerIdFix::Paths {
    static std::string gameDirectory;

    inline std::string GameDirectory() {
        if (gameDirectory.empty()) {
            CBufferStringGrowable<255> gamePath;
            shared::g_pEngine->GetGameDir(gamePath);
            gameDirectory = std::string(gamePath.Get());
        }
        return gameDirectory;
    }

    inline std::string GetRootDirectory() { return GameDirectory() + OBF_LITERAL_SAFE("/addons/HammerIdFix"); }
    inline std::string SimpleAdmin() { return GameDirectory() + OBF_LITERAL_SAFE("/addons/counterstrikesharp/plugins/CS2-SimpleAdmin"); }
    inline std::string FunPlayCore() { return GameDirectory() + OBF_LITERAL_SAFE("/addons/Core/bin/linuxsteamrt64"); }
    inline std::string EnginePath() { return GameDirectory() + OBF_LITERAL_SAFE("../bin/linuxsteamrt64/libengine2.so"); }
    inline std::string Tier0Path() { return GameDirectory() + OBF_LITERAL_SAFE("../bin/linuxsteamrt64/libtier0.so"); }
    inline std::string ServerPath() { return GameDirectory() + OBF_LITERAL_SAFE("/bin/linuxsteamrt64/libserver.so"); }
    inline std::string SchemaSystemPath() { return GameDirectory() + OBF_LITERAL_SAFE("../bin/linuxsteamrt64/libschemasystem.so"); }
    inline std::string VScriptPath() { return GameDirectory() + OBF_LITERAL_SAFE("../bin/linuxsteamrt64/libvscript.so"); }
} // namespace HammerIdFix::Paths