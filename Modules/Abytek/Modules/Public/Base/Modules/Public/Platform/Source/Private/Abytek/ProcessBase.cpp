#include "Abytek/ProcessBase.hpp"
#include "Abytek/Windows/Process.hpp"


namespace Abytek
{
    A_Process::A_Process(const F_ProcessDesc& Desc) :
        _ExecutablePath(Desc.ExecutablePath),
        _CommandLine(Desc.CommandLine),
        _Arguments(Desc.Arguments),
        _CurrentWorkingDirectory(Desc.CurrentWorkingDirectory),
    
        _InheritErrorStream(Desc.InheritErrorStream),
        _InheritInputStream(Desc.InheritInputStream),
        _InheritOutputStream(Desc.InheritOutputStream),
    
        _AutomaticallyKill(Desc.AutomaticallyKill)
    {
    }
    A_Process::~A_Process()
    {
    }

    F_FeedbackStatus A_Process::Create(const F_ProcessDesc& Desc, TU<A_Process>& OutProcess)
    {
        TU<A_Process> Process;

#ifdef ABYTEK_PLATFORM_WINDOWS
        Process = TU<F_WindowsProcess>()(Desc);
#elif defined(ABYTEK_PLATFORM_ANDROID)
        ABYTEK_LOG_FATAL() << "Android platform does not support process";
#else
#error "Unknown platform"
#endif
        const auto& InitializationStatus = Process->GetInitializationStatus(); 
        if (Process->GetInitializationStatus())
        {
            OutProcess = ABYTEK_MOVE(Process);
        }
        return InitializationStatus;
    }

    void A_Process::Init()
    {
        _InitializationStatus = OnInit(_Id);
    }

    void A_Process::Wait()
    {
        if (!_InitializationStatus)
        {
            return;
        }
        
        if (_State >= E_ProcessState::EXITED)
        {
            return;
        }
        OnWait(_ExitCode);
        _State = E_ProcessState::EXITED;
    }
    void A_Process::Destroy()
    {
        if (!_InitializationStatus)
        {
            return;
        }
        
        Wait();
        
        if (_State >= E_ProcessState::DESTROYED)
        {
            return;
        }
        OnDestroy();
        _State = E_ProcessState::DESTROYED;
    }
}
