module Common;

namespace Abytek
{
    public typealias F_RenderMeshType = uint8_t;
    public typealias F_RenderMeshId = uint2;
    public typealias F_RenderMeshHandle = uint2;

    public struct H_RenderMesh
    {
        public static void Pack(out F_RenderMeshHandle OutMeshHandle, F_RenderMeshType MeshType, F_RenderMeshId MeshId)
        {
            OutMeshHandle = uint2(
                (MeshId.x << 8) | (MeshType),
                (MeshId.y << 8) | (MeshId.x >> (32 - 8))
            );
        }
        public static void Unpack(F_RenderMeshHandle MeshHandle, F_RenderMeshType OutMeshType, F_RenderMeshId OutMeshId)
        {
            OutMeshType = uint8_t(MeshHandle.x);
            OutMeshId = uint2(
                (MeshHandle.y << (32 - 8)) | (MeshHandle.x >> 8),
                (MeshHandle.y >> 8)
            );
        }
    }
}