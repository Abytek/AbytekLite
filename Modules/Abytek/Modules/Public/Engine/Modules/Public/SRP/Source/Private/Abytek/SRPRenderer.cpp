#include "Abytek/SRPRenderer.hpp"
#include "Abytek/Renderer/RenderViewFamily.hpp"
#include "Abytek/Renderer/RenderView.hpp"
#include "Abytek/SRPBasicDrawers/Cube.hpp"


namespace Abytek
{
    void F_SRPRenderer::Init(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const F_RendererBuildParams& BuildParams
    )
    {
        A_Renderer::Init(SubmissionItemContainer, BuildParams);
    }
    void F_SRPRenderer::Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        A_Renderer::Release(SubmissionItemContainer);
    }

    void F_SRPRenderer::OnRender(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        A_Renderer::OnRender(SubmissionItemContainer);
    }
}
