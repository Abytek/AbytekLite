module ECMSMeshData;

namespace Abytek
{
    public typealias F_ECMSGlobalVertexIndex = uint;

    public typealias F_ECMSMeshTriangle = uint;
    public struct H_ECMSMeshTriangle
    {
        public static void Pack(out F_ECMSMeshTriangle OutMeshTriangle, uint3 TriangleU32X3)
        {
            OutMeshTriangle = (
                (F_ECMSMeshTriangle(TriangleU32X3.x) << F_ECMSMeshTriangle(0))
                | (F_ECMSMeshTriangle(TriangleU32X3.y) << F_ECMSMeshTriangle(8))
                | (F_ECMSMeshTriangle(TriangleU32X3.z) << F_ECMSMeshTriangle(16))
            );
        }
        public static void Unpack(F_ECMSMeshTriangle MeshTriangle, out uint3 OutTriangleU32X3)
        {
            OutTriangleU32X3 = uint3(
                uint(
                    (MeshTriangle >> F_ECMSMeshTriangle(0)) 
                    & F_ECMSMeshTriangle(0xFF)
                ),
                uint(
                    (MeshTriangle >> F_ECMSMeshTriangle(8)) 
                    & F_ECMSMeshTriangle(0xFF)
                ),
                uint(
                    (MeshTriangle >> F_ECMSMeshTriangle(16)) 
                    & F_ECMSMeshTriangle(0xFF)
                )
            );
        }
    };

    public struct F_ECMSMeshlet
    {
        public uint Offset_Triangles = 0;
        public uint Count_Triangles = 0;
        public uint Offset_VertexIndices = 0;
        public uint Count_VertexIndices = 0;
    };
}