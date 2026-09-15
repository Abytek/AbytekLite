#include "Abytek/ApplicationMountManager.hpp"
#include "Abytek/EngineRuntime.hpp"
#include "Abytek/DevelopmentData.hpp"
#include "Abytek/ModuleDevelopmentData.hpp"
#include "Abytek/ModuleList.hpp"


namespace Abytek
{
    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(F_ApplicationMountManager);
    
    F_ApplicationMountManager::F_ApplicationMountManager()
    {
        ABYTEK_BIND_OBJECT_SINGLETON_CRT();
    }
    F_ApplicationMountManager::~F_ApplicationMountManager()
    {
    }

    void F_ApplicationMountManager::AddMount(const F_Name& Name, const F_Name& ModuleName, const F_Text& RelativePath, E_ApplicationMountFlag Flags)
    {
        F_ApplicationMount Mount;
        Mount.Name = Name;
        Mount.ModuleName = ModuleName;
        Mount.RelativePath = RelativePath;
        Mount.Flags = Flags;
        _Mounts.push_back(Mount);
        
        ABYTEK_ENGINE_CORE_ASSERT(F_ModuleList::GetInstance()->Has(ModuleName)) << "Not found module: " << ModuleName;
        
        const auto& Environment = F_EngineRuntime::GetInstance()->GetSerializableEnvironment();
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        auto MainDevelopmentData = F_EngineRuntime::GetInstance()->GetMainDevelopmentData();
#else 
        auto UserDataDirectoryPath = *A_PathManager::GetInstance()->GetCommonDirectoryPath(E_CommonDirectory::USER_DATA);
#endif

#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        if (FlagHas(Mount.Flags, E_ApplicationMountFlag::DEVELOPMENT_BUILD))
        {
            const auto& ModuleDevelopmentData = MainDevelopmentData->GetModuleDevelopmentData(ModuleName);
            Environment->AddMount(
                Name,
                ModuleDevelopmentData.Directory + ABYTEK_TEXT("/") + RelativePath
            );
        }
#else
        if (FlagHas(Mount.Flags, E_ApplicationMountFlag::NON_DEVELOPMENT_BUILD))
        {
            Environment->AddMount(
                Name,
                UserDataDirectoryPath + ABYTEK_TEXT("/Abytek/Modules/") + *ModuleName + ABYTEK_TEXT("/") + RelativePath
            );
        }
#endif
    }
    void F_ApplicationMountManager::Export(const TW_Valid<F_SerializableEnvironment>& SerializableEnvironment, const F_Text& BaseDirectoryPath)
    {
        for (const auto& Mount : _Mounts)
        {
            if (
                !(
                    (
                        FlagHas(Mount.Flags, E_ApplicationMountFlag::DEVELOPMENT_BUILD)
                        && SerializableEnvironment->HasDevelopmentBuild()
                    )
                    || (
                        FlagHas(Mount.Flags, E_ApplicationMountFlag::NON_DEVELOPMENT_BUILD)
                        && !SerializableEnvironment->HasDevelopmentBuild()
                    )
                )
            )
            {
                continue;
            }
            SerializableEnvironment->AddMount(
                Mount.Name,
                BaseDirectoryPath + ABYTEK_TEXT("/") + *Mount.ModuleName + ABYTEK_TEXT("/") + Mount.RelativePath
            );
        }
    }
}
