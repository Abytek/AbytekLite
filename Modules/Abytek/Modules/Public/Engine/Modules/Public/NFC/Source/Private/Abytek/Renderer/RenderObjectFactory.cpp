#include "Abytek/Renderer/RenderObjectFactory.hpp"


namespace Abytek
{
    A_RenderObjectFactory::A_RenderObjectFactory(const TW_Valid<F_WorldRenderResource>& WorldRenderResource) :
        _WorldRenderResource(WorldRenderResource)
    {
    }
    A_RenderObjectFactory::~A_RenderObjectFactory()
    {
    }
}
