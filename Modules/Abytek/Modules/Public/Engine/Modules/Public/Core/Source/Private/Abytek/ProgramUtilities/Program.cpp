#include "Abytek/ProgramUtilities/Program.hpp"
#include "Abytek/ModuleContainer.hpp"
#include "Abytek/Module.hpp"
#include "Abytek/SubsystemContainer.hpp"
#include "Abytek/Subsystem.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(A_Program)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::A_Program"));
    }

    void A_Program::GatherConfigFilePaths(TF_Vector<F_Text>& FilePaths)
    {
    }

    A_Program::A_Program()
    {
    }
    A_Program::~A_Program()
    {
    }

    void A_Program::_RegisterSubsystems()
    {
        auto BaseSubsystemType = GetBaseSubsystemType();
        ABYTEK_ENGINE_CORE_ASSERT(BaseSubsystemType);
        
        TF_Vector<TF_ReflectionTypeHandle<F_Subsystem>> SubsystemTypes;
        
        _ModuleContainer->ForEachUnit(
            [&BaseSubsystemType, &SubsystemTypes](const TW_Valid<F_ProgramUnit>& Unit)
            {
                auto Module = Unit.FastCast<F_Module>();
                auto ReflectionSession = Module->GetReflectionSession();
                ReflectionSession->ForEachTypeDerivedFrom(
                    BaseSubsystemType,
                    [&SubsystemTypes](const TW_Valid<F_ReflectionType>& Type)
                    {
                        SubsystemTypes.push_back(Type);
                        return true;
                    }
                );
                return true;
            }
        );
        
        for (const auto& SubsystemType : SubsystemTypes)
        {
            _SubsystemContainer->EnsureUnit(SubsystemType);
        }
    }

    TU<F_ModuleContainer> A_Program::CreateModuleContainer()
    {
        F_ProgramUnitContainerBuildParams BuildParams;
        BuildParams.Console = _Console.Weak();
        return TU<F_ModuleContainer>()(BuildParams);
    }
    TU<F_SubsystemContainer> A_Program::CreateSubsystemContainer()
    {
        F_ProgramUnitContainerBuildParams BuildParams;
        BuildParams.Console = _Console.Weak();
        return TU<F_SubsystemContainer>()(BuildParams);
    }

    void A_Program::Reflect()
    {
    }
    void A_Program::PostReflectModules()
    {
    }
    void A_Program::PostCreateConsole()
    {
    }
    void A_Program::Init()
    {
    }
    void A_Program::PostInitModules()
    {
    }
    void A_Program::PostInitSubsystems()
    {
    }
    void A_Program::PostInit()
    {
    }
    void A_Program::PreRelease()
    {
    }
    void A_Program::PreReleaseSubsystems()
    {
    }
    void A_Program::PreReleaseModules()
    {
    }
    void A_Program::Release()
    {
    }
    void A_Program::PreDestroyConsole()
    {
    }
    void A_Program::OnPreCleanUpModuleReflectionSessions()
    {
    }

    TF_ReflectionTypeHandle<F_Subsystem> A_Program::GetBaseSubsystemType()
    {
        return {};
    }

    void A_Program::Start()
    {
        _ReflectionSession = F_ReflectionContext::GetGlobal()->CreateSession({});
        _ReflectionSession->Begin();
        Reflect();
        _ReflectionSession->End();
        
        _Console = TU<F_Console>()();
        {
            TF_Vector<F_Text> ConfigFilePaths;
            GatherConfigFilePaths(ConfigFilePaths);
            for (const auto& ConfigFilePath : ConfigFilePaths)
            {
                if (H_FSUtilities::Exists(ConfigFilePath, E_FSEntryType::FILE))
                {
                    GetConsole()->AddConfigFile(ConfigFilePath);
                }
            }
        }
        PostCreateConsole();
        
        _ModuleContainer = CreateModuleContainer();
        _SubsystemContainer = CreateSubsystemContainer();
        
        Init();
        
        F_ProgramUnitContainer::F_UpdateParams ModuleContainerUpdateParams;
        ModuleContainerUpdateParams.OnPostReflect = [this]
        {
            PostReflectModules();
        };
        _ModuleContainer->Update(ModuleContainerUpdateParams);
        {
            _RegisterSubsystems();
            _SubsystemContainer->Update();
            PostInitSubsystems();
        }
        PostInitModules();
        
        PostInit();
    }
    void A_Program::Stop()
    {
        PreRelease();
        
        PreReleaseModules();
        {
            PreReleaseSubsystems();
            _SubsystemContainer->EnqueueRemoveAllUnits();
            _SubsystemContainer->Update();
        }
        _ModuleContainer->EnqueueRemoveAllUnits();
        F_ProgramUnitContainer::F_UpdateParams ModuleContainerUpdateParams;
        ModuleContainerUpdateParams.OnPreCleanUpReflectionSession = [this]
        {
            OnPreCleanUpModuleReflectionSessions();
        };
        _ModuleContainer->Update();
        
        Release();
        
        _SubsystemContainer = {};
        _ModuleContainer = {};
        
        PreDestroyConsole();
        {
            TF_Vector<F_Text> ConfigFilePaths;
            GatherConfigFilePaths(ConfigFilePaths);
            for (const auto& ConfigFilePath : ConfigFilePaths)
            {
                if (GetConsole()->HasConfigFile(ConfigFilePath))
                {
                    GetConsole()->RemoveConfigFile(ConfigFilePath);
                }
            }
        }
        _Console = {};
        
        {
            _ReflectionSession->GBMUnmarkAsRoot();
            F_ReflectionContext::GetGlobal()->Flush();
        }
    }
}
