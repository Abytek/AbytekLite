#pragma once

#include "Abytek/Sandbox.MLPDemo.prerequisites.pch.hpp"
#include "Abytek/ApplicationModule.hpp"


namespace Abytek
{
    class F_MLPDemoModule final : public A_ApplicationModule
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationModule)
        ABYTEK_END_REFLECTOR(F_MLPDemoModule)
        
    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_MODULE(F_MLPDemoModule);
        
    public:
        F_MLPDemoModule(const F_ProgramUnitBuildParams& BuildParams);
        ~F_MLPDemoModule() override;

    protected:
        void OnReflect() override;
    };
}
