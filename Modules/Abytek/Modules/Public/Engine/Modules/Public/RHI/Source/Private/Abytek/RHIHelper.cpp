#include "Abytek/RHIHelper.hpp"
#include "Abytek/RHIContext.hpp"
#include "Abytek/RHIDevice.hpp"
#include "Abytek/RHISubsystem.hpp"
#include "Abytek/Frame/FrameHelper.hpp"


namespace Abytek
{
    TS<A_RHIContext> H_RHI::GetMainContext()
    {
        ABYTEK_ENGINE_RHI_ASSERT(
            H_TaskUtilities::HasTag(GetFrameParamTypeTaskTag(E_FrameParamType::RENDER))
            || H_TaskUtilities::HasTag(GetApplicationLowLevelTaskTag())
        );
        return F_RHISubsystem::GetInstance()->GetMainRHIContext();
    }
    const F_RHIFeatureSet& H_RHI::GetMainFeatureSet()
    {
        return GetMainContext()->GetDevice()->GetFeatureSet();
    }
    TS<A_RHIProcess> H_RHI::GetMainProcess()
    {
        static E_FrameParamType FrameParamTypes[] = {
            E_FrameParamType::RENDER,
            E_FrameParamType::GPU,
            E_FrameParamType::DISPLAY
        };
        for (auto FrameParamType : FrameParamTypes)
        {
            if (H_TaskUtilities::HasTag(GetFrameParamTypeTaskTag(FrameParamType)))
            {
                return GetMainProcess(FrameParamType);
            }
        }
        ABYTEK_LOG_FATAL() << "Failed to get main RHI process";
        return {};
    }
    TS<A_RHIProcess> H_RHI::GetMainProcess(E_FrameParamType FrameParamType)
    {
        switch (FrameParamType)
        {
        case E_FrameParamType::RENDER:
            ABYTEK_CHECK_FRAME_PARAM_TYPE(E_FrameParamType::RENDER);
            return F_RHISubsystem::GetInstance()->GetMainRHIProcess_Render();
        case E_FrameParamType::GPU:
            ABYTEK_CHECK_FRAME_PARAM_TYPE(E_FrameParamType::GPU);
            return F_RHISubsystem::GetInstance()->GetMainRHIProcess_GPU();
        case E_FrameParamType::DISPLAY:
            ABYTEK_CHECK_FRAME_PARAM_TYPE(E_FrameParamType::DISPLAY);
            return F_RHISubsystem::GetInstance()->GetMainRHIProcess_Final();
        default:
            ABYTEK_LOG_INFO() << "Invalid frame param type: " << static_cast<F_FrameParamTypeIndex>(FrameParamType);
            break;
        }
        return {};
    }
    void H_RHI::SignalCapture(E_RHICaptureFlag Flags)
    {
        ABYTEK_ENGINE_RHI_ASSERT(
            H_TaskUtilities::HasTag(GetFrameParamTypeTaskTag(E_FrameParamType::RENDER))
            || H_TaskUtilities::HasTag(GetApplicationLowLevelTaskTag())
        );
        F_RHISubsystem::GetInstance()->AddCaptureFlag_Render(Flags);
    }
}
