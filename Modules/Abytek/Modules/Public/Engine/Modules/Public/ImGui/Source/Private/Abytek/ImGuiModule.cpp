#include "Abytek/ImGuiModule.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_ImGuiModule)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_ImGuiModule"));
    }
    
    ABYTEK_DEFINE_STATIC_APPLICATION_MODULE(F_ImGuiModule);
    
    F_ImGuiModule::F_ImGuiModule(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationModule(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_MODULE();
        
        AddDependency<F_CoreModule>();
        AddDependency<F_RHIModule>();
        AddDependency<F_RenderCoreModule>();
    }
    F_ImGuiModule::~F_ImGuiModule()
    {
    }

    void F_ImGuiModule::OnReflect()
    {
    }
}
