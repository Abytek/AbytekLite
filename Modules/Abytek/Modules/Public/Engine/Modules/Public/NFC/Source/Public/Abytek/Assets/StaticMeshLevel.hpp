#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"

   
namespace Abytek
{
    class F_StaticMesh;
    
    struct F_StaticMeshLevel
    {
        TS<F_StaticMesh> Mesh;
    };
}