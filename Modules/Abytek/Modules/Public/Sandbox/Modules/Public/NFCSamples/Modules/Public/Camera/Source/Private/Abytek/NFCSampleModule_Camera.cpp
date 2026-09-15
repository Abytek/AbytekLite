#include "Abytek/NFCSampleModule_Camera.hpp"
#include "Abytek/CameraSampleLevel.hpp"
#include "Abytek/SampleSpectatorComponent.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_NFCSampleModule_Camera)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_NFCSampleModule_Camera"));
    }
    
    ABYTEK_DEFINE_STATIC_APPLICATION_MODULE(F_NFCSampleModule_Camera)
    
    F_NFCSampleModule_Camera::F_NFCSampleModule_Camera(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationModule(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_MODULE();
    }
    F_NFCSampleModule_Camera::~F_NFCSampleModule_Camera()
    {
    }

    void F_NFCSampleModule_Camera::OnReflect()
    {
        RegisterStaticType<F_CameraSampleLevel>();
        RegisterStaticType<F_SampleSpectatorComponent>();
    }

    void F_NFCSampleModule_Camera::OnStartup()
    {
    }
}
