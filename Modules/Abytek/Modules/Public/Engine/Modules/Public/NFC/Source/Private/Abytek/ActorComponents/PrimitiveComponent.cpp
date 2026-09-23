#include "Abytek/ActorComponents/PrimitiveComponent.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(A_PrimitiveComponent)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::A_PrimitiveComponent"));
    }
    
    A_PrimitiveComponent::A_PrimitiveComponent(const F_SerializableObjectInitParams& InitParams) :
        A_RenderableComponent(InitParams)
    {
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
        if (IsRegistered() && !HasSerializableFlags(E_SerializableObjectFlag::CDO))
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
        if (IsRegistered() && !HasSerializableFlags(E_SerializableObjectFlag::CDO))
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
}
