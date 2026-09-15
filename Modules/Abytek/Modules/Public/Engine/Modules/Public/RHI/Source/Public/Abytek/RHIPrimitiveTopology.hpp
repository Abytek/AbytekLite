#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"


namespace Abytek
{
    enum class E_RHIPrimitiveTopology
    {
        NONE = 0,
        
        POINT_LIST,
        
        LINE_LIST,
        LINE_STRIP,
        
        TRIANGLE_LIST,
        TRIANGLE_STRIP,
        
        LINE_LIST_ADJ,
        LINE_STRIP_ADJ,
        
        TRIANGLE_LIST_ADJ,
        TRIANGLE_STRIP_ADJ,
        
        NUM,
        
        DEFAULT = TRIANGLE_LIST
    };
}
