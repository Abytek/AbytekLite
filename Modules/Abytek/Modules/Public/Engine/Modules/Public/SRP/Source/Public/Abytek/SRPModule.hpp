#pragma once

#include "Abytek/Engine.SRP.prerequisites.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_SRP_API F_SRPModule final : public A_ApplicationModule
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationModule)
        ABYTEK_END_REFLECTOR(F_SRPModule)
        
    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_MODULE(F_SRPModule);
        
    public:
        F_SRPModule(const F_ProgramUnitBuildParams& BuildParams);
        ~F_SRPModule() override;

    protected:
        void OnReflect() override;
    };
}
