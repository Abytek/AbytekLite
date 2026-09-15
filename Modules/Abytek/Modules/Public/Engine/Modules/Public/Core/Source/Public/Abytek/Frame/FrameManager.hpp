#pragma once

#include "Abytek/Engine.Core.prerequisites.hpp"
#include "Abytek/Frame/FrameCommon.hpp"


namespace Abytek
{
    class F_FrameProcessor;
    class F_Frame;
    
    struct F_FrameManagerBuildParams
    {
    };
    class ABYTEK_ENGINE_CORE_API F_FrameManager final : public A_Object
    {
    public:
        friend class F_Frame;
        
    public:
        ABYTEK_BEGIN_REFLECTOR()
        ABYTEK_END_REFLECTOR(F_FrameManager)
        
    public:
        ABYTEK_DECLARE_OBJECT_SINGLETON_CRT(F_FrameManager);

    private:
        TF_Vector<TU<F_FrameProcessor>> _Processors;
        TF_ConcurrentQueue<TF_Function<void()>> _LateProcessingQueue;
        U64 _NextFrameIndex = 0;
        AU64 _NumStartedFrames = 0;
        AU64 _NumCompletedFrames = 0;
        U32 _NextTargetWorkerIndex = 0;

    public:
        ABYTEK_FORCE_INLINE const auto& GetProcessors() const noexcept
        {
            return _Processors;
        }
        ABYTEK_FORCE_INLINE auto GetProcessor(E_FrameParamType ParamType = E_FrameParamType::DEFAULT) const noexcept
        {
            return _Processors[static_cast<F_FrameParamTypeIndex>(ParamType)].Weak();
        }

    public:
        F_FrameManager(const F_FrameManagerBuildParams& BuildParams);
        ~F_FrameManager() override;
        
    private:
        void _ProcessRange(const TS<F_Frame>& Frame, F_FrameParamTypeIndex Offset, F_FrameParamTypeIndex Num);
        U64 _IncreaseNumStartedFrames();
        U64 _IncreaseNumCompletedFrames();
        void _FlushLateProcessingQueue();
        
    public:
        void Update(TF_Function<void()>&& Function, E_FramesInFlightMode FramesInFlightMode = E_FramesInFlightMode::DEFAULT);
        void Flush(U64 MinimumNumCompletedFrames);
        void Flush(U64 FrameIndex, E_FramesInFlightMode FramesInFlightMode);
    };
}
