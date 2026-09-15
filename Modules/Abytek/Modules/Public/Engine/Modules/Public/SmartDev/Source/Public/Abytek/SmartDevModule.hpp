#pragma once

#include "Abytek/Engine.SmartDev.prerequisites.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_SMART_DEV_API F_SmartDevModule final : public A_ApplicationModule
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationModule)
        ABYTEK_END_REFLECTOR(F_SmartDevModule)
        
    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_MODULE(F_SmartDevModule);
        
    public:
        F_SmartDevModule(const F_ProgramUnitBuildParams& BuildParams);
        ~F_SmartDevModule() override;

    protected:
        void OnReflect() override;
    };
}
