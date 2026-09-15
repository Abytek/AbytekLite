/**
 * @file DynamicAnalyableAllocator.hpp
 * @brief Provides a dynamic allocator with memory usage analysis capabilities.
 * 
 * This allocator overrides global new/delete operators when ABYTEK_DYNAMIC_ANALYABLE_ALLOCATOR_IMPLEMENTATION
 * is defined, allowing for memory tracking across the entire application.
 */
#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"


namespace Abytek
{
    /**
     * @brief Helper class that provides analyzable dynamic memory allocation capabilities.
     * 
     * This class tracks memory usage and provides aligned/non-aligned allocation methods.
     */
    struct ABYTEK_BASE_CORE_API H_DynamicAnalyableAllocator
    {
    private:
        static ASz _ActualUsage;  ///< Tracks total actual memory usage including overhead
        static ASz _PayloadUsage; ///< Tracks payload memory usage (requested size)

    public:
        /**
         * @brief Gets the current actual memory usage.
         * @return Size in bytes of the total memory allocated including overhead.
         */
        static ABYTEK_FORCE_INLINE Sz GetActualUsage() { return _ActualUsage.load(boost::memory_order_acquire); }
        
        /**
         * @brief Gets the current payload memory usage.
         * @return Size in bytes of the payload memory (excluding overhead).
         */
        static ABYTEK_FORCE_INLINE Sz GetPayloadUsage() { return _PayloadUsage.load(boost::memory_order_acquire); }

    public:
        /**
         * @brief Allocates memory without specific alignment requirements.
         * 
         * @param Size Size in bytes to allocate.
         * @return Pointer to the allocated memory.
         */
        static void* AllocateNonAligned(Sz Size);
        
        /**
         * @brief Deallocates memory previously allocated with AllocateNonAligned.
         * 
         * @param Pointer Pointer to the memory to deallocate.
         */
        static void DeallocateNonAligned(void* Pointer);

    public:
        /**
         * @brief Allocates aligned memory with specified alignment requirements.
         * 
         * @param Size Size in bytes to allocate.
         * @param Alignment Required alignment of the allocation.
         * @param AlignmentOffset Optional offset from the alignment boundary.
         * @return Pointer to the allocated memory.
         */
        static void* AllocateAligned(Sz Size, Sz Alignment, Sz AlignmentOffset);
        
        /**
         * @brief Deallocates memory previously allocated with AllocateAligned.
         * 
         * @param Pointer Pointer to the memory to deallocate.
         */
        static void DeallocateAligned(void* Pointer);
    };
}


#ifdef ABYTEK_DYNAMIC_ANALYABLE_ALLOCATOR_IMPLEMENTATION
/**
 * @brief Global operator new override that uses analyzable allocator.
 * 
 * @param Size Size in bytes to allocate.
 * @return Pointer to the allocated memory.
 */
void* ABYTEK_BASE_CORE_API operator new(std::size_t Size)
{
    return Abytek::H_DynamicAnalyableAllocator::AllocateNonAligned(Size);
}

/**
 * @brief Global operator delete override that uses analyzable allocator.
 * 
 * @param Pointer Pointer to the memory to deallocate.
 */
void ABYTEK_BASE_CORE_API operator delete(void* Pointer) noexcept
{
    Abytek::H_DynamicAnalyableAllocator::DeallocateNonAligned(Pointer);
}

/**
 * @brief Global aligned operator new override that uses analyzable allocator.
 * 
 * @param Size Size in bytes to allocate.
 * @param Alignment Required alignment of the allocation.
 * @return Pointer to the allocated memory.
 */
void* ABYTEK_BASE_CORE_API operator new(std::size_t Size, std::align_val_t Alignment)
{
    return Abytek::H_DynamicAnalyableAllocator::AllocateAligned(Size, (std::size_t)Alignment, 0);
}

/**
 * @brief Global aligned operator delete override that uses analyzable allocator.
 * 
 * @param Pointer Pointer to the memory to deallocate.
 * @param Alignment Alignment that was used for allocation.
 */
void ABYTEK_BASE_CORE_API operator delete(void* Pointer, std::align_val_t) noexcept
{
    Abytek::H_DynamicAnalyableAllocator::DeallocateAligned(Pointer);
}
#else
extern void* ABYTEK_BASE_CORE_API operator new(std::size_t Size);
extern void ABYTEK_BASE_CORE_API operator delete(void* Pointer) noexcept;
extern void* ABYTEK_BASE_CORE_API operator new(std::size_t Size, std::align_val_t Alignment);
extern void ABYTEK_BASE_CORE_API operator delete(void* Pointer, std::align_val_t) noexcept;
#endif