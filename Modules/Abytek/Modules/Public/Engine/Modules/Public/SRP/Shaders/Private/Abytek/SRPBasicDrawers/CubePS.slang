module CubePS;

import Abytek.SRPBasicDrawers.CubeCommon;

ConstantBuffer<Abytek::F_SRPBasicCubeUniformData> CubeUniformData;

[shader("fragment")]
float4 MainPS(Abytek::F_SRPBasicCubePSInput PSInputput) : SV_Target0
{
    return CubeUniformData.Color;
}