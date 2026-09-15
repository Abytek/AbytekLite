#include "Abytek/MFCModule.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_MFCModule)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_MFCModule"));
    }
    
    ABYTEK_DEFINE_STATIC_APPLICATION_MODULE(F_MFCModule);
    
    F_MFCModule::F_MFCModule(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationModule(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_MODULE();
        
        AddDependency<F_CoreModule>();
        AddDependency<F_AssetsModule>();
        AddDependency<F_ResourceModule>();
        AddDependency<F_IHIModule>();
        AddDependency<F_WindowModule>();
        AddDependency<F_RHIModule>();
        AddDependency<F_RenderCoreModule>();
        AddDependency<F_MathDebugger2DModule>();
        AddDependency<F_ImGuiModule>();
        AddDependency<F_SmartDevModule>();
        AddDependency<F_VMMModule>();
    }
    F_MFCModule::~F_MFCModule()
    {
    }
}
