#pragma once

#include "Abytek/Base.Platform.prerequisites.pch.hpp"
#include "Abytek/MemoryBase.hpp"


#ifdef ABYTEK_PLATFORM_WINDOWS
namespace Abytek
{
    /**
     * @brief Windows implementation of mapped memory
     * 
     * Extends the abstract mapped memory with Windows-specific handle
     */
    struct F_WindowsMappedMemory : A_WindowsMappedMemory
    {
        /**
         * @brief Internal data for Windows-specific memory mapping
         */
        struct F_Internal
        {
            /** @brief Windows memory-mapped file handle */
            HANDLE Handle = INVALID_HANDLE_VALUE;
        };
        /** @brief Internal Windows-specific data */
        F_Internal Internal;

        /**
         * @brief Boolean cast operator
         * @return True if the mapped memory is valid, false otherwise
         */
        ABYTEK_FORCE_INLINE explicit operator B8 () const noexcept
        {
            return (
                static_cast<B8>(*(A_WindowsMappedMemory*)this)
                && (Internal.Handle != INVALID_HANDLE_VALUE)
            );
        }
    };
    
    /**
     * @brief Utility class for Windows memory operations
     * 
     * Provides static methods for memory allocation, mapping, and management on Windows
     */
    struct ABYTEK_BASE_PLATFORM_API H_WindowsMemory
    {
        /**
         * @brief Gets the system memory page size
         * @return Page size in bytes
         */
        static Sz GetPageSize();
        
        /**
         * @brief Allocates a low-level memory block
         * @param Desc Description of the memory block to allocate
         * @return Pointer to the allocated memory, or nullptr on failure
         */
        static void* AllocateLLBlock(const F_LLMemoryBlockDesc& Desc);
        
        /**
         * @brief Deallocates a previously allocated low-level memory block
         * @param Pointer Pointer to the memory block to deallocate
         */
        static void DeallocateLLBlock(void* Pointer);
        
        /**
         * @brief Creates a memory-mapped file
         * @param Desc Description of the mapped memory to create
         * @return Mapped memory object
         */
        static F_WindowsMappedMemory CreateMapped(const F_MappedMemoryDesc& Desc);
        
        /**
         * @brief Opens an existing memory-mapped file
         * @param Name Name of the mapped memory to open
         * @param AccessFlags Access flags specifying desired access
         * @return Mapped memory object
         */
        static F_WindowsMappedMemory OpenMapped(const F_Name& Name, E_MemoryAccessFlag AccessFlags = E_MemoryAccessFlag::DEFAULT);
        
        /**
         * @brief Destroys or closes a memory-mapped file
         * @param MappedMemory Mapped memory to destroy or close
         */
        static void DestroyOrCloseMapped(const F_WindowsMappedMemory& MappedMemory);
    };
}
#endif