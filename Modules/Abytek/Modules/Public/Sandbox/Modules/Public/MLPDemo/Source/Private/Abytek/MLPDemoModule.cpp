#include "Abytek/MLPDemoModule.hpp"
#include "Abytek/MLPDemoApplication.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_MLPDemoModule)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_MLPDemoModule"));
    }
    
    ABYTEK_DEFINE_STATIC_APPLICATION_MODULE(F_MLPDemoModule)
    
    F_MLPDemoModule::F_MLPDemoModule(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationModule(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_MODULE();
    }
    F_MLPDemoModule::~F_MLPDemoModule()
    {
    }

    void F_MLPDemoModule::OnReflect()
    {
        RegisterStaticType<F_MLPDemoApplication>();
    }
}