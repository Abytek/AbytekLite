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
    TS<A_RenderScene> F_SRPRenderObjectFactory::CreateScene(const F_RenderSceneBuildParams& BuildParams)
    {
        return TS<F_SRPRenderScene>()(GetWorldRenderResource(), BuildParams);
    }
    TS<A_RenderViewFamily> F_SRPRenderObjectFactory::CreateViewFamily(const F_RenderViewFamilyBuildParams& BuildParams)
    {
        return TS<A_RenderViewFamily>()(GetWorldRenderResource(), BuildParams);
    }
    TS<A_RenderView> F_SRPRenderObjectFactory::CreateView(const F_RenderViewBuildParams& BuildParams)
    {
        return TS<F_SRPRenderView>()(GetWorldRenderResource(), BuildParams);
    }
    TS<A_Renderer> F_SRPRenderObjectFactory::CreateRenderer(const F_RendererBuildParams& BuildParams)
    {
        return TS<F_SRPRenderer>()(GetWorldRenderResource(), BuildParams);
    }
}
