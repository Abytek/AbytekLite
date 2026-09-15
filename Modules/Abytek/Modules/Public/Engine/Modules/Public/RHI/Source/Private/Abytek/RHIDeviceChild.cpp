#include "Abytek/RHIDeviceChild.hpp"


namespace Abytek
{
    void A_RHIDeviceChild::Build(const TW_Valid<A_RHIDevice>& Device)
    {
        A_RAObject::BuildMinimal();

        _Device = Device;
    }
    void A_RHIDeviceChild::Release()
    {
        A_RAObject::Release();

        _Device = {};
    }
}