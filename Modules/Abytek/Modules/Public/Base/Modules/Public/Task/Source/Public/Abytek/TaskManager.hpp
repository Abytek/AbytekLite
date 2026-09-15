#pragma once

#include "Abytek/Base.Task.prerequisites.pch.hpp"
#include "Abytek/Task.hpp"
#include "Abytek/Console.hpp"


namespace Abytek
{
    class F_TaskWorker;
    
    enum class E_TaskManagerProfile
    {
        SINGLE_THREADED,
        ULTRA_LOW,
        LOW,
        MEDIUM,
        HIGH,
        ULTRA_HIGH,
        
        DEFAULT = MEDIUM
    };
    struct F_TaskSubsystemConfig
    {
        U32 NumWorkers = 0;
        U32 GroupSize = 0;
        U32 NumGroups = 0;
        U32 NumHighFrequencyWorkers = 0;
        U32 NumLowFrequencyWorkers = 0;
    };
    
    class ABYTEK_BASE_TASK_API F_TaskManager final : public A_Object, public I_TaskScheduler
    {
    public:
        friend class F_TaskWorker;
        
    public:
        ABYTEK_DECLARE_OBJECT_SINGLETON_CRT(F_TaskManager);

    private:
        F_TaskSubsystemConfig _Config;
        TF_Vector<TU<F_TaskWorker>> _OwnedWorkers;
        TF_Vector<TW<F_TaskWorker>> _Workers;
        TS_Unmanaged<F_TaskPromise> _MainPromise;

        AU32 _SimpleScheduleCounter = 0;
        
        TW<TF_ConsoleVariable<E_TaskManagerProfile>> _ConsoleVariable_Profile;
        TW<TF_ConsoleVariable<I32>> _ConsoleVariable_NumWorkers;
        TW<TF_ConsoleVariable<F32>> _ConsoleVariable_LowFrequencyWorkerRatio;
        TW<TF_ConsoleVariable<I32>> _ConsoleVariable_GroupSize;
        
        AU64 _NextTaskId = 0;

    public:
        ABYTEK_FORCE_INLINE const auto& GetConfig() const noexcept
        {
            return _Config;
        }
        ABYTEK_FORCE_INLINE const auto& GetWorkers() const noexcept
        {
            return _Workers;
        }
        ABYTEK_FORCE_INLINE const auto& GetMainPromise() const noexcept
        {
            return _MainPromise;
        }

        ABYTEK_FORCE_INLINE const auto& GetConsoleVariable_Profile() const noexcept
        {
            return _ConsoleVariable_Profile;
        }
        ABYTEK_FORCE_INLINE const auto& GetConsoleVariable_NumWorkers() const noexcept
        {
            return _ConsoleVariable_NumWorkers;
        }
        ABYTEK_FORCE_INLINE const auto& GetConsoleVariable_LowFrequencyWorkerRatio() const noexcept
        {
            return _ConsoleVariable_LowFrequencyWorkerRatio;
        }
        ABYTEK_FORCE_INLINE const auto& GetConsoleVariable_GroupSize() const noexcept
        {
            return _ConsoleVariable_GroupSize;
        }
        
        ABYTEK_FORCE_INLINE auto AcquireNextTaskId(U64 Num = 1)
        {
            return _NextTaskId.fetch_add(Num, boost::memory_order_release);
        }
        
    public:
        F_TaskManager();
        ~F_TaskManager() override;

    public:
        void PreConfig(const TW_Valid<F_Console>& Console);
        void ApplyConsoleVariables();
        void Start(F_TaskInstanceSet&& MainInstanceSet);
        void Join();

    private:
        void _CreateWorkers();
        void _StartWorkers();
        void _DestroyWorkers();

        B8 _Tick();

    protected:
        virtual void OnSchedule(F_TaskInstanceSet&& InstanceSet) override;
    };
}
