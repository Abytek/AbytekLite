module NDCPosition2DToPixelPosition2D;

namespace Abytek
{
    public uint2 NDCPosition2DToPixelPosition2D(float2 NDCPosition2D, uint2 Resolution)
    {
        return uint2(
            (NDCPosition * float2(0.5f, -0.5f) + 0.5f) 
            * float2(Resolution)
        );
    }
}