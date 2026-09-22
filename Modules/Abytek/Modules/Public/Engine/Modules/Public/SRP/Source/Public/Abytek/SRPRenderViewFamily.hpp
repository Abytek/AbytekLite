#pragma once

#include "Abytek/Engine.SRP.prerequisites.hpp"
#include "Abytek/Renderer/RenderViewFamily.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_SRP_API F_SRPRenderViewFamily final : public A_RenderViewFamily
    {
    private:
        
    public:

    public:
        ABYTEK_RENDER_OBJECT_CREATABLE(F_SRPRenderViewFamily, A_RenderViewFamily);
        
    public:
        void Init(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
            const F_RenderViewFamilyBuildParams& BuildParams
        ) override;
        void Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
    };
}
