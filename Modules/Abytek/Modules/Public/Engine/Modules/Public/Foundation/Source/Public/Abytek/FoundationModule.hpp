#pragma once

#include "Abytek/Engine.Foundation.prerequisites.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_FOUNDATION_API F_FoundationModule final : public A_ApplicationModule
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationModule)
        ABYTEK_END_REFLECTOR(F_FoundationModule);
        
    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_MODULE(F_FoundationModule);
        
    public:
        F_FoundationModule(const F_ProgramUnitBuildParams& BuildParams);
        ~F_FoundationModule() override;

    protected:
        void OnReflect() override;
    };
}
