#include "Abytek/ConfigFileUtilities.hpp"
#include "Abytek/EngineRuntime.hpp"
#include "Abytek/DevelopmentData.hpp"


namespace Abytek
{
    void H_ConfigFileUtilities::Gather(TF_Vector<F_Text>& OutConfigFiles, const F_Name& FileName, const F_Name& ModuleName, B8 UseDevelopmentData)
    {
        // Load development config files
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        if (UseDevelopmentData)
        {
            if (auto MainDevelopmentData = F_EngineRuntime::GetInstance()->GetMainDevelopmentData())
            {
                for (const auto& AccessibleModuleName : MainDevelopmentData->GetAccessibleModuleNames(ModuleName))
                {
                    const auto& ModuleDevelopmentData = MainDevelopmentData->GetModuleDevelopmentData(AccessibleModuleName);
                    F_Text ConfigDirectoryPath = ModuleDevelopmentData.Directory + ABYTEK_TEXT("/Configs");
                    {
                        F_Text ConfigFilePath = ConfigDirectoryPath + ABYTEK_TEXT("/") + *FileName;
                        if (!H_FSUtilities::Exists(ConfigFilePath, E_FSEntryType::FILE))
                        {
                            continue;
                        }
                        OutConfigFiles.push_back(ConfigFilePath);
                    }
                }
            }
        }
#endif
        
        // Load bundled config files
        F_Text BundledConfigsDirectoryPath = (
            *A_PathManager::GetInstance()->GetCommonDirectoryPath(
                E_CommonDirectory::BUNDLED_DATA
            )
            + ABYTEK_TEXT("/")
            + Console::GetBundledConfigsDirectoryRelativePath()
        );
        TF_Vector<F_Text> ModuleConfigsDirectoryNames;
        if (H_FSUtilities::ListEntries<E_FSEntryType::DIRECTORY>(BundledConfigsDirectoryPath, ModuleConfigsDirectoryNames, false))
        {
            for (const auto& ModuleConfigsDirectoryName : ModuleConfigsDirectoryNames)
            {
                F_Text ModuleConfigsDirectoryPath = BundledConfigsDirectoryPath + ABYTEK_TEXT("/") + ModuleConfigsDirectoryName;
                if (!H_FSUtilities::Exists(ModuleConfigsDirectoryPath, E_FSEntryType::DIRECTORY))
                {
                    continue;
                }
                
                F_Text ConfigFilePath = ModuleConfigsDirectoryPath + ABYTEK_TEXT("/") + *FileName;
                if (!H_FSUtilities::Exists(ConfigFilePath, E_FSEntryType::FILE))
                {
                    continue;
                }
                OutConfigFiles.push_back(ConfigFilePath);
            }
        }
    }
}
