#pragma once

#include "Abytek/ActorComponents/Render/PrimitiveComponentRenderProxy.hpp"
#include "Abytek/ActorComponents/StaticMeshComponent.hpp"


namespace Abytek
{
    class F_StaticMeshRenderProxy;

    class ABYTEK_ENGINE_NFC_API F_StaticMeshComponentRenderProxy : public A_PrimitiveComponentRenderProxy
    {
    public:
        friend class F_StaticMeshComponent;
        
    private:
        TS<F_StaticMeshRenderProxy> _StaticMeshRenderProxy;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetStaticMeshRenderProxy() const noexcept
        {
            return _StaticMeshRenderProxy;
        }
        
    public:
        F_StaticMeshComponentRenderProxy(const TW_Valid<F_StaticMeshComponent>& Owner);
        ~F_StaticMeshComponentRenderProxy() override;
        
    protected:
        void OnCreateRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        void OnDestroyRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        
    protected:
        void CreatePrimitiveSets(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
            TF_Vector<TS<A_RenderPrimitiveSet>>& OutPrimitiveSets
        ) override;
        void InitPrimitiveSets(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
            const TF_Vector<TS<A_RenderPrimitiveSet>>& PrimitiveSets
        ) override;
        
    public:
        void UpdateWorldTransformMatrix_Simple(const F_Matrix4x4_F32& Value);
        void UpdateStaticMesh_Simple(const TS<F_StaticMeshRenderProxy>& StaticMeshRenderProxy);
    };
}
