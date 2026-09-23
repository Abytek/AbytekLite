#include "Abytek/ActorComponents/StaticMeshComponent.hpp"
#include "Abytek/ActorComponents/Render/StaticMeshComponentRenderProxy.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_StaticMeshComponent)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_StaticMeshComponent"));
    }
    
    F_StaticMeshComponent::F_StaticMeshComponent(const F_SerializableObjectInitParams& InitParams) :
        A_PrimitiveComponent(InitParams)
    {
    }
    F_StaticMeshComponent::~F_StaticMeshComponent()
    {
    }

    void F_StaticMeshComponent::OnRegisterComponent()
    {
        A_PrimitiveComponent::OnRegisterComponent();
    }

    void F_StaticMeshComponent::OnUnregisterComponent()
    {
        A_PrimitiveComponent::OnUnregisterComponent();
    }

    B8 F_StaticMeshComponent::IsRenderable() const
    {
        if (!A_PrimitiveComponent::IsRenderable())
        {
            return false;
        }
        return static_cast<B8>(_StaticMesh);
    }

    void F_StaticMeshComponent::OnCreateRenderState()
    {
        A_PrimitiveComponent::OnCreateRenderState();
    }
    void F_StaticMeshComponent::OnDestroyRenderState()
    {
        A_PrimitiveComponent::OnDestroyRenderState();
    }
    TS<A_RenderProxy> F_StaticMeshComponent::CreateRenderProxy()
    {
        return TS<F_StaticMeshComponentRenderProxy>()(ABYTEK_WTHIS());
    }
}
