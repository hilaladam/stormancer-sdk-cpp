#pragma once

#include "stormancer/BuildConfig.h"

#if (defined(UE_EDITOR) || defined(UE_GAME)) && defined(_WIN32) && !defined(STORM_UE_NO_WIN32_PLATFORM_TYPES)
#if PLATFORM_XBOXONE
#include "XboxOneAllowPlatformTypes.h" // Include mandatory to compile on Xbox one plateform.
#else
#include "AllowWindowsPlatformTypes.h"
#endif
#if PLATFORM_LINUX // Ignore Unreal Engine warning as error ([-Werror, -Wundef]).
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wundef"
#endif
#endif

#ifdef _WIN32
#include "Windows.h"
#endif





#if defined(_WIN32) && defined(CPPREST_FORCE_PPLX) && CPPREST_FORCE_PPLX != 1
#error "CPPREST_FORCE_PPLX must be defined to 1 on this platform"
#endif
#if !defined(CPPREST_FORCE_PPLX)
#define CPPREST_FORCE_PPLX 1
#endif
#include "pplx/pplxtasks.h"


#if (defined(UE_EDITOR) || defined(UE_GAME)) && defined(_WIN32) && !defined(STORM_UE_NO_WIN32_PLATFORM_TYPES)
#if PLATFORM_XBOXONE
#include "XboxOneHidePlatformTypes.h"
#else
#include "HideWindowsPlatformTypes.h"
#endif
#if PLATFORM_LINUX
#pragma clang diagnostic pop
#endif
#endif