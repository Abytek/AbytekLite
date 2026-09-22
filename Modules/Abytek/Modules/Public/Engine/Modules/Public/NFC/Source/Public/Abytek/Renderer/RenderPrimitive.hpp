#pragma once

#include "Abytek/Renderer/RenderObject.hpp"


namespace Abytek
{
    struct F_RenderPrimitiveBuildParams
    {
    };
    class ABYTEK_ENGINE_NFC_API A_RenderPrimitive : public A_RenderObject
    {
    public:
        A_RenderPrimitive(const TW_Valid<F_WorldRenderResource>& WorldRenderResource, const F_RenderPrimitiveBuildParams& BuildParams);
        ~A_RenderPrimitive() override;
    };
}
