#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"


namespace Abytek
{
    class A_RHIDevice;

    namespace RHIDeviceEvents
    {
        using F_OnConnected = TF_Event<void(const TW_Valid<A_RHIDevice>& Device)>;
        using F_OnDisconnected = TF_Event<void(const TW_Valid<A_RHIDevice>& Device)>;
        
        using F_OnRequestConnect = TF_Event<void(const TW_Valid<A_RHIDevice>& Device, B8& OutApprove)>;
    };
}