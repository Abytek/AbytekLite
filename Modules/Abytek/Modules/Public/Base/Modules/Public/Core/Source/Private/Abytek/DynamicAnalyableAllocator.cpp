#define ABYTEK_DYNAMIC_ANALYABLE_ALLOCATOR_IMPLEMENTATION

#include "Abytek/DynamicAnalyableAllocator.hpp"
#include "Abytek/MemoryAllocationInfo.hpp"
#include "Abytek/Align.hpp"
#include "Abytek/TypeInfo.hpp"



namespace Abytek
{
    ASz H_DynamicAnalyableAllocator::_ActualUsage = 0;
    ASz H_DynamicAnalyableAllocator::_PayloadUsage = 0;


        
    void* H_DynamicAnalyableAllocator::AllocateNonAligned(Sz Size)
    {
#ifdef ABYTEK_BASE_CORE_ENABLE_MEMORY_ALLOCATION_INFO
        return AllocateAligned(Size, ABYTEK_MIN_MEMORY_ALIGNMENT, 0);
#else
        return malloc(Size);
#endif
    }
    void H_DynamicAnalyableAllocator::DeallocateNonAligned(void* Pointer)
    {
        if (Pointer == nullptr)
        {
            return;
        }
#ifdef ABYTEK_BASE_CORE_ENABLE_MEMORY_ALLOCATION_INFO
        return DeallocateAligned(Pointer);
#else
        return free(Pointer);
#endif
    }

    void* H_DynamicAnalyableAllocator::AllocateAligned(Sz Size, Sz Alignment, Sz AlignmentOffset)
    {
#ifdef ABYTEK_BASE_CORE_ENABLE_MEMORY_ALLOCATION_INFO
        constexpr Sz HeaderSize = sizeof(void*) + sizeof(F_MemoryAllocationInfo);
#else
        constexpr Sz HeaderSize = sizeof(void*);
#endif

        Sz ActualSize = HeaderSize + Size + Alignment;

        void* RawP = malloc(ActualSize);

        void* PayloadP = (void*)(
            AlignAddress_PO2(
                (uintptr_t)(((U8*)RawP) + HeaderSize + AlignmentOffset),
                Alignment
            )
            - AlignmentOffset
        );

        void** RawPP = (void**)(((U8*)PayloadP) - HeaderSize);
        *RawPP = RawP;

#ifdef ABYTEK_BASE_CORE_ENABLE_MEMORY_ALLOCATION_INFO
        F_MemoryAllocationInfo& MemoryAllocationInfo = H_MemoryAllocationInfo::GetFromPayloadP(PayloadP);
        MemoryAllocationInfo.ValueTypeHashCode = ABYTEK_U64_MAX;
        MemoryAllocationInfo.AllocatorTypeHashCode = TypeHashCode<H_DynamicAnalyableAllocator>;
        MemoryAllocationInfo.AllocatorTypeFullName = typeid(H_DynamicAnalyableAllocator).name();
        MemoryAllocationInfo.ActualSize = ActualSize;
        MemoryAllocationInfo.PayloadSize = Size;
        MemoryAllocationInfo.Alignment = Alignment;
        MemoryAllocationInfo.AlignmentOffset = AlignmentOffset;

        H_DynamicAnalyableAllocator::_ActualUsage.fetch_add(ActualSize, boost::memory_order_release);
        H_DynamicAnalyableAllocator::_PayloadUsage.fetch_add(Size, boost::memory_order_release);
#endif

        return PayloadP;
    }
    void H_DynamicAnalyableAllocator::DeallocateAligned(void* Pointer)
    {
#ifdef ABYTEK_BASE_CORE_ENABLE_MEMORY_ALLOCATION_INFO
        constexpr Sz HeaderSize = sizeof(void*) + sizeof(F_MemoryAllocationInfo);
#else
        constexpr Sz HeaderSize = sizeof(void*);
#endif

        void** RawPP = (void**)(((U8*)Pointer) - HeaderSize);
        void* RawP = *RawPP;

#ifdef ABYTEK_BASE_CORE_ENABLE_MEMORY_ALLOCATION_INFO
        F_MemoryAllocationInfo& MemoryAllocationInfo = H_MemoryAllocationInfo::GetFromPayloadP(Pointer);
        ABYTEK_BASE_CORE_ASSERT(MemoryAllocationInfo.AllocatorTypeHashCode == TypeHashCode<H_DynamicAnalyableAllocator>)
            << "Mismatch allocator (origin: "
            << MemoryAllocationInfo.AllocatorTypeFullName
            << ", current: "
            << TypeFullName<H_DynamicAnalyableAllocator>()
            << ")";

        H_DynamicAnalyableAllocator::_ActualUsage.fetch_sub(MemoryAllocationInfo.ActualSize, boost::memory_order_release);
        H_DynamicAnalyableAllocator::_PayloadUsage.fetch_sub(MemoryAllocationInfo.PayloadSize, boost::memory_order_release);
#endif

        free(RawP);
    }
}