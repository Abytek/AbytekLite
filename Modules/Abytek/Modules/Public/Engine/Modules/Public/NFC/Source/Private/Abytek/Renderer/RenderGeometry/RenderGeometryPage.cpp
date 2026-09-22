#include "Abytek/Renderer/RenderGeometry/RenderGeometryPage.hpp"
#include "Abytek/Renderer/RenderGeometry/RenderGeometryStorage.hpp"


namespace Abytek
{
    void F_RenderGeometryPage::Init(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const TW<F_RenderGeometryStorage>& Manager, 
        U32 Index, 
        U64 SizeInBytes
    )
    {
        InitMinimal(SubmissionItemContainer);
        
        _Manager = Manager;
        _Index = Index;
        _SizeInBytes = SizeInBytes;
        
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
        _RHIBuffer->SetDebugName(GetDebugName());
#endif
        
        {
            F_RHIBufferViewBuildParams BufferViewBuildParams;
            BufferViewBuildParams.Context = H_RHI::GetMainContext().Weak();
            BufferViewBuildParams.BufferViewAspect.SizeInBytes = BufferBuildParams.BufferAspect.SizeInBytes;
            BufferViewBuildParams.BufferViewAspect.RawAccess = true;
            BufferViewBuildParams.Resource = _RHIBuffer;
            BufferViewBuildParams.Access = F_RHIResourceAccess::MakeSRV();
            _SRV = RACreateAndBuildShared<A_RHIResourceView>(BufferViewBuildParams);
#ifdef ABYTEK_DEBUG_INFO
            _SRV->SetDebugName(*GetDebugName() + ABYTEK_TEXT(".SRV"));
#endif
        }
        {
            F_RHIBufferViewBuildParams BufferViewBuildParams;
            BufferViewBuildParams.Context = H_RHI::GetMainContext().Weak();
            BufferViewBuildParams.BufferViewAspect.SizeInBytes = BufferBuildParams.BufferAspect.SizeInBytes;
            BufferViewBuildParams.BufferViewAspect.RawAccess = true;
            BufferViewBuildParams.Resource = _RHIBuffer;
            BufferViewBuildParams.Access = F_RHIResourceAccess::MakeUAV();
            _UAV = RACreateAndBuildShared<A_RHIResourceView>(BufferViewBuildParams);
#ifdef ABYTEK_DEBUG_INFO
            _UAV->SetDebugName(*GetDebugName() + ABYTEK_TEXT(".UAV"));
#endif
        }
    }
    void F_RenderGeometryPage::Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _UAV = {};
        _SRV = {};
        
        _RHIBuffer = {};
        
        _Distributor = {};
        
        _SizeInBytes = 0;
        _Index = ~U32(0);
        _Manager = {};
        
        A_RenderObject::Release(SubmissionItemContainer);
    }

    TF_Optional<F_RenderGeometryAllocation> F_RenderGeometryPage::Allocate(U64 SizeInBytes, U64 AlignmentInBytes)
    {
        if (auto OffsetInBytes = _Distributor.Allocate(SizeInBytes, AlignmentInBytes))
        {
            F_RenderGeometryAllocation Allocation;
            Allocation.Page = ABYTEK_STHIS();
            Allocation.BeginOffsetInBytes = *OffsetInBytes;
            Allocation.EndOffsetInBytes = Allocation.BeginOffsetInBytes + SizeInBytes;
            return Allocation;
        }
        return {};
    }
    void F_RenderGeometryPage::Deallocate(const F_RenderGeometryAllocation& Allocation)
    {
        _Distributor.Deallocate(Allocation.BeginOffsetInBytes);
    }
}
