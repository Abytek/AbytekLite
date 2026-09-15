#pragma once

#include "Abytek/Base.Task.prerequisites.pch.hpp"
#include "Abytek/TaskPromise.hpp"
#include "Abytek/TaskWorker.hpp"


namespace Abytek
{
    class ABYTEK_BASE_TASK_API A_TaskLoop : public A_Object
    {
    private:
        TW_Valid<F_TaskWorker> _Worker;
        E_TaskPriority _Priority;
        F_Name _Name;
        TS_Unmanaged<F_TaskPromise> _Promise;
        AB8 _IsStarted = false;
        AB8 _ShouldContinue = true;
        AB8 _IsStopped = false;

    public:
        ABYTEK_FORCE_INLINE const auto& GetWorker() const noexcept
        {
            return _Worker;
        }
        ABYTEK_FORCE_INLINE auto GetPriority() const noexcept
        {
            return _Priority;
        }
        ABYTEK_FORCE_INLINE const auto& GetName() const noexcept
        {
            return _Name;
        }
        ABYTEK_FORCE_INLINE const auto& GetPromise() const noexcept
        {
            return _Promise;
        }
        ABYTEK_FORCE_INLINE auto IsStarted() const noexcept
        {
            return _IsStarted.load(boost::memory_order_acquire);
        }
        ABYTEK_FORCE_INLINE auto ShouldContinue() const noexcept
        {
            return _ShouldContinue.load(boost::memory_order_acquire);
        }
        ABYTEK_FORCE_INLINE auto IsStopped() const noexcept
        {
            return _IsStopped.load(boost::memory_order_acquire);
        }
        
    protected:
        A_TaskLoop(const TW_Valid<F_TaskWorker>& Worker, E_TaskPriority Priority = E_TaskPriority::DEFAULT, const F_Name& Name = {});

    public:
        virtual ~A_TaskLoop();

    public:
        void Start();
        void SignalStop();
        void Join();

    protected:
        virtual void Bind();
        virtual void Tick();
        virtual void Unbind();
    };
}
