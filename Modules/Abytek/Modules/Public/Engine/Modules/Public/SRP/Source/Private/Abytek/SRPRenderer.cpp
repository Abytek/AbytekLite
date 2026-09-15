#include "Abytek/SRPRenderer.hpp"
#include "Abytek/RenderBase/RenderViewFamily.hpp"
#include "Abytek/RenderBase/RenderView.hpp"
#include "Abytek/SRPBasicDrawers/Cube.hpp"


namespace Abytek
{
    F_SRPRenderer::F_SRPRenderer(const TW_Valid<F_WorldRenderResource>& WorldRenderResource, const F_RendererBuildParams& BuildParams) :
        A_Renderer(WorldRenderResource, BuildParams)
    {
    }
    F_SRPRenderer::~F_SRPRenderer()
    {
    }

    void F_SRPRenderer::OnRender()
    {
        A_Renderer::OnRender();
    }
}
