module StaticMeshMS_ECMS;

import Abytek.SRPBasicDrawers.StaticMeshCommon;
import Abytek.Renderer.RenderView;
import Abytek.Renderer.RenderGeometry.Common;
import Abytek.Renderer.RenderGeometry.DeclarePages;
import Abytek.Renderer.RenderGeometry.Access;
import Abytek.ClipPositionDepthReverse;
import Abytek.ClipPositionToNDCPosition;

#include "Abytek/ECMS/SRPInstancedMeshletBuffer_ECMS.slangh"

ConstantBuffer<Abytek::F_RenderViewUniformData> ViewUniformData;
ConstantBuffer<Abytek::F_SRPBasicStaticMeshUniformData> StaticMeshUniformData;
ConstantBuffer<Abytek::F_StaticMeshGeometryUniformData_ECMS> StaticMeshGeometryUniformData_ECMS;

#ifdef OUTPUT_OPAQUE_VISIBILITY_BUFFER
ConstantBuffer<Abytek::F_SRPBasicStaticMeshVisibilityConfig> StaticMeshVisibilityConfig;
#endif

ABYTEK_SRP_ECMS_DEFINE_INSTANCED_MESHLET_BUFFER_RW(InstancedMeshlets);

groupshared bool GroupCulled;
groupshared Abytek::SRP::ECMS::F_InstancedMeshlet InstancedMeshlet;
groupshared Abytek::SRP::ECMS::F_InstancedMeshletId InstancedMeshletId;

[shader("mesh")]
[numthreads(NUM_THREADS, 1, 1)]
[outputtopology("triangle")]
void MainMS(
    uint3 GroupThreadID: SV_GroupThreadID,
    uint3 GroupID: SV_GroupID,

    out vertices Abytek::F_SRPBasicStaticMeshVSOutput OutVertices[ABYTEK_ECMS_MAX_VERTICES_PER_MESHLET],
    out indices uint3 OutTriangles[ABYTEK_ECMS_MAX_PRIMITIVES_PER_MESHLET],
    out primitives Abytek::F_SRPBasicStaticMeshPrimitive OutPrimitiveIds[ABYTEK_ECMS_MAX_PRIMITIVES_PER_MESHLET]
)
{
    if (GroupThreadID.x == 0)
    {
        GroupCulled = false;
    }

    if (GroupThreadID.x == 0)
    {
        InstancedMeshlet = {};
#ifdef OUTPUT_OPAQUE_VISIBILITY_BUFFER
        InstancedMeshlet.PrimitiveAddress = StaticMeshVisibilityConfig.PrimitiveAddress;
#else
        InstancedMeshlet.PrimitiveAddress = Abytek::INVALID_GPU_DATA_INSTANCE_ADDRESS;
#endif
        InstancedMeshlet.MeshletIndex = GroupID.x;
        InstancedMeshletId = ABYTEK_SRP_ECMS_PUSH_INSTANCED_MESHLET(InstancedMeshlets, InstancedMeshlet);
        GroupCulled |= (InstancedMeshletId == Abytek::SRP::ECMS::INVALID_INSTANCED_MESHLET_ID);
    }

    GroupMemoryBarrierWithGroupSync();

    Abytek::F_ECMSMeshlet Meshlet = Abytek::H_ECMSMesh::LoadMeshlet(
        StaticMeshGeometryUniformData_ECMS.GeometryAddress,
        StaticMeshGeometryUniformData_ECMS.GeometryAllocationStructure,
        GroupID.x
    );
    SetMeshOutputCounts(
        GroupCulled ? 0 : Meshlet.Count_VertexIndices,
        GroupCulled ? 0 : Meshlet.Count_Triangles
    );

    if (GroupCulled)
    {
        return;
    }

    if (GroupThreadID.x < Meshlet.Count_VertexIndices)
    {
        uint VertexIndex = Abytek::H_ECMSMesh::LoadVertexIndex(
            StaticMeshGeometryUniformData_ECMS.GeometryAddress,
            StaticMeshGeometryUniformData_ECMS.GeometryAllocationStructure,
            Meshlet.Offset_VertexIndices + GroupThreadID.x
        );

        float3 VertexPosition = Abytek::H_ECMSMesh::LoadVertexPosition(
            StaticMeshGeometryUniformData_ECMS.GeometryAddress,
            StaticMeshGeometryUniformData_ECMS.GeometryAllocationStructure,
            VertexIndex
        );
        float3 VertexNormal = Abytek::H_ECMSMesh::LoadVertexNormal(
            StaticMeshGeometryUniformData_ECMS.GeometryAddress,
            StaticMeshGeometryUniformData_ECMS.GeometryAllocationStructure,
            VertexIndex
        );
        float2 VertexUV = Abytek::H_ECMSMesh::LoadVertexUV(
            StaticMeshGeometryUniformData_ECMS.GeometryAddress,
            StaticMeshGeometryUniformData_ECMS.GeometryAllocationStructure,
            VertexIndex
        );

        float3 LocalPosition = VertexPosition;
        float3 LocalNormal = VertexNormal;

        float3 WorldPosition = mul(StaticMeshUniformData.TransformMatrix, float4(LocalPosition, 1.0f)).xyz;
        float3 WorldNormal = mul((float3x3)StaticMeshUniformData.InverseTransposeTransformMatrix, LocalNormal);

        float3 ViewSpacePosition = mul(ViewUniformData.ViewMatrix, float4(WorldPosition, 1.0f)).xyz;
        float4 ClipPosition = mul(ViewUniformData.ProjectionMatrix, float4(ViewSpacePosition, 1.0f));
        ClipPosition = Abytek::ClipPositionDepthReverse(ClipPosition);

        Abytek::F_SRPBasicStaticMeshVSOutput VSOutput;
        VSOutput.ClipOrPixelPosition = ClipPosition;
        VSOutput.NDCPosition = Abytek::ClipPositionToNDCPosition(ClipPosition);
        VSOutput.WorldPosition = WorldPosition;
        VSOutput.WorldNormal = WorldNormal;
        VSOutput.UV = VertexUV;
        OutVertices[GroupThreadID.x] = VSOutput;
    }

    if (GroupThreadID.x < Meshlet.Count_Triangles)
    {
        Abytek::F_ECMSMeshTriangle Triangle = Abytek::H_ECMSMesh::LoadTriangle(
            StaticMeshGeometryUniformData_ECMS.GeometryAddress,
            StaticMeshGeometryUniformData_ECMS.GeometryAllocationStructure,
            Meshlet.Offset_Triangles + GroupThreadID.x
        );

        uint3 TriangleU32X3;
        Abytek::H_ECMSMeshTriangle::Unpack(Triangle, TriangleU32X3);
        OutTriangles[GroupThreadID.x] = TriangleU32X3;

        Abytek::F_SRPBasicStaticMeshPrimitive Primitive;
        Primitive.Id = GroupThreadID.x;
        Primitive.InstancedMeshletId = InstancedMeshletId;
        Primitive.InstancedMeshlet = InstancedMeshlet;
        Primitive.LocalMeshletTriangleIndex = GroupThreadID.x;
        OutPrimitiveIds[GroupThreadID.x] = Primitive;
    }
}