#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/Renderer/RenderGeometry/RenderGeometryCommon.hpp"


namespace Abytek
{
    using F_RenderMaterialType = U8;
    static constexpr F_RenderMaterialType INVALID_RENDER_MATERIAL_TYPE = ~F_RenderMaterialType(0);
    
    using F_RenderMaterialId = U64;
    
    using F_RenderMaterialHandle = U64;
    static constexpr F_RenderMaterialHandle INVALID_RENDER_MATERIAL_HANDLE = ~F_RenderMaterialHandle(0);
    
    struct ABYTEK_ENGINE_NFC_API H_RenderMaterialHandle
    {
        static void Pack(F_RenderMaterialHandle& OutHandle, F_RenderMaterialType Type, F_RenderMaterialId Id);
        static void Unpack(F_RenderMaterialHandle Handle, F_RenderMaterialType& OutType, F_RenderMaterialId& OutId);
    };
}