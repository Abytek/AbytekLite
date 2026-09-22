#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"


namespace Abytek
{
    class F_WorldRenderResource;
    class A_RenderScene;
    struct F_RenderSceneBuildParams;
    class A_RenderView;
    struct F_RenderViewBuildParams;
    class A_RenderViewFamily;
    struct F_RenderViewFamilyBuildParams;
    class A_Renderer;
    struct F_RendererBuildParams;
    
    class ABYTEK_ENGINE_NFC_API A_RenderObjectFactory : public A_Object
    {
    private:
        TW<F_WorldRenderResource> _WorldRenderResource;
    
    public:
        ABYTEK_FORCE_INLINE const auto& GetWorldRenderResource() const noexcept
        {
            return _WorldRenderResource;
        }
        
    protected:
        A_RenderObjectFactory(const TW_Valid<F_WorldRenderResource>& WorldRenderResource);
        
    public:
        ~A_RenderObjectFactory() override;
        
    public:
        virtual TS<A_RenderScene> CreateScene() = 0;
        virtual TS<A_RenderViewFamily> CreateViewFamily() = 0;
        virtual TS<A_RenderView> CreateView() = 0;
        virtual TS<A_Renderer> CreateRenderer() = 0;
    };
}
