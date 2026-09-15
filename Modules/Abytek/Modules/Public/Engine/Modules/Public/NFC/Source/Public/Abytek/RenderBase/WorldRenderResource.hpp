#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/RenderProxy.hpp"


namespace Abytek
{
    class A_WorldContext;
    class F_RenderManager;
    class A_RenderObjectFactory;
    class F_GlobalGPUDataPackProxy;
    class A_RenderScene;

    class ABYTEK_ENGINE_NFC_API F_WorldRenderResource final : public A_RenderProxy
    {
    public:
        static TS<F_WorldRenderResource> Get_MainTask(const TW_Valid<A_WorldContext>& WorldContext);
    
    private:
        TS<A_RenderObjectFactory> _RenderObjectFactory;
        TS<F_GlobalGPUDataPackProxy> _GlobalGPUDataPackProxy;
        TS<A_RenderScene> _Scene;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetRenderObjectFactory() const noexcept
        {
            return _RenderObjectFactory;
        }
        ABYTEK_FORCE_INLINE const auto& GetGlobalGPUDataPackProxy() const noexcept
        {
            return _GlobalGPUDataPackProxy;
        }
        ABYTEK_FORCE_INLINE const auto& GetScene() const noexcept
        {
            return _Scene;
        }
        
    public:
        F_WorldRenderResource(const TW_Valid<F_RenderManager>& RenderManager);
        ~F_WorldRenderResource() override;
        
    protected:
        void OnCreateRenderState_RenderTask() override;
        void OnDestroyRenderState_RenderTask() override;
    };
}
