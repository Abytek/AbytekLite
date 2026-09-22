#include "Abytek/Renderer/RenderView.hpp"
#include "Abytek/ActorComponents/Render/CameraRenderProxy.hpp"


namespace Abytek
{
    void A_RenderView::Init(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const F_RenderViewBuildParams& BuildParams
    )
    {
        InitMinimal(SubmissionItemContainer);
        
        _Family = BuildParams.Family;
        _Flags = BuildParams.Flags;
        _ProjectionOptions = BuildParams.ProjectionOptions;
        _RTV = BuildParams.RTV;
        
        _UniformData.Resolution = BuildParams.Resolution;
        _UniformData.AspectRatio = BuildParams.AspectRatio;
        _UniformData.ViewMatrix = BuildParams.ViewMatrix;
        UpdateProjectionMatrix(SubmissionItemContainer);
        
        _UniformBindGroup = F_RenderViewUniformDataBinding::Instantiate(
            GetRenderRegistryRuntime()
        ).CreateBindGroup();
        UpdateUniformBindGroup(SubmissionItemContainer);
        
        UpdateDefaultViewportScissorConfig(SubmissionItemContainer);
        
        UpdateDepthBufferAndDSV(SubmissionItemContainer);
    }
    void A_RenderView::Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _RTV = {};
        _ProjectionOptions = {};
        _Flags = E_RenderViewFlag::NONE;
        _Family = {};
        
        A_RenderObject::Release(SubmissionItemContainer);
    }

    void A_RenderView::OnBeginFrame(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        UpdateProjectionMatrix(SubmissionItemContainer);
        UpdateUniformBindGroup(SubmissionItemContainer);
        UpdateDefaultViewportScissorConfig(SubmissionItemContainer);
        UpdateDepthBufferAndDSV(SubmissionItemContainer);
        
        // Prepare new frame
        {
            ABYTEK_RHI_CAPTURE_EVENT_SCOPE(
                SubmissionItemContainer,  
                ABYTEK_TEXT("Abytek::A_RenderView::PrepareNewFrame(") + *GetDebugName() + ABYTEK_TEXT(")")
            );
            if (_DSV)
            {
                H_RHISubmissionUtilities::ClearDSV(
                    SubmissionItemContainer,
                    _DSV
                );
            }
        }
    }
    void A_RenderView::OnEndFrame(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
    }

    void A_RenderView::BeginFrame(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
#ifdef ABYTEK_DEBUG_INFO
        SubmissionItemContainer->PushAdditionalStackCaptureEventStates(
            ABYTEK_TEXT("Abytek::A_RenderView::Frame(") + *GetDebugName() + ABYTEK_TEXT(")")
        );
#endif
        OnBeginFrame(SubmissionItemContainer);
    }
    void A_RenderView::EndFrame(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        OnEndFrame(SubmissionItemContainer);
#ifdef ABYTEK_DEBUG_INFO
        SubmissionItemContainer->PopAdditionalStackCaptureEventStates();
#endif
    }

    void A_RenderView::UpdateProjectionMatrix(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _UniformData.ProjectionMatrix = _ProjectionOptions.GetProjectionMatrix(_UniformData.AspectRatio);
    }
    void A_RenderView::UpdateUniformBindGroup(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _UniformBindGroup->BindUniformData(ABYTEK_NAME("ViewUniformData"), _UniformData);
        _UniformBindGroup->Commit();
#ifdef ABYTEK_DEBUG_INFO
        _UniformBindGroup->SetDebugName(
            *GetDebugName()
            + ABYTEK_TEXT(".UniformBindGroup")
        );
#endif
    }
    void A_RenderView::UpdateDefaultViewportScissorConfig(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _DefaultViewportScissorConfig = F_RHIViewportScissorConfig::Make(
            F_Vector2_F32(_UniformData.Resolution)  
        );
    }
    void A_RenderView::UpdateDepthBufferAndDSV(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        if (_DepthBuffer)
        {
            const auto& LastTextureAspect = _DepthBuffer->GetTextureAspect();
            if (
                (LastTextureAspect.Width == _UniformData.Resolution.X)
                && (LastTextureAspect.Height == _UniformData.Resolution.Y)
            )
            {
                return;
            }
        }
        
        F_RHITextureBuildParams DepthBufferBuildParams;
        DepthBufferBuildParams.Context = H_RHI::GetMainContext().Weak();
        DepthBufferBuildParams.Format = E_RHIFormat::R32_TYPELESS;
        DepthBufferBuildParams.TextureAspect.Width = _UniformData.Resolution.X;
        DepthBufferBuildParams.TextureAspect.Height = _UniformData.Resolution.Y;
        DepthBufferBuildParams.TextureAspect.DimensionCount = 2;
        DepthBufferBuildParams.AccessCapabilities = F_RHIResourceAccess::MakeDSVCapabilities();
        _DepthBuffer = RACreateAndBuildShared<A_RHIResource>(DepthBufferBuildParams);
#ifdef ABYTEK_DEBUG_INFO
        _DepthBuffer->SetDebugName(
            *GetDebugName()
            + ABYTEK_TEXT(".DepthBuffer")
        );
#endif
        
        F_RHITextureViewBuildParams DSVBuildParams;
        DSVBuildParams.Context = H_RHI::GetMainContext().Weak();
        DSVBuildParams.Format = E_RHIFormat::D32_FLOAT;
        DSVBuildParams.Resource = _DepthBuffer;
        DSVBuildParams.Access = F_RHIResourceAccess::MakeDSV();
        _DSV = RACreateAndBuildShared<A_RHIResourceView>(DSVBuildParams);
#ifdef ABYTEK_DEBUG_INFO
        _DepthBuffer->SetDebugName(
            *GetDebugName()
            + ABYTEK_TEXT(".DSV")
        );
#endif
    }
}
