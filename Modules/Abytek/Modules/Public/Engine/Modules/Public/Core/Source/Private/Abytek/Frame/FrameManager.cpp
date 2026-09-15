#include "Abytek/Frame/FrameManager.hpp"
#include "Abytek/Frame/Frame.hpp"
#include "Abytek/Frame/FrameProcessor.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_FrameManager)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_FrameManager"));
    }

    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(F_FrameManager);
    
    F_FrameManager::F_FrameManager(const F_FrameManagerBuildParams& BuildParams)
    {
        ABYTEK_BIND_OBJECT_SINGLETON_CRT();
    
        for (F_FrameParamTypeIndex ParamTypeIndex = 0; ParamTypeIndex < static_cast<F_FrameParamTypeIndex>(E_FrameParamType::NUM); ++ParamTypeIndex)
        {
            _Processors.push_back(TU<F_FrameProcessor>()(static_cast<E_FrameParamType>(ParamTypeIndex)));
        }
    }
    F_FrameManager::~F_FrameManager()
    {
        _Processors = {};
    }

    void F_FrameManager::_ProcessRange(const TS<F_Frame>& Frame, F_FrameParamTypeIndex Offset, F_FrameParamTypeIndex Num)
    {
        auto Begin = Offset;
        auto End = Offset + Num;
        for (F_FrameParamTypeIndex ParamTypeIndex = Begin; ParamTypeIndex < End; ++ParamTypeIndex)
        {
            _Processors[ParamTypeIndex]->Process(Frame);
            H_TaskUtilities::Switch();
        }
    }
    U64 F_FrameManager::_IncreaseNumStartedFrames()
    {
        return _NumStartedFrames.fetch_add(1, boost::memory_order_release);
    }
    U64 F_FrameManager::_IncreaseNumCompletedFrames()
    {
        return _NumCompletedFrames.fetch_add(1, boost::memory_order_release);
    }
    void F_FrameManager::_FlushLateProcessingQueue()
    {
        TF_Function<void()> Command;
        while (_LateProcessingQueue.TryPop(Command))
        {
            Command();
        }
    }

    void F_FrameManager::Update(TF_Function<void()>&& Function, E_FramesInFlightMode FramesInFlightMode)
    {
        U64 FrameIndex = _NextFrameIndex;
        ++_NextFrameIndex;
        auto Frame = TS<F_Frame>()(FramesInFlightMode, FrameIndex);
        Frame->EnqueueCommand(ABYTEK_MOVE(Function), E_FrameParamType::MAIN);
        
        //
        U32 NumFramesInFlight = ResolveNumFramesInFlight(FramesInFlightMode);
        U32 MaxNumFramesInFlight = ResolveNumFramesInFlight(E_FramesInFlightMode::MAX);
        U32 NumParamTypes = static_cast<F_FrameParamTypeIndex>(E_FrameParamType::NUM);
        
        //
        U32 NumParamTypesForLateProcessing = 1;
        U32 NumParamTypesOnMain = (
            MaxNumFramesInFlight
            - NumFramesInFlight
            + 1
        );
        if (FramesInFlightMode == E_FramesInFlightMode::NONE)
        {
            NumParamTypesOnMain = NumParamTypes - NumParamTypesForLateProcessing;
        }
        U32 NumParamTypesForParallelUpdate = NumParamTypes - NumParamTypesOnMain - NumParamTypesForLateProcessing;
        
        // Make sure that there is at least 1 available frame to execute
        Flush(FrameIndex, FramesInFlightMode);
        
        // main
        _ProcessRange(
            Frame, 
            0,
            static_cast<F_FrameParamTypeIndex>(NumParamTypesOnMain)
        );
        
        // parallel update
        TW<F_TaskWorker> TargetWorker;
        {
            const auto& Workers = F_TaskManager::GetInstance()->GetWorkers();
            U32 NumWorker = Workers.size();
            for (U32 Idx = 0; Idx < NumWorker; ++Idx)
            {
                U32 WorkerIdx = (_NextTargetWorkerIndex + Idx) % NumWorker;
                const auto& Worker = Workers[Idx];
                auto WorkerFlags = Worker->GetFlags();
                if (
                    FlagHas(
                        WorkerFlags,
                        E_TaskWorkerFlag::MAIN_THREAD
                    )    
                )
                {
                    continue;
                }
                if (
                    FlagHas(
                        WorkerFlags,
                        E_TaskWorkerFlag::DEDICATED_THREAD
                        | E_TaskWorkerFlag::HIGH_FREQUENCY
                    )    
                )
                {
                    TargetWorker = Worker;
                    _NextTargetWorkerIndex = WorkerIdx + 1;
                    break;
                }
            }
            if (!TargetWorker)
            {
                TargetWorker = F_TaskWorker::GetCurrent();
            }
        }
        H_TaskUtilities::Schedule(
            TargetWorker,
            [
                this, 
                Frame, 
                NumParamTypesOnMain, 
                NumParamTypesForParallelUpdate, 
                NumParamTypesForLateProcessing
            ]()
            {
                ABYTEK_PROFILER_EVENT_NAMED("F_FrameManager::ParallelUpdate");
                _ProcessRange(
                    Frame, 
                    static_cast<F_FrameParamTypeIndex>(NumParamTypesOnMain), 
                    static_cast<F_FrameParamTypeIndex>(NumParamTypesForParallelUpdate)
                );
                
                //
                _LateProcessingQueue.Push(
                    [
                        this, 
                        Frame, 
                        NumParamTypesOnMain, 
                        NumParamTypesForParallelUpdate, 
                        NumParamTypesForLateProcessing
                    ]
                    {
                        ABYTEK_PROFILER_EVENT_NAMED("F_FrameManager::LateProcess");
                        _ProcessRange(
                            Frame, 
                            static_cast<F_FrameParamTypeIndex>(NumParamTypesOnMain + NumParamTypesForParallelUpdate), 
                            static_cast<F_FrameParamTypeIndex>(NumParamTypesForLateProcessing)
                        );
                    }
                );
            },
            E_TaskPriority::EXTREME,
            ABYTEK_NAME("FrameManager::ParallelUpdate")
        );
        
        // For the case of late flushing (typically used for E_FramesInFlightMode::NONE)
        if (auto LateFlushMode = Frame->GetLateFlushMode())
        {
            Flush(FrameIndex, *LateFlushMode);
        }
    }
    void F_FrameManager::Flush(U64 MinimumNumCompletedFrames)
    {
        while (_NumCompletedFrames.load(boost::memory_order_acquire) < MinimumNumCompletedFrames)
        {
            _FlushLateProcessingQueue();
            H_TaskUtilities::Switch();
        }
    }
    void F_FrameManager::Flush(U64 FrameIndex, E_FramesInFlightMode FramesInFlightMode)
    {
        Flush(
            Max<I64>(
                0,
                I64(FrameIndex) - I64(ResolveNumFramesInFlight(FramesInFlightMode) - 1)
            )
        );
    }
}
