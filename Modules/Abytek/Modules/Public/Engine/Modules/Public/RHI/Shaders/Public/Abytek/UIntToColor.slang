module UIntToColor;

namespace Abytek
{
    uint Hash(uint x)
    {
        x ^= x >> 16;
        x *= 0x7FEB352Du;
        x ^= x >> 15;
        x *= 0x846CA68Bu;
        x ^= x >> 16;
        return x;
    }

    float3 HSVToRGB(float3 c)
    {
        float4 K = float4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);

        float3 p = abs(frac(c.xxx + K.xyz) * 6.0 - K.www);

        return c.z * lerp(
                        K.xxx,
                        saturate(p - K.xxx),
                        c.y);
    }

    public float3 UIntToColor(uint Value)
    {
        uint H = Hash(Value);

        // Hash -> [0,1)
        float Hue = float(H) / 4294967296.0;

        // Keep colors vivid and reasonably bright.
        return HSVToRGB(float3(
            Hue,
            0.75,
            0.90));
    }
}
