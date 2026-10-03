module CopyTexture2D_R32Float_SRVToDSV_VS;

import Abytek.RenderCoreExtensions.CopyTexture2D_R32Float_SRVToDSV_Common;
import Abytek.ClipPositionToNDCPosition;

[shader("vertex")]
F_VSOutput MainVS(uint VertexId : SV_VertexID)
{
    static float4 ClipPositions[] = {
        float4(-1.0f, -1.0f, 1.0f, 1.0f),
        float4(-1.0f, 1.0f, 1.0f, 1.0f),
        float4(1.0f, 1.0f, 1.0f, 1.0f),
        float4(-1.0f, -1.0f, 1.0f, 1.0f),
        float4(1.0f, 1.0f, 1.0f, 1.0f),
        float4(1.0f, -1.0f, 1.0f, 1.0f)
    };
    float4 ClipPosition = ClipPositions[VertexId];

    F_VSOutput VSOutput;
    VSOutput.ClipOrPixelPosition = ClipPosition;
    return VSOutput;
}