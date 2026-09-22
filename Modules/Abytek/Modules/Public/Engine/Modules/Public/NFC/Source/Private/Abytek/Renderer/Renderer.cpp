#include "Abytek/Renderer/Renderer.hpp"
#include "Abytek/Renderer/RenderViewFamily.hpp"
#include "Abytek/Renderer/RenderView.hpp"
#include "Abytek/ActorComponents/Render/CanvasRenderProxy.hpp"


namespace Abytek
{
    void A_Renderer::Init(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const F_RendererBuildParams& BuildParams
    )
    {
        InitMinimal(SubmissionItemContainer);
        
        _Scene = BuildParams.Scene;
        _ViewFamily = BuildParams.ViewFamily;
    }
    void A_Renderer::Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _ViewFamily = {};
        _Scene = {};
        
        A_RenderObject::Release(SubmissionItemContainer);
    }

    void A_Renderer::OnRender(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
    }

    void A_Renderer::Render(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        ABYTEK_RHI_CAPTURE_EVENT_SCOPE(
            SubmissionItemContainer,  
            ABYTEK_TEXT("Abytek::A_Renderer::Render(") + *GetDebugName() + ABYTEK_TEXT(")")
        );
        {
            TF_Function<void(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)> Command;
            while (PreRenderQueue.TryPop(Command))
            {
                Command(SubmissionItemContainer);
            }
        }
        OnRender(SubmissionItemContainer);
        {
            TF_Function<void(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)> Command;
            while (PostRenderQueue.TryPop(Command))
            {
                Command(SubmissionItemContainer);
            }
        }
    }
}
