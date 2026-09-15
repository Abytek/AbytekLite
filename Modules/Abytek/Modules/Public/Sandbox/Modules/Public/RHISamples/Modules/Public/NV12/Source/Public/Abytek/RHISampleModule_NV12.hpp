#pragma once

#include "Abytek/Sandbox.RHISamples.NV12.prerequisites.pch.hpp"


namespace Abytek
{
    class ABYTEK_SANDBOX_RHI_SAMPLES_NV12_API F_RHISampleModule_NV12 final : public A_ApplicationModule
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationModule)
        ABYTEK_END_REFLECTOR(F_RHISampleModule_NV12)
        
    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_MODULE(F_RHISampleModule_NV12);
        
    public:
        F_RHISampleModule_NV12(const F_ProgramUnitBuildParams& BuildParams);
        ~F_RHISampleModule_NV12() override;

    protected:
        void OnReflect() override;
        
    protected:
        void OnStartup() override;
    };
}
