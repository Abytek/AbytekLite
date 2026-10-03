#include "Abytek/RHITransientReadbackBuffer.hpp"
#include "Abytek/RHIContext.hpp"
#include "Abytek/RHIProcess.hpp"


namespace Abytek
{
    TS_Valid<A_RHIResource> F_RHITransientReadbackBufferRange::GetBuffer() const
    {
        return Page->GetBuffer();
    }
    void F_RHITransientReadbackBufferRange::Readback(F_RHIReadbackBufferCallback&& Callback, Sz ManualSizeInBytes, Sz AdditionalOffsetInBytes) const
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
        F_RHITransientReadbackBufferCandidate Candidate;
        static_cast<F_RHITransientReadbackBufferRangeLocal&>(Candidate) = static_cast<const F_RHITransientReadbackBufferRangeLocal&>(*this);
        Candidate.Callback = ABYTEK_MOVE(Callback);
        Candidate.BeginOffsetInBytes += AdditionalOffsetInBytes;
        Candidate.EndOffsetInBytes = Candidate.BeginOffsetInBytes + ActualSizeInBytes;
        Page->Queue.Push(Candidate);
    }

    ABYTEK_RA_OBJECT_DEFAULT(F_RHITransientReadbackBufferPage);
    void F_RHITransientReadbackBufferPage::Build(const F_RHITransientReadbackBufferPageBuildParams& BuildParams)
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
    void F_RHITransientReadbackBufferPage::Release()
    {
        _Buffer = {};
        
        _UsageInBytes = 0;
        _SizeInBytes = 0;
        _Index = 0;
        A_RHIContextChild::Release();
    }

    TF_Optional<F_RHITransientReadbackBufferRange> F_RHITransientReadbackBufferPage::Allocate(Sz SizeInBytes, Sz AlignmentInBytes)
    {
        Sz BeginOffsetInBytes = AlignAddress_PO2(_UsageInBytes, AlignmentInBytes);
        if ((BeginOffsetInBytes + SizeInBytes) > _SizeInBytes)
        {
            return {};
        }
        
        F_RHITransientReadbackBufferRange Result;
        Result.Page = ABYTEK_WTHIS();
        Result.BeginOffsetInBytes = BeginOffsetInBytes;
        Result.EndOffsetInBytes = BeginOffsetInBytes + SizeInBytes;
        _UsageInBytes = BeginOffsetInBytes + SizeInBytes;
        return Result;
    }

#ifdef ABYTEK_DEBUG_INFO
    void F_RHITransientReadbackBufferPage::SetDebugName(const F_DebugName& Value) noexcept
    {
        A_RHIContextChild::SetDebugName(Value);
        _Buffer->SetDebugName(Value);
    }
#endif

    ABYTEK_RA_OBJECT_DEFAULT(F_RHITransientReadbackBufferManager);
    void F_RHITransientReadbackBufferManager::Build(const F_RHITransientReadbackBufferManagerBuildParams& BuildParams)
    {
        A_RHIContextChild::Build(BuildParams);
        _MinPageSizeInBytes = BuildParams.MinPageSizeInBytes;
        _MaxPageSizeInBytes = BuildParams.MaxPageSizeInBytes;
    }
    void F_RHITransientReadbackBufferManager::Release()
    {
        ABYTEK_ENGINE_RHI_ASSERT(SectionData.Pages.size() == 0);
        _MaxPageSizeInBytes = 0;
        _MinPageSizeInBytes = 0;
        _EnqueuedToReleasePages.clear(boost::memory_order_release);
        A_RHIContextChild::Release();
    }

    void F_RHITransientReadbackBufferManager::ReleasePages()
    {
        SectionData.Pages = {};
        _EnqueuedToReleasePages.clear(boost::memory_order_release);
    }

    F_RHITransientReadbackBufferRange F_RHITransientReadbackBufferManager::Allocate(Sz SizeInBytes, Sz AlignmentInBytes)
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
    void F_RHITransientReadbackBufferManager::AddNewPage(Sz SizeInBytes)
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
        F_RHITransientReadbackBufferPageBuildParams PageBuildParams;
        PageBuildParams.Context = GetContext();
        PageBuildParams.Index = SectionData.Pages.size();
        PageBuildParams.SizeInBytes = ActualSizeInBytes;
        auto Page = RACreateAndBuildShared<F_RHITransientReadbackBufferPage>(PageBuildParams);
#ifdef ABYTEK_DEBUG_INFO
        Page->SetDebugName(ABYTEK_TEXT("Abytek::RHITransientReadbackBufferPages[") + ToText(PageBuildParams.Index) + ABYTEK_TEXT("]"));
#endif
        SectionData.Pages.push_back(Page);
    }
}