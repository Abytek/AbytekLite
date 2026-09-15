#pragma once

#include "Abytek/Base.Platform.prerequisites.pch.hpp"
#include "Abytek/ProcessBase.hpp"

#ifdef ABYTEK_PLATFORM_POSIX
#ifdef ABYTEK_BASE_PLATFORM_ENABLE_IPC

#include <sys/types.h>

namespace Abytek
{
    /**
     * @brief POSIX implementation of process
     */
    struct F_POSIXProcess : A_ProcessLegacy
    {
        struct F_Internal
        {
            pid_t ProcessId = -1;
        };

        F_Internal Internal;

        ABYTEK_FORCE_INLINE explicit operator B8() const noexcept
        {
            return (
                static_cast<const A_ProcessLegacy&>(*this)
                && Internal.ProcessId > 0
            );
        }
    };

    /**
     * @brief POSIX process utilities
     */
    struct ABYTEK_BASE_PLATFORM_API H_POSIXProcess
    {
        static F_POSIXProcess Create(const F_ProcessDesc& Desc);
        static void Wait(const F_POSIXProcess& Process);
        static void Destroy(F_POSIXProcess& Process);
    };
    
    struct ABYTEK_BASE_PLATFORM_API H_POSIXThisProcess
    {
        static F_ProcessId GetId();
    };
}

#endif
#endif