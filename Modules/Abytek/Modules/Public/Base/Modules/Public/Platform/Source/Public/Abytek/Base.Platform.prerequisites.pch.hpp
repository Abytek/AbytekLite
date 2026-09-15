#pragma once

#include "Abytek/Base.Platform.config.hpp"
#include "Abytek/Base.Core.hpp"
#include "Abytek/Base.Object.hpp"

/**
 * @file Platform.prerequisites.hpp
 * @brief Core prerequisites for the OS Bridge module
 * 
 * This file includes platform-specific headers and defines platform-specific
 * features required by the OS B ge core components.
 */

#ifdef ABYTEK_PLATFORM_WINDOWS
#include <shlwapi.h>
#include <evntrace.h>
#include <wrl.h>      // WRL core
#include <wrl/client.h> // ComPtr
#define ABYTEK_BASE_PLATFORM_SUPPORT_INTERNAL_WIDE_CHAR_PATH  ///< Windows uses wide character paths
#endif

#if ABYTEK_PLATFORM_POSIX
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#endif

#if ABYTEK_PLATFORM_ANDROID
#include <jni.h>
#include <game-activity/native_app_glue/android_native_app_glue.h>
#endif


namespace Abytek
{
}