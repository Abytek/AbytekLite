#include "Abytek/GPUData/Render/GPUDataComponentPage.hpp"
#include "Abytek/GPUData/Render/GPUDataComponentStorage.hpp"


namespace Abytek
{
    F_GPUDataComponentPage::F_GPUDataComponentPage(const TW<F_GPUDataComponentStorage>& Storage, U32 Index, U64 SizeInBytes) :
        _Storage(Storage),
        _Index(Index),
        _SizeInBytes(SizeInBytes)
    {
        _Distributor.Extends(_SizeInBytes);
        
        F_RHIBufferBuildParams BufferBuildParams;
        BufferBuildParams.Context = H_RHI::GetMainContext().Weak();
        BufferBuildParams.BufferAspect.SizeInBytes = _SizeInBytes;
        BufferBuildParams.AccessCapabilities = (
            F_RHIResourceAccess::MakeSRVCapabilities()
            | F_RHIResourceAccess::MakeUAVCapabilities()
            | F_RHIResourceAccess::MakeVertexBufferCapabilities()
            | F_RHIResourceAccess::MakeIndexBufferCapabilities()
        );
        _RHIBuffer = RACreateAndBuildShared<A_RHIResource>(BufferBuildParams);
#ifdef ABYTEK_DEBUG_INFO
        _RHIBuffer->SetDebugName(ABYTEK_TEXT("GPUDataComponentPages[") + ToText(Index) + ABYTEK_TEXT("]"));
#endif
        
        {
            F_RHIBufferViewBuildParams BufferViewBuildParams;
            BufferViewBuildParams.Context = H_RHI::GetMainContext().Weak();
            BufferViewBuildParams.BufferViewAspect.SizeInBytes = BufferBuildParams.BufferAspect.SizeInBytes;
            BufferViewBuildParams.BufferViewAspect.RawAccess = true;
            BufferViewBuildParams.Resource = _RHIBuffer;
            BufferViewBuildParams.Access = F_RHIResourceAccess::MakeSRV();
            _SRV = RACreateAndBuildShared<A_RHIResourceView>(BufferViewBuildParams);
        }
        {
            F_RHIBufferViewBuildParams BufferViewBuildParams;
            BufferViewBuildParams.Context = H_RHI::GetMainContext().Weak();
            BufferViewBuildParams.BufferViewAspect.SizeInBytes = BufferBuildParams.BufferAspect.SizeInBytes;
            BufferViewBuildParams.BufferViewAspect.RawAccess = true;
            BufferViewBuildParams.Resource = _RHIBuffer;
            BufferViewBuildParams.Access = F_RHIResourceAccess::MakeUAV();
            _UAV = RACreateAndBuildShared<A_RHIResourceView>(BufferViewBuildParams);
        }
    }
    F_GPUDataComponentPage::~F_GPUDataComponentPage()
    {
        _UAV = {};
        _SRV = {};
        
        _RHIBuffer = {};
        
        _Distributor = {};
    }

    TF_Optional<F_GPUDataComponentAllocation> F_GPUDataComponentPage::Allocate(U32 NumComponents)
    {
        if (auto OffsetInBytes = _Distributor.Allocate(1))
        {
            auto StrideInBytes = _Storage->GetStrideInBytes();
            
            F_GPUDataComponentAllocation Allocation;
            Allocation.Page = ABYTEK_STHIS();
            Allocation.BeginOffsetInBytes = static_cast<U64>(StrideInBytes) * *OffsetInBytes;
            Allocation.EndOffsetInBytes = Allocation.BeginOffsetInBytes + static_cast<U64>(StrideInBytes) * static_cast<U64>(NumComponents);
            return Allocation;
        }
        return {};
    }
    void F_GPUDataComponentPage::Deallocate(const F_GPUDataComponentAllocation& Allocation)
    {
        _Distributor.Deallocate(Allocation.BeginOffsetInBytes);
    }
}
