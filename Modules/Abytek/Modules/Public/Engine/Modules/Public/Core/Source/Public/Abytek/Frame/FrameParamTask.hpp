#pragma once

#include "Abytek/Engine.Core.prerequisites.hpp"


namespace Abytek
{
    /*
    using F_FrameParamIndex = U32;
    enum class E_FrameParam : F_FrameParamIndex
    {
        MAIN = 0,
        RENDER,
        PHYSICS,
        GPU,

        NUM
    };
    static constexpr F_FrameParamIndex MAX_FRAME_PARAM_DELAY = 2;

    constexpr F_FrameParamIndex GetFrameParamOffsetDirect(E_FrameParam Param)
    {
        return (
            (Param == E_FrameParam::MAIN)
            ? 0
            : (Param == E_FrameParam::RENDER)
            ? 1
            : (Param == E_FrameParam::PHYSICS)
            ? 1
            : 2 // (Param == E_FrameParam::GPU)
        );
    }
    constexpr F_FrameParamIndex GetFrameParamOffset(E_FrameParam Param, E_FrameParam BaseParam = E_FrameParam::MAIN)
    {
        return GetFrameParamOffsetDirect(Param) - GetFrameParamOffsetDirect(BaseParam);
    }
    
    struct F_FrameParamTask
    {
        E_FrameParam Param;
        F_TaskDesc Desc;
        U32 RunnableIndex = -1;
        TS_Unmanaged<F_TaskEvent> BeginEvent;
        TS_Unmanaged<F_TaskEvent> EndEvent;
    };

    struct ABYTEK_ENGINE_CORE_API F_FrameParamQueue
    {
        E_FrameParam Param;
        TF_Array<
            TF_ConcurrentQueue<F_FrameParamTask>,
            MAX_FRAME_PARAM_DELAY
        > TaskLists;
        TS_Unmanaged<F_TaskEvent> BeginEvent;
        TS_Unmanaged<F_TaskEvent> EndEvent;

        TF_ConcurrentQueue<F_FrameParamTask>& GetCurrentTaskList(F_FrameParamIndex Delay = 1)
        { 
            return TaskLists[(MAX_FRAME_PARAM_DELAY - Delay) % MAX_FRAME_PARAM_DELAY];
        }

        void BeginFrame();
        void EndFrame();

        void Schedule(const F_TaskDesc& Desc, U32 RunnableIndex, const TS_Unmanaged<F_TaskEvent>& BeginEvent, const TS_Unmanaged<F_TaskEvent>& EndEvent, F_FrameParamIndex Delay = 1);
        void Schedule(const F_TaskDesc& Desc, U32 RunnableIndex = -1, const TS_Unmanaged<F_TaskEvent>& Event = TS_Unmanaged<F_TaskEvent>()(), F_FrameParamIndex Delay = 1);
    };
    
    class ABYTEK_ENGINE_CORE_API F_FrameParamTaskSubsystem final : public A_ApplicationSubsystem
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationSubsystem)
        ABYTEK_END_REFLECTOR(F_FrameParamTaskSubsystem)

    public:
        ABYTEK_DECLARE_OBJECT_SINGLETON_CRT(F_FrameParamTaskSubsystem);

    private:
        TF_Array<F_FrameParamQueue, static_cast<F_FrameParamIndex>(E_FrameParam::NUM) - 1> _Queues;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetQueues() const noexcept
        {
            return _Queues;
        }
        
    public:
        F_FrameParamTaskSubsystem();
        ~F_FrameParamTaskSubsystem() override;

    protected:
        B8 OnPostInit() override;
        B8 OnPreRelease() override;
        B8 Release() override;

    public:
        void BeginFrame();
        void EndFrame();
        void Schedule(E_FrameParam FrameParam, const F_TaskDesc& Desc, U32 RunnableIndex, const TS_Unmanaged<F_TaskEvent>& BeginEvent, const TS_Unmanaged<F_TaskEvent>& EndEvent, F_FrameParamIndex Delay = 1);
        void Schedule(E_FrameParam FrameParam, const F_TaskDesc& Desc, U32 RunnableIndex = -1, const TS_Unmanaged<F_TaskEvent>& Event = TS_Unmanaged<F_TaskEvent>()(), F_FrameParamIndex Delay = 1);
    };

    struct ABYTEK_ENGINE_CORE_API H_FrameParamTask
    {
        static ABYTEK_FORCE_INLINE void Schedule(E_FrameParam FrameParam, const F_TaskDesc& Desc, U32 RunnableIndex, const TS_Unmanaged<F_TaskEvent>& BeginEvent, const TS_Unmanaged<F_TaskEvent>& EndEvent, F_FrameParamIndex Delay = 1)
        {
            return F_FrameParamTaskSubsystem::GetInstance()->Schedule(
                FrameParam,
                Desc,
                RunnableIndex,
                BeginEvent,
                EndEvent,
                Delay
            );   
        }
        static ABYTEK_FORCE_INLINE void Schedule(E_FrameParam FrameParam, const F_TaskDesc& Desc, U32 RunnableIndex = -1, const TS_Unmanaged<F_TaskEvent>& Event = TS_Unmanaged<F_TaskEvent>()(), F_FrameParamIndex Delay = 1)
        {
            return F_FrameParamTaskSubsystem::GetInstance()->Schedule(
                FrameParam,
                Desc,
                RunnableIndex,
                Event,
                Event,
                Delay
            );   
        }
        template<E_FrameParam __Param, E_FrameParam __CurrentParam = E_FrameParam::MAIN>
        static ABYTEK_FORCE_INLINE void Schedule(
            const F_TaskDesc& Desc,
            U32 RunnableIndex,
            const TS_Unmanaged<F_TaskEvent>& BeginEvent,
            const TS_Unmanaged<F_TaskEvent>& EndEvent
        )
        {
            static_assert(__Param != __CurrentParam, "param and current param must be different");
            return Schedule(
                __Param,
                Desc,
                RunnableIndex,
                BeginEvent,
                EndEvent,
                GetFrameParamOffset(__Param, __CurrentParam)
            );   
        }
        template<E_FrameParam __Param, E_FrameParam __CurrentParam = E_FrameParam::MAIN>
        static ABYTEK_FORCE_INLINE void Schedule(
            const F_TaskDesc& Desc,
            U32 RunnableIndex = -1,
            const TS_Unmanaged<F_TaskEvent>& Event = TS_Unmanaged<F_TaskEvent>()()
        )
        {
            static_assert(__Param != __CurrentParam, "param and current param must be different");
            return Schedule(
                __Param,
                Desc,
                RunnableIndex,
                Event,
                Event,
                GetFrameParamOffset(__Param, __CurrentParam)
            );   
        }
        template<E_FrameParam __Param, typename __F, E_FrameParam __CurrentParam = E_FrameParam::MAIN>
        static ABYTEK_FORCE_INLINE void Cache(__F&& Cache)
        {
            static_assert(__Param != __CurrentParam, "param and current param must be different");
            return Schedule<__Param, __CurrentParam>({
                [Cache2 = ABYTEK_FORWARD(Cache)](U32) {}
            });   
        }
    };
    */
}