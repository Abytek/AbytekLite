module SRPInstancedMeshletBuffer_ECMS;

__exported import Abytek.Renderer.GPUData.Common;

namespace Abytek
{
    namespace SRP::ECMS
    {
        public typealias F_InstancedMeshletId = uint;
        public static F_InstancedMeshletId INVALID_INSTANCED_MESHLET_ID = ~F_InstancedMeshletId(0);

        public struct F_InstancedMeshlet
        {
            public F_GPUDataInstanceAddress PrimitiveAddress = INVALID_GPU_DATA_INSTANCE_ADDRESS;
            public uint MeshletIndex = ~uint(0);
            private uint ___Padding___ = 0;
        };
    }
}