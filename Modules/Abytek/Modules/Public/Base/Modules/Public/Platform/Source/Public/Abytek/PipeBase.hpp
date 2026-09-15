#pragma once

#include "Abytek/Base.Platform.prerequisites.pch.hpp"
#include "Abytek/Path.hpp"


#ifdef ABYTEK_BASE_PLATFORM_ENABLE_IPC_PIPE
namespace Abytek
{
    /**
     * @brief Enumeration for pipe access modes
     * 
     * Defines how data can flow through the pipe
     */
    enum class E_PipeAccessMode : U8
    {
        BIDIRECTIONAL,       ///< Data can flow in both directions
        CLIENT_TO_SERVER,    ///< Data flows from client to server only
        SERVER_TO_CLIENT,    ///< Data flows from server to client only

        DEFAULT = BIDIRECTIONAL  ///< Default access mode
    };

    /**
     * @brief Pipe descriptor structure
     * 
     * Contains configuration for creating or opening a pipe
     */
    struct F_PipeDesc
    {
        F_Text Path;               ///< Pipe path/name
        Sz OutBufferSize = 0;      ///< Output buffer size in bytes
        Sz InBufferSize = 0;       ///< Input buffer size in bytes
        E_PipeAccessMode AccessMode = E_PipeAccessMode::DEFAULT;  ///< Pipe access mode
        Sz DefaultTimeOut = 0;     ///< Default timeout in milliseconds
    };

    /**
     * @brief Abstract pipe base structure
     * 
     * Common base for platform-specific pipe implementations
     */
    struct A_Pipe
    {
        /**
         * @brief Checks if the pipe is valid
         * @return True if the pipe is valid, false otherwise
         */
        ABYTEK_FORCE_INLINE explicit operator B8 () const noexcept
        {
            return true;
        }
    };
}
#endif