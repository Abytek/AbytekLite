#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"


namespace Abytek
{
    class A_RHIDevice;
    
    class ABYTEK_ENGINE_RHI_API A_RHIDeviceChild : public A_RAObject
    {
    private:
        TW<A_RHIDevice> _Device;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetDevice() const noexcept
        {
            return _Device;
        }
        ABYTEK_FORCE_INLINE auto& InjectDevice() noexcept
        {
            return _Device;
        }

    public:
        ABYTEK_RA_DECLARE_OBJECT(A_RHIDeviceChild);
        virtual void Release() override;

    protected:
        virtual void Build(const TW_Valid<A_RHIDevice>& Device);
    };
}
