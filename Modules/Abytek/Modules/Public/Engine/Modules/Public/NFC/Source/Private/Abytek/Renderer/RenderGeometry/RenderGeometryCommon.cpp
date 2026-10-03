#include "Abytek/Renderer/RenderGeometry/RenderGeometryCommon.hpp"
#include "Abytek/Renderer/RenderGeometry/RenderGeometryPage.hpp"


namespace Abytek
{
    F_RenderGeometryAddress F_RenderGeometryAddress::From(const F_RenderGeometryAllocation& GeometryAllocation)
    {
        F_RenderGeometryAddress Result;
        Result.PageIndex = GeometryAllocation.Page->GetIndex();
        Result.OffsetInBytes = GeometryAllocation.BeginOffsetInBytes;
        return Result;
    }

    F_StaticMeshGeometryUniformData_Simple F_StaticMeshGeometryUniformData_Simple::Make(const F_RenderGeometryAllocation& GeometryAllocation, const F_RenderGeometryAllocationStructure_Simple& GeometryAllocationStructure)
    {
        F_StaticMeshGeometryUniformData_Simple Result;
        Result.GeometryAddress.PageIndex = GeometryAllocation.Page->GetIndex();
        Result.GeometryAddress.OffsetInBytes = static_cast<U32>(GeometryAllocation.BeginOffsetInBytes);
        Result.GeometryAllocationStructure = GeometryAllocationStructure;
        return Result;
    }
    
    F_StaticMeshGeometryUniformData_ECMS F_StaticMeshGeometryUniformData_ECMS::Make(const F_RenderGeometryAllocation& GeometryAllocation, const F_RenderGeometryAllocationStructure_ECMS& GeometryAllocationStructure)
    {
        F_StaticMeshGeometryUniformData_ECMS Result;
        Result.GeometryAddress.PageIndex = GeometryAllocation.Page->GetIndex();
        Result.GeometryAddress.OffsetInBytes = static_cast<U32>(GeometryAllocation.BeginOffsetInBytes);
        Result.GeometryAllocationStructure = GeometryAllocationStructure;
        return Result;
    }
}
