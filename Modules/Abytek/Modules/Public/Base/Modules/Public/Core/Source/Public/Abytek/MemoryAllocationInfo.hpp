/**
 * @file MemoryAllocationInfo.hpp
 * @brief Provides structures for tracking memory allocation information.
 * 
 * These structures are active when ABYTEK_BASE_CORE_ENABLE_MEMORY_ALLOCATION_INFO is defined.
 */
#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"


namespace Abytek
{
#ifdef ABYTEK_BASE_CORE_ENABLE_MEMORY_ALLOCATION_INFO
    /**
     * @brief Structure containing metadata about a memory allocation.
     * 
     * This structure is attached to memory allocations when memory tracking is enabled,
     * providing information about the allocation type and size.
     */
    struct ABYTEK_BASE_CORE_API F_MemoryAllocationInfo
    {
        U64 ValueTypeHashCode = 0;      ///< Hash code of the allocated value type
        U64 AllocatorTypeHashCode = 0;  ///< Hash code of the allocator type used
        const F_Char* AllocatorTypeFullName = 0;

        Sz ActualSize = 0;           ///< Total size of the allocation including overhead
        Sz PayloadSize = 0;          ///< Size of the actual requested payload
        Sz Alignment = 0;            ///< Alignment requirement of the allocation
        Sz AlignmentOffset = 0;      ///< Offset from the alignment boundary
        
        B8 IsEmpty() const noexcept
        {
            return (
                (ActualSize == 0) 
                && (PayloadSize == 0)
            );
        }
    };

    /**
     * @brief Helper class for working with memory allocation information.
     * 
     * Provides utility functions to access memory allocation metadata.
     */
    struct ABYTEK_BASE_CORE_API H_MemoryAllocationInfo
    {
        /**
         * @brief Gets the allocation info from a payload pointer.
         * 
         * @param Pointer Pointer to the allocated memory payload.
         * @return Reference to the associated memory allocation info structure.
         */
        static ABYTEK_FORCE_INLINE F_MemoryAllocationInfo& GetFromPayloadP(void* Pointer) noexcept
        {
            return ((F_MemoryAllocationInfo*)Pointer)[-1];
        }
    };
#endif
}