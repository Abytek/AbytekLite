#include "Abytek/Renderer/RenderGeometry/RenderGeometryCommon.hpp"
#include "Abytek/Renderer/RenderGeometry/RenderGeometryPage.hpp"


namespace Abytek
{
    F_StaticMeshGeometryUniformData_Simple F_StaticMeshGeometryUniformData_Simple::Make(U32 Index, const F_RenderGeometryAllocation& GeometryAllocation, const F_RenderGeometryAllocationStructure_Simple& GeometryAllocationStructure)
    {
        F_StaticMeshGeometryUniformData_Simple Result;
        Result.Index = Index;
        Result.GeometryAddress.PageIndex = GeometryAllocation.Page->GetIndex();
        Result.GeometryAddress.OffsetInBytes = static_cast<U32>(GeometryAllocation.BeginOffsetInBytes);
        Result.GeometryAllocationStructure = GeometryAllocationStructure;
        return Result;
    }
}
