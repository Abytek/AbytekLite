#pragma once

#include "Abytek/Engine.SRP.prerequisites.hpp"
#include "Abytek/Renderer/GPUData/GPUDataCommon.hpp"
#include "Abytek/Renderer/RenderPrimitive/RenderPrimitiveProcessor.hpp"


namespace Abytek
{
    namespace SRP::SimplePrimitive
    {
        struct ABYTEK_ALIGN(16) F_PrimitiveVisibilityConfig
        {
            F_GPUDataInstanceAddress PrimitiveAddress;
            F_RenderPrimitiveProcessorId PrimitiveProcessorId = INVALID_RENDER_PRIMITIVE_PROCESSOR_ID;
        };
    }
}