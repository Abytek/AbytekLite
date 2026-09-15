#include "Abytek/ApplicationCore.hpp"

#include "Abytek/ApplicationModuleContainer.hpp"
#include "Abytek/ApplicationSubsystemContainer.hpp"
#include "Abytek/ApplicationSubsystem.hpp"
#include "Abytek/ApplicationUpdateFunction.hpp"
#include "Abytek/ApplicationMountManager.hpp"
#include "Abytek/ApplicationMountHelper.hpp"
#include "Abytek/GlobalTime.hpp"
#include "Abytek/CoreModule.hpp"
#include "Abytek/EngineRuntime.hpp"
#include "Abytek/DevelopmentData.hpp"
#include "Abytek/ConfigFileUtilities.hpp"
#include "Abytek/CoreUpdateGraph/HighLevelUpdateRange.hpp"
#include "Abytek/CoreUpdateGraph/PrimaryUpdateRange.hpp"
#include "Abytek/CoreUpdateGraph/StartupUpdateRange.hpp"
#include "Abytek/CoreUpdateGraph/PostStartupUpdateRange.hpp"
#include "Abytek/CoreUpdateGraph/TickUpdateRange.hpp"
#include "Abytek/CoreUpdateGraph/PostTickUpdateRange.hpp"
#include "Abytek/CoreUpdateGraph/PreShutdownUpdateRange.hpp"
#include "Abytek/CoreUpdateGraph/ShutdownUpdateRange.hpp"
#include "Abytek/Frame/FrameManager.hpp"
#include "Abytek/ExecutionGraph.hpp"
#include "Abytek/ModuleList.hpp"
#include "Abytek/UpdateBase/UpdateManager.hpp"
#include "Abytek/UpdateBase/UpdateUtilities.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(A_ApplicationCore)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::A_ApplicationCore"));
    }
    
    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(A_ApplicationCore)

    F_Name A_ApplicationCore::GetStartupUpdateFunctionName()
    {
        return ABYTEK_NAME("Abytek::A_ApplicationCore::Startup");
    }
    F_Name A_ApplicationCore::GetPostStartupUpdateFunctionName()
    {
        return ABYTEK_NAME("Abytek::A_ApplicationCore::PostStartup");
    }
    F_Name A_ApplicationCore::GetTickUpdateFunctionName()
    {
        return ABYTEK_NAME("Abytek::A_ApplicationCore::Tick");
    }
    F_Name A_ApplicationCore::GetPostTickUpdateFunctionName()
    {
        return ABYTEK_NAME("Abytek::A_ApplicationCore::PostTick");
    }
    F_Name A_ApplicationCore::GetPreShutdownUpdateFunctionName()
    {
        return ABYTEK_NAME("Abytek::A_ApplicationCore::PreShutdown");
    }
    F_Name A_ApplicationCore::GetShutdownUpdateFunctionName()
    {
        return ABYTEK_NAME("Abytek::A_ApplicationCore::Shutdown");
    }

    F_Text A_ApplicationCore::GetConfigFileName()
    {
        return ABYTEK_TEXT("Application.toml");
    }
    void A_ApplicationCore::GatherConfigFilePaths(TF_Vector<F_Text>& FilePaths)
    {
        H_ConfigFileUtilities::Gather(FilePaths, GetConfigFileName(), _Name);
    }

    A_ApplicationCore::A_ApplicationCore(const F_ApplicationBuildParams& BuildParams) :
        _Name(BuildParams.Name),
        _ExternalInitFunction(BuildParams.ExternalInitFunction)
    {
        ABYTEK_BIND_OBJECT_SINGLETON_CRT();
    }
    A_ApplicationCore::~A_ApplicationCore()
    {
    }

    TU<F_ModuleContainer> A_ApplicationCore::CreateModuleContainer()
    {
        F_ProgramUnitContainerBuildParams BuildParams;
        BuildParams.Console = GetConsole();
        return TU<F_ApplicationModuleContainer>()(BuildParams);
    }
    TU<F_SubsystemContainer> A_ApplicationCore::CreateSubsystemContainer()
    {
        F_ProgramUnitContainerBuildParams BuildParams;
        BuildParams.Console = GetConsole();
        return TU<F_ApplicationSubsystemContainer>()(BuildParams);
    }

    void A_ApplicationCore::Reflect()
    {
        GetReflectionSession()->RegisterType<A_ApplicationModule>();
        GetReflectionSession()->RegisterType<F_ApplicationModuleContainer>();
        GetReflectionSession()->RegisterType<A_ApplicationSubsystem>();
        GetReflectionSession()->RegisterType<F_ApplicationSubsystemContainer>();
        GetReflectionSession()->RegisterType<F_FrameManager>();
        GetReflectionSession()->RegisterType<F_ExecutionGraph>();
        GetReflectionSession()->RegisterType<F_UpdateManager>();
    }
    void A_ApplicationCore::PostReflectModules()
    {
        _SetupCDOTypes();
    }
    void A_ApplicationCore::PostCreateConsole()
    {
        _ConsoleVariable_FramesInFlightMode = GetConsole()->RegisterVariable(
            ABYTEK_NAME("Abytek.FramesInFlightMode"),
            ABYTEK_TEXT(""),
            E_FramesInFlightMode::DEFAULT
        );
        GetConsole()->Update();
        
        if (IsMemoryLeakDebuggingEnabled())
        {
            _ConsoleVariable_FramesInFlightMode->SetValue(E_FramesInFlightMode::NONE);
        }
    }
    void A_ApplicationCore::Init()
    {
        F_FrameManagerBuildParams FrameManagerBuildParams;
        
        _MountManager = TU<F_ApplicationMountManager>()();
        {
            for (const auto& ModuleName : F_ModuleList::GetInstance()->GetAll())
            {
                H_ApplicationMount::Add(
                    *ModuleName + ABYTEK_TEXT("::Assets"),
                    ModuleName,
                    ABYTEK_TEXT("Assets")
                );
                H_ApplicationMount::Add(
                    *ModuleName + ABYTEK_TEXT("::Intermediate::Assets"),
                    ModuleName,
                    ABYTEK_TEXT("Intermediate/Assets")
                );
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
                H_ApplicationMount::Add(
                    *ModuleName + ABYTEK_TEXT("::Intermediate::CookData"),
                    ModuleName,
                    ABYTEK_TEXT("Intermediate/CookData")
                );
#endif
            }
        }
        
        _FrameManager = TU<F_FrameManager>()(FrameManagerBuildParams);
        
        _UpdateManager = TU<F_UpdateManager>()();
        _HighLevelUpdateRange = TU<F_HighLevelUpdateRange>()();
        _PrimaryUpdateRange = TU<F_PrimaryUpdateRange>()();
        _StartupUpdateRange = TU<F_StartupUpdateRange>()();
        _PostStartupUpdateRange = TU<F_PostStartupUpdateRange>()();
        _TickUpdateRange = TU<F_TickUpdateRange>()();
        _PostTickUpdateRange = TU<F_PostTickUpdateRange>()();
        _PreShutdownUpdateRange = TU<F_PreShutdownUpdateRange>()();
        _ShutdownUpdateRange = TU<F_ShutdownUpdateRange>()();
        {
            auto UpdateFunction = H_ApplicationUpdateFunction::Register(
                [this]
                {
                    Startup();
                },
                GetStartupUpdateFunctionName(),
                E_ApplicationState::STARTUP
            );
            UpdateFunction->AddDependency(
                F_StartupUpdateRange::GetBeginFunctionName()  
            );
            UpdateFunction->AddReverseDependency(
                F_StartupUpdateRange::GetEndFunctionName()  
            );
        }
        {
            auto UpdateFunction = H_ApplicationUpdateFunction::Register(
                [this]
                {
                    PostStartup();
                },
                GetPostStartupUpdateFunctionName(),
                E_ApplicationState::STARTUP
            );
            UpdateFunction->AddDependency(
                F_PostStartupUpdateRange::GetBeginFunctionName()  
            );
            UpdateFunction->AddReverseDependency(
                F_PostStartupUpdateRange::GetEndFunctionName()  
            );
        }
        {
            auto UpdateFunction = H_ApplicationUpdateFunction::Register(
                [this]
                {
                    Tick();
                },
                GetTickUpdateFunctionName(),
                E_ApplicationState::TICKING
            );
            UpdateFunction->AddDependency(
                F_TickUpdateRange::GetBeginFunctionName()  
            );
            UpdateFunction->AddReverseDependency(
                F_TickUpdateRange::GetEndFunctionName()  
            );
        }
        {
            auto UpdateFunction = H_ApplicationUpdateFunction::Register(
                [this]
                {
                    PostTick();
                },
                GetPostTickUpdateFunctionName(),
                E_ApplicationState::TICKING
            );
            UpdateFunction->AddDependency(
                F_PostTickUpdateRange::GetBeginFunctionName()  
            );
            UpdateFunction->AddReverseDependency(
                F_PostTickUpdateRange::GetEndFunctionName()  
            );
        }
        {
            auto UpdateFunction = H_ApplicationUpdateFunction::Register(
                [this]
                {
                    PreShutdown();
                },
                GetPreShutdownUpdateFunctionName(),
                E_ApplicationState::SHUTDOWN
            );
            UpdateFunction->AddDependency(
                F_PreShutdownUpdateRange::GetBeginFunctionName()  
            );
            UpdateFunction->AddReverseDependency(
                F_PreShutdownUpdateRange::GetEndFunctionName()  
            );
        }
        {
            auto UpdateFunction = H_ApplicationUpdateFunction::Register(
                [this]
                {
                    Shutdown();
                },
                GetShutdownUpdateFunctionName(),
                E_ApplicationState::SHUTDOWN
            );
            UpdateFunction->AddDependency(
                F_ShutdownUpdateRange::GetBeginFunctionName()  
            );
            UpdateFunction->AddReverseDependency(
                F_ShutdownUpdateRange::GetEndFunctionName()  
            );
        }
        A_ApplicationModule::GlobalInit();
        A_ApplicationSubsystem::GlobalInit();
        
        RegisterModules();
        _ExternalInitFunction();
    }
    void A_ApplicationCore::Release()
    {
        A_ApplicationSubsystem::GlobalRelease();
        A_ApplicationModule::GlobalRelease();
        H_UpdateUtilities::UnregisterFunction(
            GetShutdownUpdateFunctionName()  
        );
        H_UpdateUtilities::UnregisterFunction(
            GetPreShutdownUpdateFunctionName()  
        );
        H_UpdateUtilities::UnregisterFunction(
            GetPostTickUpdateFunctionName()  
        );
        H_UpdateUtilities::UnregisterFunction(
            GetTickUpdateFunctionName()  
        );
        H_UpdateUtilities::UnregisterFunction(
            GetPostStartupUpdateFunctionName()  
        );
        H_UpdateUtilities::UnregisterFunction(
            GetStartupUpdateFunctionName()  
        );
        _ShutdownUpdateRange = {};
        _PreShutdownUpdateRange = {};
        _PostTickUpdateRange = {};
        _TickUpdateRange = {};
        _PostStartupUpdateRange = {};
        _StartupUpdateRange = {};
        _PrimaryUpdateRange = {};
        _HighLevelUpdateRange = {};
        _UpdateManager = {};
        
        _FrameManager = {};
        
        _MountManager = {};
    }
    void A_ApplicationCore::PreDestroyConsole()
    {
    }
    void A_ApplicationCore::OnPreCleanUpModuleReflectionSessions()
    {
        _CleanUpCDOTypes();
    }

    void A_ApplicationCore::UpdateFrame(E_ApplicationState State)
    {
        ABYTEK_PROFILER_EVENT();
        E_FramesInFlightMode FramesInFlightMode = E_FramesInFlightMode::NONE;
        switch (State)
        {
        case E_ApplicationState::INITIALIZING:
            FramesInFlightMode = E_FramesInFlightMode::NONE;
            break;
        case E_ApplicationState::LLA_INITIALIZING:
            FramesInFlightMode = E_FramesInFlightMode::NONE;
            break;
        case E_ApplicationState::STARTUP:
            FramesInFlightMode = E_FramesInFlightMode::NONE;
            break;
        case E_ApplicationState::TICKING:
            FramesInFlightMode = _ConsoleVariable_FramesInFlightMode->GetValue();
            break;
        case E_ApplicationState::SHUTDOWN:
            FramesInFlightMode = E_FramesInFlightMode::NONE;
            break;
        case E_ApplicationState::LLA_RELEASING:
            FramesInFlightMode = E_FramesInFlightMode::NONE;
            break;
        case E_ApplicationState::RELEASING:
            FramesInFlightMode = E_FramesInFlightMode::NONE;
            break;
        default:
            ABYTEK_LOG_FATAL() << "Unknown application state: " << static_cast<U32>(State);
        }
        
#ifdef ABYTEK_BASE_CORE_ENABLE_MEMORY_ALLOCATION_INFO
        ApplicationDebugMemoryLeakByFrame::F_Params MemoryLeakDebuggingParams;
        F_MemoryAnalyzedState BeginMemoryAnalyzedState;
        TF_Vector<A_Object*> BeginTrackedObjects;
        F_MemoryAnalyzedState EndMemoryAnalyzedState;
        TF_Vector<A_Object*> EndTrackedObjects;
        F_MemoryAnalyzedState DifferentMemoryAnalyzedState;
        TF_Vector<A_Object*> DifferentTrackedObjects;
#endif
        
#ifdef ABYTEK_BASE_CORE_ENABLE_MEMORY_ALLOCATION_INFO
        if (IsMemoryLeakDebuggingEnabled())
        {
            BeginMemoryAnalyzedState = F_MemoryManager::GetInstance().Analyze();
            BeginTrackedObjects = GetTrackedObjects();
            MemoryLeakDebuggingParams.BeginMemoryAnalyzedState = &BeginMemoryAnalyzedState;
            MemoryLeakDebuggingParams.BeginTrackedObjects = &BeginTrackedObjects;
        }
#endif
        _FrameManager->Update(
            [this]
            {
                F_GlobalTime::GetInstance()->NewFrame();
                _UpdateManager->Update();
            },
            FramesInFlightMode
        );
#ifdef ABYTEK_BASE_CORE_ENABLE_MEMORY_ALLOCATION_INFO
        if (IsMemoryLeakDebuggingEnabled())
        {
            EndMemoryAnalyzedState = F_MemoryManager::GetInstance().Analyze();
            EndTrackedObjects = GetTrackedObjects();
            MemoryLeakDebuggingParams.EndMemoryAnalyzedState = &EndMemoryAnalyzedState;
            MemoryLeakDebuggingParams.EndTrackedObjects = &EndTrackedObjects;
            
            DifferentMemoryAnalyzedState = EndMemoryAnalyzedState.Compare(BeginMemoryAnalyzedState);
            MemoryLeakDebuggingParams.DifferentMemoryAnalyzedState = &DifferentMemoryAnalyzedState;
        
            {
                TF_Set<A_Object*> BeginSet;
                for (auto Object : BeginTrackedObjects)
                {
                    BeginSet.insert(Object);
                }
                for (auto Object : EndTrackedObjects)
                {
                    if (BeginSet.find(Object) != BeginSet.end())
                    {
                        continue;
                    }
                    DifferentTrackedObjects.push_back(Object);
                }
            }
            MemoryLeakDebuggingParams.DifferentTrackedObjects = &DifferentTrackedObjects;
        
            Events.OnDebugMemoryLeakByFrame.Broadcast(MemoryLeakDebuggingParams);
        }
#endif
    }
    void A_ApplicationCore::UpdateState()
    {
        _State = _NextState;
        switch (_NextState)
        {
        case E_ApplicationState::INITIALIZING:
            H_TaskUtilities::AddTag(GetApplicationLowLevelTaskTag());
            Start();
            H_TaskUtilities::RemoveTag(GetApplicationLowLevelTaskTag());
            break;
        case E_ApplicationState::LLA_INITIALIZING:
            H_TaskUtilities::AddTag(GetApplicationLowLevelTaskTag());
            UpdateFrame(E_ApplicationState::LLA_INITIALIZING);
            H_TaskUtilities::RemoveTag(GetApplicationLowLevelTaskTag());
            break;
        case E_ApplicationState::STARTUP:
            UpdateFrame(E_ApplicationState::STARTUP);
            break;
        case E_ApplicationState::TICKING:
            F_ApplicationModuleContainer::GetInstance()->ForEachUnit(
                [](const TW_Valid<F_ProgramUnit>& Unit)
                {
                    Unit.FastCast<A_ApplicationModule>()->OnPrepareTick();
                    return true;
                }
            );
            F_ApplicationSubsystemContainer::GetInstance()->ForEachUnit(
                [](const TW_Valid<F_ProgramUnit>& Unit)
                {
                    Unit.FastCast<A_ApplicationSubsystem>()->OnPrepareTick();
                    return true;
                }
            );
            UpdateFrame(E_ApplicationState::TICKING);
            break;
        case E_ApplicationState::SHUTDOWN:
            UpdateFrame(E_ApplicationState::SHUTDOWN);
            break;
        case E_ApplicationState::LLA_RELEASING:
            H_TaskUtilities::AddTag(GetApplicationLowLevelTaskTag());
            UpdateFrame(E_ApplicationState::LLA_RELEASING);
            H_TaskUtilities::RemoveTag(GetApplicationLowLevelTaskTag());
            break;
        case E_ApplicationState::RELEASING:
            H_TaskUtilities::AddTag(GetApplicationLowLevelTaskTag());
            Stop();
            H_TaskUtilities::RemoveTag(GetApplicationLowLevelTaskTag());
            break;
        default:
            ABYTEK_LOG_FATAL() << "Unknown application state: " << static_cast<U32>(_NextState);
        }
        
        switch (_NextState)
        {
        case E_ApplicationState::INITIALIZING:
        case E_ApplicationState::LLA_INITIALIZING:
            if (GetLLAInitCounter() == 0)
            {
                ABYTEK_LOG_INFO() << "A_ApplicationCore::UpdateState() -> STARTUP";
                _NextState = E_ApplicationState::STARTUP;
            }
            else
            {
                ABYTEK_LOG_INFO() << "A_ApplicationCore::UpdateState() -> LLA_INITIALIZING";
                _NextState = E_ApplicationState::LLA_INITIALIZING;
            }
            break;
        case E_ApplicationState::STARTUP:
            ABYTEK_LOG_INFO() << "A_ApplicationCore::UpdateState() -> TICKING";
            _NextState = E_ApplicationState::TICKING;
            break;
        case E_ApplicationState::TICKING:
            if (ShouldShutdown())
            {
                ABYTEK_LOG_INFO() << "A_ApplicationCore::UpdateState() -> SHUTDOWN";
                _NextState = E_ApplicationState::SHUTDOWN;
            }
            break;
        case E_ApplicationState::SHUTDOWN:
        case E_ApplicationState::LLA_RELEASING:
            if (GetLLAReleaseCounter() == 0)
            {
                ABYTEK_LOG_INFO() << "A_ApplicationCore::UpdateState() -> RELEASING";
                _NextState = E_ApplicationState::RELEASING;
            }
            else
            {
                ABYTEK_LOG_INFO() << "A_ApplicationCore::UpdateState() -> LLA_RELEASING";
                _NextState = E_ApplicationState::LLA_RELEASING;
            }
            break;
        case E_ApplicationState::RELEASING:
            ABYTEK_LOG_INFO() << "A_ApplicationCore::UpdateState() -> CLOSED";
            _NextState = E_ApplicationState::CLOSED;
            break;
        default:
            ABYTEK_LOG_FATAL() << "Unknown application state: " << static_cast<U32>(_NextState);
        }
    }

    void A_ApplicationCore::Startup()
    {
    }
    void A_ApplicationCore::PostStartup()
    {
    }
    void A_ApplicationCore::Tick()
    {
    }
    void A_ApplicationCore::PostTick()
    {
    }
    void A_ApplicationCore::PreShutdown()
    {
    }
    void A_ApplicationCore::Shutdown()
    {
    }

    void A_ApplicationCore::RegisterModules()
    {
        RegisterModule<F_CoreModule>();
    }

    TF_ReflectionTypeHandle<F_Subsystem> A_ApplicationCore::GetBaseSubsystemType()
    {
        return TF_ReflectionTypeHandle<A_ApplicationSubsystem>(F_ReflectionContext::GetGlobal());
    }

    U32 A_ApplicationCore::IncreaseLLAInitCounter(U32 Num)
    {
        return _LLAInitCounter.fetch_add(Num, boost::memory_order_acq_rel);
    }
    U32 A_ApplicationCore::DecreaseLLAInitCounter(U32 Num)
    {
        return _LLAInitCounter.fetch_sub(Num, boost::memory_order_acq_rel);
    }
    U32 A_ApplicationCore::IncreaseLLAReleaseCounter(U32 Num)
    {
        return _LLAReleaseCounter.fetch_add(Num, boost::memory_order_acq_rel);
    }
    U32 A_ApplicationCore::DecreaseLLAReleaseCounter(U32 Num)
    {
        return _LLAReleaseCounter.fetch_sub(Num, boost::memory_order_acq_rel);
    }

    void A_ApplicationCore::SignalShutdown()
    {
        _ShouldShutdown = true;
    }

    void A_ApplicationCore::_SetupCDOTypes()
    {
        const auto& SerializableEnvironment = F_EngineRuntime::GetInstance()->GetSerializableEnvironment();
        
        auto BaseCDOType = TF_ReflectionTypeHandle<A_SerializableObject>(F_ReflectionContext::GetGlobal());
        ABYTEK_ENGINE_NFC_ASSERT(BaseCDOType);
        F_ApplicationModuleContainer::GetInstance()->ForEachUnit(
            [&BaseCDOType, this](const TW_Valid<F_ProgramUnit>& Unit)
            {
                auto Module = Unit.FastCast<F_Module>();
                auto ReflectionSession = Module->GetReflectionSession();
                ReflectionSession->ForEachTypeDerivedFrom(
                    BaseCDOType,
                    [this](const TW_Valid<F_ReflectionType>& Type)
                    {
                        if (Type->IsAbstract())
                        {
                            return true;
                        }
                        _CDOTypes.push_back(Type);
                        return true;
                    }
                );
                return true;
            }
        );
        
        for (const auto& Type : _CDOTypes)
        {
            SerializableEnvironment->AddCDOType(Type);
        }
    }
    void A_ApplicationCore::_CleanUpCDOTypes()
    {
        const auto& SerializableEnvironment = F_EngineRuntime::GetInstance()->GetSerializableEnvironment();
        
        for (const auto& Type : _CDOTypes)
        {
            SerializableEnvironment->RemoveCDOType(Type);
        }
        
        _CDOTypes = {};
    }
}
