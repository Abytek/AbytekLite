#pragma once

#include "Abytek/Engine.Resource.prerequisites.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_RESOURCE_API F_ResourceModule final : public A_ApplicationModule
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationModule)
        ABYTEK_END_REFLECTOR(F_ResourceModule);
        
    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_MODULE(F_ResourceModule);
        
    public:
        F_ResourceModule(const F_ProgramUnitBuildParams& BuildParams);
        ~F_ResourceModule() override;

    protected:
        void OnReflect() override;
    };
}
