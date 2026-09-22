#include "Abytek/ActorComponents/Render/CanvasRenderProxy.hpp"
#include "Abytek/ActorComponents/Render/CameraRenderProxy.hpp"
#include "Abytek/Assets/Render/TextureRenderProxy.hpp"
#include "Abytek/Frame/FrameHelper.hpp"


namespace Abytek
{
    F_CanvasRenderProxy::F_CanvasRenderProxy(const TW_Valid<F_CanvasComponent>& Owner) :
        A_RenderableComponentProxy(Owner)
    {
    }
    F_CanvasRenderProxy::~F_CanvasRenderProxy()
    {
    }

    void F_CanvasRenderProxy::OnInit_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
    }
    void F_CanvasRenderProxy::OnRelease_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
    }

    void F_CanvasRenderProxy::OnCreateRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        auto RHIContext = H_RHI::GetMainContext();
        
        if (_OutputTextureRenderProxy_Mono)
        {
            F_RHITextureViewBuildParams RTVBuildParams;
            RTVBuildParams.Context = RHIContext.Weak();
            RTVBuildParams.Resource = _OutputTextureRenderProxy_Mono->GetRHITexture();
            RTVBuildParams.Access = F_RHIResourceAccess::MakeRTV();
            _RTV_OutputTexture_Mono = RACreateAndBuildShared<A_RHIResourceView>(RTVBuildParams);
#ifdef ABYTEK_DEBUG_INFO
            _RTV_OutputTexture_Mono->SetDebugName(
                *GetDebugName()
                + ABYTEK_TEXT(".OutputTextureMono.RTV")
            );
#endif
        }
        if (_OutputTextureRenderProxy_StereoLeft)
        {
            F_RHITextureViewBuildParams RTVBuildParams;
            RTVBuildParams.Context = RHIContext.Weak();
            RTVBuildParams.Resource = _OutputTextureRenderProxy_StereoLeft->GetRHITexture();
            RTVBuildParams.Access = F_RHIResourceAccess::MakeRTV();
            _RTV_OutputTexture_StereoLeft = RACreateAndBuildShared<A_RHIResourceView>(RTVBuildParams);
#ifdef ABYTEK_DEBUG_INFO
            _RTV_OutputTexture_Mono->SetDebugName(
                *GetDebugName()
                + ABYTEK_TEXT(".OutputTextureStereoLeft.RTV")
            );
#endif
        }
        if (_OutputTextureRenderProxy_StereoRight)
        {
            F_RHITextureViewBuildParams RTVBuildParams;
            RTVBuildParams.Context = RHIContext.Weak();
            RTVBuildParams.Resource = _OutputTextureRenderProxy_StereoRight->GetRHITexture();
            RTVBuildParams.Access = F_RHIResourceAccess::MakeRTV();
            _RTV_OutputTexture_StereoRight = RACreateAndBuildShared<A_RHIResourceView>(RTVBuildParams);
#ifdef ABYTEK_DEBUG_INFO
            _RTV_OutputTexture_Mono->SetDebugName(
                *GetDebugName()
                + ABYTEK_TEXT(".OutputTextureStereoRight.RTV")
            );
#endif
        }
        
        if (_RTTextureRenderProxy_Mono)
        {
            F_RHITextureViewBuildParams RTVBuildParams;
            RTVBuildParams.Context = RHIContext.Weak();
            RTVBuildParams.Resource = _RTTextureRenderProxy_Mono->GetRHITexture();
            RTVBuildParams.Access = F_RHIResourceAccess::MakeRTV();
            _RTV_RTTexture_Mono = RACreateAndBuildShared<A_RHIResourceView>(RTVBuildParams);
#ifdef ABYTEK_DEBUG_INFO
            _RTV_RTTexture_Mono->SetDebugName(
                *GetDebugName()
                + ABYTEK_TEXT(".RTTextureMono.RTV")
            );
#endif
        }
        if (_RTTextureRenderProxy_StereoLeft)
        {
            F_RHITextureViewBuildParams RTVBuildParams;
            RTVBuildParams.Context = RHIContext.Weak();
            RTVBuildParams.Resource = _RTTextureRenderProxy_StereoLeft->GetRHITexture();
            RTVBuildParams.Access = F_RHIResourceAccess::MakeRTV();
            _RTV_RTTexture_StereoLeft = RACreateAndBuildShared<A_RHIResourceView>(RTVBuildParams);
#ifdef ABYTEK_DEBUG_INFO
            _RTV_RTTexture_StereoLeft->SetDebugName(
                *GetDebugName()
                + ABYTEK_TEXT(".RTTextureStereoLeft.RTV")
            );
#endif
        }
        if (_RTTextureRenderProxy_StereoRight)
        {
            F_RHITextureViewBuildParams RTVBuildParams;
            RTVBuildParams.Context = RHIContext.Weak();
            RTVBuildParams.Resource = _RTTextureRenderProxy_StereoRight->GetRHITexture();
            RTVBuildParams.Access = F_RHIResourceAccess::MakeRTV();
            _RTV_RTTexture_StereoRight = RACreateAndBuildShared<A_RHIResourceView>(RTVBuildParams);
#ifdef ABYTEK_DEBUG_INFO
            _RTV_RTTexture_StereoRight->SetDebugName(
                *GetDebugName()
                + ABYTEK_TEXT(".RTTextureStereoRight.RTV")
            );
#endif
        }
    }
    void F_CanvasRenderProxy::OnDestroyRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _CameraRenderProxy = {};
        
        _RHIViewport_StereoRight = {};
        _RHIViewport_StereoLeft = {};
        _RHIViewport_Mono = {};
        
        _AspectRatio_StereoRight = 0.0f;
        _AspectRatio_StereoLeft = 0.0f;
        _AspectRatio_Mono = 0.0f;
        
        _RTV_RTTexture_StereoRight = {};
        _RTV_RTTexture_StereoLeft = {};
        _RTV_RTTexture_Mono = {};
        
        _RTTextureRenderProxy_StereoRight = {};
        _RTTextureRenderProxy_StereoLeft = {};
        _RTTextureRenderProxy_Mono = {};
        
        _Topology = E_CanvasTopology::NONE;
        _PresentationMode = E_CanvasPresentationMode::NONE;
        _OutputMode = E_CanvasOutputMode::NONE;
        
        _Format = E_RHIFormat::NONE;
        
        _RTV_OutputTexture_StereoRight = {};
        _RTV_OutputTexture_StereoLeft = {};
        _RTV_OutputTexture_Mono = {};
        
        _OutputTextureRenderProxy_StereoRight = {};
        _OutputTextureRenderProxy_StereoLeft = {};
        _OutputTextureRenderProxy_Mono = {};
        
        _Window = {};
    }

    void F_CanvasRenderProxy::ApplyOfflineTexture_RenderTask(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const TS<A_RHIResourceView>& DstRTV,
        const TS<A_RHIResource>& SrcTexture,
        const F_Vector2_U32& Size,
        const F_Vector2_I32& Offset
    )
    {
        ABYTEK_RHI_CAPTURE_EVENT_SCOPE(
            SubmissionItemContainer,  
            ABYTEK_TEXT("Abytek::F_CanvasRenderProxy::ApplyOfflineTexture(Canvas = ")
                + *GetDebugName() 
                + ABYTEK_TEXT(", DstRTV = ")
                + *DstRTV->GetDebugName()
                + ABYTEK_TEXT(")")
        );
        
        auto RHIContext = H_RHI::GetMainContext();
        
        ABYTEK_ENGINE_RHI_ASSERT(_Format == E_RHIFormat::R8G8B8A8_UNORM);
        {
            F_RHISamplerBuildParams SamplerBuildParams;
            SamplerBuildParams.Context = RHIContext.Weak();
            auto Sampler = RACreateAndBuildShared<A_RHISampler>(SamplerBuildParams);
                
            auto BindGroup = CanvasRendering::F_ApplyOfflineTextureBinding::Instantiate(
                GetRenderRegistryRuntime()
            ).CreateBindGroup();
            BindGroup->BindResourceView(
                ABYTEK_NAME("SrcTexture"),
                SrcTexture
            );
            BindGroup->BindSampler(
                ABYTEK_NAME("LinearSampler"),
                Sampler
            );
            BindGroup->BindRTV(
                ABYTEK_NAME("FinalColor"),
                DstRTV
            );
            BindGroup->Commit();
            
            F_RHIViewportScissorConfig ViewportScissorConfig = F_RHIViewportScissorConfig::Make(
                F_Vector2_F32(Size),
                F_Vector2_F32(Offset)
            );
            H_RHISubmissionUtilities::DrawNonIndexed(
                SubmissionItemContainer,
                CanvasRendering::F_ApplyOfflineTexturePipeline::Instantiate(
                    GetRenderRegistryRuntime()
                ).AcquirePipelineState(),
                { BindGroup },
                ViewportScissorConfig,
                F_RHIDrawNonIndexedConfig::Make(6)
            );
        }
    }
    void F_CanvasRenderProxy::CreateRHIViewport_Window_RenderTask(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const F_Vector2_U32& NewScaledClientSize
    )
    {
        _AspectRatio_Mono = (
            static_cast<F32>(NewScaledClientSize.X)
            / static_cast<F32>(NewScaledClientSize.Y)
        );
        
        F_RHIViewportBuildParams ViewportBuildParams;
        ViewportBuildParams.Context = H_RHI::GetMainContext().Weak();
        ViewportBuildParams.Window = _Window;
        ViewportBuildParams.Size = NewScaledClientSize;
        ViewportBuildParams.Format = _Format;
        _RHIViewport_Mono = RACreateAndBuildShared<A_RHIViewport>(ViewportBuildParams);
    }
    void F_CanvasRenderProxy::ResizeRHIViewport_Window_RenderTask(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const F_Vector2_U32& NewScaledClientSize
    )
    {
        _AspectRatio_Mono = (
            static_cast<F32>(NewScaledClientSize.X)
            / static_cast<F32>(NewScaledClientSize.Y)
        );
        
        _RHIViewport_Mono->Resize(NewScaledClientSize);
    }
    void F_CanvasRenderProxy::FinalizeOutput_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        ABYTEK_RHI_CAPTURE_EVENT_SCOPE(
            SubmissionItemContainer,  
            ABYTEK_TEXT("Abytek::F_CanvasRenderProxy::FinalizeOutput(") + *GetDebugName() + ABYTEK_TEXT(")")
        );
        
        // Output display
        if (_RHIViewport_Mono)
        {
            if (_RTTextureRenderProxy_Mono)
            {
                ApplyOfflineTexture_RenderTask(
                    SubmissionItemContainer,
                    _RHIViewport_Mono->GetCurrentBackRenderTargetView(),
                    _RTTextureRenderProxy_Mono->GetRHITexture(),
                    _RHIViewport_Mono->GetSize()
                );
            }
            H_RHISubmissionUtilities::PresentViewport(
                SubmissionItemContainer,
                _RHIViewport_Mono
            );
        }
        if (_RHIViewport_StereoLeft)
        {
            if (_RTTextureRenderProxy_StereoLeft)
            {
                ApplyOfflineTexture_RenderTask(
                    SubmissionItemContainer,
                    _RHIViewport_StereoLeft->GetCurrentBackRenderTargetView(),
                    _RTTextureRenderProxy_StereoLeft->GetRHITexture(),
                    _RHIViewport_StereoLeft->GetSize()
                );
            }
            H_RHISubmissionUtilities::PresentViewport(
                SubmissionItemContainer,
                _RHIViewport_StereoLeft
            );
        }
        if (_RHIViewport_StereoRight)
        {
            if (_RTTextureRenderProxy_StereoRight)
            {
                ApplyOfflineTexture_RenderTask(
                    SubmissionItemContainer,
                    _RHIViewport_StereoRight->GetCurrentBackRenderTargetView(),
                    _RTTextureRenderProxy_StereoRight->GetRHITexture(),
                    _RHIViewport_StereoRight->GetSize()
                );
            }
            H_RHISubmissionUtilities::PresentViewport(
                SubmissionItemContainer,
                _RHIViewport_StereoRight
            );
        }
        
        // Output texture
        if (_OutputTextureRenderProxy_Mono)
        {
            if (_RTTextureRenderProxy_Mono)
            {
                ApplyOfflineTexture_RenderTask(
                    SubmissionItemContainer,
                    _RTV_OutputTexture_Mono,
                    _RTTextureRenderProxy_Mono->GetRHITexture(),
                    _OutputTextureRenderProxy_Mono->GetSize().StaticPermute<0, 1>()
                );
            }
        }
        if (_OutputTextureRenderProxy_StereoLeft)
        {
            if (_RTTextureRenderProxy_StereoLeft)
            {
                ApplyOfflineTexture_RenderTask(
                    SubmissionItemContainer,
                    _RTV_OutputTexture_StereoLeft,
                    _RTTextureRenderProxy_StereoLeft->GetRHITexture(),
                    _OutputTextureRenderProxy_StereoLeft->GetSize().StaticPermute<0, 1>()
                );
            }
        }
        if (_OutputTextureRenderProxy_StereoRight)
        {
            if (_RTTextureRenderProxy_StereoRight)
            {
                ApplyOfflineTexture_RenderTask(
                    SubmissionItemContainer,
                    _RTV_OutputTexture_StereoRight,
                    _RTTextureRenderProxy_StereoRight->GetRHITexture(),
                    _OutputTextureRenderProxy_StereoRight->GetSize().StaticPermute<0, 1>()
                );
            }
        }
    }
    void F_CanvasRenderProxy::BeginDraw_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
#ifdef ABYTEK_DEBUG_INFO
        SubmissionItemContainer->PushAdditionalStackCaptureEventStates(
            ABYTEK_TEXT("Abytek::F_CanvasRenderProxy::Draw(") + *GetDebugName() + ABYTEK_TEXT(")")
        );
#endif
        {
            ABYTEK_RHI_CAPTURE_EVENT_SCOPE(
                SubmissionItemContainer,  
                ABYTEK_TEXT("Abytek::F_CanvasRenderProxy::PrepareNewFrame(") + *GetDebugName() + ABYTEK_TEXT(")")
            );
            if (_RTV_RTTexture_Mono)
            {
                H_RHISubmissionUtilities::ClearRTV(
                    SubmissionItemContainer,
                    _RTV_RTTexture_Mono, 
                    _ClearColor
                );
            }
            if (_RTV_RTTexture_StereoLeft)
            {
                H_RHISubmissionUtilities::ClearRTV(
                    SubmissionItemContainer,
                    _RTV_RTTexture_StereoLeft, 
                    _ClearColor
                );
            }
            if (_RTV_RTTexture_StereoRight)
            {
                H_RHISubmissionUtilities::ClearRTV(
                    SubmissionItemContainer,
                    _RTV_RTTexture_StereoRight, 
                    _ClearColor
                );
            }
        }
    }
    void F_CanvasRenderProxy::EndDraw_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        FinalizeOutput_RenderTask(SubmissionItemContainer);
#ifdef ABYTEK_DEBUG_INFO
        SubmissionItemContainer->PopAdditionalStackCaptureEventStates();
#endif
    }
}
