#include "Abytek/Frame/Frame.hpp"
#include "Abytek/Frame/FrameManager.hpp"
#include "Abytek/Frame/FrameProcessor.hpp"


namespace Abytek
{
    F_Frame::F_Frame(E_FramesInFlightMode FramesInFlightMode, U64 Index) :
        _FramesInFlightMode(FramesInFlightMode),
        _Index(Index)
    {
        _Queues.resize(static_cast<F_FrameParamTypeIndex>(E_FrameParamType::NUM));
        if (FramesInFlightMode == E_FramesInFlightMode::NONE)
        {
            _LateFlushMode = E_FramesInFlightMode::NONE;
        }
        _Arena = TU_Unmanaged<F_Arena>()(Sz(0x10000));
    }
    F_Frame::~F_Frame()
    {
        _Arena = {};
    }

    void F_Frame::EnqueueCommand(F_FrameCommand&& Command, E_FrameParamType ParamType)
    {
        _Queues[static_cast<F_FrameParamTypeIndex>(ParamType)].Push(ABYTEK_MOVE(Command));
    }

    void F_Frame::Flush(E_FrameParamType ParamType)
    {
        ABYTEK_ENGINE_CORE_ASSERT(ParamType == _CurrentParamType);
        ABYTEK_ENGINE_CORE_ASSERT(ParamType < E_FrameParamType::NUM);
        auto& Queue = _Queues[static_cast<F_FrameParamTypeIndex>(ParamType)];
        if (ParamType == E_FrameParamType::FINAL)
        {
            auto MainFrameProcessor = F_FrameManager::GetInstance()->GetProcessor(E_FrameParamType::MAIN);
            F_FrameCommand Command;
            while (Queue.TryPop(Command))
            {
                MainFrameProcessor->EnqueueLLACommand(ABYTEK_MOVE(Command));
            }
        }
        else
        {
            F_FrameCommand Command;
            while (Queue.TryPop(Command))
            {
                Command();
                Command = {};
            }
        }
        _CurrentParamType = static_cast<E_FrameParamType>(
            static_cast<F_FrameParamTypeIndex>(ParamType) + 1
        );
        if (IsCompleted())
        {
            F_FrameManager::GetInstance()->_IncreaseNumCompletedFrames();
        }
    }
}
