#include "Abytek/LocalFrameHandler.hpp"
#include "Abytek/Profiling.hpp"


namespace Abytek
{
    void F_LocalFrameHandler::Init()
    {
        ABYTEK_PROFILER_EVENT();
        TransientMemoryAdapter = F_TransientMemoryManager::GetInstance()->RegisterAdapter();
        TransientObjectAdapter = F_TransientObjectManager::GetInstance()->RegisterAdapter();
    }
    void F_LocalFrameHandler::Release()
    {
        ABYTEK_PROFILER_EVENT();
        F_TransientObjectManager::GetInstance()->DeregisterAdapter(TransientObjectAdapter);
        F_TransientMemoryManager::GetInstance()->UnregisterAdapter(TransientMemoryAdapter);
    }
    void F_LocalFrameHandler::BeginFrame()
    {
        ABYTEK_PROFILER_EVENT();
        TransientMemoryAdapter->BeginFrame();
        TransientObjectAdapter->BeginFrame();
    }
    void F_LocalFrameHandler::EndFrame()
    {
        ABYTEK_PROFILER_EVENT();
        TransientObjectAdapter->EndFrame();
        TransientMemoryAdapter->EndFrame();
    }
}
