/**
* =============================================================================
 * HammerIdFix
 * Copyright (C) 2025 Michal Přikryl (slynxcz)
 *
 * CS2Fixes
 * Copyright (C) 2023-2025 Source2ZE
 *
 * DynLibUtils
 * Copyright (C) 2023 komashchenko (Phoenix)
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __linux__
#include "plat.h"
#include "tier0/memdbgon.h"

void Plat_WriteMemory(void* pPatchAddress, uint8_t* pPatch, int iPatchSize)
{
	WriteProcessMemory(GetCurrentProcess(), pPatchAddress, (void*)pPatch, iPatchSize, nullptr);
}
#endif
