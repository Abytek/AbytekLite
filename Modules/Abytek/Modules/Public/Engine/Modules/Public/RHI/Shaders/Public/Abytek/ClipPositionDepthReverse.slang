module ClipPositionDepthReverse;

namespace Abytek
{
    public float4 ClipPositionDepthReverse(float4 ClipPosition)
    {
        return float4(ClipPosition.xy, ClipPosition.w - ClipPosition.z, ClipPosition.w);
    }
}