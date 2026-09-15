#include "Abytek/Geometries/Render/RenderGeometryPage.hpp"
#include "Abytek/Geometries/Render/RenderGeometryManager.hpp"


namespace Abytek
{
    F_RenderGeometryPage::F_RenderGeometryPage(const TW<F_RenderGeometryManager>& Manager, U32 Index, U64 SizeInBytes) :
        _Manager(Manager),
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
        _RHIBuffer->SetDebugName(ABYTEK_TEXT("RenderGeometryPages[") + ToText(Index) + ABYTEK_TEXT("]"));
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
    F_RenderGeometryPage::~F_RenderGeometryPage()
    {
        _UAV = {};
        _SRV = {};
        
        _RHIBuffer = {};
        
        _Distributor = {};
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
