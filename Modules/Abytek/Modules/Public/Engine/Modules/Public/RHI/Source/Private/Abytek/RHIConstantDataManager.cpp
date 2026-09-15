#include "Abytek/RHIConstantDataManager.hpp"
#include "Abytek/RHIContext.hpp"
#include "Abytek/RHIPassUtilities.hpp"
#include "Abytek/RHIProcess.hpp"


namespace Abytek
{
    TS_Valid<A_RHIResource> F_RHIConstantDataRange::GetBuffer() const
    {
        return Page->GetBuffer();
    }
    TS_Valid<A_RHIResourceView> F_RHIConstantDataRange::CreateCBV() const
    {
        F_RHIBufferViewBuildParams BuildParams;
        BuildParams.Context = Page->GetContext();
        BuildParams.Access = F_RHIResourceAccess::MakeCBV();
        BuildParams.BufferViewAspect.OffsetInBytes = BeginOffsetInBytes;
        BuildParams.BufferViewAspect.SizeInBytes = GetSizeInBytes();
        return RACreateAndBuildShared<A_RHIResourceView>(BuildParams);
    }
    void F_RHIConstantDataRange::Upload(
        I_RHISubmissionItemContainer& SubmissionItemContainer,
        const F_RHIBufferDataView& BufferDataView,
        const F_DebugName& DebugName
    ) const
    {
        ABYTEK_ENGINE_RHI_ASSERT(BufferDataView.size() <= GetSizeInBytes()) << "Buffer data view is too big";
        H_RHIPassUtilities::UploadBuffer(
            SubmissionItemContainer,
            BufferDataView,
            GetBuffer(),
            BeginOffsetInBytes,
            DebugName
        );
    }
    void F_RHIConstantDataRange::Upload(
        I_RHISubmissionItemContainer& CPUSubmissionItemContainer,
        I_RHISubmissionItemContainer& GPUSubmissionItemContainer,
        const F_RHIBufferDataView& BufferDataView,
        const F_DebugName& DebugName
    ) const
    {
        ABYTEK_ENGINE_RHI_ASSERT(BufferDataView.size() <= GetSizeInBytes()) << "Buffer data view is too big";
        H_RHIPassUtilities::UploadBuffer(
            CPUSubmissionItemContainer,
            GPUSubmissionItemContainer,
            BufferDataView,
            GetBuffer(),
            BeginOffsetInBytes,
            DebugName
        );
    }
    F_RHIConstantDataRangeProxy F_RHIConstantDataRange::ConvertToProxy() const
    {
        F_RHIConstantDataRangeProxy Result;
        if (Page)
        {
            Result.BufferProxy = Page->GetBuffer()->GetProxy().FastCast<A_RHIResourceProxy>();
        }
        Result.BeginOffsetInBytes = BeginOffsetInBytes;
        Result.EndOffsetInBytes = EndOffsetInBytes;
        return Result;
    }

    ABYTEK_RA_OBJECT_DEFAULT(F_RHIConstantDataPage);
    void F_RHIConstantDataPage::Build(const F_RHIConstantDataPageBuildParams& BuildParams)
    {
        A_RHIContextChild::Build(BuildParams);
        _SizeInBytes = BuildParams.SizeInBytes;
         
        F_RHIBufferBuildParams BufferBuildParams;
        BufferBuildParams.Context = GetContext();
        BufferBuildParams.AccessCapabilities = F_RHIResourceAccess::MakeCBV();
        BufferBuildParams.BufferAspect.SizeInBytes = _SizeInBytes;
        _Buffer = RACreateAndBuildShared<A_RHIResource>(BufferBuildParams);
#ifdef ABYTEK_DEBUG_INFO
        _Buffer->SetDebugName(ABYTEK_DEBUG_NAME("RHIConstantDataPage"));
#endif
        
        _Distributor.Extends(_SizeInBytes);
    }
    void F_RHIConstantDataPage::Release()
    {
        _Buffer = {};
        
        _Distributor = {};
        _SizeInBytes = 0;
        A_RHIContextChild::Release();
    }

    TF_Optional<F_RHIConstantDataRange> F_RHIConstantDataPage::Allocate(Sz SizeInBytes)
    {
        if (auto Address = _Distributor.Allocate(SizeInBytes))
        {
            F_RHIConstantDataRange Result;
            Result.Page = ABYTEK_WTHIS();
            Result.BeginOffsetInBytes = *Address;
            Result.EndOffsetInBytes = Result.BeginOffsetInBytes + SizeInBytes;
            return Result;
        }
        return {};
    }
    void F_RHIConstantDataPage::Deallocate(const F_RHIConstantDataRange& Range)
    {
        _Distributor.Deallocate(Range.BeginOffsetInBytes);
    }

    ABYTEK_RA_OBJECT_DEFAULT(F_RHIConstantDataManager);
    void F_RHIConstantDataManager::Build(const F_RHIConstantDataManagerBuildParams& BuildParams)
    {
        A_RHIContextChild::Build(BuildParams);
        _MinPageSizeInBytes = BuildParams.MinPageSizeInBytes;
        _MaxPageSizeInBytes = BuildParams.MaxPageSizeInBytes;
    }
    void F_RHIConstantDataManager::Release()
    {
        SectionData.Pages = {};
        _MaxPageSizeInBytes = 0;
        _MinPageSizeInBytes = 0;
        A_RHIContextChild::Release();
    }

    F_RHIConstantDataRange F_RHIConstantDataManager::Allocate(Sz SizeInBytes)
    {
        Sz AlignedSizeInBytes = AlignSize_PO2(SizeInBytes, 256);
        
        for (auto It = SectionData.Pages.rbegin(); It != SectionData.Pages.rend(); ++It)
        {
            if (auto Allocation = (*It)->Allocate(AlignedSizeInBytes))
            {
                return *Allocation;
            }
        }
        AddNewPage(AlignedSizeInBytes);
        return *(SectionData.Pages.back()->Allocate(AlignedSizeInBytes));
    }
    void F_RHIConstantDataManager::AddNewPage(Sz SizeInBytes)
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
        F_RHIConstantDataPageBuildParams PageBuildParams;
        PageBuildParams.Context = GetContext();
        PageBuildParams.SizeInBytes = ActualSizeInBytes;
        SectionData.Pages.push_back(
            RACreateAndBuildShared<F_RHIConstantDataPage>(PageBuildParams)  
        );
    }
}
