#include "Abytek/POSIX/Process.hpp"

#ifdef ABYTEK_PLATFORM_POSIX
#ifdef ABYTEK_BASE_PLATFORM_ENABLE_IPC

#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <cstring>
#include <cstdlib>

#if defined(__linux__) || defined(__ANDROID__)
#include <sys/prctl.h>
#endif

namespace Abytek
{

    F_POSIXProcess H_POSIXProcess::Create(const F_ProcessDesc& Desc)
    {
        F_POSIXProcess Result{};

        ABYTEK_BASE_PLATFORM_ASSERT(
            Desc.ExecutablePath.has_value() ^ Desc.CommandLine.has_value()
        ) << "Exactly one execution mode must be chosen";

        pid_t pid = fork();

        if (pid < 0)
        {
            ABYTEK_LOG_WARNING() << "fork() failed" << F_NewLine{};
            return {};
        }

        if (pid == 0)
        {
            // -------------------------
            // CHILD PROCESS
            // -------------------------

#if defined(__linux__) || defined(__ANDROID__)
            if (Desc.AutomaticallyKill)
            {
                prctl(PR_SET_PDEATHSIG, SIGKILL);
            }
#endif

            // Stream handling (inheritance is default in POSIX)
            if (!Desc.InheritInputStream)
                close(STDIN_FILENO);

            if (!Desc.InheritOutputStream)
                close(STDOUT_FILENO);

            if (!Desc.InheritErrorStream)
                close(STDERR_FILENO);

            if (Desc.CurrentWorkingDirectory && !Desc.CurrentWorkingDirectory->empty())
            {
                chdir(ToString(*Desc.CurrentWorkingDirectory).c_str());
            }

            if (Desc.ExecutablePath)
            {
                auto StringExecutablePath = ToString(*Desc.ExecutablePath);
                execl(
                    StringExecutablePath.c_str(),
                    StringExecutablePath.c_str(),
                    (char*)nullptr
                );
            }
            else if (Desc.CommandLine)
            {
                execl(
                    "/bin/sh",
                    "sh",
                    "-c",
                    ToString(*Desc.CommandLine).c_str(),
                    (char*)nullptr
                );
            }

            // If exec fails:
            _exit(EXIT_FAILURE);
        }

        // -------------------------
        // PARENT PROCESS
        // -------------------------

        Result.Internal.ProcessId = pid;
        Result.Id = static_cast<Sz>(pid);

        ABYTEK_LOG_INFO()
            << "Successfully created POSIX process: PID=" << pid
            << F_NewLine{};

        return Result;
    }

    void H_POSIXProcess::Wait(const F_POSIXProcess& Process)
    {
        if (Process.Internal.ProcessId <= 0)
            return;

        int status = 0;
        waitpid(Process.Internal.ProcessId, &status, 0);
    }

    void H_POSIXProcess::Destroy(F_POSIXProcess& Process)
    {
        if (Process.Internal.ProcessId <= 0)
            return;

        kill(Process.Internal.ProcessId, SIGTERM);

        int status = 0;
        waitpid(Process.Internal.ProcessId, &status, 0);

        Process.Internal.ProcessId = -1;
    }

    F_ProcessId H_POSIXThisProcess::GetId()
    {
        return static_cast<F_ProcessId>(getpid());
    }
} // namespace Abytek

#endif
#endif