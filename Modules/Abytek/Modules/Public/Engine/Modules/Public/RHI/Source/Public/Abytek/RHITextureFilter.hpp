#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"


namespace Abytek
{
    enum class E_RHITextureFilter : U8
    {
        NONE,
        NEAREST,
        LINEAR,
        ANISOTROPIC,
        DEFAULT = LINEAR,
    };
}