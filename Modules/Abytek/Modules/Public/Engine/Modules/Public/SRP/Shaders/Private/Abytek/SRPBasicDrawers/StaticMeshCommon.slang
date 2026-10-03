module StaticMeshCommon;

__exported import Abytek.Renderer.RenderPrimitive.Processor;
__exported import Abytek.Renderer.GPUData.Common;

#ifdef ABYTEK_SRP_ECMS
__exported import Abytek.ECMS.SRPInstancedMeshletBuffer_ECMS;
#endif

namespace Abytek
{
    public struct F_SRPBasicStaticMeshVSInput_Simple
    {
        public uint VertexId : SV_VertexID;
    };

    public struct F_SRPBasicStaticMeshVSOutput
    {
        public float4 ClipOrPixelPosition : SV_Position;
        public float3 NDCPosition : NDC_POSITION;
        public float3 WorldPosition : POSITION;
        public float3 WorldNormal : NORMAL;
        public float2 UV : TEXCOORD;
    };
    public struct F_SRPBasicStaticMeshPrimitive
    {
        public uint Id : SV_PrimitiveID = 0xFFFFFFFF;

#ifdef ABYTEK_SRP_ECMS
        public uint InstancedMeshletId : INSTANCED_MESHLET_INDEX = 0xFFFFFFFF;
        public SRP::ECMS::F_InstancedMeshlet InstancedMeshlet : INSTANCED_MESHLET = {};
        public uint LocalMeshletTriangleIndex : LOCAL_MESHLET_TRIANGLE_INDEX = 0;
#endif
    };

    public struct F_SRPBasicStaticMeshPSInput
    {
        public F_SRPBasicStaticMeshVSOutput VertexOutput;
        public F_SRPBasicStaticMeshPrimitive Primitive;
    };
    public struct F_SRPBasicStaticMeshPSOutput
    {
#ifdef OUTPUT_RTV_DSV
        public float4 Color : SV_Target;
#endif
    };

    public struct F_SRPBasicStaticMeshUniformData
    {
        public float4x4 TransformMatrix;
        public float4x4 InverseTransposeTransformMatrix;
        public float4 Color;
    };

#ifdef OUTPUT_OPAQUE_VISIBILITY_BUFFER
    public struct F_SRPBasicStaticMeshVisibilityConfig
    {
        public F_GPUDataInstanceAddress PrimitiveAddress = INVALID_GPU_DATA_INSTANCE_ADDRESS;
        public F_RenderPrimitiveProcessorId PrimitiveProcessorId = INVALID_RENDER_PRIMITIVE_PROCESSOR_ID;
        private uint ___Padding___ = 0;
    };
#endif
}