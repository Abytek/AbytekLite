#include "Abytek/Renderer/RenderPrimitive/RenderPrimitiveCommon.hpp"


namespace Abytek
{
    void H_RenderPrimitiveMeshHandle::Pack(F_RenderPrimitiveMeshHandle& OutHandle, F_RenderPrimitiveMeshType Type, F_RenderPrimitiveMeshId Id)
    {
        OutHandle = (
            (static_cast<U64>(Type) << static_cast<U64>(0))
            | (static_cast<U64>(Id) << static_cast<U64>(8))
        );
    }
    void H_RenderPrimitiveMeshHandle::Unpack(F_RenderPrimitiveMeshHandle Handle, F_RenderPrimitiveMeshType& OutType, F_RenderPrimitiveMeshId& OutId)
    {
        OutType = static_cast<F_RenderPrimitiveMeshType>((Handle & static_cast<U64>(0xFF)) >> static_cast<U64>(0));
        OutId = static_cast<F_RenderPrimitiveMeshId>((Handle & static_cast<U64>(0xFFFFFFFFFFFFFF00LL)) >> static_cast<U64>(8));
    }
}
