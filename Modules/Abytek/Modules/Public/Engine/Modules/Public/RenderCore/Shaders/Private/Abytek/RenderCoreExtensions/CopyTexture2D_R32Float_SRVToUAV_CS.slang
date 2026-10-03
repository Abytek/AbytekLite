module CopyTexture2D_R32Float_SRVToUAV_CS;

public struct F_UniformData
{
    public uint2 Resolution;
};
ConstantBuffer<F_UniformData> UniformData;

Texture2D<float> SRV;
RWTexture2D<float> UAV;

[numthreads(8, 8, 1)]
[shader("compute")]
void MainCS(uint3 DispatchThreadId: SV_DispatchThreadID)
{
    if (
        (DispatchThreadId.x >= UniformData.Resolution.x)
        || (DispatchThreadId.y >= UniformData.Resolution.y)
    )
    {
        return;
    }

    UAV[DispatchThreadId.xy] = SRV[DispatchThreadId.xy];
}