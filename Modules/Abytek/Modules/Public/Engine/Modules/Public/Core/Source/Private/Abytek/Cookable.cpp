#include "Abytek/Cookable.hpp"
#include "Abytek/ApplicationCore.hpp"
#include "Abytek/EngineRuntime.hpp"
#include "Abytek/ModuleList.hpp"
#include "Abytek/DevelopmentData.hpp"
#include "Abytek/CoreUpdateGraph/HighLevelUpdateRange.hpp"
#include "Abytek/Development/Cook/CookProfile.hpp"
#include "Abytek/Development/Cook/CookUtilities.hpp"
#include "Abytek/Development/CoreCookGraph/AssetAnalyzeCookRange.hpp"
#include "Abytek/Development/CoreCookGraph/HighLevelCookRange.hpp"
#include "Abytek/World/World.hpp"
#include "Abytek/World/WorldContextHelper.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(I_Cookable)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::I_Cookable"));
    }

#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
    F_Name I_Cookable::GetSerializableEnvironmentMetadataElementName_IsCookMode()
    {
        return ABYTEK_NAME("Abytek::I_Cookable::IsCookMode");
    }
    B8 I_Cookable::GetSerializableEnvironmentMetadataElement_IsCookMode(const TW_Valid<F_SerializableEnvironment>& SerializableEnvironment)
    {
        const auto& Metadata = SerializableEnvironment->Metadata;
        if (Metadata.find(GetSerializableEnvironmentMetadataElementName_IsCookMode()) == Metadata.end())
        {
            return false;
        }
        return AnyCast<B8>(
            Metadata.find(GetSerializableEnvironmentMetadataElementName_IsCookMode())
            ->second
        );
    }
    void I_Cookable::SetSerializableEnvironmentMetadataElement_IsCookMode(const TW_Valid<F_SerializableEnvironment>& SerializableEnvironment, B8 Value)
    {
        auto& Metadata = SerializableEnvironment->Metadata;
        Metadata[GetSerializableEnvironmentMetadataElementName_IsCookMode()] = Value;
    }
    void I_Cookable::UnsetSerializableEnvironmentMetadataElement_IsCookMode(const TW_Valid<F_SerializableEnvironment>& SerializableEnvironment)
    {
        auto& Metadata = SerializableEnvironment->Metadata;
        Metadata.erase(Metadata.find(GetSerializableEnvironmentMetadataElementName_IsCookMode()));
    }
#endif
    B8 I_Cookable::IsCookModeSerializableEnvironment(const TW_Valid<F_SerializableEnvironment>& SerializableEnvironment)
    {
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        return GetSerializableEnvironmentMetadataElement_IsCookMode(SerializableEnvironment);
#else
        return false;
#endif
    }

#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
    B8 I_Cookable::ShouldCook()
    {
        return true;
    }
    
    void I_Cookable::PrepareForCooking()
    {
    }
    void I_Cookable::Cook()
    {
    }
    void I_Cookable::CleanUpAfterCooking()
    {
    }
#endif

#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
    F_Name I_Cookable::GetMainCookFunctionName()
    {
        return ABYTEK_NAME("Abytek::I_Cookable::Main");
    }

    void I_Cookable::_GlobalInit()
    {
        {
            auto CookFunction = H_CookUtilities::RegisterFunction(
                []
                {
                    auto CookProfile = F_CookProfile::GetMain();
                    auto MainDevelopmentData = F_EngineRuntime::GetInstance()->GetMainDevelopmentData();
                    
                    for (const auto& [PackageName, PackageInfo] : F_AssetAnalyzeCookRange::GetInstance()->GetPackageInfos())
                    {
                        F_YieldSpinLock Sync;
                        Sync.Lock();
                        F_HighLevelUpdateRange::EnqueueCommand(
                        [CachedPackageName = PackageName, &Sync]
                            {
                                _CookAsset(CachedPackageName);
                                Sync.Unlock();
                            }
                        );
                        Sync.Lock();
                        Sync.Unlock();
                    }
                },
                GetMainCookFunctionName()
            );
            CookFunction->AddDependency(
                F_HighLevelCookRange::GetBeginFunctionName()
            );
            CookFunction->AddReverseDependency(
                F_HighLevelCookRange::GetEndFunctionName()
            );
        }
    }
    void I_Cookable::_GlobalRelease()
    {
        H_CookUtilities::UnregisterFunction(
            GetMainCookFunctionName()  
        );
    }

    void I_Cookable::_CookAsset(const F_Name& AssetPackageName)
    {
        ABYTEK_LOG_INFO() << "Cooking asset package: " << AssetPackageName;
        
        auto CookProfile = F_CookProfile::GetMain();
        auto World = F_World::GetMain();
        
        auto Package = H_WorldContext::EnsurePackage(World, AssetPackageName);
        
        auto Objects = Package->GetOrLoadAllObjects();
        
        TF_Map<F_Name, TS<A_SerializableObject>> MapObjectPathToObject_ShouldCook;
        for (const auto& Object : Objects)
        {
            TW<I_Cookable> Cookable;
            if (!Object.TryDynamicCast<I_Cookable>(Cookable))
            {
                continue;
            }
            if (!Cookable->ShouldCook())
            {
                continue;
            }
            MapObjectPathToObject_ShouldCook.insert({
                Object->GetPath(),
                Object
            });
        }
        
        for (const auto& [ObjectPath, Object] : MapObjectPathToObject_ShouldCook)
        {
            TW<I_Cookable> Cookable = Object.DynamicCast<I_Cookable>();
            Cookable->PrepareForCooking();
        }
        
        for (const auto& [ObjectPath, Object] : MapObjectPathToObject_ShouldCook)
        {
            TW<I_Cookable> Cookable = Object.DynamicCast<I_Cookable>();
            ABYTEK_LOG_INFO() << "Cooking object: " << ObjectPath;
            Cookable->Cook();
            ABYTEK_LOG_INFO() << "Cooked object: " << ObjectPath;
        }
        
        Package->Save(CookProfile->GetSerializableEnvironment().Weak());
        
        for (const auto& [ObjectPath, Object] : MapObjectPathToObject_ShouldCook)
        {
            TW<I_Cookable> Cookable = Object.DynamicCast<I_Cookable>();
            Cookable->CleanUpAfterCooking();
        }
            
        ABYTEK_LOG_INFO() << "Cooked asset package: " << AssetPackageName;
    }
#endif
}
