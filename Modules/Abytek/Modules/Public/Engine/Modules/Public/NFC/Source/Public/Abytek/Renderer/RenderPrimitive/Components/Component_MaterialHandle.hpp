#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/Renderer/RenderMaterial/RenderMaterialHandle.hpp"
#include "Abytek/Renderer/GPUData/GPUDataComponentType.hpp"


namespace Abytek
{
    namespace RenderPrimitive
    {
        struct F_Component_MaterialHandle
        {
            ABYTEK_GPU_DATA_COMPONENT_TYPE(
                F_Component_MaterialHandle, 
                ABYTEK_NAME("MaterialHandle"), 
                ABYTEK_NAME("Abytek::RenderPrimitive::F_Component_MaterialHandle")
            );
            
            F_RenderMaterialHandle Handle = INVALID_RENDER_MATERIAL_HANDLE;
        };
    }
}
