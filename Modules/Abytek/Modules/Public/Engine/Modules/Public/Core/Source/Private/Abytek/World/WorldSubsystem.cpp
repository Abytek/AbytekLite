#include "Abytek/World/WorldSubsystem.hpp"
#include "Abytek/ApplicationSubsystem.hpp"
#include "Abytek/World/WorldSubsystemContainer.hpp"
#include "Abytek/World/World.hpp"
#include "Abytek/World/WorldManager.hpp"
#include "Abytek/UpdateBase/UpdateUtilities.hpp"
#include "Abytek/ApplicationUpdateFunction.hpp"
#include "Abytek/CoreUpdateGraph/PostTickUpdateRange.hpp"
#include "Abytek/CoreUpdateGraph/TickUpdateRange.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(A_WorldSubsystem)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::A_WorldSubsystem"));
    }

    F_Name A_WorldSubsystem::GetTickUpdateFunctionName()
    {
        return ABYTEK_NAME("Abytek::A_WorldSubsystem::Tick");
    }
    F_Name A_WorldSubsystem::GetPostTickUpdateFunctionName()
    {
        return ABYTEK_NAME("Abytek::A_WorldSubsystem::PostTick");
    }

    void A_WorldSubsystem::GlobalInit()
    {
        {
            auto UpdateFunction = H_ApplicationUpdateFunction::Register(
                []
                {
                    for (const auto& World : F_WorldManager::GetInstance()->GetWorlds())
                    {
                        World->GetSubsystemContainer()->_TickUnits();
                    }
                },
                GetTickUpdateFunctionName(),
                E_ApplicationState::TICKING
            );
            UpdateFunction->AddDependency(
                A_ApplicationSubsystem::GetTickUpdateFunctionName()  
            );
            UpdateFunction->AddReverseDependency(
                F_TickUpdateRange::GetEndFunctionName()  
            );
        }
        {
            auto UpdateFunction = H_ApplicationUpdateFunction::Register(
                []
                {
                    for (const auto& World : F_WorldManager::GetInstance()->GetWorlds())
                    {
                        World->GetSubsystemContainer()->_PostTickUnits();
                    }
                },
                GetPostTickUpdateFunctionName(),
                E_ApplicationState::TICKING
            );
            UpdateFunction->AddDependency(
                A_ApplicationSubsystem::GetPostTickUpdateFunctionName()  
            );
            UpdateFunction->AddReverseDependency(
                F_PostTickUpdateRange::GetEndFunctionName()
            );
        }
    }
    void A_WorldSubsystem::GlobalRelease()
    {
        H_UpdateUtilities::UnregisterFunction(
            GetTickUpdateFunctionName()
        );
        H_UpdateUtilities::UnregisterFunction(
            GetPostTickUpdateFunctionName()
        );
    }

    A_WorldSubsystem::A_WorldSubsystem(const F_ProgramUnitBuildParams& BuildParams) :
        F_Subsystem(BuildParams),
        _World(BuildParams.Container.FastCast<F_WorldSubsystemContainer>()->GetWorld())
    {
    }
    A_WorldSubsystem::~A_WorldSubsystem()
    {
    }

    void A_WorldSubsystem::OnTick()
    {
    }
    void A_WorldSubsystem::OnPostTick()
    {
    }

    void A_WorldSubsystem::OnStartup()
    {
    }
    void A_WorldSubsystem::OnPostStartup()
    {
    }
    void A_WorldSubsystem::OnPrepareTick()
    {
    }
    void A_WorldSubsystem::OnPreShutdown()
    {
    }
    void A_WorldSubsystem::OnShutdown()
    {
    }
}
