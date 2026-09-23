#include "Abytek/Renderer/RenderMesh/RenderMeshHandle.hpp"


namespace Abytek
{
    void H_RenderMeshHandle::Pack(F_RenderMeshHandle& OutHandle, F_RenderMeshType Type, F_RenderMeshId Id)
    {
        OutHandle = (
            (static_cast<U64>(Type) << static_cast<U64>(0))
            | (static_cast<U64>(Id) << static_cast<U64>(8))
        );
    }
    void H_RenderMeshHandle::Unpack(F_RenderMeshHandle Handle, F_RenderMeshType& OutType, F_RenderMeshId& OutId)
    {
        OutType = static_cast<F_RenderMeshType>((Handle & static_cast<U64>(0xFF)) >> static_cast<U64>(0));
        OutId = static_cast<F_RenderMeshId>((Handle & static_cast<U64>(0xFFFFFFFFFFFFFF00LL)) >> static_cast<U64>(8));
    }
}
