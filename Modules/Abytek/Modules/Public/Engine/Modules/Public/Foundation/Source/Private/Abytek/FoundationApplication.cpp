#include "Abytek/FoundationApplication.hpp"
#include "Abytek/FoundationModule.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_Application)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_Application"));
    }
    
    F_Application::F_Application(const F_ApplicationBuildParams& BuildParams) :
        A_ApplicationCore(BuildParams)
    {
    }
    F_Application::~F_Application()
    {
    }

    void F_Application::RegisterModules()
    {
        A_ApplicationCore::RegisterModules();
        RegisterModule<F_FoundationModule>();
    }
}
