module ApplyOffscreenTextureVS;

import Abytek.NFC.Canvas.ApplyOffscreenTextureCommon;

[shader("vertex")]
Abytek::NFC::Canvas::ApplyOffscreenTexture::F_PSInput MainVS(Abytek::NFC::Canvas::ApplyOffscreenTexture::F_VSInput VSInput)
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

    Abytek::NFC::Canvas::ApplyOffscreenTexture::F_PSInput PSInput;
    PSInput.ClipPosition = ClipPositions[VSInput.VertexId];
    PSInput.Texcoord = Texcoords[VSInput.VertexId];
    return PSInput;
}