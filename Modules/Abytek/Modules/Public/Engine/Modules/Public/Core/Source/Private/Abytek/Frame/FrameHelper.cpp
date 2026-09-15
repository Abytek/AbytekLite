#include "Abytek/Frame/FrameHelper.hpp"


namespace Abytek
{
    TS<F_Frame> H_Frame::Get(E_FrameParamType CurrentParamType)
    {
        ABYTEK_CHECK_FRAME_PARAM_TYPE(CurrentParamType);
        return F_FrameManager::GetInstance()->GetProcessor(CurrentParamType)->GetCurrentFrame();
    }

    void H_Frame::Flush(E_FramesInFlightMode FramesInFlightMode, E_FrameParamType CurrentParamType)
    {
        F_FrameManager::GetInstance()->Flush(
            GetIndex(CurrentParamType),
            FramesInFlightMode
        );
    }

    void H_Frame::LateFlush(E_FramesInFlightMode FramesInFlightMode)
    {
        Get(E_FrameParamType::MAIN)->SetLateFlushMode(FramesInFlightMode);
    }
}
