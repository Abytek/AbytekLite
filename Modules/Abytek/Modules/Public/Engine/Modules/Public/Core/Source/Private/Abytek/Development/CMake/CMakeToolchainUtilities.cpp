#include "Abytek/Development/CMake/CMakeToolchainUtilities.hpp"


#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
namespace Abytek
{
    F_FeedbackStatus H_CMakeToolchainUtilities::ParseConfig(const F_Name& Name, E_CMakeConfig& OutConfig)
    {
        if (Name == ABYTEK_NAME("Auto"))
        {
            OutConfig = E_CMakeConfig::AUTO;
        }
        else if (Name == ABYTEK_NAME("Debug"))
        {
            OutConfig = E_CMakeConfig::DEBUG;
        }
        else if (Name == ABYTEK_NAME("Release"))
        {
            OutConfig = E_CMakeConfig::RELEASE;
        }
        else if (Name == ABYTEK_NAME("RelWithDebInfo"))
        {
            OutConfig = E_CMakeConfig::REL_WITH_DEB_INFO;
        }
        else if (Name == ABYTEK_NAME("MinSizeRel"))
        {
            OutConfig = E_CMakeConfig::MIN_SIZE_REL;
        }
        else
        {
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Unknown config: ") + *Name);
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus H_CMakeToolchainUtilities::GetConfigName(E_CMakeConfig Config, F_Name& OutName)
    {
        switch (Config)
        {
        case E_CMakeConfig::AUTO:
            OutName = ABYTEK_NAME("Auto");
            break;
        case E_CMakeConfig::DEBUG:
            OutName = ABYTEK_NAME("Debug");
            break;
        case E_CMakeConfig::RELEASE:
            OutName = ABYTEK_NAME("Release");
            break;
        case E_CMakeConfig::REL_WITH_DEB_INFO:
            OutName = ABYTEK_NAME("RelWithDebInfo");
            break;
        case E_CMakeConfig::MIN_SIZE_REL:
            OutName = ABYTEK_NAME("MinSizeRel");
            break;
        default:
            return F_FeedbackStatus::MakeFailed(
                ABYTEK_TEXT("Unknown config: ") 
                + ToText(static_cast<TF_TypeToUInt<E_CMakeConfig>>(Config))
            );
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus H_CMakeToolchainUtilities::ParseGenerator(const F_Name& Name, E_CMakeGenerator& OutGenerator)
    {
        if (Name == ABYTEK_NAME("VisualStudio2026"))
        {
            OutGenerator = E_CMakeGenerator::VISUAL_STUDIO_2026;
        }
        else if (Name == ABYTEK_NAME("VisualStudio2022"))
        {
            OutGenerator = E_CMakeGenerator::VISUAL_STUDIO_2022;
        }
        else if (Name == ABYTEK_NAME("Xcode"))
        {
            OutGenerator = E_CMakeGenerator::XCODE;
        }
        else if (Name == ABYTEK_NAME("Ninja"))
        {
            OutGenerator = E_CMakeGenerator::NINJA;
        }
        else if (Name == ABYTEK_NAME("UnixMakefiles"))
        {
            OutGenerator = E_CMakeGenerator::UNIX_MAKEFILES;
        }
        else
        {
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Unknown generator: ") + *Name);
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus H_CMakeToolchainUtilities::GetGeneratorName(E_CMakeGenerator Generator, F_Name& OutName)
    {
        switch (Generator)
        {
        case E_CMakeGenerator::VISUAL_STUDIO_2026:
            OutName = ABYTEK_NAME("VisualStudio2026");
            break;
        case E_CMakeGenerator::VISUAL_STUDIO_2022:
            OutName = ABYTEK_NAME("VisualStudio2022");
            break;
        case E_CMakeGenerator::XCODE:
            OutName = ABYTEK_NAME("Xcode");
            break;
        case E_CMakeGenerator::NINJA:
            OutName = ABYTEK_NAME("Ninja");
            break;
        case E_CMakeGenerator::UNIX_MAKEFILES:
            OutName = ABYTEK_NAME("UnixMakefiles");
            break;
        default:
            return F_FeedbackStatus::MakeFailed(
                ABYTEK_TEXT("Unknown generator: ") 
                + ToText(static_cast<TF_TypeToUInt<E_CMakeGenerator>>(Generator))
            );
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
}
#endif