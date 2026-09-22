#pragma once

#include "Abytek/Sandbox.NFCApplication.prerequisites.pch.hpp"


namespace Abytek
{
    class F_NFCApplicationModule final : public A_ApplicationModule
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationModule)
        ABYTEK_END_REFLECTOR(F_NFCApplicationModule);
        
    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_MODULE(F_NFCApplicationModule);
        
    public:
        F_NFCApplicationModule(const F_ProgramUnitBuildParams& BuildParams);
        ~F_NFCApplicationModule() override;

    protected:
        void OnReflect() override;
    };
}
