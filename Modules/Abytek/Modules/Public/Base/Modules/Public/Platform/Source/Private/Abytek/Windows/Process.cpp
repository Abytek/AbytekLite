#include "Abytek/Windows/Process.hpp"


#ifdef ABYTEK_PLATFORM_WINDOWS
namespace Abytek
{
    namespace Internal
    {
        HANDLE WindowsCreateKillOnCloseJob() {
            HANDLE job = CreateJobObject(nullptr, nullptr);
            if (!job) return nullptr;

            JOBOBJECT_EXTENDED_LIMIT_INFORMATION info{};
            info.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
            if (!SetInformationJobObject(job, JobObjectExtendedLimitInformation, &info, sizeof(info))) {
                CloseHandle(job);
                return nullptr;
            }
            return job;
        }
    }
    
    F_WindowsProcessLegacy H_WindowsProcessLegacy::Create(const F_ProcessDesc& Desc)
    {
        F_WindowsProcessLegacy Result;

        STARTUPINFO si;
        ZeroMemory( &si, sizeof(si) );
        si.cb = sizeof(si);
        si.dwFlags = STARTF_USESTDHANDLES;
        if (Desc.InheritErrorStream)
        {
            HANDLE ErrorHandle = GetStdHandle(STD_ERROR_HANDLE);
            SetHandleInformation(ErrorHandle, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);
            si.hStdError = ErrorHandle;
        }
        else
        {
            si.hStdError = nullptr;
        }
        if (Desc.InheritInputStream)
        {
            HANDLE InputHandle = GetStdHandle(STD_INPUT_HANDLE);
            SetHandleInformation(InputHandle, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);
            si.hStdInput = InputHandle;
        }
        else
        {
            si.hStdInput = nullptr;
        }
        if (Desc.InheritOutputStream)
        {
            HANDLE OutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
            SetHandleInformation(OutputHandle, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);
            si.hStdOutput = OutputHandle;
        }
        else
        {
            si.hStdOutput = nullptr;
        }
        
        PROCESS_INFORMATION pi;
        ZeroMemory( &pi, sizeof(pi) );

        LPCWSTR ExecutablePathCSTR = 0;
        F_Text CachedCommandLine;
        TCHAR* CommandLineCSTR = 0;

        LPCWSTR CurrentWorkingDirectoryCSTR = 0;

        if(Desc.ExecutablePath)
        {
            ExecutablePathCSTR = Desc.ExecutablePath->c_str();
        }
        if(Desc.CommandLine)
        {
            if (Desc.CommandLine->length())
            {
                CachedCommandLine = *Desc.CommandLine;
                CommandLineCSTR = CachedCommandLine.data();
            }
        }

        if(Desc.CurrentWorkingDirectory) 
        {
            if (Desc.CurrentWorkingDirectory->size() != 0)
            {
                CurrentWorkingDirectoryCSTR = Desc.CurrentWorkingDirectory->c_str();
            }
        }

        ABYTEK_BASE_PLATFORM_ASSERT(ExecutablePathCSTR || CommandLineCSTR) << "require either executable path or command line";

        if(
            !CreateProcess(
                ExecutablePathCSTR,
                CommandLineCSTR,
                NULL,
                NULL,
                true,
                0,
                NULL,
                CurrentWorkingDirectoryCSTR,
                &si,
                &pi
            )
        )
        {
            DWORD error = GetLastError();
            TCHAR errorMsg[MAX_PATH];
            FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                          NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                          errorMsg, sizeof(errorMsg), NULL);
            ABYTEK_LOG_WARNING() << "cannot create process. " << errorMsg;
            return {};
        }

        HANDLE JobHandle = INVALID_HANDLE_VALUE;

        if (Desc.AutomaticallyKill)
        {
            JobHandle = Internal::WindowsCreateKillOnCloseJob();
            if (!JobHandle)
            {
                ABYTEK_LOG_WARNING() << ABYTEK_TEXT("Cannot apply AutomaticallyKill");
                // still close handles
                CloseHandle(pi.hThread);
                CloseHandle(pi.hProcess);
                return {};
            }
            if (!AssignProcessToJobObject(JobHandle, pi.hProcess))
            {
                DWORD error = GetLastError();
                TCHAR errorMsg[MAX_PATH];
                FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                              NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                              errorMsg, sizeof(errorMsg), NULL);
                // handle error (could be ERROR_ACCESS_DENIED if already in job)
                ABYTEK_LOG_WARNING() << ABYTEK_TEXT("Cannot apply AutomaticallyKill: ") << errorMsg;
                // still close handles
                CloseHandle(pi.hThread);
                CloseHandle(pi.hProcess);
                CloseHandle(JobHandle);
                return {};
            }
        }
        
        Result.Internal.ProcessHandle = pi.hProcess;
        Result.Internal.MainThreadHandle = pi.hThread;
        Result.Internal.JobHandle = JobHandle;
        Result.Id = static_cast<Sz>(pi.dwProcessId);

        ABYTEK_LOG_INFO() << "Successfully created process: " << (ExecutablePathCSTR ? ExecutablePathCSTR : CommandLineCSTR);
        return Result;
    }
    void H_WindowsProcessLegacy::Wait(const F_WindowsProcessLegacy& Process)
    {
        WaitForSingleObject(Process.Internal.ProcessHandle, INFINITE);
    }
    void H_WindowsProcessLegacy::Destroy(const F_WindowsProcessLegacy& Process)
    {
        CloseHandle(Process.Internal.ProcessHandle);
        CloseHandle(Process.Internal.MainThreadHandle);
        if (Process.Internal.JobHandle != INVALID_HANDLE_VALUE)
        {
            CloseHandle(Process.Internal.JobHandle);
        }
    }

    F_WindowsProcess::F_WindowsProcess(const F_ProcessDesc& Desc) :
        A_Process(Desc)
    {
        Init();
    }
    F_WindowsProcess::~F_WindowsProcess()
    {
        Destroy();
    }

    F_FeedbackStatus F_WindowsProcess::OnInit(F_ProcessId& OutId)
    {
        STARTUPINFO si;
        ZeroMemory( &si, sizeof(si) );
        si.cb = sizeof(si);
        si.dwFlags = STARTF_USESTDHANDLES;
        if (GetInheritErrorStream())
        {
            HANDLE ErrorHandle = GetStdHandle(STD_ERROR_HANDLE);
            SetHandleInformation(ErrorHandle, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);
            si.hStdError = ErrorHandle;
        }
        else
        {
            si.hStdError = nullptr;
        }
        if (GetInheritInputStream())
        {
            HANDLE InputHandle = GetStdHandle(STD_INPUT_HANDLE);
            SetHandleInformation(InputHandle, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);
            si.hStdInput = InputHandle;
        }
        else
        {
            si.hStdInput = nullptr;
        }
        if (GetInheritOutputStream())
        {
            HANDLE OutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
            SetHandleInformation(OutputHandle, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);
            si.hStdOutput = OutputHandle;
        }
        else
        {
            si.hStdOutput = nullptr;
        }
        
        PROCESS_INFORMATION pi;
        ZeroMemory( &pi, sizeof(pi) );

        LPCWSTR ExecutablePathCSTR = 0;
        F_Text CachedCommandLine;
        TCHAR* CommandLineCSTR = 0;

        LPCWSTR CurrentWorkingDirectoryCSTR = 0;

        auto ExecutablePath = GetExecutablePath();
        auto CommandLine = GetCommandLine_();
        if (!(ExecutablePath || CommandLine))
        {
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Require either executable path or command line"));
        }
        
        auto ParseCommandLine = [](const F_Text& Input)
        {
            F_Text ParsedInput = Input;
            boost::replace_all(ParsedInput, ABYTEK_TEXT("\\"), ABYTEK_TEXT("\\\\"));
            boost::replace_all(ParsedInput, ABYTEK_TEXT("\""), ABYTEK_TEXT("\\\""));
            return ParsedInput;
        };

        F_Text ArgumentsCommandLine;
        for (const auto& Argument : GetArguments())
        {
            if (ArgumentsCommandLine.size() > 0)
            {
                ArgumentsCommandLine += ABYTEK_TEXT(" ");
            }
            F_Text ParsedArgument = Argument;
            if (CommandLine)
            {
                ParsedArgument = ParseCommandLine(ParsedArgument);
                ParsedArgument = ParseCommandLine(ABYTEK_TEXT("\"") + ParsedArgument + ABYTEK_TEXT("\""));
            }
            else
            {
                ParsedArgument = ABYTEK_TEXT("\"") + ParseCommandLine(ParsedArgument) + ABYTEK_TEXT("\"");
            }
            ArgumentsCommandLine += ParsedArgument;
        }
        
        // Use powershell
        F_TextChar PowerShellExecutablePath[] = ABYTEK_TEXT("C:\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe");
        if (CommandLine)
        {
            ExecutablePath = PowerShellExecutablePath;
            CommandLine = (
                F_Text() 
                + ABYTEK_TEXT("-NoProfile -ExecutionPolicy Bypass -Command \"& ") 
                +  ParseCommandLine(ABYTEK_TEXT("\"") + *CommandLine  + ABYTEK_TEXT("\""))
                + ABYTEK_TEXT(" ") 
                + ArgumentsCommandLine 
                + ABYTEK_TEXT("\"")
            );
        }
        else
        {
            CommandLine = ArgumentsCommandLine;
        }

        // Get cstr
        {
            if(ExecutablePath)
            {
                ExecutablePathCSTR = ExecutablePath->c_str();
            }
            if(CommandLine)
            {
                if (CommandLine->length())
                {
                    CachedCommandLine = *CommandLine;
                    CommandLineCSTR = CachedCommandLine.data();
                }
            }
        }

        if(const auto& CurrentWorkingDirectory = GetCurrentWorkingDirectory()) 
        {
            if (CurrentWorkingDirectory->size() != 0)
            {
                CurrentWorkingDirectoryCSTR = CurrentWorkingDirectory->c_str();
            }
        }

        DWORD dwCreationFlags = 0;
#ifdef ABYTEK_ENABLE_WIDE_CHAR_TEXT
        dwCreationFlags |= CREATE_UNICODE_ENVIRONMENT;
#endif
        
        LPVOID lpEnvironment = GetEnvironmentStrings();
        
        if(
            !CreateProcess(
                ExecutablePathCSTR,
                CommandLineCSTR,
                NULL,
                NULL,
                true,
                dwCreationFlags,
                lpEnvironment,
                CurrentWorkingDirectoryCSTR,
                &si,
                &pi
            )
        )
        {
            DWORD error = GetLastError();
            TCHAR errorMsg[MAX_PATH];
            FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                          NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                          errorMsg, sizeof(errorMsg), NULL);
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("cannot create process: ") + ToText(errorMsg));
        }

        HANDLE JobHandle = INVALID_HANDLE_VALUE;

        if (GetAutomaticallyKill())
        {
            JobHandle = Internal::WindowsCreateKillOnCloseJob();
            if (JobHandle == INVALID_HANDLE_VALUE)
            {
                // still close handles
                CloseHandle(pi.hThread);
                CloseHandle(pi.hProcess);
                return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Cannot apply AutomaticallyKill"));
            }
            if (!AssignProcessToJobObject(JobHandle, pi.hProcess))
            {
                DWORD error = GetLastError();
                TCHAR errorMsg[MAX_PATH];
                FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                              NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                              errorMsg, sizeof(errorMsg), NULL);

                // still close handles
                CloseHandle(pi.hThread);
                CloseHandle(pi.hProcess);
                CloseHandle(JobHandle);
                
                return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Cannot apply AutomaticallyKill: ") + ToText(errorMsg));
                return {};
            }
        }
        
        _WinAPIProcessHandle = pi.hProcess;
        _WinAPIMainThreadHandle = pi.hThread;
        _WinAPIJobHandle = JobHandle;
        OutId = static_cast<F_ProcessId>(pi.dwProcessId);
        return F_FeedbackStatus::MakeSucceeded();
    }
    void F_WindowsProcess::OnWait(TF_Optional<F_ProcessExitCode>& OutExitCode)
    {
        WaitForSingleObject(_WinAPIProcessHandle, INFINITE);
        
        DWORD ExitCode;
        if (GetExitCodeProcess(_WinAPIProcessHandle, &ExitCode))
        {
            OutExitCode = static_cast<F_ProcessExitCode>(ExitCode);
        }
    }
    void F_WindowsProcess::OnDestroy()
    {
        CloseHandle(_WinAPIProcessHandle);
        CloseHandle(_WinAPIMainThreadHandle);
        if (_WinAPIJobHandle != INVALID_HANDLE_VALUE)
        {
            CloseHandle(_WinAPIJobHandle);
        }
    }

    F_ProcessId H_WindowsThisProcess::GetId()
    {
        return static_cast<F_ProcessId>(GetCurrentProcessId());
    }
}
#endif