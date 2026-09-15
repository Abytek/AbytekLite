#include "Abytek/Development/RenderCore/RenderCoreCookSetting.hpp"
#include "Abytek/RenderProxy.hpp"
#include "Abytek/RenderRegistry.hpp"
#include "Abytek/Development/CMake/CmakeToolchainUtilities.hpp"
#include "Abytek/Development/Cook/CookProfile.hpp"


#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
namespace Abytek
{
    ABYTEK_REFLECT(F_RenderCoreCookSetting)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_RenderCoreCookSetting"));
    }
    
    ABYTEK_DEFINE_STATIC_COOK_SETTING(F_RenderCoreCookSetting);

    F_RenderCoreCookSetting::F_RenderCoreCookSetting(const F_ProgramUnitBuildParams& BuildParams) :
        A_CookSetting(BuildParams)
    {
    }
    F_RenderCoreCookSetting::~F_RenderCoreCookSetting()
    {
    }

    void F_RenderCoreCookSetting::OnConfig()
    {
        _ConsoleVariable_RHIAPI = RegisterConsoleVariable<E_RHIAPI>(
            ABYTEK_NAME("Abytek.RenderCore.RHIAPI"),
            ABYTEK_TEXT(""),
            RHIGetDefaultAPIs()[0]
        );
        _ConsoleVariable_RHIFeatureModel = RegisterConsoleVariable<E_RHIFeatureModel>(
            ABYTEK_NAME("Abytek.RenderCore.RHIFeatureModel"),
            ABYTEK_TEXT(""),
            E_RHIFeatureModel::DEFAULT
        );
    }
    void F_RenderCoreCookSetting::OnPostConfig()
    {
        _RHIConfig.API = _ConsoleVariable_RHIAPI->GetValue();
        _RHIConfig.FeatureSupports.Model = _ConsoleVariable_RHIFeatureModel->GetValue();
    }

    void F_RenderCoreCookSetting::OnInit()
    {
        auto CookProfile = GetProfile();
        auto SerializableEnvironment = CookProfile->GetSerializableEnvironment();
            
        F_RenderRegistryBuildParams BuildParams;
        BuildParams.RHIConfig = _RHIConfig;
        _RenderRegistry = TS<F_RenderRegistry>()(BuildParams);
        
        {
            F_RenderRegistry::SetSerializableEnvironmentMetadataElement_Registry(
                SerializableEnvironment.Weak(),
                _RenderRegistry
            );
        }
    }
    void F_RenderCoreCookSetting::OnRelease()
    {
        auto CookProfile = GetProfile();
        auto SerializableEnvironment = CookProfile->GetSerializableEnvironment();
            
        {
            F_RenderRegistry::UnsetSerializableEnvironmentMetadataElement_Registry(
                SerializableEnvironment.Weak()
            );
        }
        
        _RenderRegistry = {};
    }
}
#endif