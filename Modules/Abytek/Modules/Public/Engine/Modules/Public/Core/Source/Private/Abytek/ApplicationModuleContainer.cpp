#include "Abytek/ApplicationModuleContainer.hpp"
#include "Abytek/ApplicationModule.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_ApplicationModuleContainer)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_ApplicationModuleContainer"));
    }
    
    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(F_ApplicationModuleContainer);
    
    F_ApplicationModuleContainer::F_ApplicationModuleContainer(const F_ProgramUnitContainerBuildParams& BuildParams) :
        F_ModuleContainer(BuildParams)
    {
        ABYTEK_BIND_OBJECT_SINGLETON_CRT();
    }
    F_ApplicationModuleContainer::~F_ApplicationModuleContainer()
    {
    }

    F_FeedbackStatus F_ApplicationModuleContainer::ValidateUnit(const TW_Valid<F_ProgramUnit>& Unit)
    {
        ABYTEK_FEEDBACK_STATUS_CHECK(F_ModuleContainer::ValidateUnit(Unit));
        if (!Unit.CheckPolymorphism<A_ApplicationModule>())
        {
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Require units of type A_ApplicationModule"));
        }
        return F_FeedbackStatus::MakeSucceeded();
    }

    void F_ApplicationModuleContainer::_StartupUnits()
    {
        ForEachUnit(
            [](const TW_Valid<F_ProgramUnit>& Unit)
            {
                Unit.FastCast<A_ApplicationModule>()->OnStartup();
                return true;
            }
        );
    }
    void F_ApplicationModuleContainer::_PostStartupUnits()
    {
        ForEachUnit(
            [](const TW_Valid<F_ProgramUnit>& Unit)
            {
                Unit.FastCast<A_ApplicationModule>()->OnPostStartup();
                return true;
            }
        );
    }
    void F_ApplicationModuleContainer::_TickUnits()
    {
        ForEachUnit(
            [](const TW_Valid<F_ProgramUnit>& Unit)
            {
                Unit.FastCast<A_ApplicationModule>()->OnTick();
                return true;
            }
        );
    }
    void F_ApplicationModuleContainer::_PostTickUnits()
    {
        ForEachUnit(
            [](const TW_Valid<F_ProgramUnit>& Unit)
            {
                Unit.FastCast<A_ApplicationModule>()->OnPostTick();
                return true;
            }
        );
    }
    void F_ApplicationModuleContainer::_PreShutdownUnits()
    {
        ForEachUnit(
            [](const TW_Valid<F_ProgramUnit>& Unit)
            {
                Unit.FastCast<A_ApplicationModule>()->OnPreShutdown();
                return true;
            },
            true,
            true
        );
    }
    void F_ApplicationModuleContainer::_ShutdownUnits()
    {
        ForEachUnit(
            [](const TW_Valid<F_ProgramUnit>& Unit)
            {
                Unit.FastCast<A_ApplicationModule>()->OnShutdown();
                return true;
            },
            true,
            true
        );
    }
}
