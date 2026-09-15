#pragma once

#include "Abytek/Sandbox.RHISamples.UploadBuffer.prerequisites.pch.hpp"


namespace Abytek
{
    class ABYTEK_SANDBOX_RHI_SAMPLES_UPLOAD_BUFFER_API F_RHISampleModule_UploadBuffer final : public A_ApplicationModule
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationModule)
        ABYTEK_END_REFLECTOR(F_RHISampleModule_UploadBuffer)
        
    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_MODULE(F_RHISampleModule_UploadBuffer);
        
    public:
        F_RHISampleModule_UploadBuffer(const F_ProgramUnitBuildParams& BuildParams);
        ~F_RHISampleModule_UploadBuffer() override;

    protected:
        void OnReflect() override;
        
    protected:
        void OnStartup() override;
    };
}
