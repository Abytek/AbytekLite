#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"


namespace Abytek
{
    enum class E_RHIPassPosition : U8
    {
        NONE,
        
        PROLOGUE,
        USER,
        EPILOGUE,
        
        DEFAULT = USER
    };
}