#include "Abytek/SRPRenderView.hpp"
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
        BeginOpaqueVisibilityBuffer(SubmissionItemContainer);
        
        {
            ClearOpaqueVisibilityBuffer(SubmissionItemContainer);
        }
    }
    void F_SRPRenderView::OnEndFrame(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        EndOpaqueVisibilityBuffer(SubmissionItemContainer);
        A_RenderView::OnEndFrame(SubmissionItemContainer);
    }

    void F_SRPRenderView::BeginOpaqueVisibilityBuffer(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        auto Context = H_RHI::GetMainContext();
        
        const auto& FeatureSupports = GetRHIFeatureSupports();
        const auto& Resolution = GetResolution();
        
        F_RHITextureBuildParams OpaqueVisibilityBufferBuildParams;
        OpaqueVisibilityBufferBuildParams.Context = Context.Weak();
        OpaqueVisibilityBufferBuildParams.TextureAspect.Width = Resolution.X;
        OpaqueVisibilityBufferBuildParams.TextureAspect.Height = Resolution.Y;
        OpaqueVisibilityBufferBuildParams.TextureAspect.DimensionCount = 2;
        OpaqueVisibilityBufferBuildParams.Format = SRP::GetVisibilityFormat(FeatureSupports);
        OpaqueVisibilityBufferBuildParams.AccessCapabilities = (
            F_RHIResourceAccess::MakeSRVCapabilities() 
            | F_RHIResourceAccess::MakeUAVCapabilities()
        );
        OpaqueVisibilityBufferBuildParams.AdditionalFlags |= E_RHIResourceAdditionalFlag::TRANSIENT;
        _OpaqueVisibilityBuffer = RACreateAndBuildShared<A_RHIResource>(OpaqueVisibilityBufferBuildParams);
        
        F_RHITextureViewBuildParams OpaqueVisibilitySRVBuildParams;
        OpaqueVisibilitySRVBuildParams.Context = Context.Weak();
        OpaqueVisibilitySRVBuildParams.Resource = _OpaqueVisibilityBuffer;
        OpaqueVisibilitySRVBuildParams.Access = F_RHIResourceAccess::MakeSRV();
        _OpaqueVisibilitySRV = RACreateAndBuildShared<A_RHIResourceView>(OpaqueVisibilitySRVBuildParams);
        
        F_RHITextureViewBuildParams OpaqueVisibilityUAVBuildParams;
        OpaqueVisibilityUAVBuildParams.Context = Context.Weak();
        OpaqueVisibilityUAVBuildParams.Resource = _OpaqueVisibilityBuffer;
        OpaqueVisibilityUAVBuildParams.Access = F_RHIResourceAccess::MakeUAV();
        OpaqueVisibilityUAVBuildParams.TextureViewAspect.UAVClearable = true;
        _OpaqueVisibilityUAV = RACreateAndBuildShared<A_RHIResourceView>(OpaqueVisibilityUAVBuildParams);
    }
    void F_SRPRenderView::EndOpaqueVisibilityBuffer(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _OpaqueVisibilityUAV = {};
        _OpaqueVisibilitySRV = {};
        _OpaqueVisibilityBuffer = {};
    }

    void F_SRPRenderView::ClearOpaqueVisibilityBuffer(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        F_RHIClearUAVUIntPassBuildParams PassBuildParams;
        PassBuildParams.Context = H_RHI::GetMainContext().Weak();
        PassBuildParams.UAV = _OpaqueVisibilityUAV;
        F_Vector4_U64 ClearValue(ABYTEK_U64_MAX);
        H_RHISubmissionUtilities::ClearUAVUInt(
            SubmissionItemContainer,
            _OpaqueVisibilityUAV,
            ClearValue,
            ABYTEK_DEBUG_NAME("Abytek::SRP::ClearOpaqueVisibilityBuffer")
        );
    }
}
