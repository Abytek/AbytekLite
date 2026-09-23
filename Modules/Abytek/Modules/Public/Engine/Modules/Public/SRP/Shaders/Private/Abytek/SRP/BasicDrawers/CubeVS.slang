module CubeVS;

import Abytek.SRP.BasicDrawers.CubeCommon;
import Abytek.NFC.RenderView;

ConstantBuffer<Abytek::NFC::RenderView::F_UniformData> ViewUniformData;
ConstantBuffer<Abytek::SRP::BasicDrawers::Cube::F_UniformData> CubeUniformData;

[shader("vertex")]
Abytek::SRP::BasicDrawers::Cube::F_PSInput MainVS(Abytek::SRP::BasicDrawers::Cube::F_VSInput VSInput)
{
    // 36 vertices = 6 faces * 2 triangles * 3 vertices
    static const float3 LocalPositions[] =
    {
        // Front
        float3(-1.0f, -1.0f, 1.0f),
        float3(1.0f, -1.0f, 1.0f),
        float3(1.0f, 1.0f, 1.0f),

        float3(-1.0f, -1.0f, 1.0f),
        float3(1.0f, 1.0f, 1.0f),
        float3(-1.0f, 1.0f, 1.0f),

        // Back
        float3(1.0f, -1.0f, -1.0f),
        float3(-1.0f, -1.0f, -1.0f),
        float3(-1.0f, 1.0f, -1.0f),

        float3(1.0f, -1.0f, -1.0f),
        float3(-1.0f, 1.0f, -1.0f),
        float3(1.0f, 1.0f, -1.0f),

        // Left
        float3(-1.0f, -1.0f, -1.0f),
        float3(-1.0f, -1.0f, 1.0f),
        float3(-1.0f, 1.0f, 1.0f),

        float3(-1.0f, -1.0f, -1.0f),
        float3(-1.0f, 1.0f, 1.0f),
        float3(-1.0f, 1.0f, -1.0f),

        // Right
        float3(1.0f, -1.0f, 1.0f),
        float3(1.0f, -1.0f, -1.0f),
        float3(1.0f, 1.0f, -1.0f),

        float3(1.0f, -1.0f, 1.0f),
        float3(1.0f, 1.0f, -1.0f),
        float3(1.0f, 1.0f, 1.0f),

        // Top
        float3(-1.0f, 1.0f, 1.0f),
        float3(1.0f, 1.0f, 1.0f),
        float3(1.0f, 1.0f, -1.0f),

        float3(-1.0f, 1.0f, 1.0f),
        float3(1.0f, 1.0f, -1.0f),
        float3(-1.0f, 1.0f, -1.0f),

        // Bottom
        float3(-1.0f, -1.0f, -1.0f),
        float3(1.0f, -1.0f, -1.0f),
        float3(1.0f, -1.0f, 1.0f),

        float3(-1.0f, -1.0f, -1.0f),
        float3(1.0f, -1.0f, 1.0f),
        float3(-1.0f, -1.0f, 1.0f),
    };

    Abytek::SRP::BasicDrawers::Cube::F_PSInput PSInput;

    float3 LocalPosition = LocalPositions[VSInput.VertexId];
    float3 WorldPosition = mul(CubeUniformData.TransformMatrix, float4(LocalPosition, 1.0f)).xyz;
    float3 ViewSpacePosition = mul(ViewUniformData.ViewMatrix, float4(WorldPosition, 1.0f)).xyz;
    float4 ClipPosition = mul(ViewUniformData.ProjectionMatrix, float4(ViewSpacePosition, 1.0f));

    PSInput.ClipPosition = ClipPosition;
    return PSInput;
}