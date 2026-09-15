module DrawLinePS;

import MathRenderer2D.DrawLine;

cbuffer UniformData
{
    MathRenderer2D::F_DrawLineUniformData DrawLineUniformData;
};

[shader("fragment")]
float4 MainPS(MathRenderer2D::F_PSInput iPSInputput) : SV_Target0
{
    MathRenderer2D::F_Line Line = DrawLineUniformData.Line;
    return float4(Line.Color, 1);
}