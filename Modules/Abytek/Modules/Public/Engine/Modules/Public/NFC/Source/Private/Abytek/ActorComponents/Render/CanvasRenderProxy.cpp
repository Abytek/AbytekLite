#include "Abytek/ActorComponents/Render/CanvasRenderProxy.hpp"
#include "Abytek/ActorComponents/Render/CameraRenderProxy.hpp"
#include "Abytek/Assets/Render/TextureRenderProxy.hpp"
#include "Abytek/Frame/FrameHelper.hpp"


namespace Abytek
{
    namespace CanvasRendering
    {
        ABYTEK_DEFINE_GLOBAL_RENDER_BINDING(F_ApplyOfflineTextureBinding)
        {
            ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::CanvasRendering::F_ApplyOfflineTextureBinding"));
        }
        ABYTEK_DEFINE_GLOBAL_RENDER_PIPELINE(F_ApplyOfflineTexturePipeline)
        {
            ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::CanvasRendering::F_ApplyOfflineTexturePipeline"));
        }
    }
    
    F_CanvasRenderProxy::F_CanvasRenderProxy(const TW_Valid<F_CanvasComponent>& Owner) :
        A_RenderableComponentProxy(Owner)
    {
    }
    F_CanvasRenderProxy::~F_CanvasRenderProxy()
    {
    }

    void F_CanvasRenderProxy::OnInit_RenderTask()
    {
    }
    void F_CanvasRenderProxy::OnRelease_RenderTask()
    {
    }

    void F_CanvasRenderProxy::OnCreateRenderState_RenderTask()
    {
        auto RHIContext = H_RHI::GetMainContext();
        auto RHIProcess = H_RHI::GetMainProcess();
        
        if (_OutputTextureRenderProxy_Mono)
        {
            F_RHITextureViewBuildParams RTVBuildParams;
            RTVBuildParams.Context = RHIContext.Weak();
            RTVBuildParams.Resource = _OutputTextureRenderProxy_Mono->GetRHITexture();
            RTVBuildParams.Access = F_RHIResourceAccess::MakeRTV();
            _RTV_OutputTexture_Mono = RACreateAndBuildShared<A_RHIResourceView>(RTVBuildParams);
        }
        if (_OutputTextureRenderProxy_StereoLeft)
        {
            F_RHITextureViewBuildParams RTVBuildParams;
            RTVBuildParams.Context = RHIContext.Weak();
            RTVBuildParams.Resource = _OutputTextureRenderProxy_StereoLeft->GetRHITexture();
            RTVBuildParams.Access = F_RHIResourceAccess::MakeRTV();
            _RTV_OutputTexture_StereoLeft = RACreateAndBuildShared<A_RHIResourceView>(RTVBuildParams);
        }
        if (_OutputTextureRenderProxy_StereoRight)
        {
            F_RHITextureViewBuildParams RTVBuildParams;
            RTVBuildParams.Context = RHIContext.Weak();
            RTVBuildParams.Resource = _OutputTextureRenderProxy_StereoRight->GetRHITexture();
            RTVBuildParams.Access = F_RHIResourceAccess::MakeRTV();
            _RTV_OutputTexture_StereoRight = RACreateAndBuildShared<A_RHIResourceView>(RTVBuildParams);
        }
        
        if (_RTTextureRenderProxy_Mono)
        {
            F_RHITextureViewBuildParams RTVBuildParams;
            RTVBuildParams.Context = RHIContext.Weak();
            RTVBuildParams.Resource = _RTTextureRenderProxy_Mono->GetRHITexture();
            RTVBuildParams.Access = F_RHIResourceAccess::MakeRTV();
            _RTV_RTTexture_Mono = RACreateAndBuildShared<A_RHIResourceView>(RTVBuildParams);
        }
        if (_RTTextureRenderProxy_StereoLeft)
        {
            F_RHITextureViewBuildParams RTVBuildParams;
            RTVBuildParams.Context = RHIContext.Weak();
            RTVBuildParams.Resource = _RTTextureRenderProxy_StereoLeft->GetRHITexture();
            RTVBuildParams.Access = F_RHIResourceAccess::MakeRTV();
            _RTV_RTTexture_StereoLeft = RACreateAndBuildShared<A_RHIResourceView>(RTVBuildParams);
        }
        if (_RTTextureRenderProxy_StereoRight)
        {
            F_RHITextureViewBuildParams RTVBuildParams;
            RTVBuildParams.Context = RHIContext.Weak();
            RTVBuildParams.Resource = _RTTextureRenderProxy_StereoRight->GetRHITexture();
            RTVBuildParams.Access = F_RHIResourceAccess::MakeRTV();
            _RTV_RTTexture_StereoRight = RACreateAndBuildShared<A_RHIResourceView>(RTVBuildParams);
        }
    }
    void F_CanvasRenderProxy::OnDestroyRenderState_RenderTask()
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
        const TS<A_RHIResourceView>& DstRTV,
        const TS<A_RHIResource>& SrcTexture,
        const F_Vector2_U32& Size,
        const F_Vector2_I32& Offset
    )
    {
        auto RHIContext = H_RHI::GetMainContext();
        auto RHIProcess = H_RHI::GetMainProcess();
        
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
            
            auto SubmissionList = RACreateAndBuildShared<A_RHISubmissionList>(H_RHI::GetMainProcess().Weak());
#ifdef ABYTEK_DEBUG_INFO
            SubmissionList->SetDebugName(ABYTEK_NAME("SetupCanvasRenderProxy"));
#endif
            F_RHIViewportScissorConfig ViewportScissorConfig = F_RHIViewportScissorConfig::Make(
                F_Vector2_F32(Size),
                F_Vector2_F32(Offset)
            );
            H_RHIPassUtilities::DrawNonIndexed(
                *SubmissionList,
                CanvasRendering::F_ApplyOfflineTexturePipeline::Instantiate(
                    GetRenderRegistryRuntime()
                ).AcquirePipelineState(),
                { BindGroup },
                ViewportScissorConfig,
                F_RHIDrawNonIndexedConfig::Make(6)
            );
            
            RHIProcess->AddSubmissionItem(SubmissionList);
        }
    }
    void F_CanvasRenderProxy::ClearRTV_RenderTask(const TS<A_RHIResourceView>& RTV, const F_Vector4_F32& ClearColor)
    {
        auto RHIContext = H_RHI::GetMainContext();
        auto RHIProcess = H_RHI::GetMainProcess();
        
        F_RHIClearRTVPassBuildParams BuildParams;
        BuildParams.Context = RHIContext.Weak();
        BuildParams.RTV = RTV;
        BuildParams.Color = ClearColor;
        auto Pass = RACreateAndBuildShared<A_RHIClearRTVPass>(BuildParams);
    
        RHIProcess->AddSubmissionItem(Pass);
    }
    void F_CanvasRenderProxy::CreateRHIViewport_Window_RenderTask(const F_Vector2_U32& NewScaledClientSize)
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
    void F_CanvasRenderProxy::ResizeRHIViewport_Window_RenderTask(const F_Vector2_U32& NewScaledClientSize)
    {
        _AspectRatio_Mono = (
            static_cast<F32>(NewScaledClientSize.X)
            / static_cast<F32>(NewScaledClientSize.Y)
        );
        
        _RHIViewport_Mono->Resize(NewScaledClientSize);
    }
    void F_CanvasRenderProxy::FinalizeOutput_RenderTask()
    {
        auto RHIContext = H_RHI::GetMainContext();
        auto RHIProcess = H_RHI::GetMainProcess();
        
        // Output display
        if (_RHIViewport_Mono)
        {
            if (_RTTextureRenderProxy_Mono)
            {
                ApplyOfflineTexture_RenderTask(
                    _RHIViewport_Mono->GetCurrentBackRenderTargetView(),
                    _RTTextureRenderProxy_Mono->GetRHITexture(),
                    _RHIViewport_Mono->GetSize()
                );
            }
            RHIProcess->AddViewport(_RHIViewport_Mono);
        }
        if (_RHIViewport_StereoLeft)
        {
            if (_RTTextureRenderProxy_StereoLeft)
            {
                ApplyOfflineTexture_RenderTask(
                    _RHIViewport_StereoLeft->GetCurrentBackRenderTargetView(),
                    _RTTextureRenderProxy_StereoLeft->GetRHITexture(),
                    _RHIViewport_StereoLeft->GetSize()
                );
            }
            RHIProcess->AddViewport(_RHIViewport_StereoLeft);
        }
        if (_RHIViewport_StereoRight)
        {
            if (_RTTextureRenderProxy_StereoRight)
            {
                ApplyOfflineTexture_RenderTask(
                    _RHIViewport_StereoRight->GetCurrentBackRenderTargetView(),
                    _RTTextureRenderProxy_StereoRight->GetRHITexture(),
                    _RHIViewport_StereoRight->GetSize()
                );
            }
            RHIProcess->AddViewport(_RHIViewport_StereoRight);
        }
        
        // Output texture
        if (_OutputTextureRenderProxy_Mono)
        {
            if (_RTTextureRenderProxy_Mono)
            {
                ApplyOfflineTexture_RenderTask(
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
                    _RTV_OutputTexture_StereoRight,
                    _RTTextureRenderProxy_StereoRight->GetRHITexture(),
                    _OutputTextureRenderProxy_StereoRight->GetSize().StaticPermute<0, 1>()
                );
            }
        }
    }
    void F_CanvasRenderProxy::BeginDraw_RenderTask()
    {
        auto RHIContext = H_RHI::GetMainContext();
        auto RHIProcess = H_RHI::GetMainProcess();
        
        if (_RTV_RTTexture_Mono)
        {
            ClearRTV_RenderTask(_RTV_RTTexture_Mono, _ClearColor);
        }
        if (_RTV_RTTexture_StereoLeft)
        {
            ClearRTV_RenderTask(_RTV_RTTexture_StereoLeft, _ClearColor);
        }
        if (_RTV_RTTexture_StereoRight)
        {
            ClearRTV_RenderTask(_RTV_RTTexture_StereoRight, _ClearColor);
        }
    }
    void F_CanvasRenderProxy::EndDraw_RenderTask()
    {
        auto RHIContext = H_RHI::GetMainContext();
        auto RHIProcess = H_RHI::GetMainProcess();
        
        FinalizeOutput_RenderTask();
    }
}
