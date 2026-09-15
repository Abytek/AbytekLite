#include "Abytek/ApplicationSubsystemContainer.hpp"
#include "Abytek/ApplicationSubsystem.hpp"
#include "Abytek/ApplicationCore.hpp"
#include "Abytek/Frame/FrameHelper.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_ApplicationSubsystemContainer)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_ApplicationSubsystemContainer"));
    }
    
    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(F_ApplicationSubsystemContainer);
    
    F_ApplicationSubsystemContainer::F_ApplicationSubsystemContainer(const F_ProgramUnitContainerBuildParams& BuildParams) :
        F_SubsystemContainer(BuildParams)
    {
        ABYTEK_BIND_OBJECT_SINGLETON_CRT();
    }
    F_ApplicationSubsystemContainer::~F_ApplicationSubsystemContainer()
    {
    }

    F_FeedbackStatus F_ApplicationSubsystemContainer::ValidateUnit(const TW_Valid<F_ProgramUnit>& Unit)
    {
        ABYTEK_FEEDBACK_STATUS_CHECK(F_SubsystemContainer::ValidateUnit(Unit));
        if (!Unit.CheckPolymorphism<A_ApplicationSubsystem>())
        {
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Require units of type A_ApplicationSubsystem"));
        }
        return F_FeedbackStatus::MakeSucceeded();
    }

    void F_ApplicationSubsystemContainer::_StartupUnits()
    {
        ForEachUnit(
            [](const TW_Valid<F_ProgramUnit>& Unit)
            {
                Unit.FastCast<A_ApplicationSubsystem>()->OnStartup();
                return true;
            }
        );
    }
    void F_ApplicationSubsystemContainer::_PostStartupUnits()
    {
        ForEachUnit(
            [](const TW_Valid<F_ProgramUnit>& Unit)
            {
                Unit.FastCast<A_ApplicationSubsystem>()->OnPostStartup();
                return true;
            }
        );
    }
    void F_ApplicationSubsystemContainer::_TickUnits()
    {
        ForEachUnit(
            [](const TW_Valid<F_ProgramUnit>& Unit)
            {
                Unit.FastCast<A_ApplicationSubsystem>()->OnTick();
                return true;
            }
        );
    }
    void F_ApplicationSubsystemContainer::_PostTickUnits()
    {
        ForEachUnit(
            [](const TW_Valid<F_ProgramUnit>& Unit)
            {
                Unit.FastCast<A_ApplicationSubsystem>()->OnPostTick();
                return true;
            }
        );
    }
    void F_ApplicationSubsystemContainer::_PreShutdownUnits()
    {
        ForEachUnit(
            [](const TW_Valid<F_ProgramUnit>& Unit)
            {
                Unit.FastCast<A_ApplicationSubsystem>()->OnPreShutdown();
                return true;
            },
            true,
            true
        );
    }
    void F_ApplicationSubsystemContainer::_ShutdownUnits()
    {
        ForEachUnit(
            [](const TW_Valid<F_ProgramUnit>& Unit)
            {
                Unit.FastCast<A_ApplicationSubsystem>()->OnShutdown();
                return true;
            },
            true,
            true
        );
    }
}
