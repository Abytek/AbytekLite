#pragma once

#include "Abytek/Sandbox.NFCSamples.Texture.prerequisites.pch.hpp"


namespace Abytek
{
    class ABYTEK_SANDBOX_NFC_SAMPLES_TEXTURE_API F_NFCSampleModule_Texture final : public A_ApplicationModule
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationModule)
        ABYTEK_END_REFLECTOR(F_NFCSampleModule_Texture)
        
    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_MODULE(F_NFCSampleModule_Texture);
        
    public:
        F_NFCSampleModule_Texture(const F_ProgramUnitBuildParams& BuildParams);
        ~F_NFCSampleModule_Texture() override;

    protected:
        void OnReflect() override;
        
    protected:
        void OnStartup() override;
    };
}
