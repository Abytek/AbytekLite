#pragma once

#include "Abytek/Engine.Core.prerequisites.hpp"


namespace Abytek
{
    enum class E_FrameParamType : U8
    {
        MAIN,
        RENDER,
        GPU,
        DISPLAY,
        FINAL,
        
        NUM,
        
        DEFAULT = MAIN
    };
    using F_FrameParamTypeIndex = TF_TypeToUInt<E_FrameParamType>;
    inline F_Name GetFrameParamTypeName(E_FrameParamType ParamType)
    {
        switch (ParamType)
        {
        case E_FrameParamType::MAIN:
            return ABYTEK_NAME("Main");
        case E_FrameParamType::RENDER:
            return ABYTEK_NAME("Render");
        case E_FrameParamType::GPU:
            return ABYTEK_NAME("GPU");
        case E_FrameParamType::DISPLAY:
            return ABYTEK_NAME("Display");
        case E_FrameParamType::FINAL:
            return ABYTEK_NAME("Final");
        case E_FrameParamType::NUM:
            return ABYTEK_NAME("Num");
        }
        return {};
    }
    inline F_Name GetFrameParamMinimalTaskTag()
    {
        return ABYTEK_NAME("Abytek::FrameParam");
    }
    inline F_Name GetFrameParamTypeTaskTag(E_FrameParamType ParamType)
    {
        switch (ParamType)
        {
        case E_FrameParamType::MAIN:
            return ABYTEK_NAME("Abytek::FrameParam::Main");
        case E_FrameParamType::RENDER:
            return ABYTEK_NAME("Abytek::FrameParam::Render");
        case E_FrameParamType::GPU:
            return ABYTEK_NAME("Abytek::FrameParam::GPU");
        case E_FrameParamType::DISPLAY:
            return ABYTEK_NAME("Abytek::FrameParam::Display");
        case E_FrameParamType::FINAL:
            return ABYTEK_NAME("Abytek::FrameParam::Final");
        case E_FrameParamType::NUM:
            return ABYTEK_NAME("Abytek::FrameParam::Num");
        }
        return {};
    }
    
    enum class E_FramesInFlightMode : U8
    {
        NONE,
        RENDER,
        RENDER_AND_GPU,
        
        MAX = RENDER_AND_GPU,
        
        DEFAULT = MAX
    };
    inline U32 ResolveNumFramesInFlight(E_FramesInFlightMode Mode)
    {
        switch (Mode)
        {
        case E_FramesInFlightMode::NONE:
            return 1;
        case E_FramesInFlightMode::RENDER:
            return 2;
        case E_FramesInFlightMode::RENDER_AND_GPU:
            return 3;
        default:
            ABYTEK_LOG_FATAL() << "Unknown mode: " << static_cast<U32>(Mode);
            break;
        }
        return 0;
    }
    
    static_assert(
        static_cast<U32>(E_FramesInFlightMode::MAX)    
        < static_cast<U32>(E_FrameParamType::NUM)   
    );
    
    using F_FrameCommand = TF_Function<void()>;
}
