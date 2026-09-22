#pragma once

#include "Abytek/Engine.SRP.prerequisites.hpp"
#include "Abytek/Renderer/RenderObjectFactory.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_SRP_API F_SRPRenderObjectFactory : public A_RenderObjectFactory
    {
    private:
    
    public:
        
    public:
        F_SRPRenderObjectFactory(const TW<F_WorldRenderResource>& WorldRenderResource);
        ~F_SRPRenderObjectFactory() override;
        
    public:
        TS<A_RenderScene> CreateScene() override;
        TS<A_RenderViewFamily> CreateViewFamily() override;
        TS<A_RenderView> CreateView() override;
        TS<A_Renderer> CreateRenderer() override;
    };
}
