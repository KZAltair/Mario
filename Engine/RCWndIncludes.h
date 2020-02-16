#pragma once

// target Windows 7 or later
#define _WIN32_WINNT 0x0601
#include <sdkddkver.h>
// Remove some of the below defines in case of strange windows behavior
// Minimal windows macros

#define NOMINMAX
#include <Windows.h>
