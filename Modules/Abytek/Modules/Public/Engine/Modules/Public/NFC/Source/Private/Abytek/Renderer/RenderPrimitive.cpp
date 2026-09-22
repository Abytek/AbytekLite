#include "Abytek/Renderer/RenderPrimitive.hpp"


namespace Abytek
{
    A_RenderPrimitive::A_RenderPrimitive(const TW_Valid<F_WorldRenderResource>& WorldRenderResource, const F_RenderPrimitiveBuildParams& BuildParams) :
        A_RenderObject(WorldRenderResource)
    {
    }
    A_RenderPrimitive::~A_RenderPrimitive()
    {
    }
}
