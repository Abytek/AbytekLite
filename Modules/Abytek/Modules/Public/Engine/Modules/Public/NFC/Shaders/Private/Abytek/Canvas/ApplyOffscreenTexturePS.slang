module ApplyOffscreenTexturePS;

import Abytek.Canvas.ApplyOffscreenTextureCommon;

Texture2D SrcTexture;
SamplerState LinearSampler;

[shader("fragment")]
float4 MainPS(Abytek::F_CanvasApplyOffscreenTexturePSInput PSInputput) : SV_Target0
{
    return SrcTexture.Sample(LinearSampler, PSInputput.Texcoord);
}