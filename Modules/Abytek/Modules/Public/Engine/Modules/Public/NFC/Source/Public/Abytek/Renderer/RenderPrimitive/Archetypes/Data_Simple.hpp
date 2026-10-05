#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/Renderer/RenderPrimitive/Components/Component_Transform.hpp"
#include "Abytek/Renderer/RenderPrimitive/Components/Component_InverseTransposeTransform.hpp"
#include "Abytek/Renderer/RenderPrimitive/Components/Component_GeometryAddress_ECMS.hpp"
#include "Abytek/Renderer/RenderPrimitive/Components/Component_GeometryAddress_LOD.hpp"
#include "Abytek/Renderer/GPUData/GPUData.hpp"


namespace Abytek
{
    namespace RenderPrimitive
    {
        struct F_Data_Simple
        {
            ABYTEK_GPU_DATA(
                F_Data_Simple, 
                ABYTEK_NAME("RenderPrimitive_Simple"),
                ABYTEK_NAME("Abytek::RenderPrimitive::F_Data_Simple"),
                F_Component_Transform,
                F_Component_InverseTransposeTransform,
                F_Component_GeometryAddress_ECMS,
                F_Component_GeometryAddress_LOD
            );
        };
    }
}