module ClipPositionToNDCPosition;

namespace Abytek
{
    public float3 ClipPositionToNDCPosition(float4 ClipPosition)
    {
        return (ClipPosition / ClipPosition.w).xyz;
    }
}