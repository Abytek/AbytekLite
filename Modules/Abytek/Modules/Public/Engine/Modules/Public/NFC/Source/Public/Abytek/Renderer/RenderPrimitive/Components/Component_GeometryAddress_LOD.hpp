#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/Renderer/GPUData/GPUDataComponentType.hpp"
#include "Abytek/Renderer/RenderGeometry/RenderGeometryCommon.hpp"


namespace Abytek
{
    namespace RenderPrimitive
    {
        struct F_Component_GeometryAddress_LOD
        {
            ABYTEK_GPU_DATA_COMPONENT_TYPE(
                F_Component_GeometryAddress_LOD, 
                ABYTEK_NAME("GeometryAddress_LOD"),
                ABYTEK_NAME("Abytek::RenderPrimitive::F_Component_GeometryAddress_LOD")
            );
            
            F_RenderGeometryAddress Address;
        };
    }
}
