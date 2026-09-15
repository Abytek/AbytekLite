module StaticMeshCommon;

namespace SRP::BasicDrawers::StaticMesh
{
    public struct F_VSInput
    {
        public uint VertexId : SV_VertexID;
    };

    public struct F_PSInput
    {
        public float4 ClipPosition : SV_Position;
        public float3 WorldPosition : POSITION;
        public float3 WorldNormal : NORMAL;
        public float2 UV : TEXCOORD;
        public uint TriangleId : TRIANGLE_ID;
    };

    public struct F_UniformData
    {
        public float4x4 TransformMatrix;
        public float4x4 InverseTransposeTransformMatrix;
        public float4 Color;
    };
}