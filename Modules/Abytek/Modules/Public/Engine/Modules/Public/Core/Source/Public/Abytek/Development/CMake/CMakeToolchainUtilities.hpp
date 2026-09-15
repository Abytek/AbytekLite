#pragma once

#include "Abytek/Engine.Core.prerequisites.hpp"
#include "Abytek/Development/Common/CommonToolchainUtilities.hpp"


#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
namespace Abytek
{
    enum class E_CMakeGenerator : U8
    {
        NONE,
        
        VISUAL_STUDIO_2022,
        VISUAL_STUDIO_2026,
        NINJA,
        XCODE,
        UNIX_MAKEFILES,
        
#ifdef ABYTEK_EBT_CMAKE_GENERATOR_VISUAL_STUDIO_2026
        CURRENT = VISUAL_STUDIO_2026,
#elif defined(ABYTEK_EBT_CMAKE_GENERATOR_VISUAL_STUDIO_2022)
        CURRENT = VISUAL_STUDIO_2022,
#elif defined(ABYTEK_EBT_CMAKE_GENERATOR_NINJA)
        CURRENT = NINJA,
#elif defined(ABYTEK_EBT_CMAKE_GENERATOR_XCODE)
        CURRENT = XCODE,
#elif defined(ABYTEK_EBT_CMAKE_GENERATOR_UNIX_MAKEFILES)
        CURRENT = UNIX_MAKEFILES,
#else
#error "Unknown config"
#endif
    };
    enum class E_CMakeConfig : U8
    {
        NONE,
        
        AUTO,
        DEBUG,
        RELEASE,
        REL_WITH_DEB_INFO,
        MIN_SIZE_REL,
        
#ifdef ABYTEK_EBT_CMAKE_CONFIG_AUTO
        CURRENT = AUTO,
#elif defined(ABYTEK_EBT_CMAKE_CONFIG_DEBUG)
        CURRENT = DEBUG,
#elif defined(ABYTEK_EBT_CMAKE_CONFIG_RELEASE)
        CURRENT = RELEASE,
#elif defined(ABYTEK_EBT_CMAKE_CONFIG_REL_WITH_DEB_INFO)
        CURRENT = REL_WITH_DEB_INFO,
#elif defined(ABYTEK_EBT_CMAKE_CONFIG_MIN_SIZE_REL)
        CURRENT = MIN_SIZE_REL,
#else
#error "Unknown config"
#endif
    };
    
    struct ABYTEK_ENGINE_CORE_API H_CMakeToolchainUtilities
    {
        static F_FeedbackStatus ParseConfig(const F_Name& Name, E_CMakeConfig& OutConfig);
        static F_FeedbackStatus GetConfigName(E_CMakeConfig Config, F_Name& OutName);
        static F_FeedbackStatus ParseGenerator(const F_Name& Name, E_CMakeGenerator& OutGenerator);
        static F_FeedbackStatus GetGeneratorName(E_CMakeGenerator Generator, F_Name& OutName);
    };
}
#endif