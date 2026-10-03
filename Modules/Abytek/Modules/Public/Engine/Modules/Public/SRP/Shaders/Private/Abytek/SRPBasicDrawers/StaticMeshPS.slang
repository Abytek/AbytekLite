module StaticMeshPS_Simple;

import Abytek.SRPBasicDrawers.StaticMeshCommon;
import Abytek.UIntToColor;
import Abytek.Renderer.RenderView;

#ifdef OUTPUT_OPAQUE_VISIBILITY_BUFFER
#include "Abytek/SRPVisibilityBuffer.slangh"
#ifdef ABYTEK_SRP_ECMS
import Abytek.ECMS.SRPTriangleCandidate_ECMS;
#endif
#endif

ConstantBuffer<Abytek::F_RenderViewUniformData> ViewUniformData;
ConstantBuffer<Abytek::F_SRPBasicStaticMeshUniformData> StaticMeshUniformData;

#ifdef OUTPUT_OPAQUE_VISIBILITY_BUFFER
ABYTEK_SRP_DEFINE_VISIBILITY_BUFFER_OPAQUE_RW(OpaqueVisibilityBuffer);
ConstantBuffer<Abytek::F_SRPBasicStaticMeshVisibilityConfig> StaticMeshVisibilityConfig;
#endif

[shader("fragment")]
Abytek::F_SRPBasicStaticMeshPSOutput MainPS(Abytek::F_SRPBasicStaticMeshPSInput PSInput)
{
    float3 Lighting = lerp(
        float3(0.3f, 0.3f, 0.3f),
        float3(0.84f, 0.84f, 0.84f),
        float3(
            max(
                dot(PSInput.VertexOutput.WorldNormal, -normalize(float3(-0.3f, -1.0f, 0.2f))),
                0.0f
            )
        )
    );
    float3 OutRadiance = StaticMeshUniformData.Color.xyz * Lighting;

    Abytek::F_SRPBasicStaticMeshPSOutput PSOutput;
#ifdef OUTPUT_RTV_DSV
    PSOutput.Color = float4(OutRadiance, 1.0f);
#endif

#ifdef ABYTEK_SRP_ECMS
#ifdef OUTPUT_OPAQUE_VISIBILITY_BUFFER
    uint2 OutputIndex2D = uint2(PSInput.VertexOutput.ClipOrPixelPosition.xy);
    Abytek::F_SRPTriangleCandidate TriangleCandidate = Abytek::INVALID_SRP_TRIANGLE_CANDIDATE;
    Abytek::SRP::ECMS::H_TriangleCandidate::Pack(
        TriangleCandidate,
        PSInput.Primitive.InstancedMeshletId,
        PSInput.Primitive.LocalMeshletTriangleIndex
    );
    Abytek::F_SRPVisibilityCandidate VisibilityCandidate = Abytek::INVALID_SRP_VISIBILITY_CANDIDATE;
    Abytek::H_SRPVisibilityCandidate::Pack(
        VisibilityCandidate,
        Abytek::H_SRPPackedDepth::Pack(PSInput.VertexOutput.NDCPosition.z),
        StaticMeshVisibilityConfig.PrimitiveProcessorId,
        TriangleCandidate
    );
    ABYTEK_SRP_OUTPUT_VISIBILITY_BUFFER_OPAQUE(OpaqueVisibilityBuffer, OutputIndex2D, VisibilityCandidate);
#endif
#endif

    return PSOutput;
}