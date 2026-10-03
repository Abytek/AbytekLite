#include "Abytek/RHIBufferInlineAllocator.hpp"
#include "Abytek/RHIContext.hpp"
#include "Abytek/RHIProcess.hpp"


namespace Abytek
{
    TS_Valid<A_RHIResource> F_RHIBufferInlineAllocation::GetBuffer() const
    {
        return Page->GetBuffer();
    }

    ABYTEK_RA_OBJECT_DEFAULT(F_RHIBufferInlineAllocatorPage);
    void F_RHIBufferInlineAllocatorPage::Build(const F_RHIBufferInlineAllocatorPageBuildParams& BuildParams)
    {
        A_RHIContextChild::Build(BuildParams);
        _Allocator = BuildParams.Allocator;
        _Index = BuildParams.Index;
        _SizeInBytes = BuildParams.SizeInBytes;
        _UsageInBytes = 0;
         
        F_RHIBufferBuildParams BufferBuildParams;
        BufferBuildParams.Context = GetContext();
        BufferBuildParams.AccessCapabilities = _Allocator->GetResourceAccessCapabilities();
        BufferBuildParams.BufferAspect.SizeInBytes = _SizeInBytes;
        BufferBuildParams.AdditionalFlags = _Allocator->GetResourceAdditionalFlags();
        _Buffer = RACreateAndBuildShared<A_RHIResource>(BufferBuildParams);
    }
    void F_RHIBufferInlineAllocatorPage::Release()
    {
        _Buffer = {};
        
        _UsageInBytes = 0;
        _SizeInBytes = 0;
        _Index = 0;
        _Allocator = {};
        A_RHIContextChild::Release();
    }

    TF_Optional<F_RHIBufferInlineAllocation> F_RHIBufferInlineAllocatorPage::Allocate(Sz SizeInBytes, Sz AlignmentInBytes)
    {
        Sz BeginOffsetInBytes = AlignAddress_PO2(_UsageInBytes, AlignmentInBytes);
        if ((BeginOffsetInBytes + SizeInBytes) > _SizeInBytes)
        {
            return {};
        }
        
        F_RHIBufferInlineAllocation Result;
        Result.Page = ABYTEK_WTHIS();
        Result.BeginOffsetInBytes = BeginOffsetInBytes;
        Result.EndOffsetInBytes = BeginOffsetInBytes + SizeInBytes;
        _UsageInBytes = BeginOffsetInBytes + SizeInBytes;
        return Result;
    }

#ifdef ABYTEK_DEBUG_INFO
    void F_RHIBufferInlineAllocatorPage::SetDebugName(const F_DebugName& Value) noexcept
    {
        A_RHIContextChild::SetDebugName(Value);
        _Buffer->SetDebugName(Value);
    }
#endif

    ABYTEK_RA_OBJECT_DEFAULT(F_RHIBufferInlineAllocator);
    void F_RHIBufferInlineAllocator::Build(const F_RHIBufferInlineAllocatorBuildParams& BuildParams)
    {
        A_RHIContextChild::Build(BuildParams);
        _MinPageSizeInBytes = BuildParams.MinPageSizeInBytes;
        _MaxPageSizeInBytes = BuildParams.MaxPageSizeInBytes;
        _ResourceAccessCapabilities = BuildParams.ResourceAccessCapabilities;
        _ResourceAdditionalFlags = BuildParams.ResourceAdditionalFlags;
    }
    void F_RHIBufferInlineAllocator::Release()
    {
        _Pages = {};
        _ResourceAdditionalFlags = E_RHIResourceAdditionalFlag::NONE;
        _ResourceAccessCapabilities = {};
        _MaxPageSizeInBytes = 0;
        _MinPageSizeInBytes = 0;
        A_RHIContextChild::Release();
    }

    F_RHIBufferInlineAllocation F_RHIBufferInlineAllocator::Allocate(Sz SizeInBytes, Sz AlignmentInBytes)
    {
        if (_Pages.size() > 0)
        {
            if (auto Allocation = _Pages.back()->Allocate(SizeInBytes, AlignmentInBytes))
            {
                return *Allocation;
            }
        }
        AddNewPage(SizeInBytes + AlignmentInBytes - 1);
        return *(_Pages.back()->Allocate(SizeInBytes, AlignmentInBytes));
    }
    void F_RHIBufferInlineAllocator::AddNewPage(Sz SizeInBytes)
    {
        Sz MinPageSizeInBytes = Max<Sz>(_MinPageSizeInBytes, SizeInBytes);
        if (!_Pages.empty())
        {
            MinPageSizeInBytes = Max<Sz>(
                MinPageSizeInBytes,
                _Pages.back()->GetSizeInBytes() * 2
            );
        }
        Sz ActualSizeInBytes = Min<Sz>(
            RoundUpToPowerOfTwo(MinPageSizeInBytes),
            _MaxPageSizeInBytes
        );
        ABYTEK_ENGINE_RHI_ASSERT(ActualSizeInBytes >= SizeInBytes);
        F_RHIBufferInlineAllocatorPageBuildParams PageBuildParams;
        PageBuildParams.Context = GetContext();
        PageBuildParams.Allocator = ABYTEK_WTHIS();
        PageBuildParams.Index = static_cast<U32>(_Pages.size());
        PageBuildParams.SizeInBytes = ActualSizeInBytes;
        auto Page = RACreateAndBuildShared<F_RHIBufferInlineAllocatorPage>(PageBuildParams);
#ifdef ABYTEK_DEBUG_INFO
        Page->SetDebugName(
            *GetDebugName()
            + ABYTEK_TEXT(".Pages[") + ToText(PageBuildParams.Index) + ABYTEK_TEXT("]")
        );
#endif
        _Pages.push_back(Page);
    }

    void F_RHIBufferInlineAllocator::Clear()
    {
        _Pages = {};
    }

#ifdef ABYTEK_DEBUG_INFO
    void F_RHIBufferInlineAllocator::SetDebugName(const F_DebugName& Value) noexcept
    {
        A_RHIContextChild::SetDebugName(Value);
        for (const auto& Page : _Pages)
        {
            Page->SetDebugName(
                *GetDebugName()
                + ABYTEK_TEXT(".Pages[") + ToText(Page->GetIndex()) + ABYTEK_TEXT("]")
            );
        }
    }
#endif
}
