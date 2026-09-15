#include "Abytek/FoundationModule.hpp"
#include "Abytek/FoundationApplication.hpp"
#include "Abytek/FoundationMinimalApplication.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_FoundationModule)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_FoundationModule"));
    }
    
    ABYTEK_DEFINE_STATIC_APPLICATION_MODULE(F_FoundationModule);
    
    F_FoundationModule::F_FoundationModule(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationModule(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_MODULE();
        
        AddDependency<F_MFCModule>();
        if (A_ApplicationCore::GetInstance().CheckPolymorphism<F_Application>())
        {
            AddDependency<F_NFCModule>();
            AddDependency<F_SRPModule>();
        }
    }
    F_FoundationModule::~F_FoundationModule()
    {
    }

    void F_FoundationModule::OnReflect()
    {
        RegisterStaticType<F_Application>();
        RegisterStaticType<F_MinimalApplication>();
    }
}
