#pragma once

#include "Abytek/Renderer/WorldRenderResourceChild.hpp"
#include "Abytek/Assets/Material.hpp"


namespace Abytek
{
    class F_MaterialRenderProxy;

    class ABYTEK_ENGINE_NFC_API A_MaterialInterfaceRenderProxy : public A_WorldRenderResourceChild
    {
    public:
        friend class A_MaterialInterface;
        
    private:
        F_Name _Name;
        
    public:
        ABYTEK_FORCE_INLINE auto GetName() const noexcept
        {
            return _Name;
        }
        
    protected:
        A_MaterialInterfaceRenderProxy(const TW_Valid<A_MaterialInterface>& Owner);
        
    public:
        ~A_MaterialInterfaceRenderProxy() override;
        
    protected:
        void OnCreateRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        void OnDestroyRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        
    public:
        virtual TS<F_MaterialRenderProxy> GetMaterialRenderProxy() const = 0;
    };
}
