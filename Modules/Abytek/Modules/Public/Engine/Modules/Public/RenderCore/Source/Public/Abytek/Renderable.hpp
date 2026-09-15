#pragma once

#include "Abytek/Engine.RenderCore.prerequisites.hpp"

   
namespace Abytek
{
    class A_RenderProxy;
    class F_RenderRegistry;
    
    class ABYTEK_ENGINE_RENDER_CORE_API A_Renderable
    {
    private:
        B8 _ShouldEnableProxy = false;
        TS<A_RenderProxy> _RenderProxy;
        B8 _CreatedRenderState = false;
        
    public:
        ABYTEK_FORCE_INLINE auto ShouldEnableProxy() const noexcept
        {
            return _ShouldEnableProxy;
        }
        ABYTEK_FORCE_INLINE const auto& GetRenderProxy() const noexcept
        {
            return _RenderProxy;
        }
        ABYTEK_FORCE_INLINE auto CreatedRenderState() const noexcept
        {
            return _CreatedRenderState;
        }
        
    protected:
        A_Renderable();

    public:
        virtual ~A_Renderable();
        
    protected:
        void SetupRenderable();
        void CleanUpRenderable();
        
    public:
        virtual B8 IsRenderable() const;
        
    protected:
        virtual TS<A_RenderProxy> CreateRenderProxy() = 0;
        virtual void OnCreateRenderState();
        virtual void OnDestroyRenderState();
        
    protected:
        void CreateRenderState();
        void DestroyRenderState();
        void RecreateRenderState();
        
    public:
        TS<F_RenderRegistry> GetRenderRegistry() const;
    };
}
