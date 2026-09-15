module CubeCommon;

namespace SRP::BasicDrawers::Cube
{
    public struct F_VSInput
    {
        public uint VertexId : SV_VertexID;
    };

    public struct F_PSInput
    {
        public float4 ClipPosition : SV_Position;
    };

    public struct F_UniformData
    {
        public float4x4 TransformMatrix;
        public float4 Color;
    };
}