#pragma once

#include "Abytek/Engine.Window.prerequisites.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_WINDOW_API F_WindowModule final : public A_ApplicationModule
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationModule)
        ABYTEK_END_REFLECTOR(F_WindowModule);
        
    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_MODULE(F_WindowModule);
        
    public:
        F_WindowModule(const F_ProgramUnitBuildParams& BuildParams);
        ~F_WindowModule() override;

    protected:
        void OnReflect() override;
        
    protected:
        void OnInit() override;
        void OnRelease() override;
    };
}
