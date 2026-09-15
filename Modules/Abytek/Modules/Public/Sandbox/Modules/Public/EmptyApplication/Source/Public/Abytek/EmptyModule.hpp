#pragma once

#include "Abytek/Sandbox.EmptyApplication.prerequisites.pch.hpp"


namespace Abytek
{
    class ABYTEK_SANDBOX_EMPTY_APPLICATION_API F_EmptyModule final : public A_ApplicationModule
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationModule)
        ABYTEK_END_REFLECTOR(F_EmptyModule)
        
    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_MODULE(F_EmptyModule);
        
    public:
        F_EmptyModule(const F_ProgramUnitBuildParams& BuildParams);
        ~F_EmptyModule() override;

    protected:
        void OnReflect() override;
    };
}
