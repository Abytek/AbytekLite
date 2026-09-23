#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/Renderer/RenderGeometry/RenderGeometryCommon.hpp"


namespace Abytek
{
    using F_RenderMeshType = U8;
    static constexpr F_RenderMeshType INVALID_RENDER_MESH_TYPE = ~F_RenderMeshType(0);
    
    using F_RenderMeshId = U64;
    
    using F_RenderMeshHandle = U64;
    static constexpr F_RenderMeshHandle INVALID_RENDER_MESH_HANDLE = ~F_RenderMeshHandle(0);
    
    struct ABYTEK_ENGINE_NFC_API H_RenderMeshHandle
    {
        static void Pack(F_RenderMeshHandle& OutHandle, F_RenderMeshType Type, F_RenderMeshId Id);
        static void Unpack(F_RenderMeshHandle Handle, F_RenderMeshType& OutType, F_RenderMeshId& OutId);
    };
}