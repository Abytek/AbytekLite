#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/Renderer/RenderPrimitive/RenderPrimitiveCommon.hpp"
#include "Abytek/Renderer/GPUData/GPUDataComponentType.hpp"


namespace Abytek
{
    namespace RenderPrimitive
    {
        struct F_Component_Transform
        {
            ABYTEK_GPU_DATA_COMPONENT_TYPE(
                F_Component_Transform, 
                ABYTEK_NAME("Transform"), 
                ABYTEK_NAME("Abytek::RenderPrimitive::F_Component_Transform")
            );
            
            F_Matrix4x4_F32 Matrix;
        };
    }
}
