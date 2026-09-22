#pragma once

#include "Abytek/Engine.ImGui.prerequisites.hpp"
#include "Abytek/ApplicationModule.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_IMGUI_API F_ImGuiModule final : public A_ApplicationModule
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationModule)
        ABYTEK_END_REFLECTOR(F_ImGuiModule);
        
    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_MODULE(F_ImGuiModule);
        
    public:
        F_ImGuiModule(const F_ProgramUnitBuildParams& BuildParams);
        ~F_ImGuiModule() override;

    protected:
        void OnReflect() override;
    };
}
