#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"
#include "Abytek/RHICapture.hpp"
#include "Abytek/RHIProcess.hpp"
#include "Abytek/RHISubmissionList.hpp"
#include "Abytek/RHISubmissionQueue.hpp"
#include "Abytek/RHISubsystem.hpp"
#include "Abytek/Frame/FrameHelper.hpp"


namespace Abytek
{
    class A_RHIContext;
    class A_RHIProcess;
    class A_RHISubmissionQueue;
    struct F_RHIFeatureSet;
    
    struct ABYTEK_ENGINE_RHI_API H_RHI
    {
        static TS<A_RHIContext> GetMainContext();
        static const F_RHIFeatureSet& GetMainFeatureSet();
        static TS<A_RHIProcess> GetMainProcess();
        static TS<A_RHIProcess> GetMainProcess(E_FrameParamType FrameParamType);
        static const auto& GetMainSubmissionQueue()
        {
            ABYTEK_CHECK_FRAME_PARAM_TYPE(E_FrameParamType::RENDER);
            return F_RHISubsystem::GetInstance()->GetMainRHISubmissionQueue();
        }
        static void SignalCapture(E_RHICaptureFlag Flags = E_RHICaptureFlag::DEFAULT);
    };
}

#define ABYTEK_RHI_CAPTURE_EVENT_SCOPE_MAIN(...) ABYTEK_RHI_CAPTURE_EVENT_SCOPE(Abytek::H_RHI::GetMainSubmissionQueue())
#ifdef ABYTEK_ENGINE_RHI_ENABLE_CAPTURE
#define ABYTEK_RHI_PUSH_CAPTURE_EVENT_SCOPE_MAIN(...) Abytek::H_RHI::GetMainSubmissionQueue()->PushAdditionalStackCaptureEventStates( \
                Abytek::F_RHICaptureEventState::Make( \
                    __VA_ARGS__ \
                ) \
            )
#define ABYTEK_RHI_POP_CAPTURE_EVENT_SCOPE_MAIN(...) Abytek::H_RHI::GetMainSubmissionQueue()->PopAdditionalStackCaptureEventStates()
#else
#define ABYTEK_RHI_PUSH_CAPTURE_EVENT_SCOPE_MAIN(...)
#define ABYTEK_RHI_POP_CAPTURE_EVENT_SCOPE_MAIN(...)
#endif