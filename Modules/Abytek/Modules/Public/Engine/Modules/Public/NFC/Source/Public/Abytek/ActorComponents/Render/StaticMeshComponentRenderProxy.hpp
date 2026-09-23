#pragma once

#include "Abytek/ActorComponents/Render/PrimitiveComponentRenderProxy.hpp"
#include "Abytek/ActorComponents/StaticMeshComponent.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_NFC_API F_StaticMeshComponentRenderProxy : public A_PrimitiveComponentRenderProxy
    {
    public:
        friend class F_StaticMeshComponent;
        
    private:
        
    public:
        
    public:
        F_StaticMeshComponentRenderProxy(const TW_Valid<F_StaticMeshComponent>& Owner);
        ~F_StaticMeshComponentRenderProxy() override;
        
    protected:
        void OnCreateRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        void OnDestroyRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
    };
}
