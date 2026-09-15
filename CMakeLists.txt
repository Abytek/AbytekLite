cmake_minimum_required(VERSION 3.14)


if(APPLE)
    project(Abytek LANGUAGES C CXX OBJC OBJCXX)
else()
    project(Abytek LANGUAGES C CXX)
endif()


set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_LIST_DIR}/CMake")

include(Abytek/Utilities/SetGlobal)
include(Abytek/Utilities/SetupModules)
include(Abytek/Utilities/ReadKeyValueFile)
include(Abytek/Utilities/TryFinalizeProject)

if(NOT DEFINED ABYTEK_EBT_EXTERNAL_MODE)
    set(ABYTEK_EBT_EXTERNAL_MODE ON)
endif()

if(ABYTEK_EBT_EXTERNAL_MODE)
    Abytek_SetGlobal(ABYTEK_ENGINE_DIR "${CMAKE_CURRENT_LIST_DIR}")
    Abytek_SetGlobal(ABYTEK_ENGINE_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}")
    Abytek_SetGlobal(ABYTEK_PROJECT_DIR "${ABYTEK_ENGINE_DIR}")
    Abytek_SetGlobal(ABYTEK_PROJECT_BUILD_DIR "${ABYTEK_ENGINE_BUILD_DIR}")

    if(NOT ABYTEK_EBT_ENVIRONMENT)
        Abytek_SetGlobal(ABYTEK_EBT_ENVIRONMENT "Cooked")
    endif()

    if(NOT ABYTEK_EBT_PLATFORM)
        if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
            set(ABYTEK_EBT_PLATFORM "Windows")
        # elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        #     set(ABYTEK_EBT_PLATFORM "Linux")
        # elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        #     set(ABYTEK_EBT_PLATFORM "macOS")
        elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")
            set(ABYTEK_EBT_PLATFORM "Android")
        # elseif(CMAKE_SYSTEM_NAME STREQUAL "iOS")
        #     set(ABYTEK_EBT_PLATFORM "IOS")
        else()
            message(FATAL_ERROR
                "Unsupported platform: ${CMAKE_SYSTEM_NAME}")
        endif()
    endif()

    if(NOT ABYTEK_EBT_CMAKE_CONFIG)
        if(CMAKE_BUILD_TYPE)
            Abytek_SetGlobal(ABYTEK_EBT_CMAKE_CONFIG "${CMAKE_BUILD_TYPE}")
        else()
            Abytek_SetGlobal(ABYTEK_EBT_CMAKE_CONFIG "Auto")
        endif()
    endif()

    if(NOT ABYTEK_EBT_CMAKE_GENERATOR)
        if(CMAKE_GENERATOR MATCHES "Ninja")
            set(ABYTEK_EBT_CMAKE_GENERATOR "Ninja")
        elseif(CMAKE_GENERATOR STREQUAL "Visual Studio 17 2022")
            set(ABYTEK_EBT_CMAKE_GENERATOR "VisualStudio2022")
        elseif(CMAKE_GENERATOR STREQUAL "Visual Studio 18 2026")
            set(ABYTEK_EBT_CMAKE_GENERATOR "VisualStudio2026")
        elseif(CMAKE_GENERATOR STREQUAL "Xcode")
            set(ABYTEK_EBT_CMAKE_GENERATOR "Xcode")
        elseif(CMAKE_GENERATOR STREQUAL "Unix Makefiles")
            set(ABYTEK_EBT_CMAKE_GENERATOR "UnixMakefiles")
        else()
            set(ABYTEK_EBT_CMAKE_GENERATOR "Unknown cmake generator ${CMAKE_GENERATOR}")
        endif()
    endif()

    if(NOT ABYTEK_EBT_CMAKE_CONFIG)
        if(CMAKE_BUILD_TYPE)
            Abytek_SetGlobal(ABYTEK_EBT_CMAKE_CONFIG "${CMAKE_BUILD_TYPE}")
        else()
            Abytek_SetGlobal(ABYTEK_EBT_CMAKE_CONFIG "Auto")
        endif()
    endif()

    if(NOT ABYTEK_EBT_PROCESSOR_ARCH)
        string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" _processor)
        if(_processor MATCHES "^(x86_64|amd64)$")
            set(ABYTEK_EBT_PROCESSOR_ARCH "X86_64")
        elseif(_processor MATCHES "^(x86|i386|i686)$")
            set(ABYTEK_EBT_PROCESSOR_ARCH "X86_32")
        elseif(_processor MATCHES "^(arm64|aarch64)$")
            set(ABYTEK_EBT_PROCESSOR_ARCH "ARM_64")
        elseif(_processor MATCHES "^arm")
            set(ABYTEK_EBT_PROCESSOR_ARCH "ARM_32")
        else()
            set(ABYTEK_EBT_PROCESSOR_ARCH "Unknown processor: ${CMAKE_SYSTEM_PROCESSOR}")
        endif()
    endif()
endif()

Abytek_SetGlobal(ABYTEK_SETUP_PRE_SCRIPTS "")

Abytek_SetGlobal(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}")

Abytek_SetGlobal(ABYTEK_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}")
Abytek_SetGlobal(ABYTEK_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}")

Abytek_SetGlobal(ABYTEK_SCRIPTS_DIR "${CMAKE_CURRENT_LIST_DIR}/Scripts")
Abytek_SetGlobal(ABYTEK_SUBMODULES_DIR "${CMAKE_CURRENT_LIST_DIR}/Submodules")
Abytek_SetGlobal(ABYTEK_ROOT_MODULES_DIR "${CMAKE_CURRENT_LIST_DIR}/Modules")
Abytek_SetGlobal(ABYTEK_DOCUMENTATION_DIR "${CMAKE_CURRENT_LIST_DIR}/Documentation")
Abytek_SetGlobal(ABYTEK_CMAKE_MODULE_DIR "${CMAKE_CURRENT_LIST_DIR}/Cmake")
Abytek_SetGlobal(ABYTEK_DEPENDENCIES_DIR "${CMAKE_CURRENT_BINARY_DIR}/Deps")
Abytek_SetGlobal(ABYTEK_GENERATED_SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/GeneratedSource")
Abytek_SetGlobal(ABYTEK_GENERATED_DOCUMENTATION_DIR "${CMAKE_CURRENT_BINARY_DIR}/Documentation")
Abytek_SetGlobal(ABYTEK_DEVELOPMENT_DATA_DIR "${CMAKE_BINARY_DIR}/Abytek/DevelopmentData")

Abytek_SetGlobal(ABYTEK_ENGINE_CMAKE_DEFINITIONS_FILE "${ABYTEK_ENGINE_DIR}/Build/CMakeDefinitions-${ABYTEK_EBT_PROJECT_SIGNATURE}.txt")
if(NOT ABYTEK_EBT_EXTERNAL_MODE)
    Abytek_ReadKeyValueFile("${ABYTEK_ENGINE_CMAKE_DEFINITIONS_FILE}")
endif()

if(ABYTEK_EBT_ENVIRONMENT STREQUAL "Development")
    Abytek_SetGlobal(ABYTEK_EBT_ENVIRONMENT_DEVELOPMENT ON)
elseif(ABYTEK_EBT_ENVIRONMENT STREQUAL "DevelopmentMaxOptimized")
    Abytek_SetGlobal(ABYTEK_EBT_ENVIRONMENT_DEVELOPMENT_MAX_OPTIMIZED ON)
elseif(ABYTEK_EBT_ENVIRONMENT STREQUAL "Cooked")
    Abytek_SetGlobal(ABYTEK_EBT_ENVIRONMENT_COOKED ON)
elseif(ABYTEK_EBT_ENVIRONMENT STREQUAL "CookedMaxOptimized")
    Abytek_SetGlobal(ABYTEK_EBT_ENVIRONMENT_COOKED_MAX_OPTIMIZED ON)
else()
    message(FATAL_ERROR "Invalid environment: ${ABYTEK_EBT_ENVIRONMENT}")
endif()

if(ABYTEK_EBT_PLATFORM STREQUAL "Windows")
    Abytek_SetGlobal(ABYTEK_EBT_PLATFORM_WINDOWS ON)
elseif(ABYTEK_EBT_PLATFORM STREQUAL "Android")
    Abytek_SetGlobal(ABYTEK_EBT_PLATFORM_ANDROID ON)
else()
    message(FATAL_ERROR "Invalid platform: ${ABYTEK_EBT_PLATFORM}")
endif()

if(ABYTEK_EBT_CMAKE_CONFIG STREQUAL "Auto")
    Abytek_SetGlobal(ABYTEK_EBT_CMAKE_CONFIG_AUTO ON)
elseif(ABYTEK_EBT_CMAKE_CONFIG STREQUAL "Debug")
    Abytek_SetGlobal(ABYTEK_EBT_CMAKE_CONFIG_DEBUG ON)
elseif(ABYTEK_EBT_CMAKE_CONFIG STREQUAL "Release")
    Abytek_SetGlobal(ABYTEK_EBT_CMAKE_CONFIG_RELEASE ON)
elseif(ABYTEK_EBT_CMAKE_CONFIG STREQUAL "RelWithDebInfo")
    Abytek_SetGlobal(ABYTEK_EBT_CMAKE_CONFIG_REL_WITH_DEB_INFO ON)
elseif(ABYTEK_EBT_CMAKE_CONFIG STREQUAL "MinSizeRel")
    Abytek_SetGlobal(ABYTEK_EBT_CMAKE_CONFIG_MIN_SIZE_REL ON)
else()
    message(FATAL_ERROR "Invalid config: ${ABYTEK_EBT_CMAKE_CONFIG}")
endif()

if(ABYTEK_EBT_CMAKE_GENERATOR STREQUAL "VisualStudio2026")
    Abytek_SetGlobal(ABYTEK_EBT_CMAKE_GENERATOR_VISUAL_STUDIO_2026 ON)
elseif(ABYTEK_EBT_CMAKE_GENERATOR STREQUAL "VisualStudio2022")
    Abytek_SetGlobal(ABYTEK_EBT_CMAKE_GENERATOR_VISUAL_STUDIO_2022 ON)
elseif(ABYTEK_EBT_CMAKE_GENERATOR STREQUAL "Ninja")
    Abytek_SetGlobal(ABYTEK_EBT_CMAKE_GENERATOR_NINJA ON)
elseif(ABYTEK_EBT_CMAKE_GENERATOR STREQUAL "Xcode")
    Abytek_SetGlobal(ABYTEK_EBT_CMAKE_GENERATOR_XCODE ON)
elseif(ABYTEK_EBT_CMAKE_GENERATOR STREQUAL "UnixMakefiles")
    Abytek_SetGlobal(ABYTEK_EBT_CMAKE_GENERATOR_UNIX_MAKEFILES ON)
else()
    message(FATAL_ERROR "Invalid generator: ${ABYTEK_EBT_CMAKE_GENERATOR}")
endif()

if(ABYTEK_EBT_PROCESSOR_ARCH STREQUAL "Auto")
    Abytek_SetGlobal(ABYTEK_EBT_PROCESSOR_ARCH_AUTO ON)
elseif(ABYTEK_EBT_PROCESSOR_ARCH STREQUAL "X86_32")
    Abytek_SetGlobal(ABYTEK_EBT_PROCESSOR_ARCH_X86_32 ON)
elseif(ABYTEK_EBT_PROCESSOR_ARCH STREQUAL "X86_64")
    Abytek_SetGlobal(ABYTEK_EBT_PROCESSOR_ARCH_X86_64 ON)
elseif(ABYTEK_EBT_PROCESSOR_ARCH STREQUAL "ARM_32")
    Abytek_SetGlobal(ABYTEK_EBT_PROCESSOR_ARCH_ARM_32 ON)
elseif(ABYTEK_EBT_PROCESSOR_ARCH STREQUAL "ARM_64")
    Abytek_SetGlobal(ABYTEK_EBT_PROCESSOR_ARCH_ARM_64 ON)
elseif(ABYTEK_EBT_PROCESSOR_ARCH STREQUAL "WASM")
    Abytek_SetGlobal(ABYTEK_EBT_PROCESSOR_ARCH_WASM ON)
else()
    message(FATAL_ERROR "Invalid processor arch: ${ABYTEK_EBT_PROCESSOR_ARCH}")
endif()

set(FETCHCONTENT_BASE_DIR "${ABYTEK_DEPENDENCIES_DIR}")



include(Abytek/SetupIDE)
include(Abytek/SetupCompiler)
include(Abytek/SetupDependencies)



set(ABYTEK_VERSION_MAJOR 0)
set(ABYTEK_VERSION_MINOR 0)
set(ABYTEK_VERSION_PATCH 0)



option(ABYTEK_ENABLE_OPERATOR "" OFF)
option(ABYTEK_ENABLE_ANODE "" OFF)

if(ABYTEK_ENABLE_OPERATOR OR ABYTEK_ENABLE_ANODE)
    Abytek_SetGlobal(ABYTEK_ENABLE_STANDALONE OFF)
else()
    Abytek_SetGlobal(ABYTEK_ENABLE_STANDALONE ON)
endif()

if(ABYTEK_ENABLE_OPERATOR AND ABYTEK_ENABLE_ANODE)
    message(FATAL_ERROR "Cannot enable Operator and ANode simultaneously")
endif()

option(ABYTEK_ENABLE_EDITOR "" ON)
if(NOT ABYTEK_ENABLE_STANDALONE)
    Abytek_SetGlobal(ABYTEK_ENABLE_EDITOR OFF)
endif()

file(
    WRITE 
    "${ABYTEK_GLOBAL_MODULES_INL_FILE}"
    ""
)
Abytek_SetupModules_MultiplePhases(
    Abytek 
    "${ABYTEK_ROOT_MODULES_DIR}"
)

if(EXISTS "${ABYTEK_ENGINE_BUILD_DIR}/AbytekEBT/AddPackages.cmake")
    include("${ABYTEK_ENGINE_BUILD_DIR}/AbytekEBT/AddPackages.cmake")
endif()

Abytek_TryFinalizeProject()