#include "Abytek/ModuleContainer.hpp"
#include "Abytek/Module.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_ModuleContainer)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_ModuleContainer"));
    }
    
    F_ModuleContainer::F_ModuleContainer(const F_ProgramUnitContainerBuildParams& BuildParams) :
        F_ProgramUnitContainer(BuildParams)
    {
    }
    F_ModuleContainer::~F_ModuleContainer()
    {
    }

    F_FeedbackStatus F_ModuleContainer::ValidateUnit(const TW_Valid<F_ProgramUnit>& Unit)
    {
        if (!Unit.CheckPolymorphism<F_Module>())
        {
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Require units of type F_Module"));
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
}
