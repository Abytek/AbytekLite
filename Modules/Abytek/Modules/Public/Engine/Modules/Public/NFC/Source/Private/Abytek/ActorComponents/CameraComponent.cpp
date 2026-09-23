#include "Abytek/ActorComponents/CameraComponent.hpp"
#include "Abytek/ActorComponents/CameraComponentManager.hpp"
#include "Abytek/ActorComponents/CanvasComponent.hpp"
#include "Abytek/ActorComponents/SceneComponent.hpp"
#include "Abytek/ActorComponents/Render/CameraComponentRenderProxy.hpp"
#include "Abytek/ActorComponents/Render/CanvasComponentRenderProxy.hpp"
#include "Abytek/Renderer/RenderScene.hpp"
#include "Abytek/Frame/FrameHelper.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_CameraComponent)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_CameraComponent"));
        
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(_SceneComponent);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(ManualCanvasComponent);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(RequiredTagsForAutoCanvasComponent);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(_ProjectionOptions);
    }
    
    ABYTEK_REFLECT(F_CameraProjectionOptions)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_CameraProjectionOptions"));
        
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(Mode);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(VerticalFOV);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(NearPlane);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(FarPlane);
    }
    
    F_CameraComponent::F_CameraComponent(const F_SerializableObjectInitParams& InitParams) :
        A_RenderableComponent(InitParams)
    {
        _SceneComponent = CreateDefaultSerializableSubobject<F_SceneComponent>(ABYTEK_NAME("Scene"));
        AddChildInstanceComponent(_SceneComponent);
    }
    F_CameraComponent::~F_CameraComponent()
    {
    }

    void F_CameraComponent::OnRegisterComponent()
    {
        A_RenderableComponent::OnRegisterComponent();
        
        F_CameraComponentManager::GetInstance()->_RegisterCameraComponent(ABYTEK_WTHIS());
        
        _ListenerHandle_OnRegisterCanvasComponent = F_CanvasComponentManager::GetInstance()->Events.OnRegisterComponent.AddListener(
            [this](const TW_Valid<F_CanvasComponent>& CanvasComponent)
            {
                if (_CanvasComponent)
                {
                    return;
                }
                if (!_IsEnabled)
                {
                    return;
                }
                _TryAutoChangeCanvasComponent(CanvasComponent);
            }
        );
        _ListenerHandle_OnUnregisterCanvasComponent = F_CanvasComponentManager::GetInstance()->Events.OnUnregisterComponent.AddListener(
            [this](const TW_Valid<F_CanvasComponent>& CanvasComponent)
            {
                if (_CanvasComponent != CanvasComponent)
                {
                    return;
                }
                ChangeCanvasComponent({});
            }
        );
        
        if (_IsEnabled)
        {
            _Enable_Impl();
        }
    }
    void F_CameraComponent::OnUnregisterComponent()
    {
        if (_IsEnabled)
        {
            _Disable_Impl();
        }
        
        F_CanvasComponentManager::GetInstance()->Events.OnUnregisterComponent.RemoveListener(
            _ListenerHandle_OnUnregisterCanvasComponent
        );
        F_CanvasComponentManager::GetInstance()->Events.OnRegisterComponent.RemoveListener(
            _ListenerHandle_OnRegisterCanvasComponent
        );
        
        F_CameraComponentManager::GetInstance()->_UnregisterCameraComponent(ABYTEK_WTHIS());
        
        A_RenderableComponent::OnUnregisterComponent();
    }

    B8 F_CameraComponent::IsRenderable() const
    {
        return _IsEnabled && _CanvasComponent;
    }

    void F_CameraComponent::OnCreateRenderState()
    {
        A_RenderableComponent::OnCreateRenderState();
        
        H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
            [
                RenderProxy = GetRenderProxy().FastCast<F_CameraComponentRenderProxy>(),
                CanvasComponentRenderProxy = _CanvasComponent->GetRenderProxy().FastCast<F_CanvasComponentRenderProxy>()
            ]
            {
                RenderProxy->_CanvasComponentRenderProxy = CanvasComponentRenderProxy.Weak();
            }
        );
        
        _SendCommonCameraDataToRenderProxy();
    }
    void F_CameraComponent::OnDestroyRenderState()
    {
        A_RenderableComponent::OnDestroyRenderState();
    }
    TS<A_RenderProxy> F_CameraComponent::CreateRenderProxy()
    {
        return TS<F_CameraComponentRenderProxy>()(ABYTEK_WTHIS());
    }

    void F_CameraComponent::OnEnable()
    {
    }
    void F_CameraComponent::OnDisable()
    {
    }

    void F_CameraComponent::Enable()
    {
        if (_IsEnabled)
        {
            return;
        }
        _IsEnabled = true;
        if (IsRegistered() && !HasSerializableFlags(E_SerializableObjectFlag::CDO))
        {
            _Enable_Impl();
        }
    }
    void F_CameraComponent::Disable()
    {
        if (!_IsEnabled)
        {
            return;
        }
        _IsEnabled = false;
        if (IsRegistered() && !HasSerializableFlags(E_SerializableObjectFlag::CDO))
        {
            _Disable_Impl();
        }
    }

    void F_CameraComponent::_Enable_Impl()
    {
        ChangeCanvasComponentToDefault();
        OnEnable();
    }
    void F_CameraComponent::_Disable_Impl()
    {
        OnDisable();
        ChangeCanvasComponent({});
    }

    void F_CameraComponent::OnBindCanvasComponent()
    {
    }
    void F_CameraComponent::OnUnbindCanvasComponent()
    {
    }

    void F_CameraComponent::_BindCanvasComponent(const TW<F_CanvasComponent>& CanvasComponent)
    {
        _CanvasComponent = CanvasComponent;
        _CanvasComponent->_BindCameraComponent(ABYTEK_WTHIS());
        OnBindCanvasComponent();
        MarkRenderStateDirty();
    }
    void F_CameraComponent::_UnbindCanvasComponent()
    {
        MarkRenderStateDirty();
        OnUnbindCanvasComponent();
        _CanvasComponent->_UnbindCameraComponent();
        _CanvasComponent = {};
    }

    B8 F_CameraComponent::_TryAutoChangeCanvasComponent(const TW<F_CanvasComponent>& CanvasComponent)
    {
        B8 Valid = true;
        for (const auto& Tag : RequiredTagsForAutoCanvasComponent)
        {
            if (!CanvasComponent->HasTag(Tag))
            {
                Valid = false;
                break;
            }
        }
        if (!Valid)
        {
            return false;
        }
        ChangeCanvasComponent(CanvasComponent);
        return true;
    }

    void F_CameraComponent::ChangeCanvasComponent(const TW<F_CanvasComponent>& CanvasComponent)
    {
        if (_CanvasComponent)
        {
            _UnbindCanvasComponent();
        }
        if (CanvasComponent)
        {
            _BindCanvasComponent(CanvasComponent);
        }
    }
    void F_CameraComponent::ChangeCanvasComponentToDefault()
    {
        if (ManualCanvasComponent)
        {
            ChangeCanvasComponent(ManualCanvasComponent);
        }
        else
        {
            for (const auto& CanvasComponent : F_CanvasComponentManager::GetInstance()->GetCanvasComponents())
            {
                if (CanvasComponent)
                {
                    if (_TryAutoChangeCanvasComponent(CanvasComponent))
                    {
                        break;
                    }
                }
            }
        }
    }

    void F_CameraComponent::SetProjectionOptions(const F_CameraProjectionOptions& Value)
    {
        if (_ProjectionOptions.Mode != Value.Mode)
        {
            MarkPackageDirty();
        }
        _ProjectionOptions = Value;
    }
    void F_CameraComponent::SetProjectionMode(E_CameraProjectionMode Value)
    {
        _ProjectionOptions.Mode = Value;
        MarkRenderStateDirty();
    }
    void F_CameraComponent::SetProjectionViewSize(F32 Value)
    {
        _ProjectionOptions.ViewSize = Value;
        if (auto RenderProxy = GetRenderProxy())
        {
            H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
                [
                    RenderProxy = GetRenderProxy().FastCast<F_CameraComponentRenderProxy>(),
                    CachedValue = Value
                ]
                {
                    RenderProxy->_ProjectionOptions.ViewSize = CachedValue;
                }
            );
        }
    }
    void F_CameraComponent::SetProjectionVerticalFOV(F32 Value)
    {
        _ProjectionOptions.VerticalFOV = Value;
        if (auto RenderProxy = GetRenderProxy())
        {
            H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
                [
                    RenderProxy = GetRenderProxy().FastCast<F_CameraComponentRenderProxy>(),
                    CachedValue = Value
                ]
                {
                    RenderProxy->_ProjectionOptions.VerticalFOV = CachedValue;
                }
            );
        }
    }
    void F_CameraComponent::SetProjectionNearPlane(F32 Value)
    {
        _ProjectionOptions.NearPlane = Value;
        if (auto RenderProxy = GetRenderProxy())
        {
            H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
                [
                    RenderProxy = GetRenderProxy().FastCast<F_CameraComponentRenderProxy>(),
                    CachedValue = Value
                ]
                {
                    RenderProxy->_ProjectionOptions.NearPlane = CachedValue;
                }
            );
        }
    }
    void F_CameraComponent::SetProjectionFarPlane(F32 Value)
    {
        _ProjectionOptions.FarPlane = Value;
        if (auto RenderProxy = GetRenderProxy())
        {
            H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
                [
                    RenderProxy = GetRenderProxy().FastCast<F_CameraComponentRenderProxy>(),
                    CachedValue = Value
                ]
                {
                    RenderProxy->_ProjectionOptions.FarPlane = CachedValue;
                }
            );
        }
    }

    void F_CameraComponent::OnPrepareDraw()
    {
        _SendCommonCameraDataToRenderProxy();
    }
    void F_CameraComponent::OnDraw()
    {
        H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
            [
                RenderProxy = GetRenderProxy().FastCast<F_CameraComponentRenderProxy>()
            ]
            {
                RenderProxy->Draw_RenderTask(H_RHI::GetMainSubmissionQueue());
            }
        );
    }

    void F_CameraComponent::_Draw()
    {
        OnPrepareDraw();
        OnDraw();
    }

    F_Matrix4x4 F_CameraComponent::GetViewMatrix_Mono() const
    {
        return Inverse(
            MakeTranslationMatrix(_SceneComponent->GetWorldPosition())
            * MakeRotationMatrix4x4(_SceneComponent->GetWorldRotation().EulerAngles)
        );
    }
    F_Matrix4x4 F_CameraComponent::GetViewMatrix_StereoLeft() const
    {
        return Inverse(
            MakeTranslationMatrix(_SceneComponent->GetWorldPosition())
            * MakeRotationMatrix4x4(_SceneComponent->GetWorldRotation().EulerAngles)
            * MakeTranslationMatrix(F_Vector3_F32::Left() * _StereoBaseline * 0.5f)
        );
    }
    F_Matrix4x4 F_CameraComponent::GetViewMatrix_StereoRight() const
    {
        return Inverse(
            MakeTranslationMatrix(_SceneComponent->GetWorldPosition())
            * MakeRotationMatrix4x4(_SceneComponent->GetWorldRotation().EulerAngles)
            * MakeTranslationMatrix(F_Vector3_F32::Right() * _StereoBaseline * 0.5f)
        );
    }

    void F_CameraComponent::_SendCommonCameraDataToRenderProxy()
    {
        H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
            [
                RenderProxy = GetRenderProxy().FastCast<F_CameraComponentRenderProxy>(),
                CachedViewMatrix_Mono = GetViewMatrix_Mono(),
                CachedViewMatrix_StereoLeft = GetViewMatrix_StereoLeft(),
                CachedViewMatrix_StereoRight = GetViewMatrix_StereoRight(),
                CachedProjectionOptions = _ProjectionOptions
            ]
            {
                RenderProxy->_ViewMatrix_Mono = CachedViewMatrix_Mono;
                RenderProxy->_ViewMatrix_StereoLeft = CachedViewMatrix_StereoLeft;
                RenderProxy->_ViewMatrix_StereoRight = CachedViewMatrix_StereoRight;
                RenderProxy->_ProjectionOptions = CachedProjectionOptions;
            }
        );
    }
}
