#pragma once

#include "Abytek/Base.Platform.prerequisites.pch.hpp"
#include "Abytek/PipeBase.hpp"


#ifdef ABYTEK_PLATFORM_WINDOWS
#ifdef ABYTEK_BASE_PLATFORM_ENABLE_IPC_PIPE
namespace Abytek
{
    /**
     * @brief Windows implementation of named pipe
     * 
     * Extends the abstract pipe with Windows-specific handle
     */
    struct F_WindowsPipe : A_Pipe
    {
        /**
         * @brief Internal data for Windows-specific pipe
         */
        struct F_Internal
        {
            /** @brief Windows pipe handle */
            HANDLE Handle = INVALID_HANDLE_VALUE;
        };
        /** @brief Internal Windows-specific data */
        F_Internal Internal;

        /**
         * @brief Boolean cast operator
         * @return True if the pipe is valid, false otherwise
         */
        ABYTEK_FORCE_INLINE explicit operator B8 () const noexcept
        {
            return (
                static_cast<B8>(*(A_Pipe*)this)
                && (Internal.Handle != INVALID_HANDLE_VALUE)
            );
        }
    };

    /**
     * @brief Utility class for Windows pipe operations
     * 
     * Provides static methods for pipe creation, connection, and management on Windows
     */
    struct ABYTEK_BASE_PLATFORM_API H_WindowsPipe
    {
        /**
         * @brief Creates a named pipe
         * @param Desc Description of the pipe to create
         * @param OutFileDesc Output parameter that receives the file stream for the pipe
         * @return Pipe object
         */
        static F_WindowsPipe Create(const F_PipeDesc& Desc, TS<A_FSFile>& OutFile);
        
        /**
         * @brief Waits for a client connection on the pipe
         * @param Pipe Pipe to wait for connection on
         */
        static void WaitForConnection(const F_WindowsPipe& Pipe);
        
        /**
         * @brief Disconnects the pipe from the current client
         * @param Pipe Pipe to disconnect
         */
        static void Disconnect(const F_WindowsPipe& Pipe);
        
        /**
         * @brief Destroys a pipe
         * @param Pipe Pipe to destroy
         */
        static void Destroy(const F_WindowsPipe& Pipe);
        
        /**
         * @brief Connects to a named pipe as a client
         * @param Path Path to the named pipe
         * @param AccessFlags Access flags specifying desired access
         * @return File stream for the pipe if connection successful, empty optional otherwise
         */
        static TS<A_FSFile> Client(const F_Text& Path, E_FSFileAccessFlag AccessFlags = E_FSFileAccessFlag::ALL);
    };
}
#endif
#endif