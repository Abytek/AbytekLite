#pragma once

#include "Abytek/Engine.SRP.prerequisites.hpp"
#include "Abytek/RenderBase/Renderer.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_SRP_API F_SRPRenderer : public A_Renderer
    {
    private:
        
    public:

    public:
        F_SRPRenderer(const TW_Valid<F_WorldRenderResource>& WorldRenderResource, const F_RendererBuildParams& BuildParams);
        ~F_SRPRenderer() override;
        
    protected:
        void OnRender() override;
    };
}
