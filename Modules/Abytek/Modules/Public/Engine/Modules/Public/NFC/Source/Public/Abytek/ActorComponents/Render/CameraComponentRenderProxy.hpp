#pragma once

#include "Abytek/ActorComponents/Render/RenderableComponentProxy.hpp"
#include "Abytek/ActorComponents/CameraComponent.hpp"
#include "Abytek/GlobalRenderBinding.hpp"
#include "Abytek/GlobalRenderPipeline.hpp"


namespace Abytek
{
    class A_RenderScene;
    class F_CanvasComponentRenderProxy;
    class A_RenderViewFamily;
    class A_Renderer;

    class ABYTEK_ENGINE_NFC_API F_CameraComponentRenderProxy : public A_RenderableComponentProxy
    {
    public:
        friend class F_CameraComponent;
        
    private:
        TW<F_CanvasComponentRenderProxy> _CanvasComponentRenderProxy;
        TS<A_RenderViewFamily> _ViewFamily;
        TS<A_Renderer> _Renderer;
        F_Matrix4x4_F32 _ViewMatrix_Mono = Identity<F_Matrix4x4_F32>();
        F_Matrix4x4_F32 _ViewMatrix_StereoLeft = Identity<F_Matrix4x4_F32>();
        F_Matrix4x4_F32 _ViewMatrix_StereoRight = Identity<F_Matrix4x4_F32>();
        F_CameraProjectionOptions _ProjectionOptions;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetCanvasComponentRenderProxy() const noexcept
        {
            return _CanvasComponentRenderProxy;
        }
        ABYTEK_FORCE_INLINE const auto& GetViewFamily() const noexcept
        {
            return _ViewFamily;
        }
        ABYTEK_FORCE_INLINE const auto& GetRenderer() const noexcept
        {
            return _Renderer;
        }
        ABYTEK_FORCE_INLINE const auto& GetViewMatrix_Mono() const noexcept
        {
            return _ViewMatrix_Mono;
        }
        ABYTEK_FORCE_INLINE const auto& GetViewMatrix_StereoLeft() const noexcept
        {
            return _ViewMatrix_StereoLeft;
        }
        ABYTEK_FORCE_INLINE const auto& GetViewMatrix_StereoRight() const noexcept
        {
            return _ViewMatrix_StereoRight;
        }
        ABYTEK_FORCE_INLINE auto GetProjectionOptions() const noexcept
        {
            return _ProjectionOptions;
        }
        
    public:
        F_CameraComponentRenderProxy(const TW_Valid<F_CameraComponent>& Owner);
        ~F_CameraComponentRenderProxy() override;
        
    protected:
        void OnCreateRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        void OnDestroyRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        
    protected:
        virtual void OnDraw_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        
    public:
        void Draw_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
    };
}
