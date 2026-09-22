#include "Abytek/SRPRenderObjectFactory.hpp"
#include "Abytek/SRPRenderScene.hpp"
#include "Abytek/SRPRenderViewFamily.hpp"
#include "Abytek/SRPRenderView.hpp"
#include "Abytek/SRPRenderer.hpp"


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
}
