#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"
#include "Abytek/BoostContainerTemplates.hpp"
#include "Abytek/Align.hpp"


namespace Abytek::ContainerTemplates
{
    template<B8 __AutoScale, class __F_Allocator>
    class TF_LinearAllocationDistributor
    {
    public:
        static constexpr B8 AutoScale = __AutoScale;
        using F_Allocator = __F_Allocator;
        using F_Address = U64;
        
    public:
        struct F_AllocationRange
        {
            U64 Begin = 0;
            U64 End = 0;
            B8 IsUsed = false;

            friend B8 operator == (const F_AllocationRange& a, const F_AllocationRange& b)
            {
                return (a.End > b.Begin) && (a.Begin < b.End);
                // just a fake comparision for faster searching
            }
        };
        struct F_AllocationRangeSearchLess
        {
            B8 operator()(const F_AllocationRange& Left, const F_AllocationRange& Right) const noexcept
            {
                return Left.End <= Right.Begin;
            }
        };
        
    private:
        F_Address _Capacity = 0;
        ContainerTemplates::TF_Set<
            F_AllocationRange,
            F_AllocationRangeSearchLess,
            TF_RebindAllocator<F_AllocationRange, F_Allocator>
        > _AllocationRanges;

    public:
        ABYTEK_FORCE_INLINE F_Address GetCapacity() const noexcept
        {
            return _Capacity;
        }
        ABYTEK_FORCE_INLINE const auto& GetAllocationRanges() const noexcept
        {
            return _AllocationRanges;
        }
        
    public:
        TF_LinearAllocationDistributor() :
            TF_LinearAllocationDistributor(0)
        {
        }
        ~TF_LinearAllocationDistributor()
        {
        }

        TF_LinearAllocationDistributor(F_Address Capacity) :
            _Capacity(Capacity)
        {
            if (Capacity != 0)
            {
                _AllocationRanges.emplace(
                    F_AllocationRange
                    {
                        0,
                        Capacity,
                        false
                    }
                );
            }
        }
        
        TF_LinearAllocationDistributor(const TF_LinearAllocationDistributor& Other) = default;
        TF_LinearAllocationDistributor& operator = (const TF_LinearAllocationDistributor& Other) = default;
        TF_LinearAllocationDistributor(TF_LinearAllocationDistributor&& Other) = default;
        TF_LinearAllocationDistributor& operator = (TF_LinearAllocationDistributor&& Other) = default;

    public:
        void Extends(F_Address Size)
        {
			F_Address LastCapacity = _Capacity;

            _Capacity += Size;
            
            if (LastCapacity)
            {
                auto& LastAllocationRange = *(_AllocationRanges.rbegin());
                if (!LastAllocationRange.IsUsed)
                {
                    LastAllocationRange.End += Size;
                    return;
                }
            }

            F_AllocationRange NewAllocationRange;
            NewAllocationRange.Begin = LastCapacity;
            NewAllocationRange.End = LastCapacity + Size;
            _AllocationRanges.emplace(NewAllocationRange);
        }
        TF_Optional<F_Address> Allocate(F_Address Size, F_Address Alignment = 1)
        {
            F_Address RealSize = Size + Alignment - 1;
            for (auto& AllocationRange : _AllocationRanges)
            {
                if (
                    ((AllocationRange.End - AllocationRange.Begin) >= RealSize)
                    && !AllocationRange.IsUsed
                )
                {
                    AllocationRange.IsUsed = true;

                    F_Address Begin = AllocationRange.Begin;
                    auto Result = AlignAddress_PO2(Begin, Alignment);
                    
                    F_Address LastEnd = AllocationRange.End;
                    
                    F_Address NewEnd = Result + Size;
                    AllocationRange.End = NewEnd;

                    
                    if (LastEnd != AllocationRange.End)
                    {
                        _AllocationRanges.emplace(
                            F_AllocationRange
                            {
                                NewEnd,
                                LastEnd,
                                false
                            }
                        );
                    }
                    return Result;
                }
            }
            if constexpr (AutoScale)
            {
                Extends(RealSize);
                return Allocate(RealSize, Alignment);
            }
            return {};
        }
        void Deallocate(F_Address Address)
        {
            F_AllocationRange FakeRange;
            FakeRange.Begin = Address;
            FakeRange.End = Address + 1;
            auto It = _AllocationRanges.find(FakeRange);
            ABYTEK_BASE_CORE_ASSERT(It != _AllocationRanges.end()) << "Invalid address";

            auto& FoundRange = *It;
            FoundRange.IsUsed = false;

            B8 MergeNext = false;
            B8 MergePrev = false;

            auto NextIt = It;
            ++NextIt;
            if (NextIt != _AllocationRanges.end())
            {
                auto& NextRange = *NextIt;
				ABYTEK_BASE_CORE_ASSERT(NextIt->Begin == FoundRange.End) << "Invalid next allocation range: " << FoundRange.End << " vs " << NextIt->Begin << " " << this;
                MergeNext = !(NextIt->IsUsed);
            }

            auto PrevIt = It;
            --PrevIt;
            if (It != _AllocationRanges.begin())
            {
                auto& PrevRange = *PrevIt;
                ABYTEK_BASE_CORE_ASSERT(PrevIt->End == FoundRange.Begin) << "Invalid prev allocation range: " <<FoundRange.Begin << " vs " << PrevIt->End << " " << this;
                MergePrev = !(PrevIt->IsUsed);
            }

            if (MergeNext && MergePrev)
            {
				auto NewBegin = PrevIt->Begin;
				auto NewEnd = NextIt->End;
                _AllocationRanges.erase(PrevIt);
                _AllocationRanges.erase(NextIt);
                FoundRange.Begin = NewBegin;
                FoundRange.End = NewEnd;
                return;
            }

            if (MergeNext)
            {
                auto NewEnd = NextIt->End;
                _AllocationRanges.erase(NextIt);
                FoundRange.End = NewEnd;
                return;
            }

            if (MergePrev)
            {
                auto NewBegin = PrevIt->Begin;
                _AllocationRanges.erase(PrevIt);
                FoundRange.Begin = NewBegin;
                return;
            }
        }
    };
}
