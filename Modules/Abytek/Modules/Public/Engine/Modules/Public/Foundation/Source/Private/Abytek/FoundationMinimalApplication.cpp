#include "Abytek/FoundationMinimalApplication.hpp"
#include "Abytek/FoundationModule.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_MinimalApplication)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_MinimalApplication"));
    }
    
    F_MinimalApplication::F_MinimalApplication(const F_ApplicationBuildParams& BuildParams) :
        A_ApplicationCore(BuildParams)
    {
    }
    F_MinimalApplication::~F_MinimalApplication()
    {
    }

    void F_MinimalApplication::RegisterModules()
    {
        A_ApplicationCore::RegisterModules();
        RegisterModule<F_FoundationModule>();
    }
}
