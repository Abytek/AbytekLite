#include "Abytek/SRPRenderer.hpp"
#include "Abytek/Renderer/RenderScene.hpp"
#include "Abytek/Renderer/RenderViewFamily.hpp"
#include "Abytek/Renderer/RenderView.hpp"
#include "Abytek/SRPBasicDrawers/Cube.hpp"
#include "Abytek/SimplePrimitive/SRPVisibilityBufferPass_Simple.hpp"
#include "Abytek/SimplePrimitive/SRPColorPass_Simple.hpp"


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
        
        auto ViewFamily = GetViewFamily();
        auto CameraComponentRenderProxy = ViewFamily->GetCameraComponentRenderProxy();
        auto CanvasComponentRenderProxy = ViewFamily->GetCanvasComponentRenderProxy();
        
        // Visibility buffer pass (simple primitives)
        {
            for (const auto& View : ViewFamily->GetViews())
            {
                SRP::SimplePrimitive::VisibilityBufferPass::Invoke(
                    SubmissionItemContainer,
                    View.Weak()
                );
            }
        }
        
        // Color pass (simple primitives)
        {
            for (const auto& View : ViewFamily->GetViews())
            {
                SRP::SimplePrimitive::ColorPass::Invoke(
                    SubmissionItemContainer,
                    View.Weak()
                );
            }
        }
    }
}
