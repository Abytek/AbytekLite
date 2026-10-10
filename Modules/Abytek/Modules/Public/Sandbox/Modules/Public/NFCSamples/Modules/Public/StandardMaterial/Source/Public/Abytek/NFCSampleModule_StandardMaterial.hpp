#pragma once

#include "Abytek/Sandbox.NFCSamples.StandardMaterial.prerequisites.pch.hpp"


namespace Abytek
{
    class ABYTEK_SANDBOX_NFC_SAMPLES_STANDARD_MATERIAL_API F_NFCSampleModule_StandardMaterial final : public A_ApplicationModule
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationModule)
        ABYTEK_END_REFLECTOR(F_NFCSampleModule_StandardMaterial);
        
    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_MODULE(F_NFCSampleModule_StandardMaterial);
        
    public:
        F_NFCSampleModule_StandardMaterial(const F_ProgramUnitBuildParams& BuildParams);
        ~F_NFCSampleModule_StandardMaterial() override;

    protected:
        void OnReflect() override;
        
    protected:
        void OnStartup() override;
    };
}
