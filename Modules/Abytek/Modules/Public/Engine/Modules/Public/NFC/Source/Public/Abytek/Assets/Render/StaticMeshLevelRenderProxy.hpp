#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"

   
namespace Abytek
{
    class F_StaticMeshRenderProxy;
    
    struct F_StaticMeshLevelRenderProxy
    {
        TS<F_StaticMeshRenderProxy> MeshRenderProxy;
    };
}