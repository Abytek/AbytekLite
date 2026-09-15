#include "Abytek/SRPRenderView.hpp"
#include "Abytek/RHIClearUAVUIntPass.hpp"
#include "Abytek/RenderBase/WorldRenderResource.hpp"


namespace Abytek
{
    F_SRPRenderView::F_SRPRenderView(const TW_Valid<F_WorldRenderResource>& WorldRenderResource, const F_RenderViewBuildParams& BuildParams) :
        A_RenderView(WorldRenderResource, BuildParams)
    {
    }
    F_SRPRenderView::~F_SRPRenderView()
    {
    }

    void F_SRPRenderView::OnBeginFrame()
    {
        A_RenderView::OnBeginFrame();
        BeginOpaqueVisibilityBuffer();
        
        {
            auto Context = H_RHI::GetMainContext();
            auto Process = H_RHI::GetMainProcess();
            
            auto SubmissionList = RACreateAndBuildShared<A_RHISubmissionList>(Process.Weak());
            ClearOpaqueVisibilityBuffer(*SubmissionList);
            Process->AddSubmissionItem(SubmissionList);
        }
    }
    void F_SRPRenderView::OnEndFrame()
    {
        EndOpaqueVisibilityBuffer();
        A_RenderView::OnEndFrame();
    }

    void F_SRPRenderView::BeginOpaqueVisibilityBuffer()
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
    void F_SRPRenderView::EndOpaqueVisibilityBuffer()
    {
        _OpaqueVisibilityUAV = {};
        _OpaqueVisibilitySRV = {};
        _OpaqueVisibilityBuffer = {};
    }

    void F_SRPRenderView::ClearOpaqueVisibilityBuffer(I_RHISubmissionItemContainer& SubmissionItemContainer)
    {
        F_RHIClearUAVUIntPassBuildParams PassBuildParams;
        PassBuildParams.Context = H_RHI::GetMainContext().Weak();
        PassBuildParams.UAV = _OpaqueVisibilityUAV;
        F_Vector4_U64 ClearValue(ABYTEK_U64_MAX);
        H_RHIPassUtilities::ClearUAVUInt(
            SubmissionItemContainer,
            _OpaqueVisibilityUAV,
            ClearValue,
            ABYTEK_DEBUG_NAME("Abytek::SRP::ClearOpaqueVisibilityBuffer")
        );
    }
}
