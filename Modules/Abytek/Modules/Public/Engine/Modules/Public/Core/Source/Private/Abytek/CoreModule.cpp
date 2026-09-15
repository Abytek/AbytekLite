#include "Abytek/CoreModule.hpp"
#include "Abytek/DevelopmentData.hpp"
#include "Abytek/ApplicationCore.hpp"
#include "Abytek/GlobalTime.hpp"
#include "Abytek/Development/Cook/CookProfile.hpp"
#include "Abytek/Development/Cook/CookSetting.hpp"
#include "Abytek/Development/Cook/CookSettingContainer.hpp"
#include "Abytek/Development/EBT/EBTCookManager.hpp"
#include "Abytek/Development/EBT/EBTCookSetting.hpp"
#include "Abytek/Development/Cook/CookGraph.hpp"
#include "Abytek/Development/CoreCookGraph/AssetAnalyzeCookRange.hpp"
#include "Abytek/Development/CoreCookGraph/HighLevelCookRange.hpp"
#include "Abytek/Cookable.hpp"
#include "Abytek/Level/Level.hpp"
#include "Abytek/World/World.hpp"
#include "Abytek/World/WorldBusiness.hpp"
#include "Abytek/World/WorldContext.hpp"
#include "Abytek/World/WorldManager.hpp"
#include "Abytek/World/WorldSubsystem.hpp"
#include "Abytek/World/WorldSubsystemContainer.hpp"
#include "Abytek/Actor/Actor.hpp"
#include "Abytek/Actor/ActorComponent.hpp"
#include "Abytek/Actor/ActorComponentManager.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_CoreModule)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_CoreModule"));
    }
    
    ABYTEK_DEFINE_STATIC_APPLICATION_MODULE(F_CoreModule);
    
    F_CoreModule::F_CoreModule(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationModule(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_MODULE();
    }
    F_CoreModule::~F_CoreModule()
    {
    }

    void F_CoreModule::OnReflect()
    {
        RegisterStaticType<F_World>();
        RegisterStaticType<A_WorldContext>();
        RegisterStaticType<F_WorldManager>();
        RegisterStaticType<A_WorldSubsystem>();
        RegisterStaticType<F_WorldSubsystemContainer>();
        RegisterStaticType<F_WorldBusiness>();
        RegisterStaticType<F_Level>();
        
        RegisterStaticType<F_Actor>();
        RegisterStaticType<A_ActorComponent>();
        RegisterStaticType<F_ActorComponentManager>();
        
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        RegisterStaticType<F_CookProfile>();
        RegisterStaticType<A_CookSetting>();
        RegisterStaticType<F_CookSettingContainer>();
        RegisterStaticType<F_EBTCookManager>();
        RegisterStaticType<F_EBTCookSetting>();
#endif
#ifdef ABYTEK_ENABLE_DEVELOPMENT_DATA
        RegisterStaticType<F_DevelopmentData>();
        RegisterStaticType<F_DevelopmentDataInstance>();
#endif
        RegisterStaticType<F_GlobalTime>();
    }

    void F_CoreModule::OnInit()
    {
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        _CookGraph = TU<F_CookGraph>()();
        _HighLevelCookRange = TU<F_HighLevelCookRange>()();
        _AssetAnalyzeCookRange = TU<F_AssetAnalyzeCookRange>()();
#endif
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        I_Cookable::_GlobalInit();
#endif
        A_WorldSubsystem::GlobalInit();
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        F_EBTCookManager::GlobalInit();
#endif
    }
    void F_CoreModule::OnRelease()
    {
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        F_EBTCookManager::GlobalRelease();
#endif
        A_WorldSubsystem::GlobalRelease();
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        I_Cookable::_GlobalRelease();
#endif
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        _AssetAnalyzeCookRange = {};
        _HighLevelCookRange = {};
        _CookGraph = {};
#endif
    }
}
