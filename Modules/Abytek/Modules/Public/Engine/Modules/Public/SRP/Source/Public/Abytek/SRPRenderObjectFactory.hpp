#pragma once

#include "Abytek/Engine.SRP.prerequisites.hpp"
#include "Abytek/RenderBase/RenderObjectFactory.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_SRP_API F_SRPRenderObjectFactory : public A_RenderObjectFactory
    {
    private:
    
    public:
        
    public:
        F_SRPRenderObjectFactory(const TW<F_WorldRenderResource>& WorldRenderResource);
        ~F_SRPRenderObjectFactory() override;
        
    public:
        TS<A_RenderScene> CreateScene(const F_RenderSceneBuildParams& BuildParams) override;
        TS<A_RenderViewFamily> CreateViewFamily(const F_RenderViewFamilyBuildParams& BuildParams) override;
        TS<A_RenderView> CreateView(const F_RenderViewBuildParams& BuildParams) override;
        TS<A_Renderer> CreateRenderer(const F_RendererBuildParams& BuildParams) override;
    };
}
