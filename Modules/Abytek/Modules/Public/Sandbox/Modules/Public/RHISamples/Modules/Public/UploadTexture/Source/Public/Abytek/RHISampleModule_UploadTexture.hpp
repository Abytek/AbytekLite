#pragma once

#include "Abytek/Sandbox.RHISamples.UploadTexture.prerequisites.pch.hpp"


namespace Abytek
{
    class ABYTEK_SANDBOX_RHI_SAMPLES_UPLOAD_TEXTURE_API F_RHISampleModule_UploadTexture final : public A_ApplicationModule
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationModule)
        ABYTEK_END_REFLECTOR(F_RHISampleModule_UploadTexture);
        
    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_MODULE(F_RHISampleModule_UploadTexture);
        
    public:
        F_RHISampleModule_UploadTexture(const F_ProgramUnitBuildParams& BuildParams);
        ~F_RHISampleModule_UploadTexture() override;

    protected:
        void OnReflect() override;
        
    protected:
        void OnStartup() override;
    };
}
