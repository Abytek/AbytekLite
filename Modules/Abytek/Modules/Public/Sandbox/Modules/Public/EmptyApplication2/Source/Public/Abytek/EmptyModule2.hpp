#pragma once

#include "Abytek/Sandbox.EmptyApplication2.prerequisites.pch.hpp"


namespace Abytek
{
    class ABYTEK_SANDBOX_EMPTY_APPLICATION2_API F_EmptyModule2 final : public A_ApplicationModule
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationModule)
        ABYTEK_END_REFLECTOR(F_EmptyModule2);
        
    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_MODULE(F_EmptyModule2);
        
    public:
        F_EmptyModule2(const F_ProgramUnitBuildParams& BuildParams);
        ~F_EmptyModule2() override;

    protected:
        void OnReflect() override;
    };
}
