#include "Abytek/Renderer/StandardPrimitive/MaterialTargetTypeProxy.hpp"
#include "Abytek/Renderer/StandardPrimitive/MaterialTargetTypeProxyManager.hpp"
#include "Abytek/Renderer/WorldRenderResource.hpp"
#include "Abytek/Renderer/RenderScene.hpp"


namespace Abytek
{
    A_MaterialTargetTypeProxy::A_MaterialTargetTypeProxy(const TW_Valid<A_MaterialTargetType>& MaterialTargetType) :
        A_WorldRenderResourceChild(MaterialTargetType)
    {
    }
    A_MaterialTargetTypeProxy::~A_MaterialTargetTypeProxy()
    {
    }

    void A_MaterialTargetTypeProxy::OnCreateRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        auto Manager = GetWorldRenderResource()->GetScene()->GetMaterialTargetTypeProxyManager();
        Manager->_RegisterTypeProxy(ABYTEK_WTHIS());
    }
    void A_MaterialTargetTypeProxy::OnDestroyRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        auto Manager = GetWorldRenderResource()->GetScene()->GetMaterialTargetTypeProxyManager();
        Manager->_RegisterTypeProxy(ABYTEK_WTHIS());
        
        _HashCode = 0;
    }
}
