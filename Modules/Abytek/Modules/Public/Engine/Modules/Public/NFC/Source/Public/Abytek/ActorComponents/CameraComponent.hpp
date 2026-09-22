#pragma once

#include "Abytek/ActorComponents/RenderableComponent.hpp"
#include "Abytek/ActorComponents/CanvasComponentManager.hpp"


namespace Abytek
{
    class F_CameraComponent;
    class F_SceneComponent;
    
    enum class E_CameraProjectionMode
    {
        NONE,
        PERSPECTIVE,
        ORTHOGONAL,
        DEFAULT = PERSPECTIVE
    };
    ABYTEK_ENUM_REFLECTOR_LOCALNS(E_CameraProjectionMode)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::E_CameraProjectionMode"));
    }
    
    struct F_CameraProjectionOptions
    {
        ABYTEK_BEGIN_REFLECTOR(I_Serializable)
        ABYTEK_END_REFLECTOR(F_CameraProjectionOptions);
        
        E_CameraProjectionMode Mode = E_CameraProjectionMode::DEFAULT;
        F32 ViewSize = 5.0f;
        F32 VerticalFOV = 90.0f * ABYTEK_F32_DEGREES_TO_RADS;
        F32 NearPlane = 0.01f;
        F32 FarPlane = 1000.0f;
        
        F_Matrix4x4_F32 GetProjectionMatrix(F32 AspectRatio) const noexcept
        {
            switch (Mode)
            {
            case E_CameraProjectionMode::PERSPECTIVE:
                return CalculatePerspectiveProjectionMatrix(
                    { VerticalFOV, AspectRatio },
                    NearPlane,
                    FarPlane
                );
                break;
            case E_CameraProjectionMode::ORTHOGONAL:
                return CalculateOrthogonalProjectionMatrix(
                    { ViewSize * AspectRatio, ViewSize },
                    NearPlane,
                    FarPlane
                );
                break;
            default:
                ABYTEK_LOG_FATAL() << "Invalid projection mode: " << static_cast<U32>(Mode);
            }
            return {};
        }
        
        friend B8 operator == (const F_CameraProjectionOptions& A, const F_CameraProjectionOptions& B)
        {
            return (
                (A.Mode == B.Mode)    
                && (A.ViewSize == B.ViewSize)    
                && (A.VerticalFOV == B.VerticalFOV)    
                && (A.NearPlane == B.NearPlane)    
                && (A.FarPlane == B.FarPlane)    
            );
        }
        friend B8 operator != (const F_CameraProjectionOptions& A, const F_CameraProjectionOptions& B)
        {
            return !(A == B);
        }
    };

    class ABYTEK_ENGINE_NFC_API F_CameraComponent : public A_RenderableComponent
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_RenderableComponent)
        ABYTEK_END_REFLECTOR(F_CameraComponent);
        
    public:
        friend class F_CanvasComponent;
        
    private:
        B8 _IsEnabled = true;
        TS<F_SceneComponent> _SceneComponent;
        TW<F_CanvasComponent> _CanvasComponent;
        F_CameraProjectionOptions _ProjectionOptions;
        F32 _StereoBaseline = 0.1f;
        
    private:
        CanvasComponentManagerEvents::F_OnRegisterComponent::F_ListenerHandle _ListenerHandle_OnRegisterCanvasComponent;
        CanvasComponentManagerEvents::F_OnUnregisterComponent::F_ListenerHandle _ListenerHandle_OnUnregisterCanvasComponent;
        
    public:
        TW<F_CanvasComponent> ManualCanvasComponent;
        TF_Vector<F_Name> RequiredTagsForAutoCanvasComponent;
        
    public:
        ABYTEK_FORCE_INLINE B8 IsEnabled() const noexcept
        {
            return _IsEnabled;
        }
        ABYTEK_FORCE_INLINE const auto& GetSceneComponent() const noexcept
        {
            return _SceneComponent;
        }
        ABYTEK_FORCE_INLINE const auto& GetCanvasComponent() const noexcept
        {
            return _CanvasComponent;
        }
        ABYTEK_FORCE_INLINE const auto& GetProjectionOptions() const noexcept
        {
            return _ProjectionOptions;
        }
        ABYTEK_FORCE_INLINE auto GetProjectionMode() const noexcept
        {
            return _ProjectionOptions.Mode;
        }
        ABYTEK_FORCE_INLINE auto GetProjectionViewSize() const noexcept
        {
            return _ProjectionOptions.ViewSize;
        }
        ABYTEK_FORCE_INLINE auto GetProjectionVerticalFOV() const noexcept
        {
            return _ProjectionOptions.VerticalFOV;
        }
        ABYTEK_FORCE_INLINE auto GetProjectionNearPlane() const noexcept
        {
            return _ProjectionOptions.NearPlane;
        }
        ABYTEK_FORCE_INLINE auto GetProjectionFarPlane() const noexcept
        {
            return _ProjectionOptions.FarPlane;
        }
        ABYTEK_FORCE_INLINE auto GetStereoBaseline() const noexcept
        {
            return _StereoBaseline;
        }
        
    public:
        F_CameraComponent(const F_SerializableObjectInitParams& InitParams);
        ~F_CameraComponent() override;
        
    protected:
        void OnRegisterComponent() override;
        void OnUnregisterComponent() override;
        
    public:
        B8 IsRenderable() const override;
        
    protected:
        void OnCreateRenderState() override;
        void OnDestroyRenderState() override;
        TS<A_RenderProxy> CreateRenderProxy() override;
        
    protected:
        virtual void OnEnable();
        virtual void OnDisable();
        
    public:
        void Enable();
        void Disable();
        
    private:
        void _ActualEnable();
        void _ActualDisable();
        
    protected:
        virtual void OnBindCanvasComponent();
        virtual void OnUnbindCanvasComponent();
        
    private:
        void _BindCanvasComponent(const TW<F_CanvasComponent>& CanvasComponent);
        void _UnbindCanvasComponent();
        
    private:
        B8 _TryAutoChangeCanvasComponent(const TW<F_CanvasComponent>& CanvasComponent);
        
    public:
        void ChangeCanvasComponent(const TW<F_CanvasComponent>& CanvasComponent);
        void ChangeCanvasComponentToDefault();
        
    public:
        void SetProjectionOptions(const F_CameraProjectionOptions& Value);
        void SetProjectionMode(E_CameraProjectionMode Value);
        void SetProjectionViewSize(F32 Value);
        void SetProjectionVerticalFOV(F32 Value);
        void SetProjectionNearPlane(F32 Value);
        void SetProjectionFarPlane(F32 Value);
    
    protected:
        virtual void OnPrepareDraw();
        virtual void OnDraw();
        
    private:
        void _Draw();
        
    public:
        F_Matrix4x4 GetViewMatrix_Mono() const;
        F_Matrix4x4 GetViewMatrix_StereoLeft() const;
        F_Matrix4x4 GetViewMatrix_StereoRight() const;
        
    private:
        void _SendCommonCameraDataToRenderProxy();
    };
}