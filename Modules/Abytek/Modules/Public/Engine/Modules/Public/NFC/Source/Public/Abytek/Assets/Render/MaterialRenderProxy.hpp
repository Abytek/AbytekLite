#pragma once

#include "Abytek/Assets/Render/MaterialInterfaceRenderProxy.hpp"
#include "Abytek/Assets/Material.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_NFC_API F_MaterialRenderProxy : public A_MaterialInterfaceRenderProxy
    {
    public:
        friend class A_Material;
        
    private:
        TS<F_RenderPackData> _RenderPackData;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetRenderPackData() const noexcept
        {
            return _RenderPackData;
        }
        
    public:
        F_MaterialRenderProxy(const TW_Valid<A_Material>& Owner);
        ~F_MaterialRenderProxy() override;
        
    protected:
        void OnCreateRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        void OnDestroyRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        
    public:
        TS<F_MaterialRenderProxy> GetMaterialRenderProxy() const override;
    };
}
