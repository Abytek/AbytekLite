#include "Abytek/Assets/Render/MaterialInstanceRenderProxy.hpp"


namespace Abytek
{
    F_MaterialInstanceRenderProxy::F_MaterialInstanceRenderProxy(const TW_Valid<F_MaterialInstance>& Owner) :
        A_MaterialInterfaceRenderProxy(Owner)
    {
    }
    F_MaterialInstanceRenderProxy::~F_MaterialInstanceRenderProxy()
    {
    }

    void F_MaterialInstanceRenderProxy::OnCreateRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        A_MaterialInterfaceRenderProxy::OnCreateRenderState_RenderTask(SubmissionItemContainer);
    }
    void F_MaterialInstanceRenderProxy::OnDestroyRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _MaterialRenderProxy = {};
        A_MaterialInterfaceRenderProxy::OnDestroyRenderState_RenderTask(SubmissionItemContainer);
    }

    TS<F_MaterialRenderProxy> F_MaterialInstanceRenderProxy::GetMaterialRenderProxy() const
    {
        return _MaterialRenderProxy;
    }
}
