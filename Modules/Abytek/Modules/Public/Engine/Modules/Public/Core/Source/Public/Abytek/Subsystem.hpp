#pragma once

#include "Abytek/Engine.Core.prerequisites.hpp"
#include "Abytek/ProgramUtilities/ProgramUnit.hpp"
#include "Abytek/ProgramUtilities/ProgramUnitContainer.hpp"


#define ABYTEK_DECLARE_STATIC_SUBSYSTEM(...) ABYTEK_DECLARE_STATIC_PROGRAM_UNIT(__VA_ARGS__)
#define ABYTEK_DEFINE_STATIC_SUBSYSTEM(...) ABYTEK_DEFINE_STATIC_PROGRAM_UNIT(Abytek::ToText(Abytek::TypeFullName<__VA_ARGS__>()), __VA_ARGS__)


namespace Abytek
{
    class ABYTEK_ENGINE_CORE_API F_Subsystem : public F_ProgramUnit
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(F_ProgramUnit)
        ABYTEK_END_REFLECTOR(F_Subsystem);
        
    public:
        F_Subsystem(const F_ProgramUnitBuildParams& BuildParams);
        ~F_Subsystem() override;
    };
}
