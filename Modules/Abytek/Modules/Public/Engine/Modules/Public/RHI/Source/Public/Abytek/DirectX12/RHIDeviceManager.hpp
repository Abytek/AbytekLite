#pragma once

#include "Abytek/RHIDeviceManager.hpp"


#ifdef ABYTEK_PLATFORM_WINDOWS
namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIDeviceManager : public A_RHIDeviceManager
    {
    private:

    public:

    public:
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHIDeviceManager);
        virtual void Build() override;
        virtual void Release() override;

    private:
        void _CreateDevices();
    };
}
#endif