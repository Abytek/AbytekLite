#pragma once

#include "Abytek/Engine.RenderCore.prerequisites.hpp"
#include "Abytek/ApplicationModule.hpp"


namespace Abytek
{
    class A_ApplicationSubsystem;
    
    class ABYTEK_ENGINE_RENDER_CORE_API F_RenderCoreModule final : public A_ApplicationModule
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationModule)
        ABYTEK_END_REFLECTOR(F_RenderCoreModule);
        
    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_MODULE(F_RenderCoreModule);

    private:
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        TF_Map<F_Name, TF_Vector<F_Text>> _ModuleNameToShaderSearchPaths;
#endif
        
    public:
        
    public:
        F_RenderCoreModule(const F_ProgramUnitBuildParams& BuildParams);
        ~F_RenderCoreModule() override;

    protected:
        void OnReflect() override;
        
    protected:
        void OnInit() override;
        void OnRelease() override;
        
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
    private:
        void _GatherShaderSearchPathsForModules();
        TF_Vector<F_Text> _GatherShaderSearchPaths(const F_Name& ModuleName);
        
    public:
        const TF_Vector<F_Text>& GetShaderSearchPaths(const F_Name& ModuleName);
#endif
    };
}
