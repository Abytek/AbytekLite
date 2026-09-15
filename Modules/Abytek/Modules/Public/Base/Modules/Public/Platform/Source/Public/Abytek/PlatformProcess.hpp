#pragma once

#include "Abytek/Base.Platform.prerequisites.pch.hpp"


#ifdef ABYTEK_BASE_PLATFORM_ENABLE_IPC
namespace Abytek
{
/**
 * @brief Platform-specific process type definitions
 * 
 * This header provides platform-specific typedefs for process handling
 */
#ifdef ABYTEK_PLATFORM_WINDOWS
    /** @brief Windows-specific process structure */
    struct F_WindowsProcessLegacy;
    /** @brief Windows-specific process handler */
    struct H_WindowsProcessLegacy;
    /** @brief Platform-agnostic process type, maps to Windows implementation on Windows platforms */
    using F_ProcessLegacy = F_WindowsProcessLegacy;
    /** @brief Platform-agnostic process handler, maps to Windows handler on Windows platforms */
    using H_ProcessLegacy = H_WindowsProcessLegacy;
#elif defined(ABYTEK_PLATFORM_POSIX)
    /** @brief POSIX-specific process structure */
    struct F_POSIXProcess;
    /** @brief POSIX-specific process handler */
    struct H_POSIXProcess;
    /** @brief Platform-agnostic process type, maps to POSIX implementation on POSIX platforms */
    using F_ProcessLegacy = F_POSIXProcess;
    /** @brief Platform-agnostic process handler, maps to POSIX handler on POSIX platforms */
    using H_ProcessLegacy = H_POSIXProcess;
#else
#error "Unknown platform"
#endif
}
#endif