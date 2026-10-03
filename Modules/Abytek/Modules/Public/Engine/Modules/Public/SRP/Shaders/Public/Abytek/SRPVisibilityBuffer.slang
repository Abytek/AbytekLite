module SRPVisibilityBuffer;

__exported import Abytek.Renderer.RenderPrimitive.Processor;

namespace Abytek
{
    public typealias F_SRPPackedDepth = uint;
    public struct H_SRPPackedDepth
    {
        public static F_SRPPackedDepth Pack(float Depth)
        {
            Depth = max(Depth, 0.0);

            uint Bits = asuint(Depth);

            uint Exp = (Bits >> 23) & 0xFF;
            uint Mant = Bits & 0x7FFFFF;

            // Keep the upper 20 mantissa bits, round down.
            uint PackedMant = Mant >> 3;

            return (Exp << 20) | PackedMant;
        }

        public static float Unpack(F_SRPPackedDepth PackedDepth)
        {
            uint Exp = (PackedDepth >> 20) & 0xFF;
            uint Mant = PackedDepth & 0xFFFFF;

            if (Exp == 0)
                return 0.0;

            uint Bits =
                (Exp << 23) |
                (Mant << 3);

            return asfloat(Bits);
        }
    };

    // 30bit
    public typealias F_SRPTriangleCandidate = uint;
    public static F_SRPTriangleCandidate INVALID_SRP_TRIANGLE_CANDIDATE = 0;

    // 64bit
    public typealias F_SRPVisibilityCandidate = uint64_t;
    public static F_SRPVisibilityCandidate INVALID_SRP_VISIBILITY_CANDIDATE = 0;
    public struct H_SRPVisibilityCandidate
    {
        public static void Pack(out F_SRPVisibilityCandidate OutVisibilityCandidate, F_SRPTriangleCandidate TriangleCandidate, F_RenderPrimitiveProcessorId PrimitiveProcessorId, F_SRPPackedDepth PackedDepth)
        {
            F_SRPVisibilityCandidate Result = 0;
            Result |= uint64_t(TriangleCandidate & 0x3FFFFFFF) << uint64_t(0); // 30bit
            Result |= uint64_t(
                (
                    (PrimitiveProcessorId + 1) 
                    % (INVALID_RENDER_PRIMITIVE_PROCESSOR_ID + 1)
                )
                & 0x3F
            ) << uint64_t(30); // 6bit
            Result |= uint64_t(PackedDepth & 0xFFFFFFF) << uint64_t(36); // 28bit
            OutVisibilityCandidate = Result;
        }
        public static void Unpack(F_SRPVisibilityCandidate VisibilityCandidate, out F_SRPTriangleCandidate OutTriangleCandidate, out F_RenderPrimitiveProcessorId OutPrimitiveProcessorId, out F_SRPPackedDepth OutPackedDepth)
        {
            OutTriangleCandidate = F_SRPTriangleCandidate( // 30bit
                (VisibilityCandidate >> uint64_t(0)) 
                & uint64_t(0x3FFFFFFF)
            );
            OutPrimitiveProcessorId = ( // 6bit
                F_RenderPrimitiveProcessorId(
                    (VisibilityCandidate >> uint64_t(30)) 
                    & uint64_t(0x3F)
                ) 
                + INVALID_RENDER_PRIMITIVE_PROCESSOR_ID
            ) % (INVALID_RENDER_PRIMITIVE_PROCESSOR_ID + 1);
            OutPackedDepth = F_SRPPackedDepth( // 28bit
                (VisibilityCandidate >> uint64_t(36)) 
                & uint64_t(0xFFFFFFF)
            );
        }
    };
}