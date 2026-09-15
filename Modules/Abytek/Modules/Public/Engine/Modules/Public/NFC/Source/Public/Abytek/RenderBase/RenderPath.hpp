#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/World/WorldContext.hpp"


namespace Abytek
{
    class F_WorldRenderResource;
    class A_RenderObjectFactory;
    
    class ABYTEK_ENGINE_NFC_API A_RenderPath : public A_WorldContext
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_WorldContext)
        ABYTEK_END_REFLECTOR(A_RenderPath)
        
    public:
        static TS<A_RenderPath> Get(const TW_Valid<A_WorldContext>& WorldContext);
        
    private:
    
    public:
        
    public:
        A_RenderPath(const F_SerializableObjectInitParams& InitParams);
        ~A_RenderPath() override;
        
    protected:
        void OnLoad() override;
        void OnUnload() override;
        
    public:
        virtual TS<A_RenderObjectFactory> CreateObjectFactory(const TW_Valid<F_WorldRenderResource>& WorldRenderResource) = 0;
    };
}
