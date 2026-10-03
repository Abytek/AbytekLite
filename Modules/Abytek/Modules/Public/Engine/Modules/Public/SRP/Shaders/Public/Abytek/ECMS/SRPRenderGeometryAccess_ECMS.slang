module SRPRenderGeometryAccess_ECMS;

__exported import Abytek.Renderer.RenderGeometry.Common;
__exported import Abytek.Renderer.RenderGeometry.DeclarePages;
__exported import Abytek.Renderer.RenderGeometry.Access;

namespace Abytek
{
    namespace SRP::ECMS
    {
        public typealias F_DirectTriangle = uint3;
        public F_DirectTriangle LoadDirectTriangle(
            F_RenderGeometryAddress GeometryAddress,
            F_RenderGeometryAllocationStructure_ECMS GeometryAllocationStructure,
            uint MeshletIndex,
            uint LocalMeshletTriangleIndex
        )
        {
            Abytek::F_ECMSMeshlet Meshlet = Abytek::H_ECMSMesh::LoadMeshlet(
                GeometryAddress,
                GeometryAllocationStructure,
                MeshletIndex
            );

            Abytek::F_ECMSMeshTriangle Triangle = Abytek::H_ECMSMesh::LoadTriangle(
                GeometryAddress,
                GeometryAllocationStructure,
                Meshlet.Offset_Triangles + LocalMeshletTriangleIndex
            );

            uint3 TriangleU32X3;
            Abytek::H_ECMSMeshTriangle::Unpack(Triangle, TriangleU32X3);

            uint VertexIndex0 = Abytek::H_ECMSMesh::LoadVertexIndex(
                GeometryAddress,
                GeometryAllocationStructure,
                Meshlet.Offset_VertexIndices + TriangleU32X3.x
            );
            uint VertexIndex1 = Abytek::H_ECMSMesh::LoadVertexIndex(
                GeometryAddress,
                GeometryAllocationStructure,
                Meshlet.Offset_VertexIndices + TriangleU32X3.y
            );
            uint VertexIndex2 = Abytek::H_ECMSMesh::LoadVertexIndex(
                GeometryAddress,
                GeometryAllocationStructure,
                Meshlet.Offset_VertexIndices + TriangleU32X3.z
            );

            return uint3(VertexIndex0, VertexIndex1, VertexIndex2);
        }

        public struct TF_TriangleVertexData<T>
        {
            public T Vertex0 = {};
            public T Vertex1 = {};
            public T Vertex2 = {};
        };
        public TF_TriangleVertexData<float3> LoadTriangleVertexData_Position(
            F_RenderGeometryAddress GeometryAddress,
            F_RenderGeometryAllocationStructure_ECMS GeometryAllocationStructure,
            F_DirectTriangle DirectTriangle
        )
        {
            TF_TriangleVertexData<float3> Result;
            Result.Vertex0 = H_ECMSMesh::LoadVertexPosition(
                GeometryAddress,
                GeometryAllocationStructure,
                DirectTriangle.x
            );
            Result.Vertex1 = H_ECMSMesh::LoadVertexPosition(
                GeometryAddress,
                GeometryAllocationStructure,
                DirectTriangle.y
            );
            Result.Vertex2 = H_ECMSMesh::LoadVertexPosition(
                GeometryAddress,
                GeometryAllocationStructure,
                DirectTriangle.z
            );
            return Result;
        }
        public TF_TriangleVertexData<float3> LoadTriangleVertexData_Normal(
            F_RenderGeometryAddress GeometryAddress,
            F_RenderGeometryAllocationStructure_ECMS GeometryAllocationStructure,
            F_DirectTriangle DirectTriangle
        )
        {
            TF_TriangleVertexData<float3> Result;
            Result.Vertex0 = H_ECMSMesh::LoadVertexNormal(
                GeometryAddress,
                GeometryAllocationStructure,
                DirectTriangle.x
            );
            Result.Vertex1 = H_ECMSMesh::LoadVertexNormal(
                GeometryAddress,
                GeometryAllocationStructure,
                DirectTriangle.y
            );
            Result.Vertex2 = H_ECMSMesh::LoadVertexNormal(
                GeometryAddress,
                GeometryAllocationStructure,
                DirectTriangle.z
            );
            return Result;
        }
        public TF_TriangleVertexData<float4> LoadTriangleVertexData_TangentAndSign(
            F_RenderGeometryAddress GeometryAddress,
            F_RenderGeometryAllocationStructure_ECMS GeometryAllocationStructure,
            F_DirectTriangle DirectTriangle
        )
        {
            TF_TriangleVertexData<float4> Result;
            Result.Vertex0 = H_ECMSMesh::LoadVertexTangentAndSign(
                GeometryAddress,
                GeometryAllocationStructure,
                DirectTriangle.x
            );
            Result.Vertex1 = H_ECMSMesh::LoadVertexTangentAndSign(
                GeometryAddress,
                GeometryAllocationStructure,
                DirectTriangle.y
            );
            Result.Vertex2 = H_ECMSMesh::LoadVertexTangentAndSign(
                GeometryAddress,
                GeometryAllocationStructure,
                DirectTriangle.z
            );
            return Result;
        }
        public TF_TriangleVertexData<float2> LoadTriangleVertexData_UV(
            F_RenderGeometryAddress GeometryAddress,
            F_RenderGeometryAllocationStructure_ECMS GeometryAllocationStructure,
            F_DirectTriangle DirectTriangle
        )
        {
            TF_TriangleVertexData<float2> Result;
            Result.Vertex0 = H_ECMSMesh::LoadVertexUV(
                GeometryAddress,
                GeometryAllocationStructure,
                DirectTriangle.x
            );
            Result.Vertex1 = H_ECMSMesh::LoadVertexUV(
                GeometryAddress,
                GeometryAllocationStructure,
                DirectTriangle.y
            );
            Result.Vertex2 = H_ECMSMesh::LoadVertexUV(
                GeometryAddress,
                GeometryAllocationStructure,
                DirectTriangle.z
            );
            return Result;
        }
    }
}