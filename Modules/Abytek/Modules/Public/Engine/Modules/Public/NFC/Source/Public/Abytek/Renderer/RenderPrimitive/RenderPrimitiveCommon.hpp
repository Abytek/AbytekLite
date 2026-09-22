#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/Renderer/RenderGeometry/RenderGeometryCommon.hpp"


namespace Abytek
{
    using F_RenderPrimitiveMeshType = U8;
    static constexpr F_RenderPrimitiveMeshType INVALID_RENDER_PRIMITIVE_MESH_TYPE = ~F_RenderPrimitiveMeshType(0);
    
    using F_RenderPrimitiveMeshId = U64;
    
    using F_RenderPrimitiveMeshHandle = U64;
    static constexpr F_RenderPrimitiveMeshHandle INVALID_RENDER_PRIMITIVE_MESH_HANDLE = ~F_RenderPrimitiveMeshHandle(0);
    
    struct ABYTEK_ENGINE_NFC_API H_RenderPrimitiveMeshHandle
    {
        static void Pack(F_RenderPrimitiveMeshHandle& OutHandle, F_RenderPrimitiveMeshType Type, F_RenderPrimitiveMeshId Id);
        static void Unpack(F_RenderPrimitiveMeshHandle Handle, F_RenderPrimitiveMeshType& OutType, F_RenderPrimitiveMeshId& OutId);
    };
}