#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/GPUData/GlobalGPUDataComponent.hpp"


namespace Abytek
{
    struct F_GPUDataInverseTransposeTransform
    {
        ABYTEK_BEGIN_REFLECTOR(I_GlobalGPUDataComponent)
        ABYTEK_END_REFLECTOR(F_GPUDataInverseTransposeTransform)
        
        F_Matrix4x4_F32 Matrix;
    };
}