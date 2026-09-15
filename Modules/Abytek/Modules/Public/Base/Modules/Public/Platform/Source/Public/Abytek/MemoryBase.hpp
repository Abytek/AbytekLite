#pragma once

#include "Abytek/Base.Platform.prerequisites.pch.hpp"


namespace Abytek
{
    /**
     * @brief Enumeration for memory access permissions
     * 
     * Flags defining how memory can be accessed (read/write/execute)
     */
    enum class E_MemoryAccessFlag : U8
    {
        NONE = 0,                    ///< No access
        READ = 0x1,                  ///< Read access
        WRITE = 0x2,                 ///< Write access
        EXECUTABLE = 0x4,            ///< Execute access
        
        ALL = READ | WRITE | EXECUTABLE,            ///< Full access (read/write/execute)
        EXECUTABLE_READ = EXECUTABLE | READ,        ///< Read and execute access
        READ_WRITE = READ | WRITE,                  ///< Read and write access

        DEFAULT = READ_WRITE                        ///< Default access (read/write)
    };
    ABYTEK_DEFINE_FLAG_OPERATORS(E_MemoryAccessFlag);
    
#ifdef ABYTEK_BASE_PLATFORM_ENABLE_LOW_LEVEL_MEMORY
    /**
     * @brief Low-level memory block descriptor
     * 
     * Describes a block of memory with size and access flags
     */
    struct F_LLMemoryBlockDesc
    {
        Sz Size = 0;                                     ///< Size of the memory block in bytes
        E_MemoryAccessFlag AccessFlags = E_MemoryAccessFlag::DEFAULT;  ///< Memory access permissions
    };

    /**
     * @brief Mapped memory descriptor
     * 
     * Describes a named memory mapping with size and access flags
     */
    struct F_MappedMemoryDesc
    {
        Sz Size = 0;                                     ///< Size of the memory mapping in bytes
        F_Name Name;                                     ///< Name of the memory mapping
        E_MemoryAccessFlag AccessFlags = E_MemoryAccessFlag::DEFAULT;  ///< Memory access permissions
    };

    /**
     * @brief Base Windows mapped memory structure
     * 
     * Contains common fields for Windows memory mapping
     */
    struct A_WindowsMappedMemory
    {
        void* Address = 0;                               ///< Memory mapping address

        F_Name Name;                                     ///< Name of the memory mapping
        E_MemoryAccessFlag AccessFlags = E_MemoryAccessFlag::DEFAULT;  ///< Memory access permissions

        B8 IsHost = true;                                ///< Whether this process is the mapping host

        /**
         * @brief Checks if the memory mapping is valid
         * @return True if the mapping is valid, false otherwise
         */
        ABYTEK_FORCE_INLINE explicit operator B8 () const noexcept
        {
            return (
                (Address != 0)
                && Name
            );
        }
    };
#endif
}