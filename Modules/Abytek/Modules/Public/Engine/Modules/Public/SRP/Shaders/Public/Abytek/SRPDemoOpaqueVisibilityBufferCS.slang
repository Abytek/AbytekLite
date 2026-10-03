module DemoVisibilityBufferCS;

#include "Abytek/SRPVisibilityBuffer.slangh"

ABYTEK_SRP_DEFINE_VISIBILITY_BUFFER_OPAQUE_RW(OpaqueVisibilityBuffer);

[numthreads(1, 1, 1)]
[shader("compute")]
void MainCS()
{
    ABYTEK_SRP_OUTPUT_VISIBILITY_BUFFER_OPAQUE(OpaqueVisibilityBuffer, uint2(0, 0), 2);
}