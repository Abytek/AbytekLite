#pragma once

#include "Abytek/Sandbox.RHISamples.ReadbackTexture.prerequisites.pch.hpp"


namespace Abytek
{
    class ABYTEK_SANDBOX_RHI_SAMPLES_READBACK_TEXTURE_API F_RHISampleModule_ReadbackTexture final : public A_ApplicationModule
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationModule)
        ABYTEK_END_REFLECTOR(F_RHISampleModule_ReadbackTexture)
        
    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_MODULE(F_RHISampleModule_ReadbackTexture);
        
    public:
        F_RHISampleModule_ReadbackTexture(const F_ProgramUnitBuildParams& BuildParams);
        ~F_RHISampleModule_ReadbackTexture() override;

    protected:
        void OnReflect() override;
        
    protected:
        void OnStartup() override;
    };
}
