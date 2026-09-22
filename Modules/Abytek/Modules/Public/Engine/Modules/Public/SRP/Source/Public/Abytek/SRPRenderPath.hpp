#pragma once

#include "Abytek/Engine.SRP.prerequisites.hpp"
#include "Abytek/Renderer/RenderPath.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_SRP_API F_SRPRenderPath final : public A_RenderPath
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_RenderPath)
        ABYTEK_END_REFLECTOR(F_SRPRenderPath);

    private:
        
    public:

    public:
        F_SRPRenderPath(const F_SerializableObjectInitParams& InitParams);
        ~F_SRPRenderPath() override;
        
    protected:
        TS<A_RenderObjectFactory> CreateObjectFactory(const TW_Valid<F_WorldRenderResource>& WorldRenderResource) override;
    };
}
