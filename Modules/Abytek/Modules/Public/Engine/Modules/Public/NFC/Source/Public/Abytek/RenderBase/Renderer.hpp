#pragma once

#include "Abytek/RenderBase/RenderObject.hpp"


namespace Abytek
{
    class A_RenderScene;
    class A_RenderViewFamily;

    struct F_RendererBuildParams
    {
        TW<A_RenderScene> Scene;
        TS<A_RenderViewFamily> ViewFamily;
    };
    class ABYTEK_ENGINE_NFC_API A_Renderer : public A_RenderObject
    {
    private:
        TW<A_RenderScene> _Scene;
        TS<A_RenderViewFamily> _ViewFamily;
        
    public:
        TF_ConcurrentQueue<TF_Function<void()>> PreRenderQueue;
        TF_ConcurrentQueue<TF_Function<void()>> PostRenderQueue;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetScene() const noexcept
        {
            return _Scene;
        }
        ABYTEK_FORCE_INLINE const auto& GetViewFamily() const noexcept
        {
            return _ViewFamily;
        }
        
    public:
        A_Renderer(const TW_Valid<F_WorldRenderResource>& WorldRenderResource, const F_RendererBuildParams& BuildParams);
        ~A_Renderer() override;
        
    protected:
        virtual void OnRender();
        
    public:
        void Render();
    };
}
