#pragma once

#include "Abytek/Engine.Core.prerequisites.hpp"
#include "Abytek/ProgramUtilities/ProgramUnitContainer.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_CORE_API F_SubsystemContainer : public F_ProgramUnitContainer
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(F_ProgramUnitContainer)
        ABYTEK_END_REFLECTOR(F_SubsystemContainer);
        
    public:
        F_SubsystemContainer(const F_ProgramUnitContainerBuildParams& BuildParams);
        ~F_SubsystemContainer() override;
        
    protected:
        F_FeedbackStatus ValidateUnit(const TW_Valid<F_ProgramUnit>& Unit) override;
    };
}
