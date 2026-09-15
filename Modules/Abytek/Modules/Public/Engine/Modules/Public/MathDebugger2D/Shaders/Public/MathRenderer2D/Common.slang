module Common;

namespace MathRenderer2D
{
    public struct F_Line
    {
        public float2 BeginPosition;
        public float2 EndPosition;
        public float3 Color;
        public float Thickness;
    };

    public struct F_SpaceUniformData
    {
        public float4x4 Transform;
        public float4x4 InvTransform;
    };
    public struct F_ViewUniformData
    {
        public float4x4 Transform;
        public float4x4 InvTransform;
        public float2 FrameBufferSize;
        public float AspectRatio;
        public float Scale;
    };
}