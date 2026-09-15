#pragma once

#include "Abytek/Sandbox.NFCSamples.Canvas.prerequisites.pch.hpp"


namespace Abytek
{
    class ABYTEK_SANDBOX_NFC_SAMPLES_CANVAS_API F_NFCSampleModule_Canvas final : public A_ApplicationModule
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationModule)
        ABYTEK_END_REFLECTOR(F_NFCSampleModule_Canvas)
        
    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_MODULE(F_NFCSampleModule_Canvas);
        
    public:
        F_NFCSampleModule_Canvas(const F_ProgramUnitBuildParams& BuildParams);
        ~F_NFCSampleModule_Canvas() override;

    protected:
        void OnReflect() override;
        
    protected:
        void OnStartup() override;
    };
}
