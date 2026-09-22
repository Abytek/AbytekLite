module DemoCS;

#include "NFC/GPUData/Macros.slangh"

ABYTEK_GPU_DATA_COMPONENT_TYPE_RO(Transform);
ABYTEK_GPU_DATA_COMPONENT_TYPE_RW(InverseTransposeTransform);

[numthreads(1, 1, 1)]
[shader("compute")]
void MainCS()
{
}