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

    public struct H_SimpleMesh
    {
        public static uint LoadVertexIndex(
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
        public static float3 LoadVertexPosition(
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
        public static float3 LoadVertexNormal(
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
        public static float4 LoadVertexTangentAndSign(
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
        public static float2 LoadVertexUV(
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
    };

    public struct H_ECMSMesh
    {
        public static F_ECMSMeshlet LoadMeshlet(
            F_RenderGeometryAddress GeometryAddress,
            in F_RenderGeometryAllocationStructure_ECMS GeometryAllocationStructure,
            uint MeshletIndex
        )
        {
            return H_RenderGeometryAccess::GetPage(GeometryAddress).Load<F_ECMSMeshlet>(
                H_RenderGeometryAddress::GetOffsetInBytes(GeometryAddress)
                + GeometryAllocationStructure.Meshlets_LocalOffsetInBytes
                + MeshletIndex * sizeof(F_ECMSMeshlet)
            );
        }
        public static F_ECMSMeshTriangle LoadTriangle(
            F_RenderGeometryAddress GeometryAddress,
            in F_RenderGeometryAllocationStructure_ECMS GeometryAllocationStructure,
            uint Index
        )
        {
            return H_RenderGeometryAccess::GetPage(GeometryAddress).Load(
                H_RenderGeometryAddress::GetOffsetInBytes(GeometryAddress)
                + GeometryAllocationStructure.Triangles_LocalOffsetInBytes
                + Index * sizeof(F_ECMSMeshTriangle)
            );
        }
        public static F_ECMSGlobalVertexIndex LoadVertexIndex(
            F_RenderGeometryAddress GeometryAddress,
            in F_RenderGeometryAllocationStructure_ECMS GeometryAllocationStructure,
            uint Index
        )
        {
            return H_RenderGeometryAccess::GetPage(GeometryAddress).Load(
                H_RenderGeometryAddress::GetOffsetInBytes(GeometryAddress)
                + GeometryAllocationStructure.VertexIndices_LocalOffsetInBytes
                + Index * sizeof(F_ECMSGlobalVertexIndex)
            );
        }
        public static float3 LoadVertexPosition(
            F_RenderGeometryAddress GeometryAddress,
            in F_RenderGeometryAllocationStructure_ECMS GeometryAllocationStructure,
            uint VertexId
        )
        {
            return asfloat(
                H_RenderGeometryAccess::GetPage(GeometryAddress).Load4(
                    H_RenderGeometryAddress::GetOffsetInBytes(GeometryAddress)
                    + GeometryAllocationStructure.Positions_LocalOffsetInBytes
                    + VertexId * 16
                )
            ) .xyz;
        }
        public static float3 LoadVertexNormal(
            F_RenderGeometryAddress GeometryAddress,
            in F_RenderGeometryAllocationStructure_ECMS GeometryAllocationStructure,
            uint VertexId
        )
        {
            return asfloat(
                H_RenderGeometryAccess::GetPage(GeometryAddress).Load4(
                    H_RenderGeometryAddress::GetOffsetInBytes(GeometryAddress)
                    + GeometryAllocationStructure.Normals_LocalOffsetInBytes
                    + VertexId * 16
                )
            ) .xyz;
        }
        public static float4 LoadVertexTangentAndSign(
            F_RenderGeometryAddress GeometryAddress,
            in F_RenderGeometryAllocationStructure_ECMS GeometryAllocationStructure,
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
        public static float2 LoadVertexUV(
            F_RenderGeometryAddress GeometryAddress,
            in F_RenderGeometryAllocationStructure_ECMS GeometryAllocationStructure,
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
    };
}