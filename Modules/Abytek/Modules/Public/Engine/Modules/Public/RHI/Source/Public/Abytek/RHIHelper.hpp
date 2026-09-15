#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"
#include "Abytek/RHICapture.hpp"


namespace Abytek
{
    class A_RHIContext;
    class A_RHIProcess;
    struct F_RHIFeatureSet;
    
    struct ABYTEK_ENGINE_RHI_API H_RHI
    {
        static TS<A_RHIContext> GetMainContext();
        static const F_RHIFeatureSet& GetMainFeatureSet();
        static TS<A_RHIProcess> GetMainProcess();
        static TS<A_RHIProcess> GetMainProcess(E_FrameParamType FrameParamType);
        static void SignalCapture(E_RHICaptureFlag Flags = E_RHICaptureFlag::DEFAULT);
    };
}
