#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/Renderer/RenderPrimitive/RenderPrimitiveCommon.hpp"
#include "Abytek/Renderer/GPUData/GPUDataComponentType.hpp"


namespace Abytek
{
    namespace RenderPrimitive
    {
        struct F_Component_InverseTransposeTransform
        {
            ABYTEK_GPU_DATA_COMPONENT_TYPE(
                F_Component_InverseTransposeTransform, 
                ABYTEK_NAME("InverseTransposeTransform"), 
                ABYTEK_NAME("Abytek::RenderPrimitive::F_Component_InverseTransposeTransform")
            );
            
            F_Matrix4x4_F32 Matrix;
        };
    }
}
