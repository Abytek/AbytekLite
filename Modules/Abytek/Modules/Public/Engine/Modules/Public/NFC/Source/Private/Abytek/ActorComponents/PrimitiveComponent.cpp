#include "Abytek/ActorComponents/PrimitiveComponent.hpp"
#include "Abytek/ActorComponents/SceneComponent.hpp"
#include "Abytek/ActorComponents/Render/PrimitiveComponentRenderProxy.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_PrimitiveSceneComponent)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_PrimitiveSceneComponent"));
    }
    
    F_PrimitiveSceneComponent::F_PrimitiveSceneComponent(const F_SerializableObjectInitParams& InitParams) :
        F_SceneComponent(InitParams)
    {
    }
    F_PrimitiveSceneComponent::~F_PrimitiveSceneComponent()
    {
    }

    void F_PrimitiveSceneComponent::OnTransformChanged()
    {
        _PrimitiveComponent->OnTransformChanged();
    }

    ABYTEK_REFLECT(A_PrimitiveComponent)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::A_PrimitiveComponent"));
        
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(_SceneComponent);
    }
    
    A_PrimitiveComponent::A_PrimitiveComponent(const F_SerializableObjectInitParams& InitParams) :
        A_RenderableComponent(InitParams)
    {
        _SceneComponent = CreateSerializableSubobjectDelayLoading<F_PrimitiveSceneComponent>(ABYTEK_NAME("Scene"));
        _SceneComponent->_PrimitiveComponent = ABYTEK_WTHIS();
        AddChildInstanceComponent(_SceneComponent);
    }
    A_PrimitiveComponent::~A_PrimitiveComponent()
    {
    }

    void A_PrimitiveComponent::OnRegisterComponent()
    {
        A_RenderableComponent::OnRegisterComponent();
        
        if (_IsEnabled)
        {
            _Enable_Impl();
        }
    }

    void A_PrimitiveComponent::OnUnregisterComponent()
    {
        if (_IsEnabled)
        {
            _Disable_Impl();
        }
        
        A_RenderableComponent::OnUnregisterComponent();
    }

    B8 A_PrimitiveComponent::IsRenderable() const
    {
        return _IsEnabled;
    }

    void A_PrimitiveComponent::OnCreateRenderState()
    {
        A_RenderableComponent::OnCreateRenderState();
        
        auto WorldTransformMatrix = _SceneComponent->GetWorldTransformMatrix();
        H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
            [
                CachedRenderProxy = GetRenderProxy().StaticCast<A_PrimitiveComponentRenderProxy>(),
                CachedWorldTransformMatrix = WorldTransformMatrix
            ]
            {
                CachedRenderProxy->_WorldTransformMatrix = CachedWorldTransformMatrix;
            }
        );
    }
    void A_PrimitiveComponent::OnDestroyRenderState()
    {
        A_RenderableComponent::OnDestroyRenderState();
    }
    TS<A_RenderProxy> A_PrimitiveComponent::CreateRenderProxy()
    {
        return {};
    }

    void A_PrimitiveComponent::OnEnable()
    {
    }
    void A_PrimitiveComponent::OnDisable()
    {
    }

    void A_PrimitiveComponent::Enable()
    {
        if (_IsEnabled)
        {
            return;
        }
        _IsEnabled = true;
        if (IsRegistered())
        {
            _Enable_Impl();
        }
    }
    void A_PrimitiveComponent::Disable()
    {
        if (!_IsEnabled)
        {
            return;
        }
        _IsEnabled = false;
        if (IsRegistered())
        {
            _Disable_Impl();
        }
    }

    void A_PrimitiveComponent::_Enable_Impl()
    {
        OnEnable();
    }
    void A_PrimitiveComponent::_Disable_Impl()
    {
        OnDisable();
    }

    void A_PrimitiveComponent::OnTransformChanged()
    {
        if (!CreatedRenderState())
        {
            return;
        }
        auto WorldTransformMatrix = _SceneComponent->GetWorldTransformMatrix();
        H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
            [
                CachedRenderProxy = GetRenderProxy().StaticCast<A_PrimitiveComponentRenderProxy>(),
                CachedWorldTransformMatrix = WorldTransformMatrix
            ]
            {
                CachedRenderProxy->_WorldTransformMatrix = CachedWorldTransformMatrix;
            }
        );
    }
}
