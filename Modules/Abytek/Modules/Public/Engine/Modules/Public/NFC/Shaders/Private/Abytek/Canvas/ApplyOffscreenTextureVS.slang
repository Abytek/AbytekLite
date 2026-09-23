module ApplyOffscreenTextureVS;

import Abytek.Canvas.ApplyOffscreenTextureCommon;

[shader("vertex")]
Abytek::F_CanvasApplyOffscreenTexturePSInput MainVS(Abytek::F_CanvasApplyOffscreenTextureVSInput VSInput)
{
    float4 ClipPositions[] = {
        float4(-1.0f, -1.0f, 0.0f, 1.0f),
        float4(1.0f, -1.0f, 0.0f, 1.0f),
        float4(1.0f, 1.0f, 0.0f, 1.0f),
        float4(-1.0f, -1.0f, 0.0f, 1.0f),
        float4(1.0f, 1.0f, 0.0f, 1.0f),
        float4(-1.0f, 1.0f, 0.0f, 1.0f)
    };
    float2 Texcoords[] = {
        float2(0.0f, 1.0f),
        float2(1.0f, 1.0f),
        float2(1.0f, 0.0f),
        float2(0.0f, 1.0f),
        float2(1.0f, 0.0f),
        float2(0.0f, 0.0f)
    };

    Abytek::F_CanvasApplyOffscreenTexturePSInput PSInput;
    PSInput.ClipPosition = ClipPositions[VSInput.VertexId];
    PSInput.Texcoord = Texcoords[VSInput.VertexId];
    return PSInput;
}