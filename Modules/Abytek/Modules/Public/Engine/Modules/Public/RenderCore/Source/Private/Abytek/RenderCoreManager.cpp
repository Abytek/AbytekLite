#include "Abytek/RenderCoreManager.hpp"
#include "Abytek/World/World.hpp"
#include "Abytek/World/WorldContextHelper.hpp"
#include "Abytek/ApplicationMountHelper.hpp"
#include "Abytek/RenderPack.hpp"
#include "Abytek/RenderPackProxy.hpp"
#include "Abytek/RenderFeatureSets.hpp"
#include "Abytek/GlobalRenderPack.hpp"
#include "Abytek/ModuleList.hpp"
#include "Abytek/RenderRegistry.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_RenderCoreManager)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_RenderCoreManager"));
    }

    ABYTEK_DEFINE_STATIC_SUBSYSTEM(F_RenderCoreManager);

    F_RenderCoreManager::F_RenderCoreManager(const F_ProgramUnitBuildParams& BuildParams) :
        A_WorldSubsystem(BuildParams)
    {
    }
    F_RenderCoreManager::~F_RenderCoreManager()
    {
    }

    void F_RenderCoreManager::OnInit()
    { 
        if (F_RHISubsystem::GetInstance()->IsEnabled())
        {
            _MainFeatureSupports.Model = E_RHIFeatureModel::DEFAULT;
            
            F_RenderRegistryBuildParams BuildParams;
            BuildParams.RHIConfig.API = F_RHISubsystem::GetInstance()->GetActiveAPI();
            BuildParams.RHIConfig.FeatureSupports = _MainFeatureSupports;
            _MainRegistry = TS<F_RenderRegistry>()(BuildParams);
            
            {
                F_RenderRegistry::SetSerializableEnvironmentMetadataElement_Registry(
                    GetWorld()->GetEnvironment(),
                    _MainRegistry
                );
            }
        }
    }
    void F_RenderCoreManager::OnStartup()
    {
        if (F_RHISubsystem::GetInstance()->IsEnabled())
        {
            _AllowCreateRenderPacks.test_and_set(boost::memory_order_release);
            _GlobalRenderPack = H_WorldContext::CreateObject<F_GlobalRenderPack>(
                GetWorld(),
                F_GlobalRenderPack::GetStaticName(),
                F_GlobalRenderPack::GetStaticPackageName()
            );
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
            _GlobalRenderPack->GetPackage()->SaveIfDirty();
#endif
        }
    }
    void F_RenderCoreManager::OnShutdown()
    {
        if (F_RHISubsystem::GetInstance()->IsEnabled())
        {
            _GlobalRenderPack = {};
            _AllowCreateRenderPacks.clear(boost::memory_order_release);
        }
    }
    void F_RenderCoreManager::OnRelease()
    {
        if (F_RHISubsystem::GetInstance()->IsEnabled())
        {
            {
                F_RenderRegistry::UnsetSerializableEnvironmentMetadataElement_Registry(
                    GetWorld()->GetEnvironment()
                );
            }
            
            _MainRegistry = {};
            
            _MainFeatureSupports = {};
        }
    }
}
