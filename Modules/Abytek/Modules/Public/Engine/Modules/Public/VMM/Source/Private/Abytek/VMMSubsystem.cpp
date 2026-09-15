#include "Abytek/VMMSubsystem.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_VMMSubsystem)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_VMMSubsystem"));
    }

    ABYTEK_DEFINE_STATIC_APPLICATION_SUBSYSTEM(F_VMMSubsystem);

    F_VMMSubsystem::F_VMMSubsystem(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationSubsystem(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_SUBSYSTEM();
    }
    F_VMMSubsystem::~F_VMMSubsystem()
    {
    }
}
