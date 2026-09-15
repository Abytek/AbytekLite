#pragma once

#include "Abytek/Base.Platform.prerequisites.pch.hpp"


#ifdef ABYTEK_BASE_PLATFORM_ENABLE_LOW_LEVEL_MEMORY
namespace Abytek
{
/**
 * @brief Cross-platform memory management abstraction
 * 
 * This header provides platform-specific memory management implementations
 * that are aliased based on the current platform.
 */

#ifdef ABYTEK_PLATFORM_WINDOWS
    struct F_WindowsMappedMemory;  ///< Windows memory mapping structure
    struct H_WindowsMemory;        ///< Windows memory handler structure
    using F_MappedMemory = F_WindowsMappedMemory;  ///< Platform-specific memory mapping
    using H_Memory = H_WindowsMemory;              ///< Platform-specific memory handler
#elif defined(ABYTEK_PLATFORM_POSIX)
    struct F_POSIXMappedMemory;  ///< POSIX memory mapping structure
    struct H_POSIXMemory;        ///< POSIX memory handler structure
    using F_MappedMemory = F_POSIXMappedMemory;  ///< Platform-specific memory mapping
    using H_Memory = H_POSIXMemory;              ///< Platform-specific memory handler
#else
#error "Unknown platform"
#endif
}
#endif