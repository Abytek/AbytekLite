#include "Abytek/Development/Cook/CookGraph.hpp"


#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
namespace Abytek
{
    ABYTEK_REFLECT(F_CookGraph)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_CookGraph"));
    }

    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(F_CookGraph);
    
    F_CookGraph::F_CookGraph()
    {
        ABYTEK_BIND_OBJECT_SINGLETON_CRT();
    }
    F_CookGraph::~F_CookGraph()
    {
    }
}
#endif