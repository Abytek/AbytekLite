#include "Abytek/Development/EBT/EBTCookSetting.hpp"
#include "Abytek/Development/CMake/CmakeToolchainUtilities.hpp"


#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
namespace Abytek
{
    ABYTEK_REFLECT(F_EBTCookSetting)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_EBTCookSetting"));
    }
    
    ABYTEK_DEFINE_STATIC_COOK_SETTING(F_EBTCookSetting);

    F_EBTCookSetting::F_EBTCookSetting(const F_ProgramUnitBuildParams& BuildParams) :
        A_CookSetting(BuildParams)
    {
    }
    F_EBTCookSetting::~F_EBTCookSetting()
    {
    }

    void F_EBTCookSetting::OnConfig()
    {
        F_Name DefaultEnvironmentName;
        H_EBTToolchainUtilities::GetEnvironmentName(E_EBTEnvironment::COOKED_MAX_OPTIMIZED, DefaultEnvironmentName);
        F_Name DefaultPlatformName;
        H_CommonToolchainUtilities::GetPlatformName(E_Platform::CURRENT, DefaultPlatformName);
        F_Name DefaultCMakeConfigName;
        H_CMakeToolchainUtilities::GetConfigName(E_CMakeConfig::CURRENT, DefaultCMakeConfigName);
        F_Name DefaultProcessorArchName;
        H_CommonToolchainUtilities::GetProcessorArchName(E_ProcessorArch::CURRENT, DefaultProcessorArchName);
        
        _ConsoleVariable_EnvironmentName = RegisterConsoleVariable<F_Name>(
            ABYTEK_NAME("Abytek.EBT.Environment"),
            ABYTEK_TEXT(""),
            DefaultEnvironmentName
        );
        _ConsoleVariable_PlatformName = RegisterConsoleVariable<F_Name>(
            ABYTEK_NAME("Abytek.EBT.Platform"),
            ABYTEK_TEXT(""),
            DefaultPlatformName
        );
        _ConsoleVariable_CMakeConfigName = RegisterConsoleVariable<F_Name>(
            ABYTEK_NAME("Abytek.EBT.CMakeConfig"),
            ABYTEK_TEXT(""),
            DefaultCMakeConfigName
        );
        _ConsoleVariable_ProcessorArchNames = RegisterConsoleVariable<TF_Vector<F_Name>>(
            ABYTEK_NAME("Abytek.EBT.ProcessorArch"),
            ABYTEK_TEXT(""),
            { DefaultProcessorArchName }
        );
    }
    void F_EBTCookSetting::OnPostConfig()
    {
        F_EBTProjectInstanceParams ProjectInstanceParams;
        
        const auto& CookEnvironmentName = _ConsoleVariable_EnvironmentName->GetValue();
        ABYTEK_LOG_INFO() << "Found cvar for \"Abytek.EBT.Environment\": " << *CookEnvironmentName;
        if (!H_EBTToolchainUtilities::ParseEnvironment(CookEnvironmentName, ProjectInstanceParams.Environment))
        {
            ABYTEK_LOG_FATAL() << "Invalid \"Abytek.EBT.Environment\": " << *CookEnvironmentName;
        }
        if (
            !(
                (ProjectInstanceParams.Environment == E_EBTEnvironment::COOKED)    
                || (ProjectInstanceParams.Environment == E_EBTEnvironment::COOKED_MAX_OPTIMIZED) 
            )
        )
        {
            ABYTEK_LOG_FATAL() << "Invalid cooked \"Abytek.EBT.Environment\": " << *CookEnvironmentName;
        }
    
        const auto& CookPlatformName = _ConsoleVariable_PlatformName->GetValue();
        ABYTEK_LOG_INFO() << "Found cvar for \"Abytek.EBT.Platform\": " << *CookPlatformName;
        if (!H_CommonToolchainUtilities::ParsePlatform(CookPlatformName, ProjectInstanceParams.Platform))
        {
            ABYTEK_LOG_FATAL() << "Invalid \"Abytek.EBT.Platform\": " << *CookPlatformName;
        }

        switch (ProjectInstanceParams.Platform)
        {
        case E_Platform::WINDOWS:
#ifdef ABYTEK_EBT_CMAKE_GENERATOR_VISUAL_STUDIO_2026
            ProjectInstanceParams.CMakeGenerator = E_CMakeGenerator::VISUAL_STUDIO_2026;
#elif defined(ABYTEK_EBT_CMAKE_GENERATOR_VISUAL_STUDIO_2022)
            ProjectInstanceParams.CMakeGenerator = E_CMakeGenerator::VISUAL_STUDIO_2022;
#else 
#error "Unknown generator"
#endif
            break;
        case E_Platform::ANDROID_:
            ProjectInstanceParams.CMakeGenerator = E_CMakeGenerator::NINJA;
            break;
        default:
            ABYTEK_LOG_FATAL() << "Invalid or not supported platform";
            break;
        }
    
        const auto& CookCMakeConfigName = _ConsoleVariable_CMakeConfigName->GetValue();
        ABYTEK_LOG_INFO() << "Found cvar for \"Abytek.EBT.CMakeConfig\": " << *CookCMakeConfigName;
        if (!H_CMakeToolchainUtilities::ParseConfig(CookCMakeConfigName, ProjectInstanceParams.CMakeConfig))
        {
            ABYTEK_LOG_FATAL() << "Invalid \"Abytek.EBT.CMakeConfig\": " << *CookCMakeConfigName;
        }
    
        {
            const auto& ProcessorArchNames = _ConsoleVariable_ProcessorArchNames->GetValue();
            ABYTEK_ENGINE_CORE_ASSERT(ProcessorArchNames.size() > 0) << "Requires \"Abytek.EBT.ProcessorArch\" CLI argument";
            for (const auto& ProcessorArchName : ProcessorArchNames)
            {
                ABYTEK_LOG_INFO() << "Found cvar for \"Abytek.EBT.ProcessorArch\": " << *ProcessorArchName;
                E_ProcessorArch ProcessorArch;
                if (!H_CommonToolchainUtilities::ParseProcessorArch(ProcessorArchName, ProcessorArch))
                {
                    ABYTEK_LOG_FATAL() << "Invalid \"Abytek.EBT.ProcessorArch\": " << *ProcessorArchName;
                }
                ProjectInstanceParams.ProcessorArchs.push_back(ProcessorArch);
            }
        }
        
        _ProjectInstance = ProjectInstanceParams;
    }
}
#endif