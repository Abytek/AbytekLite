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

    TS<A_RenderPrimitiveProcessor_Simple> A_RenderObjectFactory::CreatePrimitiveProcessor_Simple()
    {
        return {};
    }
    TS<A_RenderPrimitiveSet_Simple> A_RenderObjectFactory::CreatePrimitiveSet_Simple()
    {
        return {};
    }
}
