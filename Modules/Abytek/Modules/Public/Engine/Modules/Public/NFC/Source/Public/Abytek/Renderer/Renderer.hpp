#pragma once

#include "Abytek/Renderer/RenderObject.hpp"


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
        TF_ConcurrentQueue<TF_Function<void(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)>> PreRenderQueue;
        TF_ConcurrentQueue<TF_Function<void(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)>> PostRenderQueue;
        
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
        ABYTEK_RENDER_OBJECT(A_Renderer, A_RenderObject);
        
    public:
        virtual void Init(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
            const F_RendererBuildParams& BuildParams
        );
        void Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        
    protected:
        virtual void OnRender(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        
    public:
        void Render(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
    };
}
