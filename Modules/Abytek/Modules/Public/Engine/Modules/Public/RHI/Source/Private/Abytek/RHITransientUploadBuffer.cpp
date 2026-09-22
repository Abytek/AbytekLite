#include "Abytek/RHITransientUploadBuffer.hpp"
#include "Abytek/RHIContext.hpp"
#include "Abytek/RHIProcess.hpp"


namespace Abytek
{
    TS_Valid<A_RHIResource> F_RHITransientUploadBufferRange_V2::GetBuffer() const
    {
        return Page->GetBuffer();
    }
    void F_RHITransientUploadBufferRange_V2::Upload(const F_RHIBufferDataView& BufferDataView, Sz AdditionalOffsetInBytes) const
    {
        ABYTEK_ENGINE_RHI_ASSERT(AdditionalOffsetInBytes <= GetSizeInBytes()) << "Transient upload buffer range out of bounds";
        ABYTEK_ENGINE_RHI_ASSERT((AdditionalOffsetInBytes + BufferDataView.size()) <= GetSizeInBytes()) << "Transient upload buffer range out of bounds";
        if (BufferDataView.empty())
        {
            return;
        }
        F_RHITransientUploadBufferCandidate_V2 Candidate;
        static_cast<F_RHITransientUploadBufferRangeLocal_V2&>(Candidate) = static_cast<const F_RHITransientUploadBufferRangeLocal_V2&>(*this);
        Candidate.BufferDataView = BufferDataView;
        Candidate.BeginOffsetInBytes += AdditionalOffsetInBytes;
        Candidate.EndOffsetInBytes = Candidate.BeginOffsetInBytes + BufferDataView.size();
        Page->Queue.Push(Candidate);
    }

    ABYTEK_RA_OBJECT_DEFAULT(F_RHITransientUploadBufferPage_V2);
    void F_RHITransientUploadBufferPage_V2::Build(const F_RHITransientUploadBufferPageBuildParams_V2& BuildParams)
    {
        A_RHIContextChild::Build(BuildParams);
        _Index = BuildParams.Index;
        _SizeInBytes = BuildParams.SizeInBytes;
        _UsageInBytes = 0;
         
        F_RHIBufferBuildParams BufferBuildParams;
        BufferBuildParams.Context = GetContext();
        BufferBuildParams.AccessCapabilities = F_RHIResourceAccess::MakeUploadCapabilities();
        BufferBuildParams.BufferAspect.SizeInBytes = _SizeInBytes;
        BufferBuildParams.AdditionalFlags = E_RHIResourceAdditionalFlag::TRANSIENT;
        _Buffer = RACreateAndBuildShared<A_RHIResource>(BufferBuildParams);
    }
    void F_RHITransientUploadBufferPage_V2::Release()
    {
        _Buffer = {};
        
        _UsageInBytes = 0;
        _SizeInBytes = 0;
        _Index = 0;
        A_RHIContextChild::Release();
    }

    TF_Optional<F_RHITransientUploadBufferRange_V2> F_RHITransientUploadBufferPage_V2::Allocate(Sz SizeInBytes)
    {
        if ((_UsageInBytes + SizeInBytes) > _SizeInBytes)
        {
            return {};
        }
        F_RHITransientUploadBufferRange_V2 Result;
        Result.Page = ABYTEK_WTHIS();
        Result.BeginOffsetInBytes = _UsageInBytes;
        Result.EndOffsetInBytes = _UsageInBytes + SizeInBytes;
        _UsageInBytes += SizeInBytes;
        return Result;
    }

    ABYTEK_RA_OBJECT_DEFAULT(F_RHITransientUploadBufferManager_V2);
    void F_RHITransientUploadBufferManager_V2::Build(const F_RHITransientUploadBufferManagerBuildParams_V2& BuildParams)
    {
        A_RHIContextChild::Build(BuildParams);
        _MinPageSizeInBytes = BuildParams.MinPageSizeInBytes;
        _MaxPageSizeInBytes = BuildParams.MaxPageSizeInBytes;
    }
    void F_RHITransientUploadBufferManager_V2::Release()
    {
        ABYTEK_ENGINE_RHI_ASSERT(SectionData.Pages.size() == 0);
        _MaxPageSizeInBytes = 0;
        _MinPageSizeInBytes = 0;
        _EnqueuedToReleasePages.clear(boost::memory_order_release);
        A_RHIContextChild::Release();
    }

    void F_RHITransientUploadBufferManager_V2::ReleasePages()
    {
        SectionData.Pages = {};
        _EnqueuedToReleasePages.clear(boost::memory_order_release);
    }

    F_RHITransientUploadBufferRange_V2 F_RHITransientUploadBufferManager_V2::Allocate(Sz SizeInBytes)
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
    void F_RHITransientUploadBufferManager_V2::AddNewPage(Sz SizeInBytes)
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
        F_RHITransientUploadBufferPageBuildParams_V2 PageBuildParams;
        PageBuildParams.Context = GetContext();
        PageBuildParams.Index = SectionData.Pages.size();
        PageBuildParams.SizeInBytes = ActualSizeInBytes;
        auto Page = RACreateAndBuildShared<F_RHITransientUploadBufferPage_V2>(PageBuildParams);
#ifdef ABYTEK_DEBUG_INFO
        Page->SetDebugName(ABYTEK_TEXT("Abytek::RHITransientUploadBufferPages[") + ToText(PageBuildParams.Index) + ABYTEK_TEXT("]"));
#endif
        SectionData.Pages.push_back(Page);
    }
}