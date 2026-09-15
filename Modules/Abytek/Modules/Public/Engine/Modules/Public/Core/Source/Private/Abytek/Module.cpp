#include "Abytek/Module.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_Module)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_Module"));
    }
    
    F_Module::F_Module(const F_ProgramUnitBuildParams& BuildParams) :
        F_ProgramUnit(BuildParams)
    {
    }
    F_Module::~F_Module()
    {
    }
}
