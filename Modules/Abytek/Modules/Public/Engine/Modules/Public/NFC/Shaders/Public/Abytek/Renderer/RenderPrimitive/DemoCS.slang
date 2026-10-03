module DemoCS;

import Abytek.Renderer.RenderPrimitive.Components;

#include "Abytek/Renderer/GPUData/Macros.slangh"

ABYTEK_GPU_DATA_DEFINE_INSTANCE_SET_HEADER_BUFFER(
    RenderPrimitive_Simple
);

ABYTEK_GPU_DATA_DEFINE_COMPONENT_TYPE_RO(
    RenderPrimitive_Simple,
    Transform,
    Abytek::RenderPrimitive::F_Component_Transform
);
ABYTEK_GPU_DATA_DEFINE_COMPONENT_TYPE_RW(
    RenderPrimitive_Simple,
    InverseTransposeTransform,
    Abytek::RenderPrimitive::F_Component_InverseTransposeTransform
);

[numthreads(1, 1, 1)]
[shader("compute")]
void MainCS()
{
    typealias H_GPUDataComponent_InverseTransposeTransform = ABYTEK_GPU_DATA_GET_COMPONENT_TYPE_STRUCT(
        RenderPrimitive_Simple, 
        InverseTransposeTransform
    );

    StructuredBuffer<Abytek::F_GPUDataInstanceSetHeader> InstanceSetHeaders = ABYTEK_GPU_DATA_GET_INSTANCE_SET_HEADER_BUFFER(RenderPrimitive_Simple);
    Abytek::F_GPUDataInstanceSetHeader InstanceSetHeader = InstanceSetHeaders[
        ABYTEK_GPU_DATA_GET_NUM_INSTANCE_SETS(RenderPrimitive_Simple) - 1
    ];
    Abytek::F_GPUDataInstanceAddress InstanceAddress = InstanceSetHeader.Address;

    H_GPUDataComponent_InverseTransposeTransform::F_Value Value;
    Value.Matrix = float4x4(1);
    H_GPUDataComponent_InverseTransposeTransform::Store(InstanceAddress, Value);
}