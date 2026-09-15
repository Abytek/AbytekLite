#pragma once

#include "Abytek/ActorComponents/Render/RenderableComponentProxy.hpp"
#include "Abytek/ActorComponents/CameraComponent.hpp"
#include "Abytek/GlobalRenderBinding.hpp"
#include "Abytek/GlobalRenderPipeline.hpp"


namespace Abytek
{
    class A_RenderScene;
    class F_CanvasRenderProxy;
    class A_RenderViewFamily;
    class A_Renderer;

    class ABYTEK_ENGINE_NFC_API F_CameraRenderProxy : public A_RenderableComponentProxy
    {
    public:
        friend class F_CameraComponent;
        
    private:
        TW<F_CanvasRenderProxy> _CanvasRenderProxy;
        TS<A_RenderViewFamily> _ViewFamily;
        TS<A_Renderer> _Renderer;
        F_Matrix4x4_F32 _ViewMatrix_Mono = Identity<F_Matrix4x4_F32>();
        F_Matrix4x4_F32 _ViewMatrix_StereoLeft = Identity<F_Matrix4x4_F32>();
        F_Matrix4x4_F32 _ViewMatrix_StereoRight = Identity<F_Matrix4x4_F32>();
        F_CameraProjectionOptions _ProjectionOptions;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetCanvasRenderProxy() const noexcept
        {
            return _CanvasRenderProxy;
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
        F_CameraRenderProxy(const TW_Valid<F_CameraComponent>& Owner);
        ~F_CameraRenderProxy() override;
        
    protected:
        void OnInit_RenderTask() override;
        void OnRelease_RenderTask() override;
        
    protected:
        void OnCreateRenderState_RenderTask() override;
        void OnDestroyRenderState_RenderTask() override;
        
    protected:
        virtual void OnDraw_RenderTask();
        
    public:
        void Draw_RenderTask();
    };
}
