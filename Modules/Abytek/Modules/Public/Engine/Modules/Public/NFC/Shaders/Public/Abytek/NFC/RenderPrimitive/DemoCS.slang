module DemoCS;

import Abytek.NFC.RenderPrimitive.Common;

#include "Abytek/NFC/GPUData/Macros.slangh"

ABYTEK_GPU_DATA_DEFINE_COMPONENT_TYPE_RO(
    RenderPrimitive,
    Transform,
    Abytek::NFC::RenderPrimitive::F_Component_Transform
);
ABYTEK_GPU_DATA_DEFINE_COMPONENT_TYPE_RW(
    RenderPrimitive,
    InverseTransposeTransform,
    Abytek::NFC::RenderPrimitive::F_Component_InverseTransposeTransform
);

[numthreads(1, 1, 1)]
[shader("compute")]
void MainCS()
{
    typealias H_GPUDataComponent_InverseTransposeTransform = ABYTEK_GPU_DATA_GET_COMPONENT_TYPE_STRUCT(
        RenderPrimitive, 
        InverseTransposeTransform
    );

    H_GPUDataComponent_InverseTransposeTransform::F_Value Value;
    Value.Matrix = float4x4(1);
    H_GPUDataComponent_InverseTransposeTransform::Store(0, Value);
}