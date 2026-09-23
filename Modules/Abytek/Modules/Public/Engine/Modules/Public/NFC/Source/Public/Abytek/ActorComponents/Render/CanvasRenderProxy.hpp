#pragma once

#include "Abytek/ActorComponents/Render/RenderableComponentProxy.hpp"
#include "Abytek/ActorComponents/CanvasComponent.hpp"
#include "Abytek/GlobalRenderBinding.hpp"
#include "Abytek/GlobalRenderPipeline.hpp"


namespace Abytek
{
    class F_TextureRenderProxy;
    class F_CameraRenderProxy;
    
    namespace CanvasRendering 
    {
        struct ABYTEK_ENGINE_NFC_API F_ApplyOfflineTextureBinding : F_GlobalRenderBinding
        {
            ABYTEK_GLOBAL_RENDER_BINDING(F_ApplyOfflineTextureBinding, ABYTEK_NAME("Abytek::CanvasRendering::F_ApplyOfflineTextureBinding"));

            static F_FeedbackStatus Build(F_Config& Config)
            {
                Config.Slots.push_back(
                    F_RHIBindGroupTemplateSlot::MakeResourceView(
                        ABYTEK_NAME("SrcTexture"),
                        F_RHIResourceAccess::MakeSRV(E_RHIShaderFrequency::PIXEL)
                    )
                );
                Config.Slots.push_back(
                    F_RHIBindGroupTemplateSlot::MakeSampler(
                        ABYTEK_NAME("LinearSampler")
                    )
                );
                Config.Slots.push_back(
                    F_RHIBindGroupTemplateSlot::MakeRTV(
                        ABYTEK_NAME("FinalColor"),
                        E_RHIFormat::R8G8B8A8_UNORM
                    )
                );
                return F_FeedbackStatus::MakeSucceeded();
            }
        };
        struct ABYTEK_ENGINE_NFC_API F_ApplyOfflineTexturePipeline : F_GlobalRenderPipeline
        {
            ABYTEK_GLOBAL_RENDER_PIPELINE(F_ApplyOfflineTexturePipeline, ABYTEK_NAME("Abytek::CanvasRendering::F_ApplyOfflineTexturePipeline"));

            static F_FeedbackStatus Build(F_Config& Config)
            {
                Config.Type = E_RHIPipelineStateType::GRAPHICS;
                Config.Rasterizer.FillMode = E_RHIFillMode::SOLID;
                Config.Rasterizer.CullMode = E_RHICullMode::NONE;
                Config.VertexShader = ABYTEK_GLOBAL_SHADER("MainVS", "Abytek/NFC/Canvas/ApplyOffscreenTextureVS", E_RHIShaderFrequency::VERTEX);
                Config.PixelShader = ABYTEK_GLOBAL_SHADER("MainPS", "Abytek/NFC/Canvas/ApplyOffscreenTexturePS", E_RHIShaderFrequency::PIXEL);
                Config.BindGroups.push_back(
                    F_RHIPipelineStateTemplateBindGroup::Make(
                        F_ApplyOfflineTextureBinding::GetTemplateHashCode()    
                    )
                );
                return F_FeedbackStatus::MakeSucceeded();
            }
        };
    }

    class ABYTEK_ENGINE_NFC_API F_CanvasRenderProxy : public A_RenderableComponentProxy
    {
    public:
        friend class F_CanvasComponent;
        
    private:
        F_Vector4_F32 _ClearColor = F_Vector4_F32::One();
        
        F_Vector2_U32 _Resolution_Mono = F_Vector2_U32::Zero();
        F_Vector2_U32 _Resolution_StereoLeft = F_Vector2_U32::Zero();
        F_Vector2_U32 _Resolution_StereoRight = F_Vector2_U32::Zero();
        
        E_RHIFormat _Format = E_RHIFormat::NONE;
        
        TW<A_Window> _Window;
        TS<F_TextureRenderProxy> _OutputTextureRenderProxy_Mono;
        TS<F_TextureRenderProxy> _OutputTextureRenderProxy_StereoLeft;
        TS<F_TextureRenderProxy> _OutputTextureRenderProxy_StereoRight;
        
        TS<A_RHIResourceView> _RTV_OutputTexture_Mono;
        TS<A_RHIResourceView> _RTV_OutputTexture_StereoLeft;
        TS<A_RHIResourceView> _RTV_OutputTexture_StereoRight;
        
        E_CanvasOutputMode _OutputMode = E_CanvasOutputMode::NONE;
        E_CanvasPresentationMode _PresentationMode = E_CanvasPresentationMode::NONE;
        E_CanvasTopology _Topology = E_CanvasTopology::NONE;
        
        TS<F_TextureRenderProxy> _RTTextureRenderProxy_Mono;
        TS<F_TextureRenderProxy> _RTTextureRenderProxy_StereoLeft;
        TS<F_TextureRenderProxy> _RTTextureRenderProxy_StereoRight;
        
        TS<A_RHIResourceView> _RTV_RTTexture_Mono;
        TS<A_RHIResourceView> _RTV_RTTexture_StereoLeft;
        TS<A_RHIResourceView> _RTV_RTTexture_StereoRight;
        
        TS<A_RHIViewport> _RHIViewport_Mono;
        TS<A_RHIViewport> _RHIViewport_StereoLeft;
        TS<A_RHIViewport> _RHIViewport_StereoRight;
        
        F32 _AspectRatio_Mono = 0.0f;
        F32 _AspectRatio_StereoLeft = 0.0f;
        F32 _AspectRatio_StereoRight = 0.0f;
        
        TS<F_CameraRenderProxy> _CameraRenderProxy;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetResolution_Mono() const noexcept
        {
            return _Resolution_Mono;
        }
        ABYTEK_FORCE_INLINE const auto& GetResolution_StereoLeft() const noexcept
        {
            return _Resolution_StereoLeft;
        }
        ABYTEK_FORCE_INLINE const auto& GetResolution_StereoRight() const noexcept
        {
            return _Resolution_StereoRight;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetClearColor() const noexcept
        {
            return _ClearColor;
        }
        
        ABYTEK_FORCE_INLINE auto GetFormat() const noexcept
        {
            return _Format;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetWindow() const noexcept
        {
            return _Window;
        }
        ABYTEK_FORCE_INLINE const auto& GetOutputTextureRenderProxy() const noexcept
        {
            return _OutputTextureRenderProxy_Mono;
        }
        ABYTEK_FORCE_INLINE const auto& GetOutputTextureRenderProxy_StereoLeft() const noexcept
        {
            return _OutputTextureRenderProxy_StereoLeft;
        }
        ABYTEK_FORCE_INLINE const auto& GetOutputTextureRenderProxy_StereoRight() const noexcept
        {
            return _OutputTextureRenderProxy_StereoRight;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetRTV_OutputTexture_Mono() const noexcept
        {
            return _RTV_OutputTexture_Mono;
        }
        ABYTEK_FORCE_INLINE const auto& GetRTV_OutputTexture_StereoLeft() const noexcept
        {
            return _RTV_OutputTexture_StereoLeft;
        }
        ABYTEK_FORCE_INLINE const auto& GetRTV_OutputTexture_StereoRight() const noexcept
        {
            return _RTV_OutputTexture_StereoRight;
        }
        
        ABYTEK_FORCE_INLINE auto GetOutputMode() const noexcept
        {
            return _OutputMode;
        }
        ABYTEK_FORCE_INLINE auto GetPresentationMode() const noexcept
        {
            return _PresentationMode;
        }
        ABYTEK_FORCE_INLINE auto GetTopology() const noexcept
        {
            return _Topology;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetRTTextureRenderProxy() const noexcept
        {
            return _RTTextureRenderProxy_Mono;
        }
        ABYTEK_FORCE_INLINE const auto& GetRTTextureRenderProxy_StereoLeft() const noexcept
        {
            return _RTTextureRenderProxy_StereoLeft;
        }
        ABYTEK_FORCE_INLINE const auto& GetRTTextureRenderProxy_StereoRight() const noexcept
        {
            return _RTTextureRenderProxy_StereoRight;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetRTV_RTTexture_Mono() const noexcept
        {
            return _RTV_RTTexture_Mono;
        }
        ABYTEK_FORCE_INLINE const auto& GetRTV_RTTexture_StereoLeft() const noexcept
        {
            return _RTV_RTTexture_StereoLeft;
        }
        ABYTEK_FORCE_INLINE const auto& GetRTV_RTTexture_StereoRight() const noexcept
        {
            return _RTV_RTTexture_StereoRight;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetRHIViewport_Mono() const noexcept
        {
            return _RHIViewport_Mono;
        }
        ABYTEK_FORCE_INLINE const auto& GetRHIViewport_StereoLeft() const noexcept
        {
            return _RHIViewport_StereoLeft;
        }
        ABYTEK_FORCE_INLINE const auto& GetRHIViewport_StereoRight() const noexcept
        {
            return _RHIViewport_StereoRight;
        }
        
        ABYTEK_FORCE_INLINE auto GetAspectRatio_Mono() const noexcept
        {
            return _AspectRatio_Mono;
        }
        ABYTEK_FORCE_INLINE auto GetAspectRatio_StereoLeft() const noexcept
        {
            return _AspectRatio_StereoLeft;
        }
        ABYTEK_FORCE_INLINE auto GetAspectRatio_StereoRight() const noexcept
        {
            return _AspectRatio_StereoRight;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetCameraRenderProxy() const noexcept
        {
            return _CameraRenderProxy;
        }
        
    public:
        F_CanvasRenderProxy(const TW_Valid<F_CanvasComponent>& Owner);
        ~F_CanvasRenderProxy() override;
        
    protected:
        void OnInit_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        void OnRelease_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        
    protected:
        void OnCreateRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        void OnDestroyRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        
    public:
        void ApplyOfflineTexture_RenderTask(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
            const TS<A_RHIResourceView>& DstRTV,
            const TS<A_RHIResource>& SrcTexture,
            const F_Vector2_U32& Size,
            const F_Vector2_I32& Offset = F_Vector2_I32::Zero()
        );
        void CreateRHIViewport_Window_RenderTask(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
            const F_Vector2_U32& NewScaledClientSize
        );
        void ResizeRHIViewport_Window_RenderTask(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
            const F_Vector2_U32& NewScaledClientSize
        );
        void FinalizeOutput_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        void BeginDraw_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        void EndDraw_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
    };
}
