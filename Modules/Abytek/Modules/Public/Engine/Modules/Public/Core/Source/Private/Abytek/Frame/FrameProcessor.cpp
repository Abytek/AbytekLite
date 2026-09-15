#include "Abytek/Frame/FrameProcessor.hpp"
#include "Abytek/Frame/Frame.hpp"
#include "Abytek/ApplicationCore.hpp"


namespace Abytek
{
    F_FrameProcessor::F_FrameProcessor(E_FrameParamType ParamType) :
        _ParamType(ParamType)
    {
    }
    F_FrameProcessor::~F_FrameProcessor()
    {
    }

    void F_FrameProcessor::Process(const TS<F_Frame>& Frame)
    {
        while (_NumStartedFrames.load(boost::memory_order_acquire) < Frame->GetIndex())
        {
            H_TaskUtilities::Switch();
        }
        
        auto ParamMinimalTaskTag = GetFrameParamMinimalTaskTag();
        auto ParamTypeTaskTag = GetFrameParamTypeTaskTag(_ParamType);
        
        H_TaskUtilities::AddTag(ParamMinimalTaskTag);
        H_TaskUtilities::AddTag(ParamTypeTaskTag);
        
        ABYTEK_PROFILER_EVENT();
        ABYTEK_PROFILER_EVENT_SET_NAME(ABYTEK_TEXT("Abytek::Frame(") + ToText(Frame->GetIndex()) + ABYTEK_TEXT(")::") + *GetFrameParamTypeName(_ParamType));
        
        _CurrentFrame = Frame;
        {
            F_FrameCommand Command;
            while (_LLAQueue.TryPop(Command))
            {
                Command();
                A_ApplicationCore::GetInstance()->DecreaseLLAReleaseCounter();
            }
        }
        Frame->Flush(_ParamType);
        _CurrentFrame = {};
        
        H_TaskUtilities::RemoveTag(ParamTypeTaskTag);
        H_TaskUtilities::RemoveTag(ParamMinimalTaskTag);
        
        _NumStartedFrames.fetch_add(1, boost::memory_order_release);
    }

    void F_FrameProcessor::EnqueueLLACommand(F_FrameCommand&& Command)
    {
        A_ApplicationCore::GetInstance()->IncreaseLLAReleaseCounter();
        _LLAQueue.Push(ABYTEK_MOVE(Command));
    }
}
