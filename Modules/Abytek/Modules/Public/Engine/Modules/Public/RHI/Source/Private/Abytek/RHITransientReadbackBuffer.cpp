#include "Abytek/RHITransientReadbackBuffer.hpp"
#include "Abytek/RHIContext.hpp"
#include "Abytek/RHIProcess.hpp"


namespace Abytek
{
    TS_Valid<A_RHIResource> F_RHITransientReadbackBufferRange_V2::GetBuffer() const
    {
        return Page->GetBuffer();
    }
    void F_RHITransientReadbackBufferRange_V2::Readback(F_RHIReadbackBufferCallback&& Callback, Sz ManualSizeInBytes, Sz AdditionalOffsetInBytes) const
    {
        ABYTEK_ENGINE_RHI_ASSERT(AdditionalOffsetInBytes <= GetSizeInBytes()) << "Transient readback buffer range out of bounds";
        Sz ActualSizeInBytes = ManualSizeInBytes;
        if (ActualSizeInBytes == 0)
        {
            ActualSizeInBytes = GetSizeInBytes() - AdditionalOffsetInBytes;
        }
        ABYTEK_ENGINE_RHI_ASSERT((AdditionalOffsetInBytes + ActualSizeInBytes) <= GetSizeInBytes()) << "Transient readback buffer range out of bounds";
        if (ActualSizeInBytes == 0)
        {
            return;
        }
        F_RHITransientReadbackBufferCandidate_V2 Candidate;
        static_cast<F_RHITransientReadbackBufferRangeLocal_V2&>(Candidate) = static_cast<const F_RHITransientReadbackBufferRangeLocal_V2&>(*this);
        Candidate.Callback = ABYTEK_MOVE(Callback);
        Candidate.BeginOffsetInBytes += AdditionalOffsetInBytes;
        Candidate.EndOffsetInBytes = Candidate.BeginOffsetInBytes + ActualSizeInBytes;
        Page->Queue.Push(Candidate);
    }

    ABYTEK_RA_OBJECT_DEFAULT(F_RHITransientReadbackBufferPage_V2);
    void F_RHITransientReadbackBufferPage_V2::Build(const F_RHITransientReadbackBufferPageBuildParams_V2& BuildParams)
    {
        A_RHIContextChild::Build(BuildParams);
        _Index = BuildParams.Index;
        _SizeInBytes = BuildParams.SizeInBytes;
        _UsageInBytes = 0;
         
        F_RHIBufferBuildParams BufferBuildParams;
        BufferBuildParams.Context = GetContext();
        BufferBuildParams.AccessCapabilities = F_RHIResourceAccess::MakeReadbackCapabilities();
        BufferBuildParams.BufferAspect.SizeInBytes = _SizeInBytes;
        BufferBuildParams.AdditionalFlags = E_RHIResourceAdditionalFlag::TRANSIENT;
        _Buffer = RACreateAndBuildShared<A_RHIResource>(BufferBuildParams);
    }
    void F_RHITransientReadbackBufferPage_V2::Release()
    {
        _Buffer = {};
        
        _UsageInBytes = 0;
        _SizeInBytes = 0;
        _Index = 0;
        A_RHIContextChild::Release();
    }

    TF_Optional<F_RHITransientReadbackBufferRange_V2> F_RHITransientReadbackBufferPage_V2::Allocate(Sz SizeInBytes)
    {
        if ((_UsageInBytes + SizeInBytes) > _SizeInBytes)
        {
            return {};
        }
        F_RHITransientReadbackBufferRange_V2 Result;
        Result.Page = ABYTEK_WTHIS();
        Result.BeginOffsetInBytes = _UsageInBytes;
        Result.EndOffsetInBytes = _UsageInBytes + SizeInBytes;
        _UsageInBytes += SizeInBytes;
        return Result;
    }

    ABYTEK_RA_OBJECT_DEFAULT(F_RHITransientReadbackBufferManager_V2);
    void F_RHITransientReadbackBufferManager_V2::Build(const F_RHITransientReadbackBufferManagerBuildParams_V2& BuildParams)
    {
        A_RHIContextChild::Build(BuildParams);
        _MinPageSizeInBytes = BuildParams.MinPageSizeInBytes;
        _MaxPageSizeInBytes = BuildParams.MaxPageSizeInBytes;
    }
    void F_RHITransientReadbackBufferManager_V2::Release()
    {
        ABYTEK_ENGINE_RHI_ASSERT(SectionData.Pages.size() == 0);
        _MaxPageSizeInBytes = 0;
        _MinPageSizeInBytes = 0;
        _EnqueuedToReleasePages.clear(boost::memory_order_release);
        A_RHIContextChild::Release();
    }

    void F_RHITransientReadbackBufferManager_V2::ReleasePages()
    {
        SectionData.Pages = {};
        _EnqueuedToReleasePages.clear(boost::memory_order_release);
    }

    F_RHITransientReadbackBufferRange_V2 F_RHITransientReadbackBufferManager_V2::Allocate(Sz SizeInBytes)
    {
        if (SectionData.Pages.size() > 0)
        {
            if (auto Allocation = SectionData.Pages.back()->Allocate(SizeInBytes))
            {
                return *Allocation;
            }
        }
        AddNewPage(SizeInBytes);
        if (_EnqueuedToReleasePages.test_and_set(boost::memory_order_release) == false)
        {
            GetContext()->GetCurrentProcess()->EnqueuePostCompileCommand(
                [this]()
                {
                    ReleasePages();
                }
            );
        }
        return *(SectionData.Pages.back()->Allocate(SizeInBytes));
    }
    void F_RHITransientReadbackBufferManager_V2::AddNewPage(Sz SizeInBytes)
    {
        Sz MinPageSizeInBytes = Max<Sz>(_MinPageSizeInBytes, SizeInBytes);
        if (SectionData.Pages.size() > 0)
        {
            MinPageSizeInBytes = Max<Sz>(
                MinPageSizeInBytes,
                SectionData.Pages.back()->GetSizeInBytes() * 2
            );
        }
        Sz ActualSizeInBytes = Min<Sz>(
            RoundUpToPowerOfTwo(MinPageSizeInBytes),
            _MaxPageSizeInBytes
        );
        F_RHITransientReadbackBufferPageBuildParams_V2 PageBuildParams;
        PageBuildParams.Context = GetContext();
        PageBuildParams.Index = SectionData.Pages.size();
        PageBuildParams.SizeInBytes = ActualSizeInBytes;
        auto Page = RACreateAndBuildShared<F_RHITransientReadbackBufferPage_V2>(PageBuildParams);
#ifdef ABYTEK_DEBUG_INFO
        Page->SetDebugName(ABYTEK_TEXT("Abytek::RHITransientReadbackBufferPages[") + ToText(PageBuildParams.Index) + ABYTEK_TEXT("]"));
#endif
        SectionData.Pages.push_back(Page);
    }
}