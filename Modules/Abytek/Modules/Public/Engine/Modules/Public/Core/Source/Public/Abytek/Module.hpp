#pragma once

#include "Abytek/Engine.Core.prerequisites.hpp"
#include "Abytek/ProgramUtilities/ProgramUnit.hpp"
#include "Abytek/ProgramUtilities/ProgramUnitContainer.hpp"
#include "Abytek/Subsystem.hpp"


#define ABYTEK_DECLARE_STATIC_MODULE(...) ABYTEK_DECLARE_STATIC_PROGRAM_UNIT(__VA_ARGS__)
#define ABYTEK_DEFINE_STATIC_MODULE(...) ABYTEK_DEFINE_STATIC_PROGRAM_UNIT(ABYTEK_MODULE_NAME, __VA_ARGS__)


namespace Abytek
{
    class F_Subsystem;
    
    class ABYTEK_ENGINE_CORE_API F_Module : public F_ProgramUnit
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(F_ProgramUnit)
        ABYTEK_END_REFLECTOR(F_Module)
        
    private:
        
    public:
        
    public:
        F_Module(const F_ProgramUnitBuildParams& BuildParams);
        ~F_Module() override;
    };
}
