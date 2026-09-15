#pragma once

#include "Abytek/Base.Platform.prerequisites.pch.hpp"
#include "Abytek/ProcessBase.hpp"


#ifdef ABYTEK_PLATFORM_WINDOWS
#ifdef ABYTEK_BASE_PLATFORM_ENABLE_IPC
namespace Abytek
{
    /**
     * @brief Windows implementation of process
     * 
     * Extends the abstract process with Windows-specific handles
     */
    struct F_WindowsProcessLegacy : A_ProcessLegacy
    {
        /**
         * @brief Internal data for Windows-specific process
         */
        struct F_Internal
        {
            /** @brief Windows process handle */
            HANDLE ProcessHandle = INVALID_HANDLE_VALUE;
            /** @brief Windows main thread handle */
            HANDLE MainThreadHandle = INVALID_HANDLE_VALUE;
            HANDLE JobHandle = INVALID_HANDLE_VALUE;
        };
        /** @brief Internal Windows-specific data */
        F_Internal Internal;

        /**
         * @brief Boolean cast operator
         * @return True if the process is valid, false otherwise
         */
        ABYTEK_FORCE_INLINE explicit operator B8 () const noexcept
        {
            return (
                static_cast<B8>(*(A_ProcessLegacy*)this)
                && (Internal.ProcessHandle != INVALID_HANDLE_VALUE)
                && (Internal.MainThreadHandle != INVALID_HANDLE_VALUE)
            );
        }
    };

    /**
     * @brief Utility class for Windows process operations
     * 
     * Provides static methods for process creation and management on Windows
     */
    struct ABYTEK_BASE_PLATFORM_API H_WindowsProcessLegacy
    {
        /**
         * @brief Creates a new process
         * @param Desc Description of the process to create
         * @return Process object
         */
        static F_WindowsProcessLegacy Create(const F_ProcessDesc& Desc);
        
        /**
         * @brief Waits for a process to complete
         * @param Process Process to wait for
         */ 
        static void Wait(const F_WindowsProcessLegacy& Process);
        
        /**
         * @brief Destroys a process
         * @param Process Process to destroy
         */
        static void Destroy(const F_WindowsProcessLegacy& Process);
    };
    
    class F_WindowsProcess final : public A_Process
    {
    private:
        HANDLE _WinAPIProcessHandle = INVALID_HANDLE_VALUE;
        HANDLE _WinAPIMainThreadHandle = INVALID_HANDLE_VALUE;
        HANDLE _WinAPIJobHandle = INVALID_HANDLE_VALUE;
        
    public:
        ABYTEK_FORCE_INLINE auto GetWinAPIProcessHandle() const noexcept
        {
            return _WinAPIProcessHandle;
        }
        ABYTEK_FORCE_INLINE auto GetWinAPIMainThreadHandle() const noexcept
        {
            return _WinAPIMainThreadHandle;
        }
        ABYTEK_FORCE_INLINE auto GetWinAPIJobHandle() const noexcept
        {
            return _WinAPIJobHandle;
        }
        
    public:
        F_WindowsProcess(const F_ProcessDesc& Desc);
        virtual ~F_WindowsProcess() override;
        
    public:
        virtual F_FeedbackStatus OnInit(F_ProcessId& OutId);
        virtual void OnWait(TF_Optional<F_ProcessExitCode>& OutExitCode) override;
        virtual void OnDestroy() override;
    };
    
    struct ABYTEK_BASE_PLATFORM_API H_WindowsThisProcess
    {
        static F_ProcessId GetId();
    };
}
#endif
#endif