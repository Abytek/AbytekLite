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

    void A_RenderProxy::OnInit_RenderTask()
    {
    }
    void A_RenderProxy::OnRelease_RenderTask()
    {
    }

    void A_RenderProxy::OnCreateRenderState_RenderTask()
    {
    }
    void A_RenderProxy::OnDestroyRenderState_RenderTask()
    {
    }
}
