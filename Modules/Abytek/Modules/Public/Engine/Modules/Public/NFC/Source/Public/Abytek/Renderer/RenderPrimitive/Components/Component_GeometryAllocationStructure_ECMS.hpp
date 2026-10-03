#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/Renderer/GPUData/GPUDataComponentType.hpp"
#include "Abytek/Renderer/RenderGeometry/RenderGeometryCommon.hpp"


namespace Abytek
{
    namespace RenderPrimitive
    {
        struct F_Component_GeometryAllocationStructure_ECMS
        {
            ABYTEK_GPU_DATA_COMPONENT_TYPE(
                F_Component_GeometryAllocationStructure_ECMS, 
                ABYTEK_NAME("GeometryAllocationStructure_ECMS"),
                ABYTEK_NAME("Abytek::RenderPrimitive::F_Component_GeometryAllocationStructure_ECMS")
            );
            
            F_RenderGeometryAllocationStructure_ECMS AllocationStructure;
        };
    }
}
