#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"
#include "Abytek/RHIFeature.hpp"
#include "Abytek/RHIDeviceEvents.hpp"
#include "Abytek/RHIDeviceInfo.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API A_RHIDevice : public A_RAObject
    {
    public:
        friend class A_RHIDeviceManager;
        
    private:
        B8 _IsActivated = false;
        F_RHIDeviceInfo _Info;
        F_RHIFeatureSet _FeatureSet;
        
    public:
        ABYTEK_FORCE_INLINE auto IsActivated() const noexcept
        {
            return _IsActivated;
        }
        ABYTEK_FORCE_INLINE const auto& GetInfo() const noexcept
        {
            return _Info;
        }
        ABYTEK_FORCE_INLINE auto& InjectInfo() noexcept
        {
            return _Info;
        }
        ABYTEK_FORCE_INLINE const auto& GetFeatureSet() const noexcept
        {
            return _FeatureSet;
        }

    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIDevice);
        virtual void Build();
        virtual void Release() override;

    public:
        virtual void Activate();
        virtual void Deactivate();

    protected:
        virtual F_RHIFeatureSet GenerateFeatureSet() { return {}; }
    };
}
