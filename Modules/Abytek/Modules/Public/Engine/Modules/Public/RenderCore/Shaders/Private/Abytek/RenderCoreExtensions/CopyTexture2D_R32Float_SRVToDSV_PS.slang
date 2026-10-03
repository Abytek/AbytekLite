module CopyTexture2D_R32Float_SRVToDSV_PS;

import Abytek.RenderCoreExtensions.CopyTexture2D_R32Float_SRVToDSV_Common;

ConstantBuffer<F_UniformData> UniformData;
Texture2D<float> SRV;

[shader("pixel")]
F_PSOutput MainPS(F_PSInput PSInput)
{
    uint2 OutputIndex2D = uint2(PSInput.VSOutput.ClipOrPixelPosition.xy);

    F_PSOutput PSOutput;
    PSOutput.Depth = SRV[OutputIndex2D];
    return PSOutput;
}