#include "Abytek/ActorComponents/Render/PrimitiveComponentRenderProxy.hpp"
#include "Abytek/Renderer/RenderScene.hpp"
#include "Abytek/Renderer/RenderViewFamily.hpp"
#include "Abytek/Renderer/RenderView.hpp"
#include "Abytek/Renderer/Renderer.hpp"
#include "Abytek/Renderer/RenderObjectFactory.hpp"
#include "Abytek/Renderer/WorldRenderResource.hpp"


namespace Abytek
{
    A_PrimitiveComponentRenderProxy::A_PrimitiveComponentRenderProxy(const TW_Valid<A_PrimitiveComponent>& Owner) :
        A_RenderableComponentProxy(Owner)
    {
    }
    A_PrimitiveComponentRenderProxy::~A_PrimitiveComponentRenderProxy()
    {
    }

    void A_PrimitiveComponentRenderProxy::OnCreateRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _CreateAndInitPrimitiveSets(SubmissionItemContainer);
    }
    void A_PrimitiveComponentRenderProxy::OnDestroyRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        for (auto It = _PrimitiveSets.rbegin(); It != _PrimitiveSets.rend(); ++It)
        {
            (*It)->Deactivate(SubmissionItemContainer);
        }
        for (auto It = _PrimitiveSets.rbegin(); It != _PrimitiveSets.rend(); ++It)
        {
            (*It)->Release(SubmissionItemContainer);
        }
        _PrimitiveSets = {};
        _WorldTransformMatrix = Identity<F_Matrix4x4_F32>();
    }

    void A_PrimitiveComponentRenderProxy::_CreateAndInitPrimitiveSets(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        CreatePrimitiveSets(SubmissionItemContainer, _PrimitiveSets);
#ifdef ABYTEK_DEBUG_INFO
        for (U32 Idx = 0; Idx < _PrimitiveSets.size(); ++Idx)
        {
            _PrimitiveSets[Idx]->SetDebugName(
                *GetDebugName()
                + ABYTEK_TEXT("[")
                + ToText(Idx)
                + ABYTEK_TEXT("]")
            );
        }
#endif
        InitPrimitiveSets(SubmissionItemContainer, _PrimitiveSets);
        for (const auto& PrimitiveSet : _PrimitiveSets)
        {
            PrimitiveSet->Activate(SubmissionItemContainer);
        }
    }
}
