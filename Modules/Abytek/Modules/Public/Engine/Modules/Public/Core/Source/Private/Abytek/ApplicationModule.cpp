#include "Abytek/ApplicationModule.hpp"
#include "Abytek/ApplicationCore.hpp"
#include "Abytek/ApplicationModuleContainer.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(A_ApplicationModule)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::A_ApplicationModule"));
    }

    F_Name A_ApplicationModule::GetStartupUpdateFunctionName()
    {
        return ABYTEK_NAME("Abytek::A_ApplicationModule::Startup");
    }
    F_Name A_ApplicationModule::GetPostStartupUpdateFunctionName()
    {
        return ABYTEK_NAME("Abytek::A_ApplicationModule::PostStartup");
    }
    F_Name A_ApplicationModule::GetTickUpdateFunctionName()
    {
        return ABYTEK_NAME("Abytek::A_ApplicationModule::Tick");
    }
    F_Name A_ApplicationModule::GetPostTickUpdateFunctionName()
    {
        return ABYTEK_NAME("Abytek::A_ApplicationModule::PostTick");
    }
    F_Name A_ApplicationModule::GetPreShutdownUpdateFunctionName()
    {
        return ABYTEK_NAME("Abytek::A_ApplicationModule::PreShutdown");
    }
    F_Name A_ApplicationModule::GetShutdownUpdateFunctionName()
    {
        return ABYTEK_NAME("Abytek::A_ApplicationModule::Shutdown");
    }

    void A_ApplicationModule::GlobalInit()
    {
        {
            auto UpdateFunction = H_ApplicationUpdateFunction::Register(
                []
                {
                    F_ApplicationModuleContainer::GetInstance()->_StartupUnits();
                },
                GetStartupUpdateFunctionName(),
                E_ApplicationState::STARTUP
            );
            UpdateFunction->AddDependency(
                A_ApplicationCore::GetStartupUpdateFunctionName()  
            );
            UpdateFunction->AddReverseDependency(
                F_StartupUpdateRange::GetEndFunctionName()  
            );
        }
        {
            auto UpdateFunction = H_ApplicationUpdateFunction::Register(
                []
                {
                    F_ApplicationModuleContainer::GetInstance()->_PostStartupUnits();
                },
                GetPostStartupUpdateFunctionName(),
                E_ApplicationState::STARTUP
            );
            UpdateFunction->AddDependency(
                A_ApplicationCore::GetPostStartupUpdateFunctionName()  
            );
            UpdateFunction->AddReverseDependency(
                F_PostStartupUpdateRange::GetEndFunctionName()  
            );
        }
        {
            auto UpdateFunction = H_ApplicationUpdateFunction::Register(
                []
                {
                    F_ApplicationModuleContainer::GetInstance()->_TickUnits();
                },
                GetTickUpdateFunctionName(),
                E_ApplicationState::TICKING
            );
            UpdateFunction->AddDependency(
                A_ApplicationCore::GetTickUpdateFunctionName()  
            );
            UpdateFunction->AddReverseDependency(
                F_TickUpdateRange::GetEndFunctionName()  
            );
        }
        {
            auto UpdateFunction = H_ApplicationUpdateFunction::Register(
                []
                {
                    F_ApplicationModuleContainer::GetInstance()->_PostTickUnits();
                },
                GetPostTickUpdateFunctionName(),
                E_ApplicationState::TICKING
            );
            UpdateFunction->AddDependency(
                A_ApplicationCore::GetPostTickUpdateFunctionName()  
            );
            UpdateFunction->AddReverseDependency(
                F_PostTickUpdateRange::GetEndFunctionName()  
            );
        }
        {
            auto UpdateFunction = H_ApplicationUpdateFunction::Register(
                []
                {
                    F_ApplicationModuleContainer::GetInstance()->_PreShutdownUnits();
                },
                GetPreShutdownUpdateFunctionName(),
                E_ApplicationState::SHUTDOWN
            );
            UpdateFunction->AddDependency(
                F_PreShutdownUpdateRange::GetBeginFunctionName()  
            );
            UpdateFunction->AddReverseDependency(
                A_ApplicationCore::GetPreShutdownUpdateFunctionName()  
            );
        }
        {
            auto UpdateFunction = H_ApplicationUpdateFunction::Register(
                []
                {
                    F_ApplicationModuleContainer::GetInstance()->_ShutdownUnits();
                },
                GetShutdownUpdateFunctionName(),
                E_ApplicationState::SHUTDOWN
            );
            UpdateFunction->AddDependency(
                F_ShutdownUpdateRange::GetBeginFunctionName()  
            );
            UpdateFunction->AddReverseDependency(
                A_ApplicationCore::GetShutdownUpdateFunctionName()  
            );
        }
    }
    void A_ApplicationModule::GlobalRelease()
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
    
    A_ApplicationModule::A_ApplicationModule(const F_ProgramUnitBuildParams& BuildParams) :
        F_Module(BuildParams)
    {
    }
    A_ApplicationModule::~A_ApplicationModule()
    {
    }

    TW<F_Console> A_ApplicationModule::GetConsole() const noexcept
    {
        return A_ApplicationCore::GetInstance()->GetConsole().Weak();
    }

    void A_ApplicationModule::OnStartup()
    {
    }
    void A_ApplicationModule::OnPostStartup()
    {
    }
    void A_ApplicationModule::OnPrepareTick()
    {
    }
    void A_ApplicationModule::OnTick()
    {
    }
    void A_ApplicationModule::OnPostTick()
    {
    }
    void A_ApplicationModule::OnPreShutdown()
    {
    }
    void A_ApplicationModule::OnShutdown()
    {
    }
}
