#pragma once

#include "Abytek/Engine.Core.prerequisites.hpp"


#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
namespace Abytek
{
    enum class E_Platform : U16
    {
        NONE = 0x0,
        
        WINDOWS = 0x1,
        POSIX = 0x2,
        ANDROID_ = 0x4 | POSIX,

#ifdef ABYTEK_PLATFORM_WINDOWS
        CURRENT = WINDOWS,
#elif defined(ABYTEK_PLATFORM_ANDROID)
        CURRENT = ANDROID_,
#else
#error "Unknown platform tag"
#endif
    };
    enum class E_ProcessorArch : U8
    {
        NONE,
        
        AUTO,
        X86_32,
        X86_64,
        ARM_32,
        ARM_64,
        WASM,
        
#ifdef ABYTEK_EBT_PROCESSOR_ARCH_AUTO
        CURRENT = AUTO,
#elif defined(ABYTEK_EBT_PROCESSOR_ARCH_X86_32)
        CURRENT = X86_32,
#elif defined(ABYTEK_EBT_PROCESSOR_ARCH_X86_64)
        CURRENT = X86_64,
#elif defined(ABYTEK_EBT_PROCESSOR_ARCH_ARM_32)
        CURRENT = ARM_32,
#elif defined(ABYTEK_EBT_PROCESSOR_ARCH_ARM_64)
        CURRENT = ARM_64,
#elif defined(ABYTEK_EBT_PROCESSOR_ARCH_WASM)
        CURRENT = WASM,
#else
#error "Unknown processor arch"
#endif
    };
    
    struct ABYTEK_ENGINE_CORE_API H_CommonToolchainUtilities
    {
        static F_FeedbackStatus ParsePlatform(const F_Name& Name, E_Platform& OutPlatform);
        static F_FeedbackStatus GetPlatformName(E_Platform Platform, F_Name& OutName);
        static F_FeedbackStatus ParseProcessorArch(const F_Name& Name, E_ProcessorArch& OutProcessorArch);
        static F_FeedbackStatus GetProcessorArchName(E_ProcessorArch ProcessorArch, F_Name& OutName);
    };
}
#endif