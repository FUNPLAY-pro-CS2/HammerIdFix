//
// Created by Michal Přikryl on 28.02.2026.
// Copyright (c) 2026 slynxcz. All rights reserved.
//
#ifndef __linux__
#include "plat.h"
#include "tier0/memdbgon.h"

void Plat_WriteMemory(void* pPatchAddress, uint8_t* pPatch, int iPatchSize)
{
    WriteProcessMemory(GetCurrentProcess(), pPatchAddress, (void*)pPatch, iPatchSize, nullptr);
}
#endif