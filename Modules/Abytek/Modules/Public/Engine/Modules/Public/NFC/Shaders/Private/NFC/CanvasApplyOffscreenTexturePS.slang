module DrawLinePS;

import NFC.CanvasApplyOffscreenTextureCommon;

Texture2D SrcTexture;
SamplerState LinearSampler;

[shader("fragment")]
float4 MainPS(CanvasApplyOffscreenTexture::F_PSInput PSInputput) : SV_Target0
{
    return SrcTexture.Sample(LinearSampler, PSInputput.Texcoord);
}