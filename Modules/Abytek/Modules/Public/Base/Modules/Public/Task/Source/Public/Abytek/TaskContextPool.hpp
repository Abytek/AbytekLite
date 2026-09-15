#pragma once

#include "Abytek/Base.Task.prerequisites.pch.hpp"


namespace Abytek
{
    class F_TaskContext;
    
    class ABYTEK_BASE_TASK_API F_TaskContextPool final : public A_Object
    {
    private:
        TF_Stack<TS_Unmanaged<F_TaskContext>> _Contexts;
        Sz _StackSize = 0;
        U32 _Capacity = 0;

    public:
        ABYTEK_FORCE_INLINE const auto& GetContexts() const noexcept
        {
            return _Contexts;
        }
        ABYTEK_FORCE_INLINE auto GetStackSize() const noexcept
        {
            return _StackSize;
        }
        ABYTEK_FORCE_INLINE auto GetCapacity() const noexcept
        {
            return _Capacity;
        }
        
    public:
        F_TaskContextPool(Sz StackSize, U32 Capacity = 256);
        ~F_TaskContextPool();

    public:
        TS_Unmanaged<F_TaskContext> Pop();
        void Push(const TS_Unmanaged<F_TaskContext>& Context);
    };
}