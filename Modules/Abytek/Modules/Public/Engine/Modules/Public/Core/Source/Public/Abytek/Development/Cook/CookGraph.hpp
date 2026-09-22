#pragma once

#include "Abytek/ExecutionGraph.hpp"


#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
namespace Abytek
{
    class ABYTEK_ENGINE_CORE_API F_CookGraph final : public F_ExecutionGraph
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(F_ExecutionGraph)
        ABYTEK_END_REFLECTOR(F_CookGraph);
        
    public:
        ABYTEK_DECLARE_OBJECT_SINGLETON_CRT(F_CookGraph);

    private:

    public:
        
    public:
        F_CookGraph();
        ~F_CookGraph() override;
    };
}
#endif