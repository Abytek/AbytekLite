#include "Abytek/Assets/Render/MaterialRenderProxy.hpp"


namespace Abytek
{
    F_MaterialRenderProxy::F_MaterialRenderProxy(const TW_Valid<A_Material>& Owner) :
        A_MaterialInterfaceRenderProxy(Owner)
    {
    }
    F_MaterialRenderProxy::~F_MaterialRenderProxy()
    {
    }

    void F_MaterialRenderProxy::OnCreateRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        A_MaterialInterfaceRenderProxy::OnCreateRenderState_RenderTask(SubmissionItemContainer);
    }
    void F_MaterialRenderProxy::OnDestroyRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _RenderPackData = {};
        A_MaterialInterfaceRenderProxy::OnDestroyRenderState_RenderTask(SubmissionItemContainer);
    }

    TS<F_MaterialRenderProxy> F_MaterialRenderProxy::GetMaterialRenderProxy() const
    {
        return ABYTEK_STHIS_MUTABLE();
    }
}
