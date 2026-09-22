#pragma once

#include "Abytek/Engine.SRP.prerequisites.hpp"
#include "Abytek/Renderer/Renderer.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_SRP_API F_SRPRenderer : public A_Renderer
    {
    private:
        
    public:
        
    public:
        ABYTEK_RENDER_OBJECT_CREATABLE(F_SRPRenderer, A_Renderer);
        
    public:
        void Init(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
            const F_RendererBuildParams& BuildParams
        ) override;
        void Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        
    protected:
        void OnRender(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
    };
}
