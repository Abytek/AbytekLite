#include "Abytek/SRPRenderViewFamily.hpp"


namespace Abytek
{
    void F_SRPRenderViewFamily::Init(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const F_RenderViewFamilyBuildParams& BuildParams
    )
    {
        A_RenderViewFamily::Init(SubmissionItemContainer, BuildParams);
    }
    void F_SRPRenderViewFamily::Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        A_RenderViewFamily::Release(SubmissionItemContainer);
    }
}
