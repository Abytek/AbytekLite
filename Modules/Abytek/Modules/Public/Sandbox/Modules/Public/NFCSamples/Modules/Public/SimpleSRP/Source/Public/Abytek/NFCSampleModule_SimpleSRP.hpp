#pragma once

#include "Abytek/Sandbox.NFCSamples.SimpleSRP.prerequisites.pch.hpp"


namespace Abytek
{
    class ABYTEK_SANDBOX_NFC_SAMPLES_SIMPLE_SRP_API F_NFCSampleModule_SimpleSRP final : public A_ApplicationModule
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationModule)
        ABYTEK_END_REFLECTOR(F_NFCSampleModule_SimpleSRP);
        
    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_MODULE(F_NFCSampleModule_SimpleSRP);
        
    public:
        F_NFCSampleModule_SimpleSRP(const F_ProgramUnitBuildParams& BuildParams);
        ~F_NFCSampleModule_SimpleSRP() override;

    protected:
        void OnReflect() override;
        
    protected:
        void OnStartup() override;
    };
}
