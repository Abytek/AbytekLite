#include "Abytek/Thread.hpp"
#include "Abytek/Platform.hpp"


namespace Abytek
{
    F_Thread::F_Thread()
    {
    }
    F_Thread::~F_Thread()
    {
    }

    F_Thread::F_Thread(F_Thread&& X) noexcept :
        _BoostThread(
            boost::move(X._BoostThread)
        )
    {
    }
    F_Thread& F_Thread::operator = (F_Thread &&X) noexcept
    {
        _BoostThread = boost::move(X._BoostThread);
        return *this;
    }

    F_Thread::F_Thread(TF_Function<void()>&& Callback, const F_Name& Name) :
        F_Thread(
            boost::thread(
                [CachedCallback = ABYTEK_MOVE(Callback)]
                {
                    A_Platform::GetInstance()->OnBeginCurrentThread();
                    CachedCallback();
                    A_Platform::GetInstance()->OnEndCurrentThread();
                }
            )
        )
    {
        SetName(Name);
    }
    F_Thread::F_Thread(boost::thread&& BoostThread) noexcept :
        _BoostThread(
            boost::move(BoostThread)
        )
    {
    }

    void F_Thread::Join() noexcept
    {
        _BoostThread.join();
    }
    void F_Thread::Detach() noexcept
    {
        _BoostThread.detach();
    }

    void F_Thread::SetName(const F_Name& Name) noexcept
    {
#ifdef ABYTEK_PLATFORM_WINDOWS
        if (Name)
        {
            HANDLE WinAPIThreadHandle = _BoostThread.native_handle();
            SetThreadDescription(
                WinAPIThreadHandle,
                Name->c_str()
            );
        }
        _Name = Name;
#endif
    }

    F_ThreadId H_ThisThread::GetId() noexcept
    {
        return boost::this_thread::get_id();
    }
    void H_ThisThread::YieldThread() noexcept
    {
        boost::this_thread::yield();
    }

    F_Name H_ThisThread::GetName() noexcept
    {
#ifdef ABYTEK_PLATFORM_WINDOWS
        PWSTR ThreadName_WStr;
        HRESULT HR = GetThreadDescription(
            GetCurrentThread(),
            &ThreadName_WStr
        );
        if (SUCCEEDED(HR))
        {
            F_Text Result(ThreadName_WStr);
            LocalFree(ThreadName_WStr);
            return Result;
        }
#endif
        return {};
    }
    void H_ThisThread::SetName(const F_Name& Name) noexcept
    {
#ifdef ABYTEK_PLATFORM_WINDOWS
        if (Name)
        {
            SetThreadDescription(
                GetCurrentThread(),
                Name->c_str()
            );
        }
#endif
    }
}
