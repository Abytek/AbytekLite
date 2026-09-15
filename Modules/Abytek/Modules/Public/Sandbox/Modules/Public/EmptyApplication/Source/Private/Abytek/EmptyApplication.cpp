#include "Abytek/EmptyApplication.hpp"
#include "Abytek/EmptyModule.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_EmptyApplication)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_EmptyApplication"));
    }
    
    F_EmptyApplication::F_EmptyApplication(const F_ApplicationBuildParams& BuildParams) :
        F_Application(BuildParams)
    {
    }
    F_EmptyApplication::~F_EmptyApplication()
    {
    }

    void F_EmptyApplication::Init()
    {
        F_Application::Init();
        RegisterModule<F_EmptyModule>();
        SignalShutdown();
    }
}
