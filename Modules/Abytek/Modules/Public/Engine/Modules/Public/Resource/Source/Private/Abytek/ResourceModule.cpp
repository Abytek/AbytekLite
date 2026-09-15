#include "Abytek/ResourceModule.hpp"
#include "Abytek/RootResource.hpp"
#include "Abytek/Resource.hpp"
#include "Abytek/ResourceSubsystem.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_ResourceModule)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_ResourceModule"));
    }
    
    ABYTEK_DEFINE_STATIC_APPLICATION_MODULE(F_ResourceModule);
    
    F_ResourceModule::F_ResourceModule(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationModule(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_MODULE();
        
        AddDependency<F_CoreModule>();
    }
    F_ResourceModule::~F_ResourceModule()
    {
    }

    void F_ResourceModule::OnReflect()
    {
        RegisterStaticType<F_ResourceSubsystem>();
        RegisterStaticType<F_ResourceManager>();
        RegisterStaticType<F_Resource>();
        RegisterStaticType<F_RootResource>();
    }
}
