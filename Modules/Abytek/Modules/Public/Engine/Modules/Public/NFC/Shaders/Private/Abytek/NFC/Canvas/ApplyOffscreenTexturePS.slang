module ApplyOffscreenTexturePS;

import Abytek.NFC.Canvas.ApplyOffscreenTextureCommon;

Texture2D SrcTexture;
SamplerState LinearSampler;

[shader("fragment")]
float4 MainPS(Abytek::NFC::Canvas::ApplyOffscreenTexture::F_PSInput PSInputput) : SV_Target0
{
    return SrcTexture.Sample(LinearSampler, PSInputput.Texcoord);
}