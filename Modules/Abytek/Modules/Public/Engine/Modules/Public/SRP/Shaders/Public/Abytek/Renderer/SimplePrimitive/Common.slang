module StaticMeshCommon;

__exported import Abytek.Renderer.RenderPrimitive.Processor;
__exported import Abytek.Renderer.GPUData.Common;

__exported import Abytek.Renderer.ECMS.SRPInstancedMeshletBuffer;

namespace Abytek
{
    public struct F_SRPSimplePrimitiveVSOutput
    {
        public float4 ClipOrPixelPosition : SV_Position;
        public float3 NDCPosition : NDC_POSITION;
        public float3 WorldPosition : POSITION;
        public float3 WorldNormal : NORMAL;
        public float2 UV : TEXCOORD;
    };
    public struct F_SRPSimplePrimitive
    {
        public uint Id : SV_PrimitiveID = 0xFFFFFFFF;

        public uint InstancedMeshletId : INSTANCED_MESHLET_INDEX = 0xFFFFFFFF;
        public SRP::ECMS::F_InstancedMeshlet InstancedMeshlet : INSTANCED_MESHLET = {};
        public uint LocalMeshletTriangleIndex : LOCAL_MESHLET_TRIANGLE_INDEX = 0;
    };

    public struct F_SRPSimplePrimitivePSInput
    {
        public F_SRPSimplePrimitiveVSOutput VertexOutput;
        public F_SRPSimplePrimitive Primitive;
    };
    public struct F_SRPSimplePrimitivePSOutput
    {
    };

    public struct F_SRPSimplePrimitiveVisibilityConfig
    {
        public F_GPUDataInstanceAddress PrimitiveAddress = INVALID_GPU_DATA_INSTANCE_ADDRESS;
        public F_RenderPrimitiveProcessorId PrimitiveProcessorId = INVALID_RENDER_PRIMITIVE_PROCESSOR_ID;
        private uint ___Padding___ = 0;
    };
}