#pragma once

#include "Abytek/Base.Task.prerequisites.pch.hpp"
#include "Abytek/TaskQueue.hpp"


namespace Abytek
{
    class F_TaskContextPool;
    
    enum class E_TaskWorkerFlag : U8
    {
        NONE = 0x0,
        LOW_FREQUENCY = 0x1,
        HIGH_FREQUENCY = 0x2,
        MAIN_THREAD = 0x4,
        DEDICATED_THREAD = 0x8,
        
        DEFAULT = NONE
    };
    ABYTEK_DEFINE_FLAG_OPERATORS(E_TaskWorkerFlag);
    
    class ABYTEK_BASE_TASK_API F_TaskWorker final :
        public A_Object, 
        public I_TaskScheduler
    {
    public:
        friend class F_TaskManager;

    private:
        U32 _Index = 0;
        E_TaskWorkerFlag _Flags = E_TaskWorkerFlag::DEFAULT;
        F_Name _Name;
        F_String _NameString;
        F_Thread _Thread;
        AB8 _ShouldContinue = false;
        F_TaskQueue _Queue;
        TF_Array<TS<F_TaskContextPool>, static_cast<F_TaskStackSizeIndex>(E_TaskStackSize::NUM)> _ContextPools;

        U32 _MaxIdleTicksToSleep;
        U32 _IdleTickCounter = 0;
        
    public:
        ABYTEK_FORCE_INLINE U32 GetIndex() const noexcept { return _Index; }
        ABYTEK_FORCE_INLINE E_TaskWorkerFlag GetFlags() const noexcept { return _Flags; }
        ABYTEK_FORCE_INLINE const auto& GetName() const noexcept { return _Name; }
        ABYTEK_FORCE_INLINE const auto& GetNameString() const noexcept { return _NameString; }
        ABYTEK_FORCE_INLINE const auto& GetThread() const noexcept { return _Thread; }
        ABYTEK_FORCE_INLINE auto& GetQueue() noexcept { return _Queue; }
        ABYTEK_FORCE_INLINE const auto& GetQueue() const noexcept { return _Queue; }
        ABYTEK_FORCE_INLINE const auto& GetContextPools() const noexcept { return _ContextPools; }
        ABYTEK_FORCE_INLINE B8 IsMain() const noexcept
        {
            return _Index == 0;
        }

    public:
        F_TaskWorker(U32 Index, E_TaskWorkerFlag Flags);
        ~F_TaskWorker();

    private:
        void _Start();
        void _Join();
        void _Bind();
        void _Unbind();
        void _Tick();
        B8 _ProcessTasks();

    public:
        static TW<F_TaskWorker> GetCurrent();
        static TW<F_TaskWorker> Search(E_TaskWorkerFlag Flags = E_TaskWorkerFlag::DEFAULT, U32 SearchedIndex = 0);

    protected:
        virtual void OnSchedule(F_TaskInstanceSet&& InstanceSet) override;
    };
}
