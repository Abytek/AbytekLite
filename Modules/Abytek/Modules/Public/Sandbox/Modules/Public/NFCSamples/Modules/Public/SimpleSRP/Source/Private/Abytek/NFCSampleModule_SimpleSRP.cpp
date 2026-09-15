#include "Abytek/NFCSampleModule_SimpleSRP.hpp"
#include "Abytek/SimpleSRPSampleLevel.hpp"
#include "Abytek/SampleSpectatorComponent.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_NFCSampleModule_SimpleSRP)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_NFCSampleModule_SimpleSRP"));
    }
    
    ABYTEK_DEFINE_STATIC_APPLICATION_MODULE(F_NFCSampleModule_SimpleSRP)
    
    F_NFCSampleModule_SimpleSRP::F_NFCSampleModule_SimpleSRP(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationModule(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_MODULE();
    }
    F_NFCSampleModule_SimpleSRP::~F_NFCSampleModule_SimpleSRP()
    {
    }

    void F_NFCSampleModule_SimpleSRP::OnReflect()
    {
        RegisterStaticType<F_SimpleSRPSampleLevel>();
        RegisterStaticType<F_SampleSpectatorComponent>();
    }

    void F_NFCSampleModule_SimpleSRP::OnStartup()
    {
    }
}
