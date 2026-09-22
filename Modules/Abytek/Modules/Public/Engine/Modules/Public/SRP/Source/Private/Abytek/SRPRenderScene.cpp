#include "Abytek/SRPRenderScene.hpp"


namespace Abytek
{
    void F_SRPRenderScene::Init(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const F_RenderSceneBuildParams& BuildParams
    )
    {
        A_RenderScene::Init(SubmissionItemContainer, BuildParams);
    }
    void F_SRPRenderScene::Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        A_RenderScene::Release(SubmissionItemContainer);
    }

    void F_SRPRenderScene::OnBeginUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        A_RenderScene::OnBeginUpdate(SubmissionItemContainer);
    }
    void F_SRPRenderScene::OnEndUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        A_RenderScene::OnEndUpdate(SubmissionItemContainer);
    }
    void F_SRPRenderScene::OnBeginPostUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        A_RenderScene::OnBeginPostUpdate(SubmissionItemContainer);
    }
    void F_SRPRenderScene::OnEndPostUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        A_RenderScene::OnEndPostUpdate(SubmissionItemContainer);
    }
}
