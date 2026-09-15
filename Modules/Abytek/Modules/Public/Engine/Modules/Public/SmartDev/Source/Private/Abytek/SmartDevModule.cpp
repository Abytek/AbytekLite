#include "Abytek/SmartDevModule.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_SmartDevModule)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_SmartDevModule"));
    }
    
    ABYTEK_DEFINE_STATIC_APPLICATION_MODULE(F_SmartDevModule);
    
    F_SmartDevModule::F_SmartDevModule(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationModule(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_MODULE();
        
        AddDependency<F_CoreModule>();
    }
    F_SmartDevModule::~F_SmartDevModule()
    {
    }

    void F_SmartDevModule::OnReflect()
    {
    }
}
