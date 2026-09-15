#pragma once

#include "Abytek/Engine.SRP.prerequisites.hpp"
#include "Abytek/RenderBase/RenderViewFamily.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_SRP_API F_SRPRenderViewFamily final : public A_RenderViewFamily
    {
    private:
        
    public:

    public:
        F_SRPRenderViewFamily(const TW_Valid<F_WorldRenderResource>& WorldRenderResource, const F_RenderViewFamilyBuildParams& BuildParams);
        ~F_SRPRenderViewFamily() override;
    };
}
