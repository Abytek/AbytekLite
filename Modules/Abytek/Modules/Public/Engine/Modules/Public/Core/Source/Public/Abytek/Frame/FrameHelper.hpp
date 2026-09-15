#pragma once

#include "Abytek/Engine.Core.prerequisites.hpp"
#include "Abytek/Frame/FrameCommon.hpp"
#include "Abytek/Frame/Frame.hpp"
#include "Abytek/Frame/FrameProcessor.hpp"
#include "Abytek/Frame/FrameManager.hpp"
#include "Abytek/EngineRuntime.hpp"


namespace Abytek
{
    struct ABYTEK_ENGINE_CORE_API H_Frame
    {
    public:
        static TS<F_Frame> Get(E_FrameParamType CurrentParamType = E_FrameParamType::DEFAULT);
        static auto GetArena(E_FrameParamType CurrentParamType = E_FrameParamType::DEFAULT)
        {
            return Get(CurrentParamType)->GetArena().Weak();
        }
        
    public:
        template<E_FrameParamType __CurrentParamType = E_FrameParamType::DEFAULT>
        static E_FramesInFlightMode GetFramesInFlightMode(E_FrameParamType CurrentParamType = __CurrentParamType)
        {
            return Get(CurrentParamType)->GetFramesInFlightMode();
        }
        template<E_FrameParamType __CurrentParamType = E_FrameParamType::DEFAULT>
        static U32 GetNumFramesInFlight(E_FrameParamType CurrentParamType = __CurrentParamType)
        {
            return ResolveNumFramesInFlight(
                GetFramesInFlightMode(CurrentParamType)    
            );
        }
        template<E_FrameParamType __CurrentParamType = E_FrameParamType::DEFAULT>
        static U64 GetIndex(E_FrameParamType CurrentParamType = __CurrentParamType)
        {
            return Get(CurrentParamType)->GetIndex();
        }
        
    public:
        static void EnqueueCommand(F_FrameCommand&& Command, E_FrameParamType ParamType, E_FrameParamType CurrentParamType = E_FrameParamType::DEFAULT)
        {
            ABYTEK_ENGINE_CORE_ASSERT((static_cast<F_FrameParamTypeIndex>(ParamType) - static_cast<F_FrameParamTypeIndex>(CurrentParamType)) != 0);
            if ((static_cast<F_FrameParamTypeIndex>(ParamType) - static_cast<F_FrameParamTypeIndex>(CurrentParamType)) == 1)
            {
                Get(CurrentParamType)->EnqueueCommand(ABYTEK_MOVE(Command), ParamType);
                return;
            }
            
            E_FrameParamType NextFrameParamType = static_cast<E_FrameParamType>(static_cast<F_FrameParamTypeIndex>(CurrentParamType) + 1);
            Get(CurrentParamType)->EnqueueCommand(
                [CachedCommand = ABYTEK_MOVE(Command), ParamType, NextFrameParamType] () mutable
                {
                    EnqueueCommand(ABYTEK_MOVE(CachedCommand), ParamType, NextFrameParamType);
                },
                NextFrameParamType
            );
        }
        template<E_FrameParamType ParamType, E_FrameParamType __CurrentParamType = E_FrameParamType::DEFAULT>
        static void EnqueueCommand(F_FrameCommand&& Command, E_FrameParamType CurrentParamType = __CurrentParamType)
        {
            EnqueueCommand(ABYTEK_MOVE(Command), ParamType, CurrentParamType);
        }
        
    public:
        static void Flush(E_FramesInFlightMode FramesInFlightMode = E_FramesInFlightMode::NONE, E_FrameParamType CurrentParamType = E_FrameParamType::DEFAULT);
        template<E_FramesInFlightMode __FramesInFlightMode = E_FramesInFlightMode::NONE, E_FrameParamType __CurrentParamType = E_FrameParamType::DEFAULT>
        static void Flush(E_FrameParamType CurrentParamType = __CurrentParamType)
        {
            Flush(__FramesInFlightMode, CurrentParamType);
        }
        
    public:
        static void LateFlush(E_FramesInFlightMode FramesInFlightMode = E_FramesInFlightMode::NONE);
        template<E_FramesInFlightMode __FramesInFlightMode = E_FramesInFlightMode::NONE>
        static void LateFlush()
        {
            LateFlush(__FramesInFlightMode);
        }
    };
}
 
#define ABYTEK_CHECK_FRAME_PARAM_TYPE_ANY() \
            ABYTEK_CHECK_TASK_TAG(Abytek::GetFrameParamMinimalTaskTag())
 
#define ABYTEK_CHECK_NOT_FRAME_PARAM_TYPE_ANY() \
            ABYTEK_CHECK_NOT_TASK_TAG(Abytek::GetFrameParamMinimalTaskTag())

#define ABYTEK_CHECK_FRAME_PARAM_TYPE(...) \
            ABYTEK_CHECK_TASK_TAG(Abytek::GetFrameParamTypeTaskTag(__VA_ARGS__))

#define ABYTEK_CHECK_NOT_FRAME_PARAM_TYPE(...) \
            ABYTEK_CHECK_NOT_TASK_TAG(Abytek::GetFrameParamTypeTaskTag(__VA_ARGS__))