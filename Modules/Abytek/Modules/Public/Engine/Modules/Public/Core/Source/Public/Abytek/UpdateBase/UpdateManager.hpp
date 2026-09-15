#pragma once

#include "Abytek/ExecutionGraph.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_CORE_API F_UpdateManager final : public F_ExecutionGraph
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(F_ExecutionGraph)
        ABYTEK_END_REFLECTOR(F_UpdateManager)
        
    public:
        ABYTEK_DECLARE_OBJECT_SINGLETON_CRT(F_UpdateManager);

    private:

    public:
        
    public:
        F_UpdateManager();
        ~F_UpdateManager() override;
    };
}
