#include "Abytek/EngineRuntime.hpp"
#include "Abytek/ApplicationCore.hpp"
#include "Abytek/DevelopmentData.hpp"
#include "Abytek/ModuleList.hpp"
#include "Abytek/LLEvents/LLEvents.hpp"
#include "Abytek/ProgramUtilities/Program.hpp"
#include "Abytek/ProgramUtilities/ProgramUnit.hpp"
#include "Abytek/ProgramUtilities/ProgramUnitContainer.hpp"
#include "Abytek/Module.hpp"
#include "Abytek/ModuleContainer.hpp"
#include "Abytek/Subsystem.hpp"
#include "Abytek/SubsystemContainer.hpp"
#include "Abytek/ConfigFileUtilities.hpp"


namespace Abytek
{
    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(F_EngineRuntime);

    ABYTEK_REFLECT(F_EngineRuntime)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_EngineRuntime"));
    }

    F_Text F_EngineRuntime::GetConfigFileName()
    {
        return ABYTEK_TEXT("EngineRuntime.toml");
    }
    void F_EngineRuntime::GatherConfigFilePaths(TF_Vector<F_Text>& FilePaths)
    {
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        FilePaths.push_back(_ProjectDirectoryPath + ABYTEK_TEXT("/") + GetConfigFileName());
#endif
        H_ConfigFileUtilities::Gather(FilePaths, GetConfigFileName(), ABYTEK_NAME("Abytek.EngineRuntime"));
    }

    F_Name F_EngineRuntime::GetMainTaskTag()
    {
        return ABYTEK_NAME("Abytek::F_EngineRuntime::Main");
    }

    F_EngineRuntime::F_EngineRuntime(const F_EngineRuntimeBuildParams& BuildParams) :
        A_Runtime(),
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        _ProjectDirectoryPath(BuildParams.ProjectDirectoryPath),
#endif
        _ApplicationCreator(BuildParams.ApplicationCreator)
    {
        ABYTEK_BIND_OBJECT_SINGLETON_CRT();
    }
    F_EngineRuntime::~F_EngineRuntime()
    {
    }

    void F_EngineRuntime::Reflect()
    {
        GetReflectionSession()->RegisterType<A_Program>();
        GetReflectionSession()->RegisterType<F_ProgramUnit>();
        GetReflectionSession()->RegisterType<F_ProgramUnitContainer>();
        GetReflectionSession()->RegisterType<F_Module>();
        GetReflectionSession()->RegisterType<F_ModuleContainer>();
        GetReflectionSession()->RegisterType<F_Subsystem>();
        GetReflectionSession()->RegisterType<F_SubsystemContainer>();
        GetReflectionSession()->RegisterType<A_ApplicationCore>();
        GetReflectionSession()->RegisterType<F_ModuleList>();
    }
    void F_EngineRuntime::PostCreateConsole()
    {
    }
    void F_EngineRuntime::PostCreateSerializableEnvironment()
    {
    }
    void F_EngineRuntime::Main()
    {
        H_TaskUtilities::AddTag(GetMainTaskTag());
        
        _LLEvents = A_LLEvents::Create();
        _LLEvents->IncreaseLiveCounter(1);
        
        _Init();
        _LLEvents->Start();
        _Release();
        
        _LLEvents = {};
        
        H_TaskUtilities::RemoveTag(GetMainTaskTag());
    }
    void F_EngineRuntime::PreDestroySerializableEnvironment()
    {
    }
    void F_EngineRuntime::PreDestroyConsole()
    {
    }

    void F_EngineRuntime::_Init()
    {
        ABYTEK_LOG_INFO() << "F_EngineRuntime::Init() Begin";
        
        _ModuleList = TU<F_ModuleList>()();
        
        _TickHandle = _LLEvents->OnTick.AddListener(
            [this]
            {
                _Tick();
            }
        );
        
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        if (
            auto Status = F_DevelopmentData::Create(
                F_DevelopmentDataBuildParams::MakeCurrent(
                    _ProjectDirectoryPath
                ), 
                _MainDevelopmentData
            );    
            !Status
        )
        {
            ABYTEK_LOG_FATAL() << Status.ErrorMessage;
        }
#endif
        
        // Setup serializable environment
        /*{
            const auto& Environment = GetSerializableEnvironment();
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
            auto MainDevelopmentData = F_EngineRuntime::GetInstance()->GetMainDevelopmentData();
#else 
            auto UserDataDirectoryPath = *A_PathManager::GetInstance()->GetCommonDirectoryPath(E_CommonDirectory::USER_DATA);
#endif
            for (const auto& ModuleName : F_ModuleList::GetInstance()->GetAll())
            {
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
                const auto& ModuleDevelopmentData = MainDevelopmentData->GetModuleDevelopmentData(ModuleName);
                Environment->AddMount(
                    ModuleName,
                    ModuleDevelopmentData.Directory + ABYTEK_TEXT("/Assets")
                );
                Environment->AddMount(
                    *ModuleName + ABYTEK_TEXT("::Intermediate::Assets"),
                    ModuleDevelopmentData.Directory + ABYTEK_TEXT("/Intermediate/Assets")
                );
#else
                Environment->AddMount(
                    ModuleName,
                    UserDataDirectoryPath + ABYTEK_TEXT("/Abytek/") + *ModuleName + ABYTEK_TEXT("/Assets")
                );
                Environment->AddMount(
                    *ModuleName + ABYTEK_TEXT("::Intermediate::Assets"),
                    UserDataDirectoryPath + ABYTEK_TEXT("/Abytek/") + *ModuleName + ABYTEK_TEXT("/Intermediate/Assets")
                );
#endif
            }
        }*/
        
        ABYTEK_LOG_INFO() << "F_EngineRuntime::Init() End";
    }
    void F_EngineRuntime::_Tick()
    {
        if (_IsFirstTick)
        {
            _Application = _ApplicationCreator();
            _IsFirstTick = false;
        }
        _Application->UpdateState();
        if (A_ApplicationCore::GetInstance()->GetState() == E_ApplicationState::CLOSED)
        {
            _LLEvents->DecreaseLiveCounter(1);
            _Application = {};
        }
    }
    void F_EngineRuntime::_Release()
    {
        ABYTEK_LOG_INFO() << "F_EngineRuntime::Release() Begin";
        
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        _MainDevelopmentData = {};
#endif
        
        _LLEvents->OnTick.RemoveListener(_TickHandle);
        
        _ModuleList = {};
        
        ABYTEK_LOG_INFO() << "F_EngineRuntime::Release() End";
    }
}
