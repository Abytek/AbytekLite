#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/Renderer/RenderPrimitive/Components/Component_Transform.hpp"
#include "Abytek/Renderer/RenderPrimitive/Components/Component_InverseTransposeTransform.hpp"
#include "Abytek/Renderer/RenderPrimitive/Components/Component_MeshHandle.hpp"
#include "Abytek/Renderer/RenderPrimitive/Components/Component_MaterialHandle.hpp"
#include "Abytek/Renderer/GPUData/GPUData.hpp"


namespace Abytek
{
    namespace RenderPrimitive
    {
        struct F_Data
        {
            ABYTEK_GPU_DATA(
                F_Data, 
                ABYTEK_NAME("RenderPrimitive"),
                ABYTEK_NAME("Abytek::RenderPrimitive::F_Data"),
                F_Component_Transform,
                F_Component_InverseTransposeTransform,
                F_Component_MeshHandle,
                F_Component_MaterialHandle
            );
        };
    }
}