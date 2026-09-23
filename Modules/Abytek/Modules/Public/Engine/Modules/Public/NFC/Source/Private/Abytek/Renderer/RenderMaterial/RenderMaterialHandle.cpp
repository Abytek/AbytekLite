#include "Abytek/Renderer/RenderMaterial/RenderMaterialHandle.hpp"


namespace Abytek
{
    void H_RenderMaterialHandle::Pack(F_RenderMaterialHandle& OutHandle, F_RenderMaterialType Type, F_RenderMaterialId Id)
    {
        OutHandle = (
            (static_cast<U64>(Type) << static_cast<U64>(0))
            | (static_cast<U64>(Id) << static_cast<U64>(8))
        );
    }
    void H_RenderMaterialHandle::Unpack(F_RenderMaterialHandle Handle, F_RenderMaterialType& OutType, F_RenderMaterialId& OutId)
    {
        OutType = static_cast<F_RenderMaterialType>((Handle & static_cast<U64>(0xFF)) >> static_cast<U64>(0));
        OutId = static_cast<F_RenderMaterialId>((Handle & static_cast<U64>(0xFFFFFFFFFFFFFF00LL)) >> static_cast<U64>(8));
    }
}
