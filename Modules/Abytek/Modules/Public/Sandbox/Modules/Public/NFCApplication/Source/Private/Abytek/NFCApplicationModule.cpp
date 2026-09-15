#include "Abytek/NFCApplicationModule.hpp"
#include "Abytek/DemoLevel.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_NFCApplicationModule)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_NFCApplicationModule"));
    }
    
    ABYTEK_DEFINE_STATIC_APPLICATION_MODULE(F_NFCApplicationModule)
    
    F_NFCApplicationModule::F_NFCApplicationModule(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationModule(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_MODULE();
    }
    F_NFCApplicationModule::~F_NFCApplicationModule()
    {
    }

    void F_NFCApplicationModule::OnReflect()
    {
        RegisterStaticType<F_DemoLevel>();
    }
}