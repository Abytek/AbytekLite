module Access;

__exported import Common;
__exported import DeclarePages;

#ifdef ABYTEK_RENDER_GEOMETRY_ENABLE_MULTIPLE_PAGES
#define ABYTEK_RENDER_GEOMETRY_GET_PAGE_FROM_ALLOCATION(...) RenderGeometryPages[(__VA_ARGS__).PageIndex]
#else
#define ABYTEK_RENDER_GEOMETRY_GET_PAGE_FROM_ALLOCATION(...) RenderGeometryPage_Single
#endif

namespace NFC::RenderGeometry
{
    public uint SimpleMesh_LoadVertexIndex(
        in F_GeometryAllocation GeometryAllocation,
        in F_GeometryAllocationStructure_Simple GeometryAllocationStructure,
        uint IndexId
    )
    {
        return ABYTEK_RENDER_GEOMETRY_GET_PAGE_FROM_ALLOCATION(GeometryAllocation).Load(
            GeometryAllocation.BeginOffsetInBytes
            + GeometryAllocationStructure.Indices_LocalOffsetInBytes
            + IndexId * 4
        );
    }
    public float3 SimpleMesh_LoadVertexPosition(
        in F_GeometryAllocation GeometryAllocation,
        in F_GeometryAllocationStructure_Simple GeometryAllocationStructure,
        uint VertexId
    )
    {
        return asfloat(
                   ABYTEK_RENDER_GEOMETRY_GET_PAGE_FROM_ALLOCATION(GeometryAllocation).Load4(
                GeometryAllocation.BeginOffsetInBytes
                + GeometryAllocationStructure.Positions_LocalOffsetInBytes
                + VertexId * 16
            )
        ).xyz;
    }
    public float3 SimpleMesh_LoadVertexNormal(
        in F_GeometryAllocation GeometryAllocation,
        in F_GeometryAllocationStructure_Simple GeometryAllocationStructure,
        uint VertexId
    )
    {
        return asfloat(
                   ABYTEK_RENDER_GEOMETRY_GET_PAGE_FROM_ALLOCATION(GeometryAllocation).Load4(
                GeometryAllocation.BeginOffsetInBytes
                + GeometryAllocationStructure.Normals_LocalOffsetInBytes
                + VertexId * 16
            )
        ).xyz;
    }
    public float4 SimpleMesh_LoadVertexTangentAndSign(
        in F_GeometryAllocation GeometryAllocation,
        in F_GeometryAllocationStructure_Simple GeometryAllocationStructure,
        uint VertexId
    )
    {
        return asfloat(
                   ABYTEK_RENDER_GEOMETRY_GET_PAGE_FROM_ALLOCATION(GeometryAllocation).Load4(
                GeometryAllocation.BeginOffsetInBytes
                + GeometryAllocationStructure.TangentsAndSigns_LocalOffsetInBytes
                + VertexId * 16
            )
        );
    }
    public float2 SimpleMesh_LoadVertexUV(
        in F_GeometryAllocation GeometryAllocation,
        in F_GeometryAllocationStructure_Simple GeometryAllocationStructure,
        uint VertexId
    )
    {
        return asfloat(
                   ABYTEK_RENDER_GEOMETRY_GET_PAGE_FROM_ALLOCATION(GeometryAllocation).Load2(
                GeometryAllocation.BeginOffsetInBytes
                + GeometryAllocationStructure.UVs_LocalOffsetInBytes
                + VertexId * 8
            )
        );
    }
}