#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/RenderBase/WorldContextRenderProxy.hpp"


namespace Abytek
{
    class A_RenderableComponent;

    class ABYTEK_ENGINE_NFC_API A_RenderableComponentProxy : public A_WorldContextRenderProxy
    {
    private:
        
    public:
        
    protected:
        A_RenderableComponentProxy(const TW_Valid<A_RenderableComponent>& Component);
        
    public:
        ~A_RenderableComponentProxy() override;
    };
}
