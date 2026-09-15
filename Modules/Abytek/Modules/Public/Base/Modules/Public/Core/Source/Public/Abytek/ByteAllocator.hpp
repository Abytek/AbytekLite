/**
 * @file ByteAllocator.hpp
 * @brief Provides byte-oriented memory allocators with alignment capabilities.
 */
#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"
#include "Abytek/Allocator.hpp"
#include "Abytek/Align.hpp"
#include "Abytek/MemoryAllocationInfo.hpp"
#include "Abytek/MemoryManager.hpp"
#include "Abytek/TypeInfo.hpp"


namespace Abytek
{
    /**
     * @brief Options specific to byte allocators.
     * 
     * Extends the base allocator options with alignment settings.
     */
    struct F_ByteAllocatorOptions : F_AllocatorOptions
    {
        static constexpr Sz MinAlignment = ABYTEK_MIN_MEMORY_ALIGNMENT;
        static constexpr B8 EnableManualAlignment = false;
    };

    /**
     * @brief Byte-oriented allocator template that provides aligned memory allocation.
     * 
     * @tparam __F_Allocator The derived allocator type implementing the allocation logic.
     * @tparam __F_Value The value type this allocator works with.
     * @tparam __F_Options Configuration options for the allocator.
     */
    template<
        typename __F_Allocator,
        typename __F_Value,
        typename __F_Options = F_ByteAllocatorOptions
    >
    struct TA_ByteAllocator :
        TA_Allocator<
            __F_Allocator,
            __F_Value,
            __F_Options
        >
    {
    private:
        using F_Base = TA_Allocator<
            __F_Allocator,
            __F_Value,
            __F_Options
        >;

    public:
        using typename F_Base::F_Allocator;

        using typename F_Base::F_Value;
        using typename F_Base::F_ValueP;
        using typename F_Base::F_ValueConstP;

        using typename F_Base::F_Options;

    protected:
        constexpr TA_ByteAllocator() noexcept = default;

    public:
        /**
         * @brief Allocates memory for Count objects of type F_Value.
         * 
         * @param Count Number of objects to allocate memory for.
         * @return Pointer to the allocated memory.
         */
        F_Value* Allocate(Sz Count)
        {
            ABYTEK_BASE_CORE_ASSERT(Count);

            Sz PayloadSize = Count * sizeof(F_Value);
            return (F_Value*)AnalyzableAllocateBytes<
                F_Allocator,
                false,
                ABYTEK_ALIGNOF(F_Value),
                (ABYTEK_ALIGNOF(F_Value) <= F_Options::MinAlignment) || F_Options::EnableManualAlignment
            >(PayloadSize, 0);
        }
        
        /**
         * @brief Deallocates memory previously allocated by this allocator.
         * 
         * @param Pointer Pointer to the memory to deallocate.
         * @param Optional size parameter (unused).
         */
        void Deallocate(void* Pointer, Sz = 0)
        {
            ABYTEK_BASE_CORE_ASSERT(Pointer);
            AnalyzableDeallocateBytes<
                F_Allocator,
                false,
                ABYTEK_ALIGNOF(F_Value),
                (ABYTEK_ALIGNOF(F_Value) <= F_Options::MinAlignment) || F_Options::EnableManualAlignment
            >(Pointer);
        }

    public:
        /**
         * @brief Allocates a block of bytes with specified alignment.
         * 
         * @param Size Size in bytes to allocate.
         * @param Alignment Required alignment of the allocation.
         * @param AlignmentOffset Optional offset from the alignment boundary.
         * @return Pointer to the allocated memory.
         */
        ABYTEK_FORCE_INLINE void* AllocateBytes(Sz Size, Sz Alignment, Sz AlignmentOffset)
        {
            ABYTEK_BASE_CORE_ASSERT(Size);
            ABYTEK_BASE_CORE_ASSERT(Alignment <= ABYTEK_MIN_MEMORY_ALIGNMENT);
            return malloc(Size);
        }
        
        /**
         * @brief Deallocates memory allocated with AllocateBytes.
         * 
         * @param Pointer Pointer to the memory to deallocate.
         */
        ABYTEK_FORCE_INLINE void DeallocateBytes(void* Pointer)
        {
            ABYTEK_BASE_CORE_ASSERT(Pointer);
            free(Pointer);
        }

    public:
        /**
         * @brief Allocates memory with analysis information when enabled.
         * 
         * This function handles memory alignment and tracking for diagnostic purposes.
         * 
         * @tparam __F_OverridedAllocator Type of allocator being used.
         * @tparam __IsDefaultAlloc Whether this is using the default allocation path.
         * @tparam __Alignment Required alignment of the allocation.
         * @tparam __NoAlign Whether alignment should be skipped.
         * @param Size Size in bytes to allocate.
         * @param AlignmentOffset Optional offset from the alignment boundary.
         * @return Pointer to the allocated memory.
         */
        template<
            typename __F_OverridedAllocator = TA_ByteAllocator,
            B8 __IsDefaultAlloc = true,
            Sz __Alignment = ABYTEK_ALIGNOF(F_Value),
            B8 __NoAlign = (__Alignment <= F_Options::MinAlignment)
        >
        ABYTEK_FORCE_INLINE void* AnalyzableAllocateBytes(Sz Size, Sz AlignmentOffset)
        {
#ifndef ABYTEK_BASE_CORE_ENABLE_MEMORY_ALLOCATION_INFO
            if constexpr (__NoAlign)
            {
                return ((__F_Allocator*)this)->AllocateBytes(Size, __Alignment, AlignmentOffset);
            }
            else
#endif
            {
#ifdef ABYTEK_BASE_CORE_ENABLE_MEMORY_ALLOCATION_INFO
                constexpr Sz HeaderSize = sizeof(void*) + sizeof(F_MemoryAllocationInfo);
#else
                constexpr Sz HeaderSize = sizeof(void*);
#endif

                Sz ActualSize = HeaderSize + Size + __Alignment;

                void* RawP = ((__F_Allocator*)this)->AllocateBytes(ActualSize, F_Options::MinAlignment, 0);

                void* PayloadP = (void*)(
                    AlignAddress_PO2(
                        (uintptr_t)(((U8*)RawP) + HeaderSize + AlignmentOffset),
                        __Alignment
                    )
                    - AlignmentOffset
                );

                void** RawPP = (void**)(((U8*)PayloadP) - HeaderSize);
                *RawPP = RawP;

#ifdef ABYTEK_BASE_CORE_ENABLE_MEMORY_ALLOCATION_INFO
                F_MemoryManager& MemoryManager = F_MemoryManager::GetInstance();

                F_MemoryAllocationInfo& MemoryAllocationInfo = H_MemoryAllocationInfo::GetFromPayloadP(PayloadP);
                MemoryAllocationInfo.ValueTypeHashCode = TypeHashCode<F_Value>;
                MemoryAllocationInfo.AllocatorTypeHashCode = TypeHashCode<__F_OverridedAllocator>;
                MemoryAllocationInfo.AllocatorTypeFullName = TypeFullName<__F_OverridedAllocator>();
                MemoryAllocationInfo.ActualSize = ActualSize;
                MemoryAllocationInfo.PayloadSize = Size;
                MemoryAllocationInfo.Alignment = __Alignment;
                MemoryAllocationInfo.AlignmentOffset = AlignmentOffset;
                if constexpr (F_Options::EnableTracking)
                {
                    MemoryManager.Track(MemoryAllocationInfo);
                }
#endif

                return PayloadP;
            }
        }
        
        /**
         * @brief Deallocates memory previously allocated with AnalyzableAllocateBytes.
         * 
         * @tparam __F_OverridedAllocator Type of allocator being used.
         * @tparam __IsDefaultAlloc Whether this is using the default allocation path.
         * @tparam __Alignment Required alignment of the allocation.
         * @tparam __NoAlign Whether alignment was skipped.
         * @param Pointer Pointer to the memory to deallocate.
         */
        template<
            typename __F_OverridedAllocator = TA_ByteAllocator,
            B8 __IsDefaultAlloc = true,
            Sz __Alignment = ABYTEK_ALIGNOF(F_Value),
            B8 __NoAlign = (__Alignment <= F_Options::MinAlignment)
        >
        ABYTEK_FORCE_INLINE void AnalyzableDeallocateBytes(void* Pointer)
        {
#ifndef ABYTEK_BASE_CORE_ENABLE_MEMORY_ALLOCATION_INFO
            if constexpr (__NoAlign)
            {
                ((__F_Allocator*)this)->DeallocateBytes(Pointer);
            }
            else
#endif
            {
#ifdef ABYTEK_BASE_CORE_ENABLE_MEMORY_ALLOCATION_INFO
                constexpr Sz HeaderSize = sizeof(void*) + sizeof(F_MemoryAllocationInfo);
#else
                constexpr Sz HeaderSize = sizeof(void*);
#endif

                void** RawPP = (void**)(((U8*)Pointer) - HeaderSize);
                void* RawP = *RawPP;

#ifdef ABYTEK_BASE_CORE_ENABLE_MEMORY_ALLOCATION_INFO
                F_MemoryManager& MemoryManager = F_MemoryManager::GetInstance();

                F_MemoryAllocationInfo& MemoryAllocationInfo = H_MemoryAllocationInfo::GetFromPayloadP(Pointer);
                ABYTEK_BASE_CORE_ASSERT(MemoryAllocationInfo.AllocatorTypeHashCode == TypeHashCode<__F_OverridedAllocator>)
                    << "Mismatch allocator (origin: "
                    << MemoryAllocationInfo.AllocatorTypeFullName
                    << ", current: "
                    << TypeFullName<__F_OverridedAllocator>()
                    << ")";
                if constexpr (F_Options::EnableTracking)
                {
                    MemoryManager.Untrack(MemoryAllocationInfo);
                }
#endif

                ((__F_Allocator*)this)->DeallocateBytes(RawP);
            }
        }
    };
}