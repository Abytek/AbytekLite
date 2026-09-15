module Common;

namespace NFC::GPUScene
{
    public typealias F_VisibilityCandidate = uint64_t;

    public typealias F_TrianglePersistentId = uint64_t;
    public typealias F_TriangleVisibilityId = uint32_t;

    public typealias F_PackedDepth = uint32_t;
    
    public typealias F_VertexFactoryId = uint8_t;

    public typealias F_InstanceArchetypeId = uint32_t;

    public struct F_InstanceAllocation
    {
        public uint BeginOffsetInBytes = 0;
        public uint EndOffsetInBytes = 0;
        public uint PageIndex = 0;

        private uint ___Padding___;
    };

    public F_PackedDepth PackDepth(float Depth)
    {
        Depth = max(Depth, 0.0);

        uint Bits = asuint(Depth);

        uint Exp = (Bits >> 23) & 0xFF;
        uint Mant = Bits & 0x7FFFFF;

        // Keep the upper 16 mantissa bits, round-to-nearest.
        uint PackedMant = (Mant + 0x40) >> 7;

        if (PackedMant > 0xFFFF)
        {
            PackedMant = 0;

            if (Exp < 0xFE)
            {
                Exp++;
            }
            else
            {
                // Saturate to largest finite representable value
                Exp = 0xFE;
                PackedMant = 0xFFFF;
            }
        }

        return (Exp << 16) | PackedMant;
    }
    public float UnpackDepth(F_PackedDepth PackedDepth)
    {
        uint exp = (PackedDepth >> 16) & 0xff;
        uint mant = PackedDepth & 0xffff;

        if (exp == 0)
            return 0.0;

        uint bits =
            (exp << 23) |
            (mant << 7);

        return asfloat(bits);
    }

    public void PackVisibilityCandidate(out F_VisibilityCandidate OutVisibilityCandidate, F_PackedDepth PackedDepth, F_VertexFactoryId VertexFactoryId, F_TriangleVisibilityId TriangleVisibilityId)
    {
        F_VisibilityCandidate Result = 0;
        Result |= (uint64_t(PackedDepth) & uint64_t(0xFFFFFF)) << uint64_t(40);
        Result |= (uint64_t(VertexFactoryId) & uint64_t(0xFF)) << uint64_t(32);
        Result |= uint64_t(TriangleVisibilityId);
        OutVisibilityCandidate = Result;
    }
    public void UnpackVisibilityCandidate(F_VisibilityCandidate VisibilityCandidate, out F_PackedDepth OutPackedDepth, out F_VertexFactoryId OutVertexFactoryId, out F_TriangleVisibilityId OutTriangleVisibilityId)
    {
        OutPackedDepth = uint((VisibilityCandidate >> uint64_t(40)) & uint64_t(0xFFFFFF));
        OutVertexFactoryId = uint((VisibilityCandidate >> uint64_t(32)) & uint64_t(0xFF));
        OutTriangleVisibilityId = uint(VisibilityCandidate & uint64_t(0xFFFFFFFF));
    }
}