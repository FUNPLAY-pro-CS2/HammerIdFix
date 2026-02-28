//
// Created by Michal Přikryl on 28.02.2026.
// Copyright (c) 2026 slynxcz. All rights reserved.
//

#pragma once
#include <cstdint>
#include "metamod_oslink.h"

struct Module
{
#ifndef _WIN32
    void* pHandle;
#endif
    uint8_t* pBase;
    unsigned int nSize;
};

#ifndef _WIN32
int GetModuleInformation(HINSTANCE module, void** base, size_t* length);
#endif

#ifdef _WIN32
#define MODULE_PREFIX ""
#define MODULE_EXT ".dll"
#else
#define MODULE_PREFIX "lib"
#define MODULE_EXT ".so"
#endif

void Plat_WriteMemory(void* pPatchAddress, uint8_t *pPatch, int iPatchSize);