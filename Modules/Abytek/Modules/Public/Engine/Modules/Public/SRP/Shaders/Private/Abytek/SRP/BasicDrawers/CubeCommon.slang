module CubeCommon;

namespace Abytek
{
    public struct F_SRPBasicCubeVSInput
    {
        public uint VertexId : SV_VertexID;
    };

    public struct F_SRPBasicCubePSInput
    {
        public float4 ClipPosition : SV_Position;
    };

    public struct F_SRPBasicCubeUniformData
    {
        public float4x4 TransformMatrix;
        public float4 Color;
    };
}