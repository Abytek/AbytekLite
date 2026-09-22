#include "Abytek/RenderProxy.hpp"
#include "Abytek/Frame/FrameHelper.hpp"


namespace Abytek
{
    A_RenderProxy::A_RenderProxy(const TW_Valid<A_Renderable>& Owner) :
        _Owner(Owner)
    {
    }
    A_RenderProxy::~A_RenderProxy()
    {
    }

    void A_RenderProxy::OnInit_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
    }
    void A_RenderProxy::OnRelease_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
    }

    void A_RenderProxy::OnCreateRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
    }
    void A_RenderProxy::OnDestroyRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
    }
}
