#include "Abytek/Development/EBT/EBTCookManager.hpp"
#include "Abytek/Development/EBT/EBTCookSetting.hpp"
#include "Abytek/DevelopmentData.hpp"
#include "Abytek/EngineRuntime.hpp"
#include "Abytek/ApplicationCore.hpp"
#include "Abytek/Development/Cook/CookProfile.hpp"
#include "Abytek/Development/Cook/CookSettingContainer.hpp"
#include "Abytek/Development/CoreCookGraph/HighLevelCookRange.hpp"
#include "Abytek/Development/Cook/CookUtilities.hpp"
#include "Abytek/World/World.hpp"


#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
namespace Abytek
{
    ABYTEK_REFLECT(F_EBTCookManager)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_EBTCookManager"));
    }

    ABYTEK_DEFINE_STATIC_APPLICATION_SUBSYSTEM(F_EBTCookManager)

    F_Name F_EBTCookManager::GetCreateDevelopmentDataCookFunctionName()
    {
        return ABYTEK_NAME("Abytek::F_EBTCookManager::CreateDevelopmentData");
    }
    F_Name F_EBTCookManager::GetBuildApplicationCookFunctionName()
    {
        return ABYTEK_NAME("Abytek::F_EBTCookManager::BuildApplication");
    }
    void F_EBTCookManager::GlobalInit()
    {
        {
            auto CookFunction = H_CookUtilities::RegisterFunction(
                  []
                  {
                      GetInstance()->_CreateDevelopmentData();
                  },
                  GetCreateDevelopmentDataCookFunctionName()
            );
            CookFunction->AddReverseDependency(F_HighLevelCookRange::GetBeginFunctionName());
        }
        {
            auto CookFunction = H_CookUtilities::RegisterFunction(
                  []
                  {
                      GetInstance()->_BuildApplication();
                  },
                  GetBuildApplicationCookFunctionName()
            );
            CookFunction->AddDependency(F_HighLevelCookRange::GetEndFunctionName());
        }
    }
    void F_EBTCookManager::GlobalRelease()
    {
        H_CookUtilities::UnregisterFunction(
            GetCreateDevelopmentDataCookFunctionName()    
        );
        H_CookUtilities::UnregisterFunction(
            GetBuildApplicationCookFunctionName()    
        );
    }

    F_EBTCookManager::F_EBTCookManager(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationSubsystem(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_SUBSYSTEM();
    }
    F_EBTCookManager::~F_EBTCookManager()
    {
    }

    void F_EBTCookManager::_CreateDevelopmentData()
    {
        auto CookProfile = F_CookProfile::GetMain();
        auto CookSetting = CookProfile->GetSettingContainer()->GetUnit<F_EBTCookSetting>();
        
        F_DevelopmentDataBuildParams DevelopmentDataBuildParams;
        DevelopmentDataBuildParams.ProjectPath = F_EngineRuntime::GetInstance()->GetProjectDirectoryPath();
        DevelopmentDataBuildParams.ProjectInstance = CookSetting->GetProjectInstance();
        DevelopmentDataBuildParams.ApplicationFilter = { A_ApplicationCore::GetInstance()->GetName() };
        ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
            F_DevelopmentData::Create(DevelopmentDataBuildParams, _DevelopmentData)
        );
    }
    void F_EBTCookManager::_BuildApplication()
    {
        auto CookProfile = F_CookProfile::GetMain();
        auto CookSetting = CookProfile->GetSettingContainer()->GetUnit<F_EBTCookSetting>();
        
        const auto& ProjectInstance = CookSetting->GetProjectInstance();
        ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
            H_EBTToolchainUtilities::Prepare(
                F_EngineRuntime::GetInstance()->GetProjectDirectoryPath(),
                ProjectInstance,
                { A_ApplicationCore::GetInstance()->GetName() },
                E_EBTPrepareMode::BUILD_APPLICATION
            )
        );
        ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
            H_EBTToolchainUtilities::GetApplicationBuildResult(
                A_ApplicationCore::GetInstance()->GetName(),
                _DevelopmentData.Weak(),
                ProjectInstance,
                _ApplicationBuildResult
            )    
        );
        
        // Copy cooked data if windows platform
        if (ProjectInstance.Platform == E_Platform::WINDOWS)
        {
            ABYTEK_LOG_INFO() << "Copying necessary cooked data into built Windows application";
            {
                F_Text DstDirectoryPath = _ApplicationBuildResult.DirectoryPath + ABYTEK_TEXT("/Abytek/Modules");
                if (H_FSUtilities::Exists(DstDirectoryPath, E_FSEntryType::DIRECTORY))
                {
                    ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                        H_FSUtilities::DeleteDirectory_(DstDirectoryPath)
                    );
                }
                ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                    H_FSUtilities::CopyDirectory_(
                        DstDirectoryPath,
                        CookProfile->GetIntermediateModulesDirectoryPath()
                    )  
                );
            }
            {
                F_Text DstDirectoryPath = _ApplicationBuildResult.DirectoryPath + ABYTEK_TEXT("/Abytek/Configs");
                if (H_FSUtilities::Exists(DstDirectoryPath, E_FSEntryType::DIRECTORY))
                {
                    ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                        H_FSUtilities::DeleteDirectory_(DstDirectoryPath)
                    );
                }
                ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                    H_FSUtilities::CopyDirectory_(
                        DstDirectoryPath,
                        CookProfile->GetIntermediateConfigsDirectoryPath()
                    )  
                );
            }
            ABYTEK_LOG_INFO() << "Copied necessary cooked data into built Windows application";
        }
        
        {
            F_Text LogText;
            LogText += ABYTEK_TEXT("Cooked application:\n");
            LogText += ABYTEK_TEXT("- Directory: ") + _ApplicationBuildResult.DirectoryPath + ABYTEK_TEXT("\n");
            if (!_ApplicationBuildResult.ExecutableFilePath.empty())
            {
                LogText += ABYTEK_TEXT("- Executable file: ") + _ApplicationBuildResult.ExecutableFilePath + ABYTEK_TEXT("\n");
            }
            ABYTEK_LOG_INFO() << LogText;
        }
    }
}
#endif
