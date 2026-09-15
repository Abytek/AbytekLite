#include "Abytek/VMMModule.hpp"
#include "Abytek/VMMSubsystem.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_VMMModule)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_VMMModule"));
    }
    
    ABYTEK_DEFINE_STATIC_APPLICATION_MODULE(F_VMMModule);
    
    F_VMMModule::F_VMMModule(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationModule(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_MODULE();
        
        AddDependency<F_CoreModule>();
    }
    F_VMMModule::~F_VMMModule()
    {
    }

    void F_VMMModule::OnReflect()
    {
        RegisterStaticType<F_VMMSubsystem>();
    }
}
