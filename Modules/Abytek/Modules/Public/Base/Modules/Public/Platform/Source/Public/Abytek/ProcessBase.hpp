#pragma once

#include "Abytek/Base.Platform.prerequisites.pch.hpp"
#include "Abytek/Path.hpp"


#ifdef ABYTEK_BASE_PLATFORM_ENABLE_IPC
namespace Abytek
{
    using F_ProcessId = Sz;
    using F_ProcessExitCode = U32;
    
    /**
     * @brief Description for creating a new process
     * 
     * Contains parameters used when creating a new process
     */
    struct F_ProcessDesc
    {
        TF_Optional<F_Text> ExecutablePath;
        TF_Optional<F_Text> CommandLine;
        TF_Vector<F_Text> Arguments;
        TF_Optional<F_Text> CurrentWorkingDirectory;
        
        B8 InheritErrorStream = true;
        B8 InheritInputStream = true;
        B8 InheritOutputStream = true;
        
        B8 AutomaticallyKill = true;
    };

    /**
     * @brief Base class for platform-specific process implementations
     * 
     * Provides common functionality for process handling
     */
    struct A_ProcessLegacy
    {
        F_ProcessId Id;
        
        /**
         * @brief Boolean cast operator
         * @return True if the process is valid, false otherwise
         */
        ABYTEK_FORCE_INLINE explicit operator B8 () const noexcept
        {
            return true;
        }
    };
    
    enum class E_ProcessState : U8
    {
        RUNNING,
        EXITED,
        DESTROYED
    };
    
    class A_Process : public A_Object
    {
    private:
        TF_Optional<F_Text> _ExecutablePath;
        TF_Optional<F_Text> _CommandLine;
        TF_Vector<F_Text> _Arguments;
        TF_Optional<F_Text> _CurrentWorkingDirectory;
        
        B8 _InheritErrorStream = true;
        B8 _InheritInputStream = true;
        B8 _InheritOutputStream = true;
        
        B8 _AutomaticallyKill = true;
        
        F_FeedbackStatus _InitializationStatus;
        
        E_ProcessState _State = E_ProcessState::RUNNING;
        F_ProcessId _Id = 0;
        TF_Optional<F_ProcessExitCode> _ExitCode;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetExecutablePath() const noexcept
        {
            return _ExecutablePath;
        }
        ABYTEK_FORCE_INLINE const auto& GetCommandLine_() const noexcept
        {
            return _CommandLine;
        }
        ABYTEK_FORCE_INLINE const auto& GetArguments() const noexcept
        {
            return _Arguments;
        }
        ABYTEK_FORCE_INLINE const auto& GetCurrentWorkingDirectory() const noexcept
        {
            return _CurrentWorkingDirectory;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetInheritErrorStream() const noexcept
        {
            return _InheritErrorStream;
        }
        ABYTEK_FORCE_INLINE const auto& GetInheritInputStream() const noexcept
        {
            return _InheritInputStream;
        }
        ABYTEK_FORCE_INLINE const auto& GetInheritOutputStream() const noexcept
        {
            return _InheritOutputStream;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetAutomaticallyKill() const noexcept
        {
            return _AutomaticallyKill;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetInitializationStatus() const noexcept
        {
            return _InitializationStatus;
        }
        
        ABYTEK_FORCE_INLINE auto GetState() const noexcept
        {
            ABYTEK_BASE_PLATFORM_ASSERT(GetInitializationStatus()) << "Cannot get state on invalid process";
            return _State;
        }
        ABYTEK_FORCE_INLINE const auto& GetExitCode() const noexcept
        {
            ABYTEK_BASE_PLATFORM_ASSERT(GetInitializationStatus()) << "Cannot get exit code on invalid process";
            return _ExitCode;
        }
        ABYTEK_FORCE_INLINE const auto& GetId() const noexcept
        {
            ABYTEK_BASE_PLATFORM_ASSERT(GetInitializationStatus()) << "Cannot get id on invalid process";
            return _Id;
        }
        
    protected:
        A_Process(const F_ProcessDesc& Desc);
        
    public:
        virtual ~A_Process() override;
        
    public:
        static F_FeedbackStatus Create(const F_ProcessDesc& Desc, TU<A_Process>& OutProcess);
        
    protected:
        virtual F_FeedbackStatus OnInit(F_ProcessId& OutId) = 0;
        virtual void OnWait(TF_Optional<F_ProcessExitCode>& OutExitCode) = 0;
        virtual void OnDestroy() = 0;
        
    protected:
        virtual void Init();
        
    public:
        virtual void Wait();
        virtual void Destroy();
    };
    
#ifdef ABYTEK_PLATFORM_WINDOWS
    struct H_WindowsThisProcess;
    using H_ThisProcess = H_WindowsThisProcess;
#elif defined(ABYTEK_PLATFORM_POSIX)
    struct H_POSIXThisProcess;
    using H_ThisProcess = H_POSIXThisProcess;
#else
#error Unknown platform
#endif
}
#endif