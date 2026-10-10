#include "Abytek/ActorComponents/StaticMeshComponent.hpp"
#include "Abytek/ActorComponents/Render/StaticMeshComponentRenderProxy.hpp"
#include "Abytek/Assets/StaticMesh.hpp"
#include "Abytek/Assets/Render/StaticMeshRenderProxy.hpp"


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
        
        H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
            [
                CachedRenderProxy = GetRenderProxy().StaticCast<F_StaticMeshComponentRenderProxy>(),
                CachedStaticMeshRenderProxy = _StaticMesh->GetRenderProxy().StaticCast<F_StaticMeshRenderProxy>()
            ]
            {
                CachedRenderProxy->_StaticMeshRenderProxy = CachedStaticMeshRenderProxy;
            }
        );
    }
    void F_StaticMeshComponent::OnDestroyRenderState()
    {
        A_PrimitiveComponent::OnDestroyRenderState();
    }
    TS<A_RenderProxy> F_StaticMeshComponent::CreateRenderProxy()
    {
        return TS<F_StaticMeshComponentRenderProxy>()(ABYTEK_WTHIS());
    }

    void F_StaticMeshComponent::SetStaticMesh(const TS<F_StaticMesh>& StaticMesh)
    {
        _StaticMesh = StaticMesh;
        if (auto RenderProxy = GetRenderProxy())
        {
            H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
                [
                    CachedRenderProxy = RenderProxy.StaticCast<F_StaticMeshComponentRenderProxy>(),
                    CachedStaticMeshRenderProxy = StaticMesh->GetRenderProxy().StaticCast<F_StaticMeshRenderProxy>()
                ]
                {
                    CachedRenderProxy->_StaticMeshRenderProxy = CachedStaticMeshRenderProxy;
                    CachedRenderProxy->UpdateStaticMesh(CachedStaticMeshRenderProxy);
                }
            );
        }
    }

    void F_StaticMeshComponent::OnTransformChanged()
    {
        A_PrimitiveComponent::OnTransformChanged();
        
        if (auto RenderProxy = GetRenderProxy())
        {
            H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
                [
                    CachedRenderProxy = RenderProxy.StaticCast<F_StaticMeshComponentRenderProxy>()
                ]
                {
                    CachedRenderProxy->UpdateWorldTransformMatrix(
                        CachedRenderProxy->GetWorldTransformMatrix()    
                    );
                }
            );
        }
    }
}
