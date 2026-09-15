#pragma once

#include "Abytek/Engine.Assets.prerequisites.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_ASSETS_API F_AssetsModule final : public A_ApplicationModule
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationModule)
        ABYTEK_END_REFLECTOR(F_AssetsModule)
        
    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_MODULE(F_AssetsModule);
        
    public:
        F_AssetsModule(const F_ProgramUnitBuildParams& BuildParams);
        ~F_AssetsModule() override;

    protected:
        void OnReflect() override;
    };
}
