#include "Abytek/Renderer/SRPRenderView.hpp"
#include "Abytek/RHIClearUAVUIntPass.hpp"
#include "Abytek/Renderer/WorldRenderResource.hpp"


namespace Abytek
{
    void F_SRPRenderView::Init(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const F_RenderViewBuildParams& BuildParams
    )
    {
        A_RenderView::Init(SubmissionItemContainer, BuildParams);
    }
    void F_SRPRenderView::Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        A_RenderView::Release(SubmissionItemContainer);
    }

    void F_SRPRenderView::OnBeginFrame(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        A_RenderView::OnBeginFrame(SubmissionItemContainer);
        
        _OpaqueVisibilityBuffer = SRP::VisibilityBuffer::F_OpaqueInstance::Create(
            SubmissionItemContainer,
            GetResolution(),
            GetRHIFeatureSupports(),
            E_RHIResourceAdditionalFlag::TRANSIENT
        );
#ifdef ABYTEK_DEBUG_INFO
        _OpaqueVisibilityBuffer.SetDebugName(*GetDebugName() + ABYTEK_TEXT(".OpaqueVisibilityBuffer"));
#endif
        
        _InstancedMeshletBuffer_ECMS = SRP::ECMS::F_InstancedMeshletBuffer::Create(
            SubmissionItemContainer,
            ABYTEK_WTHIS(),
            1000000,
            E_RHIResourceAdditionalFlag::TRANSIENT
        );
#ifdef ABYTEK_DEBUG_INFO
        _InstancedMeshletBuffer_ECMS.SetDebugName(*GetDebugName() + ABYTEK_TEXT(".InstancedMeshletBuffer_ECMS"));
#endif
        
        {
            ClearOpaqueVisibilityBuffer(SubmissionItemContainer);
        }
    }
    void F_SRPRenderView::OnEndFrame(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _InstancedMeshletBuffer_ECMS.Release(SubmissionItemContainer);
        _InstancedMeshletBuffer_ECMS = {};
        
        _OpaqueVisibilityBuffer.Release(SubmissionItemContainer);
        _OpaqueVisibilityBuffer = {};
        A_RenderView::OnEndFrame(SubmissionItemContainer);
    }

    void F_SRPRenderView::ClearOpaqueVisibilityBuffer(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _OpaqueVisibilityBuffer.Clear(
            SubmissionItemContainer
#ifdef ABYTEK_DEBUG_INFO
            , ABYTEK_TEXT("Abytek::SRP::ClearOpaqueVisibilityBuffer(")
            + *GetDebugName()
            + ABYTEK_TEXT(")")
#endif
        );
    }
}
