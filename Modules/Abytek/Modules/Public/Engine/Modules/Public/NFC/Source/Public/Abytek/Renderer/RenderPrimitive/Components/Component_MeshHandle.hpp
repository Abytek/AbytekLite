#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/Renderer/RenderPrimitive/RenderPrimitiveCommon.hpp"
#include "Abytek/Renderer/GPUData/GPUDataComponentType.hpp"


namespace Abytek
{
    namespace RenderPrimitive
    {
        struct F_Component_MeshHandle
        {
            ABYTEK_GPU_DATA_COMPONENT_TYPE(
                F_Component_MeshHandle, 
                ABYTEK_NAME("MeshHandle"), 
                ABYTEK_NAME("Abytek::RenderPrimitive::F_Component_MeshHandle")
            );
            
            F_RenderPrimitiveMeshHandle Handle = INVALID_RENDER_PRIMITIVE_MESH_HANDLE;
        };
    }
}
