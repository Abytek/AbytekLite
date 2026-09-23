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
        TS<F_RenderPrimitiveSet> _PrimitiveSet;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetPrimitiveSet() const noexcept
        {
            return _PrimitiveSet;
        }
        
    public:
        A_PrimitiveComponentRenderProxy(const TW_Valid<A_PrimitiveComponent>& Owner);
        ~A_PrimitiveComponentRenderProxy() override;
        
    protected:
        void OnCreateRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        void OnDestroyRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        
    protected:
        virtual TS<F_RenderPrimitiveSet> CreatePrimitiveSet(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        virtual void SetupPrimitiveSetBuildParams(F_RenderPrimitiveSetBuildParams& OutBuildParams);
    };
}
