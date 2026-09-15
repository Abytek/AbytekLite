#include "Abytek/Development/Cook/CookSettingContainer.hpp"
#include "Abytek/Development/Cook/CookProfile.hpp"


#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
namespace Abytek
{
    ABYTEK_REFLECT(F_CookSettingContainer)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_CookSettingContainer"));
    }
    
    TW<F_CookSettingContainer> F_CookSettingContainer::GetMain()
    {
        if (auto Profile = F_CookProfile::GetMain())
        {
            return Profile->GetSettingContainer();
        }
        return {};
    }

    F_CookSettingContainer::F_CookSettingContainer(const F_ProgramUnitContainerBuildParams& BuildParams) :
        F_ProgramUnitContainer(BuildParams)
    {
    }
    F_CookSettingContainer::~F_CookSettingContainer()
    {
    }

    F_FeedbackStatus F_CookSettingContainer::ValidateUnit(const TW_Valid<F_ProgramUnit>& Unit)
    {
        ABYTEK_FEEDBACK_STATUS_CHECK(F_ProgramUnitContainer::ValidateUnit(Unit));
        if (!Unit.CheckPolymorphism<A_CookSetting>())
        {
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Require units of type A_CookSetting"));
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
}
#endif