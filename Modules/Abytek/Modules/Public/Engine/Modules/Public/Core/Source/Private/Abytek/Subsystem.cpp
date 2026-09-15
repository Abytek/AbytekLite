#include "Abytek/Subsystem.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_Subsystem)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_Subsystem"));
    }
    
    F_Subsystem::F_Subsystem(const F_ProgramUnitBuildParams& BuildParams) :
        F_ProgramUnit(BuildParams)
    {
    }
    F_Subsystem::~F_Subsystem()
    {
    }
}
