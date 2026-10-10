module SRPVisibilityBufferPassPS_Simple;

import Abytek.Renderer.SimplePrimitive.Common;
import Abytek.UIntToColor;
import Abytek.Renderer.RenderView;
import Abytek.Renderer.ECMS.SRPTriangleCandidate;

#include "Abytek/Renderer/SRPVisibilityBuffer.slangh"

ConstantBuffer<Abytek::F_RenderViewUniformData> ViewUniformData;
ConstantBuffer<Abytek::F_SRPSimplePrimitiveVisibilityConfig> PrimitiveVisibilityConfig;

ABYTEK_SRP_DEFINE_VISIBILITY_BUFFER_OPAQUE_RW(OpaqueVisibilityBuffer);

[shader("fragment")]
Abytek::F_SRPSimplePrimitivePSOutput MainPS(Abytek::F_SRPSimplePrimitivePSInput PSInput)
{
    Abytek::F_SRPSimplePrimitivePSOutput PSOutput;

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
        TriangleCandidate,
        PrimitiveVisibilityConfig.PrimitiveProcessorId,
        Abytek::H_SRPPackedDepth::Pack(PSInput.VertexOutput.NDCPosition.z)
    );
    ABYTEK_SRP_OUTPUT_VISIBILITY_BUFFER_OPAQUE(OpaqueVisibilityBuffer, OutputIndex2D, VisibilityCandidate);
    return PSOutput;
}