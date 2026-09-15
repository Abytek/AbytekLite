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
    }

    void A_PrimitiveComponent::OnUnregisterComponent()
    {
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

    B8 A_PrimitiveComponent::Enable()
    {
        B8 Result = _IsEnabled;
        _IsEnabled = true;
        return Result;
    }
    B8 A_PrimitiveComponent::Disable()
    {
        B8 Result = _IsEnabled;
        _IsEnabled = false;
        return Result;
    }
}
