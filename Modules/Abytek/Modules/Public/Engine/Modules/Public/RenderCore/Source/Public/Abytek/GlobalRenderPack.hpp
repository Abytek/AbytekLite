#pragma once

#include "Abytek/RenderPack.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_RENDER_CORE_API F_GlobalRenderPack final : public F_RenderPack
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(F_RenderPack)
        ABYTEK_END_REFLECTOR(F_GlobalRenderPack)
    
    public:
        static F_Name GetStaticName();
        static F_Name GetStaticPackageName();
     
    private:
        
    public:
        
    public:
        F_GlobalRenderPack(const F_SerializableObjectInitParams& InitParams);
        ~F_GlobalRenderPack() override;
        
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
    public:
        void PrepareTemplates(
            const TW_Valid<F_SerializableEnvironment>& SerializableEnvironment,
            const TW_Valid<A_RenderPackTemplateMap>& RenderPackTemplateMap
        ) override;
#endif
    };
}