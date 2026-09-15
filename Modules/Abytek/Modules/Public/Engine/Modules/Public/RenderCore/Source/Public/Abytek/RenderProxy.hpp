#pragma once

#include "Abytek/RenderResource.hpp"


namespace Abytek
{
    class A_Renderable;

    class ABYTEK_ENGINE_RENDER_CORE_API A_RenderProxy : public A_RenderResource
    {
    public:
        friend class A_Renderable;
        
    private:
        TW<A_Renderable> _Owner;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetOwner() const noexcept
        {
            return _Owner;
        }
        
    protected:
        A_RenderProxy(const TW_Valid<A_Renderable>& Owner);
        
    public:
        ~A_RenderProxy() override;
        
    protected:
        void OnInit_RenderTask() override;
        void OnRelease_RenderTask() override;
        
    protected:
        virtual void OnCreateRenderState_RenderTask();
        virtual void OnDestroyRenderState_RenderTask();
    };
}
