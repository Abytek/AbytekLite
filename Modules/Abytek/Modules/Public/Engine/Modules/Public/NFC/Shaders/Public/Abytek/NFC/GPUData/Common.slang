module Common;

namespace Abytek::NFC::GPUData
{
    public typealias F_InstanceAddress = uint2;
    public void PackInstanceAddress(out F_InstanceAddress OutInstanceAddress, uint PageIndex, uint LocalIndex)
    {
        OutInstanceAddress = uint2(
            PageIndex,
            LocalIndex
        );
    }
    public uint GetPageIndexFromInstanceAddress(F_InstanceAddress InstanceAddress)
    {
        return InstanceAddress.x;
    }
    public uint GetLocalIndexFromInstanceAddress(F_InstanceAddress InstanceAddress)
    {
        return InstanceAddress.y;
    }
    public void UnpackInstanceAddress(F_InstanceAddress InstanceAddress, out uint OutPageIndex, out uint OutLocalIndex)
    {
        OutPageIndex = GetPageIndexFromInstanceAddress(InstanceAddress);
        OutLocalIndex = GetLocalIndexFromInstanceAddress(InstanceAddress);
    }
}