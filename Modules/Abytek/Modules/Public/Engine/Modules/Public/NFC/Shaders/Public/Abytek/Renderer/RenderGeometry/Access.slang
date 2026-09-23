module Access;

__exported import Abytek.Renderer.RenderGeometry.Common;
__exported import Abytek.Renderer.RenderGeometry.DeclarePages;

#include "Abytek/Renderer/RenderGeometry/Macros.slangh"

namespace Abytek
{
    public struct H_RenderGeometryAccess
    {
        public static F_DeclaredRenderGeometryPage GetPage(F_RenderGeometryAddress Address)
        {
#ifdef ABYTEK_RENDER_GEOMETRY_ENABLE_MULTIPLE_PAGES
            return ABYTEK_RENDER_GEOMETRY_GET_RESOURCE()[H_RenderGeometryAddress::GetPageIndex(Address)];
#else
            return ABYTEK_RENDER_GEOMETRY_GET_RESOURCE();
#endif
        }
    }

    public uint SimpleMesh_LoadVertexIndex(
        F_RenderGeometryAddress GeometryAddress,
        in F_RenderGeometryAllocationStructure_Simple GeometryAllocationStructure,
        uint IndexId
    )
    {
        return H_RenderGeometryAccess::GetPage(GeometryAddress).Load(
            H_RenderGeometryAddress::GetOffsetInBytes(GeometryAddress)
            + GeometryAllocationStructure.Indices_LocalOffsetInBytes
            + IndexId * 4
        );
    }
    public float3 SimpleMesh_LoadVertexPosition(
        F_RenderGeometryAddress GeometryAddress,
        in F_RenderGeometryAllocationStructure_Simple GeometryAllocationStructure,
        uint VertexId
    )
    {
        return asfloat(
            H_RenderGeometryAccess::GetPage(GeometryAddress).Load4(
                H_RenderGeometryAddress::GetOffsetInBytes(GeometryAddress)
                + GeometryAllocationStructure.Positions_LocalOffsetInBytes
                + VertexId * 16
            )
        ).xyz;
    }
    public float3 SimpleMesh_LoadVertexNormal(
        F_RenderGeometryAddress GeometryAddress,
        in F_RenderGeometryAllocationStructure_Simple GeometryAllocationStructure,
        uint VertexId
    )
    {
        return asfloat(
            H_RenderGeometryAccess::GetPage(GeometryAddress).Load4(
                H_RenderGeometryAddress::GetOffsetInBytes(GeometryAddress)
                + GeometryAllocationStructure.Normals_LocalOffsetInBytes
                + VertexId * 16
            )
        ).xyz;
    }
    public float4 SimpleMesh_LoadVertexTangentAndSign(
        F_RenderGeometryAddress GeometryAddress,
        in F_RenderGeometryAllocationStructure_Simple GeometryAllocationStructure,
        uint VertexId
    )
    {
        return asfloat(
            H_RenderGeometryAccess::GetPage(GeometryAddress).Load4(
                H_RenderGeometryAddress::GetOffsetInBytes(GeometryAddress)
                + GeometryAllocationStructure.TangentsAndSigns_LocalOffsetInBytes
                + VertexId * 16
            )
        );
    }
    public float2 SimpleMesh_LoadVertexUV(
        F_RenderGeometryAddress GeometryAddress,
        in F_RenderGeometryAllocationStructure_Simple GeometryAllocationStructure,
        uint VertexId
    )
    {
        return asfloat(
            H_RenderGeometryAccess::GetPage(GeometryAddress).Load2(
                H_RenderGeometryAddress::GetOffsetInBytes(GeometryAddress)
                + GeometryAllocationStructure.UVs_LocalOffsetInBytes
                + VertexId * 8
            )
        );
    }
}