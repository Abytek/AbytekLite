#pragma once

#include "Abytek/Engine.Core.prerequisites.hpp"
#include "Abytek/ProgramUtilities/ProgramUnitContainer.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_CORE_API F_ModuleContainer : public F_ProgramUnitContainer
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(F_ProgramUnitContainer)
        ABYTEK_END_REFLECTOR(F_ModuleContainer);
        
    public:
        F_ModuleContainer(const F_ProgramUnitContainerBuildParams& BuildParams);
        ~F_ModuleContainer() override;
        
    protected:
        F_FeedbackStatus ValidateUnit(const TW_Valid<F_ProgramUnit>& Unit) override;
    };
}
