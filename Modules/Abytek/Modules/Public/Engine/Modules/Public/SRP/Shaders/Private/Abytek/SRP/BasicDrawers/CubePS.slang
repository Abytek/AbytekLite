module CubePS;

import Abytek.SRP.BasicDrawers.CubeCommon;

ConstantBuffer<Abytek::SRP::BasicDrawers::Cube::F_UniformData> CubeUniformData;

[shader("fragment")]
float4 MainPS(Abytek::SRP::BasicDrawers::Cube::F_PSInput PSInputput) : SV_Target0
{
    return CubeUniformData.Color;
}