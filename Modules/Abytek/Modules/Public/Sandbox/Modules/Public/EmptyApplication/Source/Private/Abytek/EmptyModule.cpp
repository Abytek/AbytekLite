#include "Abytek/EmptyModule.hpp"
#include "Abytek/EmptySubsystem.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_EmptyModule)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_EmptyModule"));
    }
    
    ABYTEK_DEFINE_STATIC_APPLICATION_MODULE(F_EmptyModule)
    
    F_EmptyModule::F_EmptyModule(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationModule(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_MODULE();
    }
    F_EmptyModule::~F_EmptyModule()
    {
    }

    void F_EmptyModule::OnReflect()
    {
        RegisterStaticType<F_EmptySubsystem>();
    }
}