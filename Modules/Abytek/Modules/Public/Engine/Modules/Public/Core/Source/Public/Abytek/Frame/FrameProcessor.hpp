#pragma once

#include "Abytek/Engine.Core.prerequisites.hpp"
#include "Abytek/Frame/FrameCommon.hpp"


namespace Abytek
{
    class F_Frame;
    
    class ABYTEK_ENGINE_CORE_API F_FrameProcessor final : public A_Object
    {
    private:
        E_FrameParamType _ParamType = E_FrameParamType::NUM;
        F_SpinLock _Lock;
        AU64 _NumStartedFrames = 0;
        AU64 _NumCompletedFrames = 0;
        TS<F_Frame> _CurrentFrame;
        TF_ConcurrentQueue<F_FrameCommand> _LLAQueue;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetParamType() const noexcept
        {
            return _ParamType;
        }
        ABYTEK_FORCE_INLINE const auto& GetCurrentFrame() const noexcept
        {
            return _CurrentFrame;
        }

    public:
        F_FrameProcessor(E_FrameParamType ParamType);
        ~F_FrameProcessor() override;
    
    public:
        void Process(const TS<F_Frame>& Frame);
        
    public:
        void EnqueueLLACommand(F_FrameCommand&& Command);
    };
}
