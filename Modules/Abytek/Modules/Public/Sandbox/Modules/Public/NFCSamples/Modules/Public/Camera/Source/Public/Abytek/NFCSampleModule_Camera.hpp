#pragma once

#include "Abytek/Sandbox.NFCSamples.Camera.prerequisites.pch.hpp"


namespace Abytek
{
    class ABYTEK_SANDBOX_NFC_SAMPLES_CAMERA_API F_NFCSampleModule_Camera final : public A_ApplicationModule
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationModule)
        ABYTEK_END_REFLECTOR(F_NFCSampleModule_Camera)
        
    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_MODULE(F_NFCSampleModule_Camera);
        
    public:
        F_NFCSampleModule_Camera(const F_ProgramUnitBuildParams& BuildParams);
        ~F_NFCSampleModule_Camera() override;

    protected:
        void OnReflect() override;
        
    protected:
        void OnStartup() override;
    };
}
