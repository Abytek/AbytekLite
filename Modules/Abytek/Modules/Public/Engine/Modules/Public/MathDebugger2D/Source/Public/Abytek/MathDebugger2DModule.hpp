#pragma once

#include "Abytek/Engine.MathDebugger2D.prerequisites.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_MATH_DEBUGGER_2D_API F_MathDebugger2DModule final : public A_ApplicationModule
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationModule)
        ABYTEK_END_REFLECTOR(F_MathDebugger2DModule);
        
    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_MODULE(F_MathDebugger2DModule);
        
    public:
        F_MathDebugger2DModule(const F_ProgramUnitBuildParams& BuildParams);
        ~F_MathDebugger2DModule() override;

    protected:
        void OnReflect() override;
    };
}
