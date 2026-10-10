#include "Abytek/Assets/Render/MaterialInterfaceRenderProxy.hpp"


namespace Abytek
{
    A_MaterialInterfaceRenderProxy::A_MaterialInterfaceRenderProxy(const TW_Valid<A_MaterialInterface>& Owner) :
        A_WorldRenderResourceChild(Owner)
    {
    }
    A_MaterialInterfaceRenderProxy::~A_MaterialInterfaceRenderProxy()
    {
    }

    void A_MaterialInterfaceRenderProxy::OnCreateRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
    }
    void A_MaterialInterfaceRenderProxy::OnDestroyRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _Name = {};
    }
}
