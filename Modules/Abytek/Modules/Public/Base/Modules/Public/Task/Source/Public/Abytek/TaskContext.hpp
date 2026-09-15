#pragma once

#include "Abytek/Base.Task.prerequisites.pch.hpp"
#include "Abytek/TaskCondition.hpp"
#include "Abytek/Task.hpp"


namespace Abytek
{
    class F_TaskContextPool;

    class ABYTEK_BASE_TASK_API F_TaskContext final
    {
    public:
        friend class F_TaskWorker;
        
    private:
        TW<F_TaskContextPool> _Pool;
        F_Name _Name;
        F_String _NameString;
        B8 _HasInstanceSet = false;
        U32 _InstanceIndex = 0;
        F_TaskInstanceSet _InstanceSet;
        TF_Vector<TF_Vector<F_Name>> _InstanceIndexToTags;
        F_TaskId _BeginTaskId = 0;
        F_Fiber _Fiber;
        B8 _ShouldContinue = true;

        A_TaskCondition* _Condition = nullptr;
        
        F_Fiber _ParentFiber_Temp;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetPool() const noexcept
        {
            return _Pool;
        }
        ABYTEK_FORCE_INLINE const auto& GetName() const noexcept
        {
            return _Name;
        }
        ABYTEK_FORCE_INLINE const auto& GetNameString() const noexcept
        {
            return _NameString;
        }
        ABYTEK_FORCE_INLINE B8 HasInstanceSet() const noexcept
        {
            return _HasInstanceSet;
        }
        ABYTEK_FORCE_INLINE U32 GetInstanceIndex() const noexcept
        {
            return _InstanceIndex;
        }
        ABYTEK_FORCE_INLINE const auto& GetInstanceSet() const noexcept
        {
            return _InstanceSet;
        }
        ABYTEK_FORCE_INLINE const auto& GetInstanceIndexToTags() const noexcept
        {
            return _InstanceIndexToTags;
        }
        ABYTEK_FORCE_INLINE auto& GetInstanceIndexToTags() noexcept
        {
            return _InstanceIndexToTags;
        }
        ABYTEK_FORCE_INLINE auto GetBeginTaskId() const noexcept
        {
            return _BeginTaskId;
        }
        ABYTEK_FORCE_INLINE const auto& GetFiber() const noexcept
        {
            return _Fiber;
        }
        ABYTEK_FORCE_INLINE const auto& GetShouldContinue() const noexcept
        {
            return _ShouldContinue;
        }

        ABYTEK_FORCE_INLINE const auto& GetCondition() const noexcept
        {
            return *_Condition;
        }

    public:
        F_TaskContext(const TW_Valid<F_TaskContextPool>& Pool);
        ~F_TaskContext();

    private:
        void _SetupTask(F_TaskInstanceSet&& InInstanceSet);
        template<typename __F_PassedSplitCallback>
        B8 _Resume(__F_PassedSplitCallback&& PassedSplitCallback)
        {
            if (_Condition)
            {
                if (!(_Condition->ShouldContinue()))
                {
                    return true;
                }
            }
            
            _SetThisAsCurrent();
            _Fiber = ABYTEK_MOVE(_Fiber).Resume();
            _UnsetThisFromCurrent();

            U32 SplitedInstanceSetCount = _InstanceSet.Count - _InstanceIndex - 1;
            if (_HasInstanceSet && SplitedInstanceSetCount)
            {
                F_TaskInstanceSet SplitedInstanceSet = _InstanceSet;
                SplitedInstanceSet.Offset = _InstanceIndex + 1;
                SplitedInstanceSet.Count = SplitedInstanceSetCount;
                _InstanceSet.Count = _InstanceSet.Count - SplitedInstanceSetCount;
                PassedSplitCallback(ABYTEK_MOVE(SplitedInstanceSet));
            }
            return _HasInstanceSet;
        }

        F_Fiber _Loop(F_Fiber&& ParentFiber);
        void _SetThisAsCurrent();
        void _UnsetThisFromCurrent();

    public:
        void ChangePriority(E_TaskPriority NewPriority);
        void Switch();
        template<typename __F_Condition>
        void Wait(__F_Condition&& Condition = {})
        {
            _Condition = &Condition;
            Switch();
            _Condition = nullptr;
        }

    public:
        static F_TaskContext& GetCurrentRef();
        static U32 GetCurrentInstanceIndex();
    };
}