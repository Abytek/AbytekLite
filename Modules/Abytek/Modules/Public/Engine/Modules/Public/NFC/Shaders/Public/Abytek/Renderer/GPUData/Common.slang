module Common;

namespace Abytek
{
    public typealias F_GPUDataInstanceAddress = uint2;
    public static F_GPUDataInstanceAddress INVALID_GPU_DATA_INSTANCE_ADDRESS = uint2(0xFFFFFFFF, 0xFFFFFFFF);
    public struct H_GPUDataInstanceAddress
    {
        public static void Pack(out F_GPUDataInstanceAddress OutInstanceAddress, uint PageIndex, uint LocalIndex)
        {
            OutInstanceAddress = uint2(
                PageIndex,
                LocalIndex
            );
        }
        public static uint GetPageIndex(F_GPUDataInstanceAddress InstanceAddress)
        {
            return InstanceAddress.x;
        }
        public static uint GetLocalIndex(F_GPUDataInstanceAddress InstanceAddress)
        {
            return InstanceAddress.y;
        }
        public static void Unpack(F_GPUDataInstanceAddress InstanceAddress, out uint OutPageIndex, out uint OutLocalIndex)
        {
            OutPageIndex = GetPageIndex(InstanceAddress);
            OutLocalIndex = GetLocalIndex(InstanceAddress);
        }
    }

    public struct F_GPUDataInstanceSetHeader
    {
        public F_GPUDataInstanceAddress Address;
        public uint Num = 0;
        private uint ___Padding___ = 0;
    }

    public struct F_InstanceSetHeadersUniformData
    {
        public uint Num = 0;
        private uint3 ___Padding___ = 0;
    };
}