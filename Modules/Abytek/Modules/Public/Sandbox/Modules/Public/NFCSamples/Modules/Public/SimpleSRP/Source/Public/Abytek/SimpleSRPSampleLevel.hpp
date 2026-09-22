#pragma once

#include "Abytek/Sandbox.NFCSamples.SimpleSRP.prerequisites.pch.hpp"
#include "Abytek/Level/Level.hpp"


namespace Abytek
{
    class ABYTEK_SANDBOX_NFC_SAMPLES_SIMPLE_SRP_API F_SimpleSRPSampleLevel final : public F_Level
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(F_Level)
        ABYTEK_END_REFLECTOR(F_SimpleSRPSampleLevel);
        
    public:
        F_SimpleSRPSampleLevel(const F_SerializableObjectInitParams& InitParams);
        ~F_SimpleSRPSampleLevel() override;
        
    protected:
        void OnLoadContent() override;
    };
}
