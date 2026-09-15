#pragma once

#include "Abytek/Base.Task.prerequisites.pch.hpp"


namespace Abytek
{
    class ABYTEK_BASE_TASK_API F_TaskPromise
    {
    private:
        AU32 _Counter = 0;
#ifdef ABYTEK_DEBUG_INFO
        F_DebugName _DebugName;
#endif

    public:
        ABYTEK_FORCE_INLINE U32 GetCounter() const noexcept
        {
            return _Counter.load(boost::memory_order_acquire);
        }
        ABYTEK_FORCE_INLINE B8 IsDone() const noexcept
        {
            return GetCounter() == 0;
        }
        
    public:
        F_TaskPromise(U32 InitialCounter = 1);
        virtual ~F_TaskPromise();

    public:
        U32 IncreaseCounter(U32 Num = 1) noexcept;
        U32 DecreaseCounter(U32 Num = 1) noexcept;
        
    public:
#ifdef ABYTEK_DEBUG_INFO
        ABYTEK_FORCE_INLINE F_DebugName GetDebugName() const noexcept
        {
            return _DebugName;
        }
        ABYTEK_FORCE_INLINE void SetDebugName(const F_DebugName& Value) noexcept
        {
            _DebugName = Value;
        }
#else
        ABYTEK_FORCE_INLINE F_DebugName GetDebugName() const noexcept
        {
            return {};
        }
        ABYTEK_FORCE_INLINE void SetDebugName(const F_DebugName& Value) noexcept
        {
        }
#endif
    };
}