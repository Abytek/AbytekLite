#include "Abytek/World/WorldSubsystemContainer.hpp"
#include "Abytek/World/WorldSubsystem.hpp"
#include "Abytek/World/World.hpp"
#include "Abytek/ApplicationCore.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_WorldSubsystemContainer)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_WorldSubsystemContainer"));
    }
    
    namespace Internal::WorldSubsystemContainer
    {
        F_ProgramUnitContainerBuildParams MakeBuildParams(const TW_Valid<F_World>& World)
        {
            F_ProgramUnitContainerBuildParams Result;
            Result.Console = A_ApplicationCore::GetInstance()->GetConsole();
            return Result;
        }
    }
    
    F_WorldSubsystemContainer::F_WorldSubsystemContainer(const TW_Valid<F_World>& World) :
        F_SubsystemContainer(Internal::WorldSubsystemContainer::MakeBuildParams(World)),
        _World(World)
    {
    }
    F_WorldSubsystemContainer::~F_WorldSubsystemContainer()
    {
    }

    F_FeedbackStatus F_WorldSubsystemContainer::ValidateUnit(const TW_Valid<F_ProgramUnit>& Unit)
    {
        ABYTEK_FEEDBACK_STATUS_CHECK(F_SubsystemContainer::ValidateUnit(Unit));
        if (!Unit.CheckPolymorphism<A_WorldSubsystem>())
        {
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Require units of type A_WorldSubsystem"));
        }
        return F_FeedbackStatus::MakeSucceeded();
    }

    void F_WorldSubsystemContainer::_TickUnits()
    {
        ForEachUnit(
            [](const TW_Valid<F_ProgramUnit>& Unit)
            {
                Unit.FastCast<A_WorldSubsystem>()->OnTick();
                return true;
            }
        );
    }
    void F_WorldSubsystemContainer::_PostTickUnits()
    {
        ForEachUnit(
            [](const TW_Valid<F_ProgramUnit>& Unit)
            {
                Unit.FastCast<A_WorldSubsystem>()->OnPostTick();
                return true;
            }
        );
    }
}
