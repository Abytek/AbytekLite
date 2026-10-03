#include "Abytek/SRPRenderObjectFactory.hpp"
#include "Abytek/SRPRenderScene.hpp"
#include "Abytek/SRPRenderViewFamily.hpp"
#include "Abytek/SRPRenderView.hpp"
#include "Abytek/SRPRenderer.hpp"
#include "Abytek/SimplePrimitive/SRPRenderPrimitiveProcessor_Simple.hpp"
#include "Abytek/SimplePrimitive/SRPRenderPrimitiveSet_Simple.hpp"


namespace Abytek
{
    F_SRPRenderObjectFactory::F_SRPRenderObjectFactory(const TW<F_WorldRenderResource>& WorldRenderResource) :
        A_RenderObjectFactory(WorldRenderResource)
    {
    }
    F_SRPRenderObjectFactory::~F_SRPRenderObjectFactory()
    {
    }    
    TS<A_RenderScene> F_SRPRenderObjectFactory::CreateScene()
    {
        return F_SRPRenderScene::Create(GetWorldRenderResource());
    }
    TS<A_RenderViewFamily> F_SRPRenderObjectFactory::CreateViewFamily()
    {
        return F_SRPRenderViewFamily::Create(GetWorldRenderResource());
    }
    TS<A_RenderView> F_SRPRenderObjectFactory::CreateView()
    {
        return F_SRPRenderView::Create(GetWorldRenderResource());
    }
    TS<A_Renderer> F_SRPRenderObjectFactory::CreateRenderer()
    {
        return F_SRPRenderer::Create(GetWorldRenderResource());
    }

    TS<A_RenderPrimitiveProcessor_Simple> F_SRPRenderObjectFactory::CreatePrimitiveProcessor_Simple()
    {
        return F_SRPRenderPrimitiveProcessor_Simple::Create(GetWorldRenderResource());
    }
    TS<A_RenderPrimitiveSet_Simple> F_SRPRenderObjectFactory::CreatePrimitiveSet_Simple()
    {
        return F_SRPRenderPrimitiveSet_Simple::Create(GetWorldRenderResource());
    }
}
