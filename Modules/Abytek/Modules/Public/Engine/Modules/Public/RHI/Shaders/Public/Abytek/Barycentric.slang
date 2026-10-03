module Barycentric;

namespace Abytek
{
    public struct H_Barycentric
    {
        public static float3 CalculateWeights(float2 P, float2 A, float2 B, float2 C)
        {
            float2 v0 = B - A;
            float2 v1 = C - A;
            float2 v2 = P - A;

            float d00 = dot(v0, v0);
            float d01 = dot(v0, v1);
            float d11 = dot(v1, v1);
            float d20 = dot(v2, v0);
            float d21 = dot(v2, v1);

            float denom = d00 * d11 - d01 * d01;

            float v = (d11 * d20 - d01 * d21) / denom;
            float w = (d00 * d21 - d01 * d20) / denom;
            float u = 1.0 - v - w;

            return float3(u, v, w);
        }

        public static T Interpolate<T : IFloat>(float3 Weights, T A, T B, T C)
        {
            return A * T(Weights.x) + B * T(Weights.y) + C * T(Weights.z);
        }
    };
}