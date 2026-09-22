#include "Abytek/RenderCoreModule.hpp"
#include "Abytek/ApplicationMountHelper.hpp"
#include "Abytek/RenderCoreManager.hpp"
#include "Abytek/RenderRegistry.hpp"
#include "Abytek/RenderRegistryRuntime.hpp"
#include "Abytek/RenderPack.hpp"
#include "Abytek/GlobalRenderPack.hpp"
#include "Abytek/GlobalRenderBinding.hpp"
#include "Abytek/GlobalRenderPipeline.hpp"
#include "Abytek/ModuleList.hpp"
#include "Abytek/Development/RenderCore/RenderCoreCookSetting.hpp"
#include "Abytek/EngineRuntime.hpp"
#include "Abytek/DevelopmentData.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_RenderCoreModule)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_RenderCoreModule"));
    }
    
    ABYTEK_DEFINE_STATIC_APPLICATION_MODULE(F_RenderCoreModule);
    
    F_RenderCoreModule::F_RenderCoreModule(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationModule(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_MODULE();
        
        AddDependency<F_CoreModule>();
        AddDependency<F_WindowModule>();
        AddDependency<F_AssetsModule>();
        AddDependency<F_RHIModule>();
        AddDependency<F_ResourceModule>();
    }
    F_RenderCoreModule::~F_RenderCoreModule()
    {
    }

    void F_RenderCoreModule::OnReflect()
    {
        RegisterStaticType<F_RenderCoreManager>();
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        RegisterStaticType<F_RenderCoreCookSetting>();
#endif
        RegisterStaticType<F_RenderRegistry>();
        RegisterStaticType<F_RenderRegistryRuntime>();
        RegisterStaticType<F_RenderPack>();
        RegisterStaticType<F_GlobalRenderPack>();
        RegisterStaticType<F_GlobalRenderBinding>();
        RegisterStaticType<F_GlobalRenderPipeline>();
    }

    void F_RenderCoreModule::OnInit()
    {
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        for (const auto& ModuleName : F_ModuleList::GetInstance()->GetAll())
        {
            H_ApplicationMount::Add(
                *ModuleName + ABYTEK_TEXT("::Shaders"),
                ModuleName,
                ABYTEK_TEXT("Shaders")
            );
            H_ApplicationMount::Add(
                *ModuleName + ABYTEK_TEXT("::Intermediate::Shaders"),
                ModuleName,
                ABYTEK_TEXT("Intermediate/Shaders")
            );
        }
#endif
        
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        _GatherShaderSearchPathsForModules();
#endif
    }
    void F_RenderCoreModule::OnRelease()
    {
    }

#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
    void F_RenderCoreModule::_GatherShaderSearchPathsForModules()
    {
        const auto& ModuleNames = F_EngineRuntime::GetInstance()->GetMainDevelopmentData()->GetModuleNames();
        for (const auto& ModuleName : ModuleNames)
        {
            _ModuleNameToShaderSearchPaths[ModuleName] = _GatherShaderSearchPaths(ModuleName);
        }
    }
    TF_Vector<F_Text> F_RenderCoreModule::_GatherShaderSearchPaths(const F_Name& ModuleName)
    {
        auto MainDevelopmentData = F_EngineRuntime::GetInstance()->GetMainDevelopmentData();
        if (!MainDevelopmentData->HasModule(ModuleName))
        {
            return {};
        }
        TF_Vector<F_Text> Result;
        const auto& AccessibleModuleNames = MainDevelopmentData->GetAccessibleModuleNames(ModuleName);
        for (const auto& AccessibleModuleName : AccessibleModuleNames)
        {
            const auto& AccessibleModuleDevelopmentData = MainDevelopmentData->GetModuleDevelopmentData(AccessibleModuleName);
            if (AccessibleModuleName != ModuleName)
            {
                Result.push_back(AccessibleModuleDevelopmentData.Directory + ABYTEK_TEXT("/Shaders/Internal"));
                Result.push_back(AccessibleModuleDevelopmentData.Directory + ABYTEK_TEXT("/Intermediate/Shaders/Internal"));
            }
            if (AccessibleModuleName == ModuleName)
            {
                Result.push_back(AccessibleModuleDevelopmentData.Directory + ABYTEK_TEXT("/Shaders/Private"));
                Result.push_back(AccessibleModuleDevelopmentData.Directory + ABYTEK_TEXT("/Intermediate/Shaders/Private"));
            }
            Result.push_back(AccessibleModuleDevelopmentData.Directory + ABYTEK_TEXT("/Shaders/Public"));
            Result.push_back(AccessibleModuleDevelopmentData.Directory + ABYTEK_TEXT("/Intermediate/Shaders/Public"));
        }
        return ABYTEK_MOVE(Result);
    }

    const TF_Vector<F_Text>& F_RenderCoreModule::GetShaderSearchPaths(const F_Name& ModuleName)
    {
        auto It = _ModuleNameToShaderSearchPaths.find(ModuleName);
        ABYTEK_ENGINE_RENDER_CORE_ASSERT(It != _ModuleNameToShaderSearchPaths.end()) << "Not found module " << ModuleName;
        return It->second;
    }
#endif
}
