#include "Abytek/Runtime.hpp"
#include "Abytek/Console.hpp"
#include "Abytek/Profiling.hpp"
#include "Abytek/TaskManager.hpp"
#include "Abytek/TaskUtilities.hpp"
#include "Abytek/RuntimeLogMonitor.hpp"


namespace Abytek
{
    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(A_Runtime);

    ABYTEK_REFLECT(A_Runtime)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::A_Runtime"));
    }

    void A_Runtime::GatherConfigFilePaths(TF_Vector<F_Text>& FilePaths)
    {
    }

    A_Runtime::A_Runtime()
    {
        ABYTEK_BIND_OBJECT_SINGLETON_CRT();
        
        _LogMonitor = TU<F_RuntimeLogMonitor>()();
    }
    A_Runtime::~A_Runtime()
    {
        _LogMonitor = {};
    }

    void A_Runtime::Start()
    {
        {
            _ReflectionContext = TS<F_ReflectionContext>()();
            F_ReflectionContext::SetGlobal(_ReflectionContext.Weak());
        }
        {
            _ReflectionSession = _ReflectionContext->CreateSession({});
            _ReflectionSession->Begin();
            SetupReflectionSession_Base_Serializable(_ReflectionSession);
            SetupReflectionSession_Base_Math(_ReflectionSession);
            _ReflectionSession->RegisterType<A_Runtime>();
            Reflect();
            _ReflectionSession->End();
        }
        
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
        _Console->Update();
        
        {
            _ConsoleVariable_EnableMemoryLeakDebugging = _Console->RegisterVariable<B8>(
                ABYTEK_NAME("Abytek.EnableMemoryLeakDebugging"), 
                ABYTEK_TEXT("Enable memory leak debugging"),
                false
            );
            _Console->Update();
            if (_ConsoleVariable_EnableMemoryLeakDebugging->GetValue())
            {
                EnableMemoryLeakDebugging();
            }
        }
        
        _TaskManager = TU<F_TaskManager>()();
        _TaskManager->PreConfig(_Console.Weak());
        
        _RegisterLLConsoleVariables();
        _ApplyLLConsoleVariables();
        
        _SerializableEnvironment = TS<F_SerializableEnvironment>()();
        PostCreateSerializableEnvironment();
        
        {
            ABYTEK_PROFILER_THREAD();
        
            F_TaskInstanceSet MainTaskInstanceSet;
            MainTaskInstanceSet.Name = ABYTEK_NAME("Abytek::MainTaskInstance");
            MainTaskInstanceSet.StackSize = E_TaskStackSize::EXTREME;
            MainTaskInstanceSet.Functor = [this]
            {
                ABYTEK_PROFILER_EVENT();
                Main();
            };
            _TaskManager->Start(ABYTEK_MOVE(MainTaskInstanceSet));
            _TaskManager->Join();
        }
        
        PreDestroySerializableEnvironment();
        _SerializableEnvironment = {};
        
        _UnregisterLLConsoleVariables();
        
        _TaskManager = {};
        
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
            
            F_ReflectionContext::SetGlobal({});
            _ReflectionContext = {};
        }
    }

    void A_Runtime::_RegisterLLConsoleVariables()
    {
        _ConsoleVariable_EnableProfiler = _Console->RegisterVariable<B8>(
            ABYTEK_NAME("Abytek.EnableProfiler"), 
            ABYTEK_TEXT("Enable profiler"),
            false
        );
        _ConsoleVariable_EnableObjectTracking = _Console->RegisterVariable<B8>(
            ABYTEK_NAME("Abytek.EnableObjectTracking"), 
            ABYTEK_TEXT("Enable object tracking"),
            false
        );
        _ConsoleVariable_EnableRAObjectPooling = _Console->RegisterVariable<B8>(
            ABYTEK_NAME("Abytek.EnableRAObjectPooling"), 
            ABYTEK_TEXT("Enable RA object pooling"),
            true
        );
        _Console->Update();
        
        if (IsMemoryLeakDebuggingEnabled())
        {
            _ConsoleVariable_EnableProfiler->SetValue(false);
            _ConsoleVariable_EnableObjectTracking->SetValue(true);
            _ConsoleVariable_EnableRAObjectPooling->SetValue(false);
        }
    }
    void A_Runtime::_ApplyLLConsoleVariables()
    {
        if (_ConsoleVariable_EnableProfiler->GetValue())
        {
            ABYTEK_LOG_INFO() << "Found console flag \"Abytek.EnableProfiler\"";
            EnableProfiler();
        }
        if (_ConsoleVariable_EnableObjectTracking->GetValue())
        {
            ABYTEK_LOG_INFO() << "Found console flag \"Abytek.EnableObjectTracking\"";
#ifdef ABYTEK_DEBUG_INFO
            EnableObjectTracking();
#endif
        }
        if (_ConsoleVariable_EnableRAObjectPooling->GetValue())
        {
            ABYTEK_LOG_INFO() << "Found console flag \"Abytek.EnableRAObjectPooling\"";
            EnableRAObjectPooling();
        }
    }
    void A_Runtime::_UnregisterLLConsoleVariables()
    {
        _Console->UnregisterVariable(_ConsoleVariable_EnableRAObjectPooling->GetName());
        _Console->UnregisterVariable(_ConsoleVariable_EnableObjectTracking->GetName());
        _Console->UnregisterVariable(_ConsoleVariable_EnableProfiler->GetName());
    }

    void A_Runtime::GatherReflectionSessions(TF_Vector<TW<F_ReflectionSession>>& Out) const
    {
        Out.push_back(_ReflectionSession);
    }

    void A_Runtime::Reflect()
    {
    }
    void A_Runtime::PostCreateConsole()
    {
    }
    void A_Runtime::PostCreateSerializableEnvironment()
    {
    }
    void A_Runtime::Main()
    {
    }
    void A_Runtime::PreDestroySerializableEnvironment()
    {
    }
    void A_Runtime::PreDestroyConsole()
    {
    }
}
