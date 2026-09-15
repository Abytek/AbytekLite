#include "Abytek/Geometries/Render/RenderGeometryCommon.hpp"
#include "Abytek/Geometries/Render/RenderGeometryPage.hpp"


namespace Abytek
{
    F_StaticMeshGeometryUniformData_Simple F_StaticMeshGeometryUniformData_Simple::Make(U32 Index, const F_RenderGeometryAllocation& GeometryAllocation, const F_RenderGeometryAllocationStructure_Simple& GeometryAllocationStructure)
    {
        F_StaticMeshGeometryUniformData_Simple Result;
        Result.GeometryAllocation.PageIndex = GeometryAllocation.Page->GetIndex();
        Result.GeometryAllocation.BeginOffsetInBytes = static_cast<U32>(GeometryAllocation.BeginOffsetInBytes);
        Result.GeometryAllocation.EndOffsetInBytes = static_cast<U32>(GeometryAllocation.EndOffsetInBytes);
        Result.GeometryAllocationStructure = GeometryAllocationStructure;
        Result.Index = Index;
        return Result;
    }
}
