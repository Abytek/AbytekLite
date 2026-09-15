#pragma once

#include "Abytek/Base.Platform.prerequisites.pch.hpp"


#ifdef ABYTEK_BASE_PLATFORM_ENABLE_IPC_PIPE
namespace Abytek
{
/**
 * @brief Cross-platform IPC pipe abstraction
 * 
 * This header provides platform-specific IPC pipe implementations
 * that are aliased based on the current platform.
 */

#ifdef ABYTEK_PLATFORM_WINDOWS
    struct F_WindowsPipe;        ///< Windows pipe structure
    struct H_WindowsPipe;        ///< Windows pipe handler
    using F_Pipe = F_WindowsPipe;  ///< Platform-specific pipe implementation
    using H_Pipe = H_WindowsPipe;  ///< Platform-specific pipe handler
#elif defined(ABYTEK_PLATFORM_POSIX)
    struct F_POSIXPipe;        ///< POSIX pipe structure
    struct H_POSIXPipe;        ///< POSIX pipe handler
    using F_Pipe = F_POSIXPipe;  ///< Platform-specific pipe implementation
    using H_Pipe = HPOSIXPipe;  ///< Platform-specific pipe handler
#else
#error "Unknown platform"
#endif
}
#endif