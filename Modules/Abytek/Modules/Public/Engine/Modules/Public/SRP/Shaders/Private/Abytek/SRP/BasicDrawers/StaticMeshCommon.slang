module StaticMeshCommon;

namespace Abytek
{
    public struct F_SRPBasicStaticMeshVSInput
    {
        public uint VertexId : SV_VertexID;
    };

    public struct F_SRPBasicStaticMeshPSInput
    {
        public float4 ClipPosition : SV_Position;
        public float3 WorldPosition : POSITION;
        public float3 WorldNormal : NORMAL;
        public float2 UV : TEXCOORD;
        public uint TriangleId : TRIANGLE_ID;
    };

    public struct F_SRPBasicStaticMeshUniformData
    {
        public float4x4 TransformMatrix;
        public float4x4 InverseTransposeTransformMatrix;
        public float4 Color;
    };
}