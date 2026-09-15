#include "Abytek/SubsystemContainer.hpp"
#include "Abytek/Subsystem.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_SubsystemContainer)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_SubsystemContainer"));
    }
    
    F_SubsystemContainer::F_SubsystemContainer(const F_ProgramUnitContainerBuildParams& BuildParams) :
        F_ProgramUnitContainer(BuildParams)
    {
    }
    F_SubsystemContainer::~F_SubsystemContainer()
    {
    }

    F_FeedbackStatus F_SubsystemContainer::ValidateUnit(const TW_Valid<F_ProgramUnit>& Unit)
    {
        if (!Unit.CheckPolymorphism<F_Subsystem>())
        {
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Require units of type F_Subsystem"));
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
}
