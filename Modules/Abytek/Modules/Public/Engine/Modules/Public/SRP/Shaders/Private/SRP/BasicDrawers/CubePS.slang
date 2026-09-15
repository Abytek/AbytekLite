module CubePS;

import SRP.BasicDrawers.CubeCommon;

ConstantBuffer<SRP::BasicDrawers::Cube::F_UniformData> CubeUniformData;

[shader("fragment")]
float4 MainPS(SRP::BasicDrawers::Cube::F_PSInput PSInputput) : SV_Target0
{
    return CubeUniformData.Color;
}