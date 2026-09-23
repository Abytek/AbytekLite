module Common;

namespace Abytek
{
    public typealias F_GPUDataInstanceAddress = uint2;
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
}