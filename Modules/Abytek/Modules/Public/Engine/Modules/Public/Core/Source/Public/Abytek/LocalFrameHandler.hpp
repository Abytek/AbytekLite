#pragma once

#include "Abytek/Engine.Core.prerequisites.hpp"
#include "Abytek/TransientAllocator.hpp"
#include "Abytek/TransientObjectSubsystem.hpp"


namespace Abytek
{
    struct F_LocalFrameHandler
    {
        TW<F_TransientMemoryAdapter> TransientMemoryAdapter;
        TW<F_TransientObjectAdapter> TransientObjectAdapter;
        
        void Init();
        void Release();
        void BeginFrame();
        void EndFrame();
    };
}
