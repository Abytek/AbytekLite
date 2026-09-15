#include "Abytek/Development/Common/CommonToolchainUtilities.hpp"


#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
namespace Abytek
{
    F_FeedbackStatus H_CommonToolchainUtilities::ParsePlatform(const F_Name& Name, E_Platform& OutPlatform)
    {
        if (Name == ABYTEK_NAME("Windows"))
        {
            OutPlatform = E_Platform::WINDOWS;
        }
        else if (Name == ABYTEK_TEXT("Android"))
        {
            OutPlatform = E_Platform::ANDROID_;
        }
        else
        {
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Unknown platform: ") + *Name);
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus H_CommonToolchainUtilities::GetPlatformName(E_Platform Platform, F_Name& OutName)
    {
        if (Platform == E_Platform::WINDOWS)
        {
            OutName = ABYTEK_NAME("Windows");
        }
        else if (Platform == E_Platform::ANDROID_)
        {
            OutName = ABYTEK_NAME("Android");
        }
        else 
        {
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Unknown platform"));
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus H_CommonToolchainUtilities::ParseProcessorArch(const F_Name& Name, E_ProcessorArch& OutProcessorArch)
    {
        if (Name == ABYTEK_NAME("Auto"))
        {
            OutProcessorArch = E_ProcessorArch::AUTO;
        }
        else if (Name == ABYTEK_NAME("X86_32"))
        {
            OutProcessorArch = E_ProcessorArch::X86_32;
        }
        else if (Name == ABYTEK_NAME("X86_64"))
        {
            OutProcessorArch = E_ProcessorArch::X86_64;
        }
        else if (Name == ABYTEK_NAME("ARM_32"))
        {
            OutProcessorArch = E_ProcessorArch::ARM_32;
        }
        else if (Name == ABYTEK_NAME("ARM_64"))
        {
            OutProcessorArch = E_ProcessorArch::ARM_64;
        }
        else if (Name == ABYTEK_NAME("WASM"))
        {
            OutProcessorArch = E_ProcessorArch::WASM;
        }
        else
        {
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Unknown config: ") + *Name);
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus H_CommonToolchainUtilities::GetProcessorArchName(E_ProcessorArch ProcessorArch, F_Name& OutName)
    {
        switch (ProcessorArch)
        {
        case E_ProcessorArch::AUTO:
            OutName = ABYTEK_NAME("Auto");
            break;
        case E_ProcessorArch::X86_32:
            OutName = ABYTEK_NAME("X86_32");
            break;
        case E_ProcessorArch::X86_64:
            OutName = ABYTEK_NAME("X86_64");
            break;
        case E_ProcessorArch::ARM_32:
            OutName = ABYTEK_NAME("ARM_32");
            break;
        case E_ProcessorArch::ARM_64:
            OutName = ABYTEK_NAME("ARM_64");
            break;
        case E_ProcessorArch::WASM:
            OutName = ABYTEK_NAME("WASM");
            break;
        default:
            return F_FeedbackStatus::MakeFailed(
                ABYTEK_TEXT("Unknown processor arch: ") 
                + ToText(static_cast<TF_TypeToUInt<E_ProcessorArch>>(ProcessorArch))
            );
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
}
#endif