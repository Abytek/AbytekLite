module Common;

namespace Abytek
{
    public typealias F_RenderMaterialType = uint8_t;
    public typealias F_RenderMaterialId = uint2;
    public typealias F_RenderMaterialHandle = uint2;

    public struct H_RenderMaterial
    {
        public static void Pack(out F_RenderMaterialHandle OutMaterialHandle, F_RenderMaterialType MaterialType, F_RenderMaterialId MaterialId)
        {
            OutMaterialHandle = uint2(
                (MaterialId.x << 8) | (MaterialType),
                (MaterialId.y << 8) | (MaterialId.x >> (32 - 8))
            );
        }
        public static void Unpack(F_RenderMaterialHandle MaterialHandle, F_RenderMaterialType OutMaterialType, F_RenderMaterialId OutMaterialId)
        {
            OutMaterialType = uint8_t(MaterialHandle.x);
            OutMaterialId = uint2(
                (MaterialHandle.y << (32 - 8)) | (MaterialHandle.x >> 8),
                (MaterialHandle.y >> 8)
            );
        }
    }
}