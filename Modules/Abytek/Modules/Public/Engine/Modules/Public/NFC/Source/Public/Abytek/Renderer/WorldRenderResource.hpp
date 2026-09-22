#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/RenderProxy.hpp"


namespace Abytek
{
    class A_WorldContext;
    class F_RendererManager;
    class A_RenderObjectFactory;
    class A_RenderScene;

    class ABYTEK_ENGINE_NFC_API F_WorldRenderResource final : public A_RenderProxy
    {
    public:
        static TS<F_WorldRenderResource> Get_MainTask(const TW_Valid<A_WorldContext>& WorldContext);
    
    private:
#ifdef ABYTEK_DEBUG_INFO
        F_Name _WorldDebugName;
#endif
        TS<A_RenderObjectFactory> _RenderObjectFactory;
        TS<A_RenderScene> _Scene;
        
    public:
#ifdef ABYTEK_DEBUG_INFO
        ABYTEK_FORCE_INLINE const auto& GetWorldDebugName() const noexcept
        {
            return _WorldDebugName;
        }
#endif
        ABYTEK_FORCE_INLINE const auto& GetRenderObjectFactory() const noexcept
        {
            return _RenderObjectFactory;
        }
        ABYTEK_FORCE_INLINE const auto& GetScene() const noexcept
        {
            return _Scene;
        }
        
    public:
        F_WorldRenderResource(const TW_Valid<F_RendererManager>& RendererManager);
        ~F_WorldRenderResource() override;
        
    protected:
        void OnCreateRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        void OnDestroyRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
    };
}
