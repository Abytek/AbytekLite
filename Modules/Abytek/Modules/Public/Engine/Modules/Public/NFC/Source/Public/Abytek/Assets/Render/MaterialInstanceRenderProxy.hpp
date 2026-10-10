#pragma once

#include "Abytek/Assets/Render/MaterialInterfaceRenderProxy.hpp"
#include "Abytek/Assets/MaterialInstance.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_NFC_API F_MaterialInstanceRenderProxy : public A_MaterialInterfaceRenderProxy
    {
    public:
        friend class F_MaterialInstance;
        
    private:
        TS<F_MaterialRenderProxy> _MaterialRenderProxy;
        
    public:
        
    public:
        F_MaterialInstanceRenderProxy(const TW_Valid<F_MaterialInstance>& Owner);
        ~F_MaterialInstanceRenderProxy() override;
        
    protected:
        void OnCreateRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        void OnDestroyRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        
    public:
        TS<F_MaterialRenderProxy> GetMaterialRenderProxy() const override;
    };
}
