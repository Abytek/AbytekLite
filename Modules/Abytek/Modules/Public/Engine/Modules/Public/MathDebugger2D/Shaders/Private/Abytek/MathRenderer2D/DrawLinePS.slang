module DrawLinePS;

import Abytek.MathRenderer2D.DrawLine;

cbuffer UniformData
{
    Abytek::MathRenderer2D::F_DrawLineUniformData DrawLineUniformData;
};

[shader("fragment")]
float4 MainPS(Abytek::MathRenderer2D::F_PSInput iPSInputput) : SV_Target0
{
    Abytek::MathRenderer2D::F_Line Line = DrawLineUniformData.Line;
    return float4(Line.Color, 1);
}