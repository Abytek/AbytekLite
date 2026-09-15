#include "Abytek/ApplicationSubsystem.hpp"
#include "Abytek/ApplicationSubsystemContainer.hpp"
#include "Abytek/ApplicationModule.hpp"
#include "Abytek/ApplicationCore.hpp"
#include "Abytek/ApplicationUpdateFunction.hpp"
#include "Abytek/CoreUpdateGraph/HighLevelUpdateRange.hpp"
#include "Abytek/CoreUpdateGraph/StartupUpdateRange.hpp"
#include "Abytek/CoreUpdateGraph/PostStartupUpdateRange.hpp"
#include "Abytek/CoreUpdateGraph/TickUpdateRange.hpp"
#include "Abytek/CoreUpdateGraph/PostTickUpdateRange.hpp"
#include "Abytek/CoreUpdateGraph/PreShutdownUpdateRange.hpp"
#include "Abytek/CoreUpdateGraph/ShutdownUpdateRange.hpp"
#include "Abytek/UpdateBase/UpdateUtilities.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(A_ApplicationSubsystem)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::A_ApplicationSubsystem"));
    }

    F_Name A_ApplicationSubsystem::GetStartupUpdateFunctionName()
    {
        return ABYTEK_NAME("Abytek::A_ApplicationSubsystem::Startup");
    }
    F_Name A_ApplicationSubsystem::GetPostStartupUpdateFunctionName()
    {
        return ABYTEK_NAME("Abytek::A_ApplicationSubsystem::PostStartup");
    }
    F_Name A_ApplicationSubsystem::GetTickUpdateFunctionName()
    {
        return ABYTEK_NAME("Abytek::A_ApplicationSubsystem::Tick");
    }
    F_Name A_ApplicationSubsystem::GetPostTickUpdateFunctionName()
    {
        return ABYTEK_NAME("Abytek::A_ApplicationSubsystem::PostTick");
    }
    F_Name A_ApplicationSubsystem::GetPreShutdownUpdateFunctionName()
    {
        return ABYTEK_NAME("Abytek::A_ApplicationSubsystem::PreShutdown");
    }
    F_Name A_ApplicationSubsystem::GetShutdownUpdateFunctionName()
    {
        return ABYTEK_NAME("Abytek::A_ApplicationSubsystem::Shutdown");
    }

    void A_ApplicationSubsystem::GlobalInit()
    {
        {
            auto UpdateFunction = H_ApplicationUpdateFunction::Register(
                []
                {
                    F_ApplicationSubsystemContainer::GetInstance()->_StartupUnits();
                },
                GetStartupUpdateFunctionName(),
                E_ApplicationState::STARTUP
            );
            UpdateFunction->AddDependency(
                A_ApplicationModule::GetStartupUpdateFunctionName()  
            );
            UpdateFunction->AddReverseDependency(
                F_StartupUpdateRange::GetEndFunctionName()  
            );
        }
        {
            auto UpdateFunction = H_ApplicationUpdateFunction::Register(
                []
                {
                    F_ApplicationSubsystemContainer::GetInstance()->_PostStartupUnits();
                },
                GetPostStartupUpdateFunctionName(),
                E_ApplicationState::STARTUP
            );
            UpdateFunction->AddDependency(
                A_ApplicationModule::GetPostStartupUpdateFunctionName()  
            );
            UpdateFunction->AddReverseDependency(
                F_PostStartupUpdateRange::GetEndFunctionName()  
            );
        }
        {
            auto UpdateFunction = H_ApplicationUpdateFunction::Register(
                []
                {
                    F_ApplicationSubsystemContainer::GetInstance()->_TickUnits();
                },
                GetTickUpdateFunctionName(),
                E_ApplicationState::TICKING
            );
            UpdateFunction->AddDependency(
                A_ApplicationModule::GetTickUpdateFunctionName()  
            );
            UpdateFunction->AddReverseDependency(
                F_TickUpdateRange::GetEndFunctionName()  
            );
        }
        {
            auto UpdateFunction = H_ApplicationUpdateFunction::Register(
                []
                {
                    F_ApplicationSubsystemContainer::GetInstance()->_PostTickUnits();
                },
                GetPostTickUpdateFunctionName(),
                E_ApplicationState::TICKING
            );
            UpdateFunction->AddDependency(
                A_ApplicationModule::GetPostTickUpdateFunctionName()  
            );
            UpdateFunction->AddReverseDependency(
                F_PostTickUpdateRange::GetEndFunctionName()  
            );
        }
        {
            auto UpdateFunction = H_ApplicationUpdateFunction::Register(
                []
                {
                    F_ApplicationSubsystemContainer::GetInstance()->_PreShutdownUnits();
                },
                GetPreShutdownUpdateFunctionName(),
                E_ApplicationState::SHUTDOWN
            );
            UpdateFunction->AddDependency(
                F_PreShutdownUpdateRange::GetBeginFunctionName()  
            );
            UpdateFunction->AddReverseDependency(
                A_ApplicationModule::GetPreShutdownUpdateFunctionName()  
            );
        }
        {
            auto UpdateFunction = H_ApplicationUpdateFunction::Register(
                []
                {
                    F_ApplicationSubsystemContainer::GetInstance()->_ShutdownUnits();
                },
                GetShutdownUpdateFunctionName(),
                E_ApplicationState::SHUTDOWN
            );
            UpdateFunction->AddDependency(
                F_ShutdownUpdateRange::GetBeginFunctionName()  
            );
            UpdateFunction->AddReverseDependency(
                A_ApplicationModule::GetShutdownUpdateFunctionName()  
            );
        }
    }
    void A_ApplicationSubsystem::GlobalRelease()
    {
        H_UpdateUtilities::UnregisterFunction(
            GetStartupUpdateFunctionName()
        );
        H_UpdateUtilities::UnregisterFunction(
            GetPostStartupUpdateFunctionName()
        );
        H_UpdateUtilities::UnregisterFunction(
            GetTickUpdateFunctionName()
        );
        H_UpdateUtilities::UnregisterFunction(
            GetPostTickUpdateFunctionName()
        );
        H_UpdateUtilities::UnregisterFunction(
            GetPreShutdownUpdateFunctionName()
        );
        H_UpdateUtilities::UnregisterFunction(
            GetShutdownUpdateFunctionName()
        );
    }

    A_ApplicationSubsystem::A_ApplicationSubsystem(const F_ProgramUnitBuildParams& BuildParams) :
        F_Subsystem(BuildParams)
    {
    }
    A_ApplicationSubsystem::~A_ApplicationSubsystem()
    {
    }

    TW<F_Console> A_ApplicationSubsystem::GetConsole() const noexcept
    {
        return A_ApplicationCore::GetInstance()->GetConsole().Weak();
    }

    void A_ApplicationSubsystem::OnTick()
    {
    }
    void A_ApplicationSubsystem::OnPostTick()
    {
    }

    void A_ApplicationSubsystem::OnStartup()
    {
    }
    void A_ApplicationSubsystem::OnPostStartup()
    {
    }
    void A_ApplicationSubsystem::OnPrepareTick()
    {
    }
    void A_ApplicationSubsystem::OnPreShutdown()
    {
    }
    void A_ApplicationSubsystem::OnShutdown()
    {
    }
}
