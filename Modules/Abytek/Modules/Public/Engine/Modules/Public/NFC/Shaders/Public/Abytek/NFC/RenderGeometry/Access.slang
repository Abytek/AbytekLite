module Access;

__exported import Abytek.NFC.RenderGeometry.Common;
__exported import Abytek.NFC.RenderGeometry.DeclarePages;

namespace Abytek::NFC::RenderGeometry
{
    public F_DeclaredPage GetPage(F_GeometryAddress Address)
    {
#ifdef ABYTEK_RENDER_GEOMETRY_ENABLE_MULTIPLE_PAGES
        return RenderGeometryPages[NFC::RenderGeometry::GetPageIndexFromGeometryAddress(Address)];
#else
        return RenderGeometryPage_Single;
#endif
    }

    public uint SimpleMesh_LoadVertexIndex(
        F_GeometryAddress GeometryAddress,
        in F_GeometryAllocationStructure_Simple GeometryAllocationStructure,
        uint IndexId
    )
    {
        return GetPage(GeometryAddress).Load(
            NFC::RenderGeometry::GetOffsetInBytesFromGeometryAddress(GeometryAddress)
            + GeometryAllocationStructure.Indices_LocalOffsetInBytes
            + IndexId * 4
        );
    }
    public float3 SimpleMesh_LoadVertexPosition(
        F_GeometryAddress GeometryAddress,
        in F_GeometryAllocationStructure_Simple GeometryAllocationStructure,
        uint VertexId
    )
    {
        return asfloat(
            GetPage(GeometryAddress).Load4(
                NFC::RenderGeometry::GetOffsetInBytesFromGeometryAddress(GeometryAddress)
                + GeometryAllocationStructure.Positions_LocalOffsetInBytes
                + VertexId * 16
            )
        ).xyz;
    }
    public float3 SimpleMesh_LoadVertexNormal(
        F_GeometryAddress GeometryAddress,
        in F_GeometryAllocationStructure_Simple GeometryAllocationStructure,
        uint VertexId
    )
    {
        return asfloat(
            GetPage(GeometryAddress).Load4(
                NFC::RenderGeometry::GetOffsetInBytesFromGeometryAddress(GeometryAddress)
                + GeometryAllocationStructure.Normals_LocalOffsetInBytes
                + VertexId * 16
            )
        ).xyz;
    }
    public float4 SimpleMesh_LoadVertexTangentAndSign(
        F_GeometryAddress GeometryAddress,
        in F_GeometryAllocationStructure_Simple GeometryAllocationStructure,
        uint VertexId
    )
    {
        return asfloat(
            GetPage(GeometryAddress).Load4(
                NFC::RenderGeometry::GetOffsetInBytesFromGeometryAddress(GeometryAddress)
                + GeometryAllocationStructure.TangentsAndSigns_LocalOffsetInBytes
                + VertexId * 16
            )
        );
    }
    public float2 SimpleMesh_LoadVertexUV(
        F_GeometryAddress GeometryAddress,
        in F_GeometryAllocationStructure_Simple GeometryAllocationStructure,
        uint VertexId
    )
    {
        return asfloat(
            GetPage(GeometryAddress).Load2(
                NFC::RenderGeometry::GetOffsetInBytesFromGeometryAddress(GeometryAddress)
                + GeometryAllocationStructure.UVs_LocalOffsetInBytes
                + VertexId * 8
            )
        );
    }
}