#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"
#include "Abytek/RHIDeviceEvents.hpp"


namespace Abytek
{
    class A_RHIDevice;
    
    class ABYTEK_ENGINE_RHI_API A_RHIDeviceManager : public A_RAObject
    {
    public:
        ABYTEK_DECLARE_OBJECT_SINGLETON_CRT(A_RHIDeviceManager);
        
    private:
        TW<A_RHIDevice> _MainDevice;
        TF_UniqueObjectSet<A_RHIDevice> _Devices;
        
        RHIDeviceEvents::F_OnConnected _OnConnectedEvent;
        RHIDeviceEvents::F_OnDisconnected _OnDisconnectedEvent;
        
        RHIDeviceEvents::F_OnRequestConnect _OnRequestConnectEvent;

    public:
        ABYTEK_FORCE_INLINE const auto& GetDevices() const noexcept
        {
            return _Devices;
        }
        ABYTEK_FORCE_INLINE auto& InjectDevices() noexcept
        {
            return _Devices;
        }
        ABYTEK_FORCE_INLINE auto GetMainDevice() const noexcept
        {
            return _MainDevice;
        }
        
        ABYTEK_FORCE_INLINE auto& GetOnConnectedEvent() noexcept
        {
            return _OnConnectedEvent;
        }
        ABYTEK_FORCE_INLINE auto& GetOnDisconnectedEvent() noexcept
        {
            return _OnDisconnectedEvent;
        }
        
        ABYTEK_FORCE_INLINE auto& GetOnRequestConnectEvent() noexcept
        {
            return _OnRequestConnectEvent;
        }

    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIDeviceManager);
        virtual void Build();
        virtual void Release() override;

    protected:
        TW_Valid<A_RHIDevice> CreateDevice(
            TF_Function<void(const TW_Valid<A_RHIDevice>& Device)>&& InitFunction
        );
        void DestroyDevice(const TW_Valid<A_RHIDevice>& Device);
    };
}
