module SRPTriangleCandidate_ECMS;

__exported import Abytek.SRPVisibilityBuffer;
__exported import Abytek.ECMS.SRPInstancedMeshletBuffer_ECMS;

namespace Abytek
{
    namespace SRP::ECMS
    {
        public struct H_TriangleCandidate
        {
            public static void Pack(out F_SRPTriangleCandidate OutTriangleCandidate, F_InstancedMeshletId InstancedMeshletId, uint LocalMeshletTriangleIndex)
            {
                OutTriangleCandidate = F_SRPTriangleCandidate(
                    ((InstancedMeshletId & F_SRPTriangleCandidate(0x7FFFFF)) << F_SRPTriangleCandidate(0))
                    | ((LocalMeshletTriangleIndex & F_SRPTriangleCandidate(0x7F)) << F_SRPTriangleCandidate(23))
                );
            }
            public static void Unpack(F_SRPTriangleCandidate TriangleCandidate, out F_InstancedMeshletId OutInstancedMeshletId, out uint OutLocalMeshletTriangleIndex)
            {
                OutInstancedMeshletId = (TriangleCandidate >> F_SRPTriangleCandidate(0)) & F_SRPTriangleCandidate(0x7FFFFF);
                OutLocalMeshletTriangleIndex = (TriangleCandidate >> F_SRPTriangleCandidate(23)) & F_SRPTriangleCandidate(0x7F);
            }
        };
    }
}