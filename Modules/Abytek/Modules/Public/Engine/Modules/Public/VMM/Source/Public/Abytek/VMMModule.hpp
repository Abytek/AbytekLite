#pragma once

#include "Abytek/Engine.VMM.prerequisites.hpp"
#include "Abytek/ApplicationModule.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_VMM_API F_VMMModule final : public A_ApplicationModule
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationModule)
        ABYTEK_END_REFLECTOR(F_VMMModule);
        
    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_MODULE(F_VMMModule);
        
    public:
        F_VMMModule(const F_ProgramUnitBuildParams& BuildParams);
        ~F_VMMModule() override;

    protected:
        void OnReflect() override;
    };
}
