#include "Abytek/RHITransientUploadBufferManager.hpp"
#include "Abytek/RHIContext.hpp"
#include "Abytek/RHIProcess.hpp"


namespace Abytek
{
    TS_Valid<A_RHIResource> F_RHITransientUploadBufferRange::GetBuffer() const
    {
        return Page->GetBuffer();
    }

    ABYTEK_RA_OBJECT_DEFAULT(F_RHITransientUploadBufferPage);
    void F_RHITransientUploadBufferPage::Build(const F_RHITransientUploadBufferPageBuildParams& BuildParams)
    {
        A_RHIContextChild::Build(BuildParams);
        _SizeInBytes = BuildParams.SizeInBytes;
        _UsageInBytes = 0;
         
        F_RHIBufferBuildParams BufferBuildParams;
        BufferBuildParams.Context = GetContext();
        BufferBuildParams.AccessCapabilities = F_RHIResourceAccess::MakeUploadCapabilities();
        BufferBuildParams.BufferAspect.SizeInBytes = _SizeInBytes;
        BufferBuildParams.AdditionalFlags = E_RHIResourceAdditionalFlag::TRANSIENT;
        _Buffer = RACreateAndBuildShared<A_RHIResource>(BufferBuildParams);
    }
    void F_RHITransientUploadBufferPage::Release()
    {
        _Buffer = {};
        
        _UsageInBytes = 0;
        _SizeInBytes = 0;
        A_RHIContextChild::Release();
    }

    TF_Optional<F_RHITransientUploadBufferRange> F_RHITransientUploadBufferPage::Allocate(Sz SizeInBytes)
    {
        if ((_UsageInBytes + SizeInBytes) > _SizeInBytes)
        {
            return {};
        }
        F_RHITransientUploadBufferRange Result;
        Result.Page = ABYTEK_WTHIS();
        Result.BeginOffsetInBytes = _UsageInBytes;
        Result.EndOffsetInBytes = _UsageInBytes + SizeInBytes;
        _UsageInBytes += SizeInBytes;
        return Result;
    }

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

    F_RHITransientUploadBufferRange F_RHITransientUploadBufferManager::Allocate(Sz SizeInBytes)
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
    void F_RHITransientUploadBufferManager::AddNewPage(Sz SizeInBytes)
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
        F_RHITransientUploadBufferPageBuildParams PageBuildParams;
        PageBuildParams.Context = GetContext();
        PageBuildParams.SizeInBytes = ActualSizeInBytes;
        SectionData.Pages.push_back(
            RACreateAndBuildShared<F_RHITransientUploadBufferPage>(PageBuildParams)  
        );
    }
}