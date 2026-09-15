#pragma once

#include "Abytek/Engine.Core.prerequisites.hpp"
#include "Abytek/ApplicationModule.hpp"


namespace Abytek
{
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
    class F_CookGraph;
    struct F_AssetAnalyzeCookRange; 
    struct F_HighLevelCookRange;
#endif
    
    class ABYTEK_ENGINE_CORE_API F_CoreModule final : public A_ApplicationModule
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationModule)
        ABYTEK_END_REFLECTOR(F_CoreModule)
        
    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_MODULE(F_CoreModule);
        
    private:
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        TU<F_CookGraph> _CookGraph;
        TU<F_AssetAnalyzeCookRange> _AssetAnalyzeCookRange;
        TU<F_HighLevelCookRange> _HighLevelCookRange;
#endif
        
    public:
        
    public:
        F_CoreModule(const F_ProgramUnitBuildParams& BuildParams);
        ~F_CoreModule() override;

    protected:
        void OnReflect() override;
        
    protected:
        void OnInit() override;
        void OnRelease() override;
    };
}
