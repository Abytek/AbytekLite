#pragma once

#include "Abytek/Engine.IHI.prerequisites.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_IHI_API F_IHIModule final : public A_ApplicationModule
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationModule)
        ABYTEK_END_REFLECTOR(F_IHIModule);
        
    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_MODULE(F_IHIModule);
        
    public:
        F_IHIModule(const F_ProgramUnitBuildParams& BuildParams);
        ~F_IHIModule() override;

    protected:
        void OnReflect() override;
        
    protected:
        void OnInit() override;
        void OnRelease() override;
    };
}
