module CopyTexture2D_R32Float_SRVToDSV_Common;

public struct F_UniformData
{
    public uint2 Resolution;
};
public struct F_VSOutput
{
    public float4 ClipOrPixelPosition : SV_Position;
};

public struct F_PSInput
{
    public F_VSOutput VSOutput = {};
};
public struct F_PSOutput
{
    public float Depth : SV_Depth;
};