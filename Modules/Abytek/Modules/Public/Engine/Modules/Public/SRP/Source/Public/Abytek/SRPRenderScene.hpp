#pragma once

#include "Abytek/Engine.SRP.prerequisites.hpp"
#include "Abytek/RenderBase/RenderScene.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_SRP_API F_SRPRenderScene final : public A_RenderScene
    {
    private:
        
    public:

    public:
        F_SRPRenderScene(const TW_Valid<F_WorldRenderResource>& WorldRenderResource, const F_RenderSceneBuildParams& BuildParams);
        ~F_SRPRenderScene() override;
        
    protected:
        void OnBeginUpdate() override;
        void OnEndUpdate() override;
    };
}
