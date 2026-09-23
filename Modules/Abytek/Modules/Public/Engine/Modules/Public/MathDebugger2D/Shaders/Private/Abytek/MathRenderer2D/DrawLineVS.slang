module DrawLineVS;

import Abytek.MathRenderer2D.DrawLine;

cbuffer UniformData
{
    Abytek::MathRenderer2D::F_DrawLineUniformData DrawLineUniformData;
};

[shader("vertex")]
Abytek::MathRenderer2D::F_PSInput MainVS(Abytek::MathRenderer2D::F_VSInput VSInput)
{
    float2 RecPositions[] = {
        float2(0, 0),
        float2(1, 0),
        float2(1, 1),
        float2(0, 0),
        float2(1, 1),
        float2(0, 1)
    };

    float2 RecPosition = RecPositions[VSInput.VertexId];

    float2 LineBeginPosition = DrawLineUniformData.Line.BeginPosition;
    float2 LineEndPosition = DrawLineUniformData.Line.EndPosition;
    float2 LineTangent = normalize(LineEndPosition - LineBeginPosition);
    float2 LineNormal = float2(-LineTangent.y, LineTangent.x);

    float2 ScaledLineTangent = DrawLineUniformData.Line.Thickness * LineTangent * 0.5f * DrawLineUniformData.View.Scale;
    float2 ScaledLineNormal = DrawLineUniformData.Line.Thickness * LineNormal * 0.5f * DrawLineUniformData.View.Scale;

    float2 LineBeginPosition1 = (
        LineBeginPosition
        - ScaledLineTangent
        - ScaledLineNormal
    );
    float2 LineBeginPosition2 = (
        LineBeginPosition
        - ScaledLineTangent
        + ScaledLineNormal
    );
    float2 LineEndPosition1 = (
        LineEndPosition
        + ScaledLineTangent
        - ScaledLineNormal
    );
    float2 LineEndPosition2 = (
        LineEndPosition
        + ScaledLineTangent
        + ScaledLineNormal
    );

    float4 LocalPosition = float4(
        lerp(
            lerp(
                LineBeginPosition1,
                LineBeginPosition2,
                RecPosition.x
            ),
            lerp(
                LineEndPosition1,
                LineEndPosition2,
                RecPosition.x
            ),
            RecPosition.y
        ),
        0.5f,
        1.0f
    );
    float4 WorldPosition = mul(DrawLineUniformData.Space.Transform, LocalPosition);

    Abytek::MathRenderer2D::F_PSInput PSInput;
    PSInput.ClipPosition = mul(DrawLineUniformData.View.InvTransform, WorldPosition);
    return PSInput;
}