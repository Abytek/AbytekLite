#pragma once

#include "Abytek/Engine.SRP.prerequisites.hpp"
#include "Abytek/Renderer/RenderScene.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_SRP_API F_SRPRenderScene final : public A_RenderScene
    {
    private:
        
    public:

    public:
        ABYTEK_RENDER_OBJECT_CREATABLE(F_SRPRenderScene, A_RenderScene);
        
    public:
        virtual void Init(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
            const F_RenderSceneBuildParams& BuildParams
        );
        void Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        
    protected:
        void OnBeginUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        void OnEndUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        void OnBeginPostUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        void OnEndPostUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
    };
}
