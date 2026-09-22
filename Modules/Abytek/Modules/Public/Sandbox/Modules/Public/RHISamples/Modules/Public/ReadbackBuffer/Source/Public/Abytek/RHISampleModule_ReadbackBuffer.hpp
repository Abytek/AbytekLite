#pragma once

#include "Abytek/Sandbox.RHISamples.ReadbackBuffer.prerequisites.pch.hpp"


namespace Abytek
{
    class ABYTEK_SANDBOX_RHI_SAMPLES_READBACK_BUFFER_API F_RHISampleModule_ReadbackBuffer final : public A_ApplicationModule
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationModule)
        ABYTEK_END_REFLECTOR(F_RHISampleModule_ReadbackBuffer);
        
    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_MODULE(F_RHISampleModule_ReadbackBuffer);
        
    public:
        F_RHISampleModule_ReadbackBuffer(const F_ProgramUnitBuildParams& BuildParams);
        ~F_RHISampleModule_ReadbackBuffer() override;

    protected:
        void OnReflect() override;
        
    protected:
        void OnStartup() override;
    };
}
