#pragma once

#include "Abytek/ActorComponents/Render/RenderableComponentProxy.hpp"
#include "Abytek/ActorComponents/PrimitiveComponent.hpp"
#include "Abytek/Renderer/RenderPrimitive/RenderPrimitiveSet.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_NFC_API A_PrimitiveComponentRenderProxy : public A_RenderableComponentProxy
    {
    public:
        friend class A_PrimitiveComponent;
        
    private:
        TF_Vector<TS<A_RenderPrimitiveSet>> _PrimitiveSets;
        F_Matrix4x4_F32 _WorldTransformMatrix = Identity<F_Matrix4x4_F32>();
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetPrimitiveSets() const noexcept
        {
            return _PrimitiveSets;
        }
        ABYTEK_FORCE_INLINE const auto& GetWorldTransformMatrix() const noexcept
        {
            return _WorldTransformMatrix;
        }
        
    public:
        A_PrimitiveComponentRenderProxy(const TW_Valid<A_PrimitiveComponent>& Owner);
        ~A_PrimitiveComponentRenderProxy() override;
        
    protected:
        void OnCreateRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        void OnDestroyRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        
    private:
        void _CreateAndInitPrimitiveSets(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        
    protected:
        virtual void CreatePrimitiveSets(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
            TF_Vector<TS<A_RenderPrimitiveSet>>& OutPrimitiveSets
        ) = 0;
        virtual void InitPrimitiveSets(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
            const TF_Vector<TS<A_RenderPrimitiveSet>>& PrimitiveSets
        ) = 0;
    };
}
