module RenderView;

namespace Abytek::NFC::RenderView
{
    public struct F_UniformData
    {
        public float4x4 ViewMatrix;
        public float4x4 ProjectionMatrix;
        public float2 Resolution;
        public float AspectRatio;
        private float ___Padding___;
    };
}