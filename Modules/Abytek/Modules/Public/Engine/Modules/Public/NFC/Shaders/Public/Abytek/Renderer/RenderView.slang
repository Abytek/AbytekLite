module RenderView;

namespace Abytek
{
    public struct F_RenderViewUniformData
    {
        public float4x4 ViewMatrix;
        public float4x4 ProjectionMatrix;
        public uint2 Resolution;
        public float AspectRatio;
        private float ___Padding___;
    };
}