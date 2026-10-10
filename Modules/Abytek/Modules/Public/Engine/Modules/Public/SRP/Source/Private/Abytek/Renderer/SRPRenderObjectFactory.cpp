#include "Abytek/Renderer/SRPRenderObjectFactory.hpp"
#include "Abytek/Renderer/SRPRenderScene.hpp"
#include "Abytek/Renderer/SRPRenderViewFamily.hpp"
#include "Abytek/Renderer/SRPRenderView.hpp"
#include "Abytek/Renderer/SRPRenderer.hpp"
#include "Abytek/Renderer/SimplePrimitive/SRPRenderPrimitiveProcessor.hpp"
#include "Abytek/Renderer/SimplePrimitive/SRPRenderPrimitiveSet.hpp"
#include "Abytek/Renderer/StandardPrimitive/SRPRenderPrimitiveProcessor.hpp"
#include "Abytek/Renderer/StandardPrimitive/SRPRenderPrimitiveSet.hpp"


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

    TS<A_RenderPrimitiveProcessor_Standard> F_SRPRenderObjectFactory::CreatePrimitiveProcessor_Standard()
    {
        return F_SRPRenderPrimitiveProcessor_Standard::Create(GetWorldRenderResource());
    }
    TS<A_RenderPrimitiveSet_Standard> F_SRPRenderObjectFactory::CreatePrimitiveSet_Standard()
    {
        return F_SRPRenderPrimitiveSet_Standard::Create(GetWorldRenderResource());
    }
}
