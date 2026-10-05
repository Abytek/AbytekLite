module SRPColorPass_Simple;

import Abytek.SimplePrimitive.Common;
import Abytek.Renderer.RenderView;
import Abytek.Renderer.RenderGeometry.Common;
import Abytek.Renderer.RenderGeometry.DeclarePages;
import Abytek.Renderer.RenderGeometry.Access;
import Abytek.Renderer.RenderPrimitive.Components;
import Abytek.ECMS.SRPTriangleCandidate_ECMS;
import Abytek.ECMS.SRPRenderGeometryAccess_ECMS;
import Abytek.ClipPositionDepthReverse;
import Abytek.ClipPositionToNDCPosition;
import Abytek.PixelPosition2DToNDCPosition2D;
import Abytek.Barycentric;

#include "Abytek/ECMS/SRPInstancedMeshletBuffer_ECMS.slangh"
#include "Abytek/Renderer/GPUData/Macros.slangh"
#include "Abytek/SRPVisibilityBuffer.slangh"

ConstantBuffer<Abytek::F_RenderViewUniformData> ViewUniformData;
ConstantBuffer<Abytek::F_SRPSimplePrimitiveVisibilityConfig> PrimitiveVisibilityConfig;

RWTexture2D<float4> ColorTexture;
RWTexture2D<float> DepthTexture;

ABYTEK_SRP_DEFINE_VISIBILITY_BUFFER_OPAQUE_RO(OpaqueVisibilityBuffer);

ABYTEK_SRP_ECMS_DEFINE_INSTANCED_MESHLET_BUFFER_RO(InstancedMeshlets);

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

public struct F_Params_CalculateVertexNDCPosition
{
    public float4x4 TransformMatrix;
    public float3 VertexPosition;
};
public float3 CalculateVertexNDCPosition(F_Params_CalculateVertexNDCPosition Params)
{
    float3 WorldPosition = mul(Params.TransformMatrix, float4(Params.VertexPosition, 1.0f)).xyz;

    float3 ViewSpacePosition = mul(ViewUniformData.ViewMatrix, float4(WorldPosition, 1.0f)).xyz;
    float4 ClipPosition = mul(ViewUniformData.ProjectionMatrix, float4(ViewSpacePosition, 1.0f));
    ClipPosition = Abytek::ClipPositionDepthReverse(ClipPosition);

    return Abytek::ClipPositionToNDCPosition(ClipPosition);
}

public struct F_Params_CalculateVertexData_NDCPosition
{
    public float4x4 TransformMatrix;
    public Abytek::SRP::ECMS::TF_TriangleVertexData<float3> VertexData_Position;
};
public Abytek::SRP::ECMS::TF_TriangleVertexData<float3> CalculateVertexData_NDCPosition(F_Params_CalculateVertexData_NDCPosition Params)
{
    Abytek::SRP::ECMS::TF_TriangleVertexData<float3> Result;
    {
        F_Params_CalculateVertexNDCPosition VParams;
        VParams.TransformMatrix = Params.TransformMatrix;
        VParams.VertexPosition = Params.VertexData_Position.Vertex0;
        Result.Vertex0 = CalculateVertexNDCPosition(VParams);
    }
    {
        F_Params_CalculateVertexNDCPosition VParams;
        VParams.TransformMatrix = Params.TransformMatrix;
        VParams.VertexPosition = Params.VertexData_Position.Vertex1;
        Result.Vertex1 = CalculateVertexNDCPosition(VParams);
    }
    {
        F_Params_CalculateVertexNDCPosition VParams;
        VParams.TransformMatrix = Params.TransformMatrix;
        VParams.VertexPosition = Params.VertexData_Position.Vertex2;
        Result.Vertex2 = CalculateVertexNDCPosition(VParams);
    }
    return Result;
}

[numthreads(NUM_THREADS_X, NUM_THREADS_Y, NUM_THREADS_Z)]
[shader("compute")]
void MainCS(uint3 DispatchThreadId: SV_DispatchThreadID)
{
    DepthTexture[DispatchThreadId.xy] = 0.0f;

    if (
        (DispatchThreadId.x >= ViewUniformData.Resolution.x)
        || (DispatchThreadId.y >= ViewUniformData.Resolution.y)
    )
    {
        return;
    }

    Abytek::F_SRPVisibilityCandidate VisibilityCandidate = ABYTEK_SRP_LOAD_VISIBILITY_BUFFER_OPAQUE(OpaqueVisibilityBuffer, DispatchThreadId.xy);
    if (VisibilityCandidate == Abytek::INVALID_SRP_VISIBILITY_CANDIDATE)
    {
        return;
    }

    Abytek::F_SRPTriangleCandidate TriangleCandidate;
    Abytek::F_RenderPrimitiveProcessorId PrimitiveProcessorId;
    Abytek::F_SRPPackedDepth PackedDepth;
    Abytek::H_SRPVisibilityCandidate::Unpack(
        VisibilityCandidate,
        TriangleCandidate,
        PrimitiveProcessorId,
        PackedDepth
    );

    if (PrimitiveProcessorId != PrimitiveVisibilityConfig.PrimitiveProcessorId)
    {
        return;
    }

    float Depth = Abytek::H_SRPPackedDepth::Unpack(PackedDepth);

    Abytek::SRP::ECMS::F_InstancedMeshletId InstancedMeshletId;
    uint LocalMeshletTriangleIndex;
    Abytek::SRP::ECMS::H_TriangleCandidate::Unpack(
        TriangleCandidate,
        InstancedMeshletId,
        LocalMeshletTriangleIndex
    );

    Abytek::SRP::ECMS::F_InstancedMeshlet InstancedMeshlet = ABYTEK_SRP_ECMS_LOAD_INSTANCED_MESHLET(InstancedMeshlets, InstancedMeshletId);
    
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

    float4x4 TransformMatrix = H_GPUDataComponent_Transform::Load(
        InstancedMeshlet.PrimitiveAddress
    ).Matrix;
    float4x4 InverseTransposeTransformMatrix = H_GPUDataComponent_InverseTransposeTransform::Load(
        InstancedMeshlet.PrimitiveAddress
    ).Matrix;
    Abytek::F_RenderGeometryAddress GeometryAddress = H_GPUDataComponent_GeometryAddress::Load(
        InstancedMeshlet.PrimitiveAddress
    ).Address;

    Abytek::F_RenderGeometryAllocationStructure_ECMS GeometryAllocationStructure = Abytek::H_ECMSMesh::LoadGeometryAllocationStructure(
        GeometryAddress
    );

    Abytek::SRP::ECMS::F_DirectTriangle DirectTriangle = Abytek::SRP::ECMS::LoadDirectTriangle(
        GeometryAddress,
        GeometryAllocationStructure,
        InstancedMeshlet.MeshletIndex,
        LocalMeshletTriangleIndex
    );

    Abytek::SRP::ECMS::TF_TriangleVertexData<float3> TriangleVertexData_Position = Abytek::SRP::ECMS::LoadTriangleVertexData_Position(
        GeometryAddress,
        GeometryAllocationStructure,
        DirectTriangle
    );
    Abytek::SRP::ECMS::TF_TriangleVertexData<float3> TriangleVertexData_Normal = Abytek::SRP::ECMS::LoadTriangleVertexData_Normal(
        GeometryAddress,
        GeometryAllocationStructure,
        DirectTriangle
    );

    Abytek::SRP::ECMS::TF_TriangleVertexData<float3> TriangleVertexData_NDCPosition;
    {
        F_Params_CalculateVertexData_NDCPosition Params;
        Params.TransformMatrix = TransformMatrix;
        Params.VertexData_Position = TriangleVertexData_Position;
        TriangleVertexData_NDCPosition = CalculateVertexData_NDCPosition(Params);
    }

    float2 NDCPosition2D = Abytek::PixelPosition2DToNDCPosition2D(DispatchThreadId.xy, ViewUniformData.Resolution);
    float3 BarycentricWeights = Abytek::H_Barycentric::CalculateWeights(
        NDCPosition2D,
        TriangleVertexData_NDCPosition.Vertex0.xy,
        TriangleVertexData_NDCPosition.Vertex1.xy,
        TriangleVertexData_NDCPosition.Vertex2.xy
    );

    float3 VertexPosition = Abytek::H_Barycentric::Interpolate(
        BarycentricWeights,
        TriangleVertexData_Position.Vertex0,
        TriangleVertexData_Position.Vertex1,
        TriangleVertexData_Position.Vertex2
    );
    float3 VertexNormal = Abytek::H_Barycentric::Interpolate(
        BarycentricWeights,
        TriangleVertexData_Normal.Vertex0,
        TriangleVertexData_Normal.Vertex1,
        TriangleVertexData_Normal.Vertex2
    );

    float3 LocalPosition = VertexPosition;
    float3 LocalNormal = VertexNormal;

    float3 WorldPosition = mul(TransformMatrix, float4(LocalPosition, 1.0f)).xyz;
    float3 WorldNormal = mul((float3x3)InverseTransposeTransformMatrix, LocalNormal);

    float3 Lighting = lerp(
        float3(0.3f, 0.3f, 0.3f),
        float3(0.84f, 0.84f, 0.84f),
        float3(
            max(
                dot(WorldNormal, -normalize(float3(-0.3f, -1.0f, 0.2f))),
                0.0f
            )
        )
    );
    float3 OutRadiance = Lighting;

    ColorTexture[DispatchThreadId.xy] = float4(OutRadiance, 1.0f);
    DepthTexture[DispatchThreadId.xy] = Depth;
}