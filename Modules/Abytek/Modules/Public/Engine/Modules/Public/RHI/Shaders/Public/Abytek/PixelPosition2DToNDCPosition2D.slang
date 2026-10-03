module PixelPosition2DToNDCPosition2D;

namespace Abytek
{
    public float2 PixelPosition2DToNDCPosition2D(uint2 PixelPosition2D, uint2 Resolution)
    {
        return float2(
            (float2(PixelPosition2D) / float2(Resolution)) 
            * float2(2.0f, -2.0f)
            - float2(1.0f, -1.0f)
        );
    }
}