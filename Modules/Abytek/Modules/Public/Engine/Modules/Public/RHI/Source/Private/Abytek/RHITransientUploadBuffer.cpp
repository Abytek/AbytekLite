#include "Abytek/RHITransientUploadBuffer.hpp"
#include "Abytek/RHIContext.hpp"
#include "Abytek/RHIProcess.hpp"


namespace Abytek
{
    TS_Valid<A_RHIResource> F_RHITransientUploadBufferRange::GetBuffer() const
    {
        return Page->GetBuffer();
    }
    void F_RHITransientUploadBufferRange::Upload(const F_RHIBufferDataView& BufferDataView, Sz AdditionalOffsetInBytes) const
    {
        ABYTEK_ENGINE_RHI_ASSERT(AdditionalOffsetInBytes <= GetSizeInBytes()) << "Transient upload buffer range out of bounds";
        ABYTEK_ENGINE_RHI_ASSERT((AdditionalOffsetInBytes + BufferDataView.size()) <= GetSizeInBytes()) << "Transient upload buffer range out of bounds";
        if (BufferDataView.empty())
        {
            return;
        }
        F_RHITransientUploadBufferCandidate Candidate;
        static_cast<F_RHITransientUploadBufferRangeLocal&>(Candidate) = static_cast<const F_RHITransientUploadBufferRangeLocal&>(*this);
        Candidate.BufferDataView = BufferDataView;
        Candidate.BeginOffsetInBytes += AdditionalOffsetInBytes;
        Candidate.EndOffsetInBytes = Candidate.BeginOffsetInBytes + BufferDataView.size();
        Page->Queue.Push(Candidate);
    }

    ABYTEK_RA_OBJECT_DEFAULT(F_RHITransientUploadBufferPage);
    void F_RHITransientUploadBufferPage::Build(const F_RHITransientUploadBufferPageBuildParams& BuildParams)
    {
        A_RHIContextChild::Build(BuildParams);
        _Index = BuildParams.Index;
        _SizeInBytes = BuildParams.SizeInBytes;
        _UsageInBytes = 0;
         
        F_RHIBufferBuildParams BufferBuildParams;
        BufferBuildParams.Context = GetContext();
        BufferBuildParams.AccessCapabilities = F_RHIResourceAccess::MakeUploadCapabilities();
        BufferBuildParams.BufferAspect.SizeInBytes = _SizeInBytes;
        BufferBuildParams.AdditionalFlags = E_RHIResourceAdditionalFlag::AUTO_PLACED;
        _Buffer = RACreateAndBuildShared<A_RHIResource>(BufferBuildParams);
    }
    void F_RHITransientUploadBufferPage::Release()
    {
        _Buffer = {};
        
        _UsageInBytes = 0;
        _SizeInBytes = 0;
        _Index = 0;
        A_RHIContextChild::Release();
    }

    TF_Optional<F_RHITransientUploadBufferRange> F_RHITransientUploadBufferPage::Allocate(Sz SizeInBytes, Sz AlignmentInBytes)
    {
        Sz BeginOffsetInBytes = AlignAddress_PO2(_UsageInBytes, AlignmentInBytes);
        if ((BeginOffsetInBytes + SizeInBytes) > _SizeInBytes)
        {
            return {};
        }
        
        F_RHITransientUploadBufferRange Result;
        Result.Page = ABYTEK_WTHIS();
        Result.BeginOffsetInBytes = BeginOffsetInBytes;
        Result.EndOffsetInBytes = BeginOffsetInBytes + SizeInBytes;
        _UsageInBytes = BeginOffsetInBytes + SizeInBytes;
        return Result;
    }

#ifdef ABYTEK_DEBUG_INFO
    void F_RHITransientUploadBufferPage::SetDebugName(const F_DebugName& Value) noexcept
    {
        A_RHIContextChild::SetDebugName(Value);
        _Buffer->SetDebugName(Value);
    }
#endif

    ABYTEK_RA_OBJECT_DEFAULT(F_RHITransientUploadBufferManager);
    void F_RHITransientUploadBufferManager::Build(const F_RHITransientUploadBufferManagerBuildParams& BuildParams)
    {
        A_RHIContextChild::Build(BuildParams);
        _MinPageSizeInBytes = BuildParams.MinPageSizeInBytes;
        _MaxPageSizeInBytes = BuildParams.MaxPageSizeInBytes;
    }
    void F_RHITransientUploadBufferManager::Release()
    {
        ABYTEK_ENGINE_RHI_ASSERT(SectionData.Pages.size() == 0);
        _MaxPageSizeInBytes = 0;
        _MinPageSizeInBytes = 0;
        _EnqueuedToReleasePages.clear(boost::memory_order_release);
        A_RHIContextChild::Release();
    }

    void F_RHITransientUploadBufferManager::ReleasePages()
    {
        SectionData.Pages = {};
        _EnqueuedToReleasePages.clear(boost::memory_order_release);
    }

    F_RHITransientUploadBufferRange F_RHITransientUploadBufferManager::Allocate(Sz SizeInBytes, Sz AlignmentInBytes)
    {
        if (SectionData.Pages.size() > 0)
        {
            if (auto Allocation = SectionData.Pages.back()->Allocate(SizeInBytes, AlignmentInBytes))
            {
                return *Allocation;
            }
        }
        AddNewPage(SizeInBytes + AlignmentInBytes - 1);
        if (_EnqueuedToReleasePages.test_and_set(boost::memory_order_release) == false)
        {
            GetContext()->GetCurrentProcess()->EnqueuePostCompileCommand(
                [this]()
                {
                    ReleasePages();
                }
            );
        }
        return *(SectionData.Pages.back()->Allocate(SizeInBytes, AlignmentInBytes));
    }
    void F_RHITransientUploadBufferManager::AddNewPage(Sz SizeInBytes)
    {
        Sz MinPageSizeInBytes = Max<Sz>(_MinPageSizeInBytes, SizeInBytes);
        if (!SectionData.Pages.empty())
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
        F_RHITransientUploadBufferPageBuildParams PageBuildParams;
        PageBuildParams.Context = GetContext();
        PageBuildParams.Index = SectionData.Pages.size();
        PageBuildParams.SizeInBytes = ActualSizeInBytes;
        auto Page = RACreateAndBuildShared<F_RHITransientUploadBufferPage>(PageBuildParams);
#ifdef ABYTEK_DEBUG_INFO
        Page->SetDebugName(ABYTEK_TEXT("Abytek::RHITransientUploadBufferPages[") + ToText(PageBuildParams.Index) + ABYTEK_TEXT("]"));
#endif
        SectionData.Pages.push_back(Page);
    }
}