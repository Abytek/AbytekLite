module SRPVisibilityBufferPassMS_Simple;

import Abytek.SimplePrimitive.Common;
import Abytek.Renderer.RenderView;
import Abytek.Renderer.RenderGeometry.Common;
import Abytek.Renderer.RenderGeometry.DeclarePages;
import Abytek.Renderer.RenderGeometry.Access;
import Abytek.Renderer.RenderPrimitive.Components;
import Abytek.SimplePrimitive.SRPMaterial_Simple;
import Abytek.ClipPositionDepthReverse;

#include "Abytek/ECMS/SRPInstancedMeshletBuffer_ECMS.slangh"
#include "Abytek/Renderer/GPUData/Macros.slangh"

#include "Abytek/SimplePrimitive/MATERIAL_Simple.slangh"

ConstantBuffer<Abytek::F_RenderViewUniformData> ViewUniformData;
ConstantBuffer<Abytek::F_SRPSimplePrimitiveVisibilityConfig> PrimitiveVisibilityConfig;

ABYTEK_GPU_DATA_DEFINE_INSTANCE_SET_HEADER_BUFFER(
    RenderPrimitive_Simple
);

ABYTEK_GPU_DATA_DEFINE_COMPONENT_TYPE_RO(
    RenderPrimitive_Simple,
    Transform,
    Abytek::RenderPrimitive::F_Component_Transform
);
ABYTEK_GPU_DATA_DEFINE_COMPONENT_TYPE_RO(
    RenderPrimitive_Simple,
    InverseTransposeTransform,
    Abytek::RenderPrimitive::F_Component_InverseTransposeTransform
);
ABYTEK_GPU_DATA_DEFINE_COMPONENT_TYPE_RO(
    RenderPrimitive_Simple,
    GeometryAddress_ECMS,
    Abytek::RenderPrimitive::F_Component_GeometryAddress_ECMS
);

ABYTEK_SRP_ECMS_DEFINE_INSTANCED_MESHLET_BUFFER_RW(InstancedMeshlets);

groupshared bool GroupCulled;
groupshared Abytek::SRP::ECMS::F_InstancedMeshlet InstancedMeshlet;
groupshared Abytek::SRP::ECMS::F_InstancedMeshletId InstancedMeshletId;

groupshared float4x4 TransformMatrix;
groupshared float4x4 InverseTransposeTransformMatrix;
groupshared Abytek::F_RenderGeometryAddress GeometryAddress;
groupshared Abytek::F_RenderGeometryAllocationStructure_ECMS GeometryAllocationStructure;

[shader("mesh")]
[numthreads(NUM_THREADS, 1, 1)]
[outputtopology("triangle")]
void MainMS(
    uint3 GroupThreadID: SV_GroupThreadID,
    uint3 GroupID: SV_GroupID,

    out vertices Abytek::F_SRPSimplePrimitiveVSOutput OutVertices[ABYTEK_ECMS_MAX_VERTICES_PER_MESHLET],
    out indices uint3 OutTriangles[ABYTEK_ECMS_MAX_PRIMITIVES_PER_MESHLET],
    out primitives Abytek::F_SRPSimplePrimitive OutPrimitiveIds[ABYTEK_ECMS_MAX_PRIMITIVES_PER_MESHLET]
)
{
    if (GroupThreadID.x == 0)
    {
        GroupCulled = false;
    }

    if (GroupThreadID.x == 0)
    {
        InstancedMeshlet = {};
        InstancedMeshlet.PrimitiveAddress = PrimitiveVisibilityConfig.PrimitiveAddress;
        InstancedMeshlet.MeshletIndex = GroupID.x;
        InstancedMeshletId = ABYTEK_SRP_ECMS_PUSH_INSTANCED_MESHLET(InstancedMeshlets, InstancedMeshlet);
        GroupCulled |= (InstancedMeshletId == Abytek::SRP::ECMS::INVALID_INSTANCED_MESHLET_ID);
    }

    if (GroupThreadID.x == 0)
    {
        typealias H_GPUDataComponent_Transform = ABYTEK_GPU_DATA_GET_COMPONENT_TYPE_STRUCT(
            RenderPrimitive_Simple,
            Transform
        );
        typealias H_GPUDataComponent_InverseTransposeTransform = ABYTEK_GPU_DATA_GET_COMPONENT_TYPE_STRUCT(
            RenderPrimitive_Simple,
            InverseTransposeTransform
        );
        typealias H_GPUDataComponent_GeometryAddress = ABYTEK_GPU_DATA_GET_COMPONENT_TYPE_STRUCT(
            RenderPrimitive_Simple,
            GeometryAddress_ECMS
        );

        H_GPUDataComponent_Transform::F_Value Component_Transform = H_GPUDataComponent_Transform::Load(PrimitiveVisibilityConfig.PrimitiveAddress);
        H_GPUDataComponent_InverseTransposeTransform::F_Value Component_InverseTransposeTransform = H_GPUDataComponent_InverseTransposeTransform::Load(PrimitiveVisibilityConfig.PrimitiveAddress);
        H_GPUDataComponent_GeometryAddress::F_Value Component_GeometryAddress = H_GPUDataComponent_GeometryAddress::Load(PrimitiveVisibilityConfig.PrimitiveAddress);

        TransformMatrix = Component_Transform.Matrix;
        InverseTransposeTransformMatrix = Component_InverseTransposeTransform.Matrix;
        GeometryAddress = Component_GeometryAddress.Address;
        
        GeometryAllocationStructure = Abytek::H_ECMSMesh::LoadGeometryAllocationStructure(
            GeometryAddress
        );
    }

    GroupMemoryBarrierWithGroupSync();

    Abytek::F_ECMSMeshlet Meshlet = Abytek::H_ECMSMesh::LoadMeshlet(
        GeometryAddress,
        GeometryAllocationStructure,
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
            GeometryAddress,
            GeometryAllocationStructure,
            Meshlet.Offset_VertexIndices + GroupThreadID.x
        );

        float3 VertexPosition = Abytek::H_ECMSMesh::LoadVertexPosition(
            GeometryAddress,
            GeometryAllocationStructure,
            VertexIndex
        );
        float3 VertexNormal = Abytek::H_ECMSMesh::LoadVertexNormal(
            GeometryAddress,
            GeometryAllocationStructure,
            VertexIndex
        );
        float2 VertexUV = Abytek::H_ECMSMesh::LoadVertexUV(
            GeometryAddress,
            GeometryAllocationStructure,
            VertexIndex
        );

        float3 LocalPosition = VertexPosition;
        float3 LocalNormal = VertexNormal;

        Abytek::SRP::SimplePrimitive::F_MaterialDataVS MaterialDataVS;
        MaterialVS(MaterialDataVS);

        float3 WorldPosition = mul(TransformMatrix, float4(LocalPosition, 1.0f)).xyz;
        float3 WorldNormal = mul((float3x3)InverseTransposeTransformMatrix, LocalNormal);

        float3 ViewSpacePosition = mul(ViewUniformData.ViewMatrix, float4(WorldPosition, 1.0f)).xyz;
        float4 ClipPosition = mul(ViewUniformData.ProjectionMatrix, float4(ViewSpacePosition, 1.0f));
        ClipPosition = Abytek::ClipPositionDepthReverse(ClipPosition);

        Abytek::F_SRPSimplePrimitiveVSOutput VSOutput;
        VSOutput.ClipOrPixelPosition = ClipPosition;
        VSOutput.NDCPosition = (ClipPosition / ClipPosition.w).xyz;
        VSOutput.WorldPosition = WorldPosition;
        VSOutput.WorldNormal = WorldNormal;
        VSOutput.UV = VertexUV;
        OutVertices[GroupThreadID.x] = VSOutput;
    }

    if (GroupThreadID.x < Meshlet.Count_Triangles)
    {
        Abytek::F_ECMSMeshTriangle Triangle = Abytek::H_ECMSMesh::LoadTriangle(
            GeometryAddress,
            GeometryAllocationStructure,
            Meshlet.Offset_Triangles + GroupThreadID.x
        );

        uint3 TriangleU32X3;
        Abytek::H_ECMSMeshTriangle::Unpack(Triangle, TriangleU32X3);
        OutTriangles[GroupThreadID.x] = TriangleU32X3;

        Abytek::F_SRPSimplePrimitive Primitive;
        Primitive.Id = GroupThreadID.x;
        Primitive.InstancedMeshletId = InstancedMeshletId;
        Primitive.InstancedMeshlet = InstancedMeshlet;
        Primitive.LocalMeshletTriangleIndex = GroupThreadID.x;
        OutPrimitiveIds[GroupThreadID.x] = Primitive;
    }
}