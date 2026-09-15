
include(Abytek/Utilities/AddEnableAssertionsOption)
include(Abytek/Utilities/AddEnableLogOption)
include(Abytek/Utilities/AddEnableUnitTestsOption)
include(Abytek/Utilities/InstructionExtensionFlags)
include(Abytek/Utilities/ApplyTempGeneratedFile)
include(Abytek/Utilities/CommonFlags)


if(ABYTEK_MODULE_PHASE_INIT)
    set(Minimal.MacroName ABYTEK_BASE_MINIMAL)

    option(ABYTEK_DISABLE_THREADS "Disable threads" OFF)
    option(ABYTEK_FORCE_DISABLE_DEBUG_INFO "Force disable debug info" OFF)
    if(ABYTEK_AUTO_OPTIMIZATION_FLAGS AND ABYTEK_ENABLE_MAX_OPTIMIZATION)
        Abytek_SetGlobal(ABYTEK_FORCE_DISABLE_DEBUG_INFO ON)
    else()
        Abytek_SetGlobal(ABYTEK_FORCE_DISABLE_DEBUG_INFO ${ABYTEK_FORCE_DISABLE_DEBUG_INFO})
    endif()
    option(ABYTEK_ENABLE_LOG_STREAM_V2 "Enable log stream V2" ON)

    # version
    include(Abytek/Utilities/VersionHelper)
    Abytek_VersionHelper_CreateVersionMacros(
        PROJECT_NAME Abytek
        OUTPUT_FILE "${Minimal.GeneratedSource.TempDirectory}/Public/Abytek/Version.hpp"
        MAJOR ${ABYTEK_VERSION_MAJOR}
        MINOR ${ABYTEK_VERSION_MINOR}
        PATCH ${ABYTEK_VERSION_PATCH}
    )
    Abytek_ApplyTempGeneratedFile(
        "${Minimal.GeneratedSource.TempDirectory}/Public/Abytek/Version.hpp"
        "${Minimal.GeneratedSource.PublicDirectory}/Abytek/Version.hpp"
    )

    # test
    Abytek_SetGlobal(ABYTEK_UNIT_TEST_DEFINES_HPP_FILE "${Minimal.GeneratedSource.PublicDirectory}/Abytek/Base.Minimal.UnitTestDefines.hpp")
    Abytek_SetGlobal(ABYTEK_UNIT_TEST_DEFINES_HPP_TEMP_FILE "${Minimal.GeneratedSource.TempDirectory}/Public/Abytek/Base.Minimal.UnitTestDefines.hpp")
    file(WRITE "${ABYTEK_UNIT_TEST_DEFINES_HPP_TEMP_FILE}" "#pragma once\n")
    file(APPEND "${ABYTEK_UNIT_TEST_DEFINES_HPP_TEMP_FILE}" "#include <Abytek/Base.Minimal.BaseAssertions.hpp>\n")
    include(Abytek/AddEnableUnitTestsOptionForCurrentModule)

    # log
    Abytek_SetGlobal(ABYTEK_LOG_DEFINES_HPP_FILE "${Minimal.GeneratedSource.PublicDirectory}/Abytek/Base.Minimal.LogDefines.hpp")
    Abytek_SetGlobal(ABYTEK_LOG_DEFINES_HPP_TEMP_FILE "${Minimal.GeneratedSource.TempDirectory}/Public/Abytek/Base.Minimal.LogDefines.hpp")
    file(WRITE "${ABYTEK_LOG_DEFINES_HPP_TEMP_FILE}" "")
    file(APPEND "${ABYTEK_LOG_DEFINES_HPP_TEMP_FILE}" "#include <Abytek/Base.Minimal.BaseLog.hpp>\n")
    option(ABYTEK_DISABLE_ALL_LOG "" OFF)
    option(ABYTEK_ENABLE_ALL_LOG "" OFF)
    option(ABYTEK_ENABLE_LOG_COLORS "" ON)
    Abytek_AddEnableLogOption(ABYTEK_ _FATAL Abytek::E_LogType::LT_FATAL "" ON)
    Abytek_AddEnableLogOption(ABYTEK_ _ERROR Abytek::E_LogType::LT_ERROR "" ON)
    Abytek_AddEnableLogOption(ABYTEK_ _WARNING Abytek::E_LogType::LT_WARNING "" ON)
    Abytek_AddEnableLogOption(ABYTEK_ _INFO Abytek::E_LogType::LT_INFO "" ON)
    Abytek_AddEnableLogOption(ABYTEK_ _VERBOSE Abytek::E_LogType::LT_VERBOSE "" ON)
    Abytek_AddEnableLogOption(ABYTEK_ _VERY_VERBOSE Abytek::E_LogType::LT_VERY_VERBOSE "" ON)

    # assert
    Abytek_SetGlobal(ABYTEK_ASSERTION_DEFINES_HPP_FILE "${Minimal.GeneratedSource.PublicDirectory}/Abytek/Base.Minimal.AssertionDefines.hpp")
    Abytek_SetGlobal(ABYTEK_ASSERTION_DEFINES_HPP_TEMP_FILE "${Minimal.GeneratedSource.TempDirectory}/Public/Abytek/Base.Minimal.AssertionDefines.hpp")
    file(WRITE "${ABYTEK_ASSERTION_DEFINES_HPP_TEMP_FILE}" "#pragma once\n")
    option(ABYTEK_DISABLE_ALL_ASSERTIONS "" OFF)
    option(ABYTEK_ENABLE_ALL_ASSERTIONS "" OFF)
    include(Abytek/AddEnableAssertionsOptionForCurrentModule)
    Abytek_AddEnableAssertionsOption(ABYTEK_ "_GENERAL" "" ON)

    # platform specific instruction extension options
    if(
        ABYTEK_EBT_PROCESSOR_ARCH_X86_32 
        OR ABYTEK_EBT_PROCESSOR_ARCH_X86_64
    )
        Abytek_SetGlobal(ABYTEK_ENABLE_SIMD_X86_DEFAULT ON)
    else()
        Abytek_SetGlobal(ABYTEK_ENABLE_SIMD_X86_DEFAULT OFF)
    endif()
    option(ABYTEK_ENABLE_SSE "Enable SSE instructions" ${ABYTEK_ENABLE_SIMD_X86_DEFAULT})
    option(ABYTEK_ENABLE_SSE2 "Enable SSE2 instructions" ${ABYTEK_ENABLE_SIMD_X86_DEFAULT})
    option(ABYTEK_ENABLE_SSE3 "Enable SSE3 instructions" ${ABYTEK_ENABLE_SIMD_X86_DEFAULT})
    option(ABYTEK_ENABLE_SSE4 "Enable SSE4 instructions" ${ABYTEK_ENABLE_SIMD_X86_DEFAULT})
    option(ABYTEK_ENABLE_AVX "Enable AVX instructions" ${ABYTEK_ENABLE_SIMD_X86_DEFAULT})
    option(ABYTEK_ENABLE_AVX2 "Enable AVX2 instructions" ${ABYTEK_ENABLE_SIMD_X86_DEFAULT})
    option(ABYTEK_ENABLE_FMA3 "Enable FMA3 instructions" ${ABYTEK_ENABLE_SIMD_X86_DEFAULT})

    if(
        ABYTEK_EBT_PROCESSOR_ARCH_ARM_32 
        OR ABYTEK_EBT_PROCESSOR_ARCH_ARM_64
    )
        Abytek_SetGlobal(ABYTEK_ENABLE_SIMD_ARM_DEFAULT ON)
    else()
        Abytek_SetGlobal(ABYTEK_ENABLE_SIMD_ARM_DEFAULT OFF)
    endif()
    option(ABYTEK_ENABLE_ARM_NEON "Enable ARM Neon instructions" ${ABYTEK_ENABLE_SIMD_ARM_DEFAULT})

    # Setup dependent instruction extensions
    if(ABYTEK_ENABLE_AVX2)
        set(ABYTEK_ENABLE_AVX ON)
    endif()
    if(ABYTEK_ENABLE_AVX)
        set(ABYTEK_ENABLE_SSE4 ON)
    endif()
    if(ABYTEK_ENABLE_FMA3)
        set(ABYTEK_ENABLE_SSE4 ON)
    endif()
    if(ABYTEK_ENABLE_SSE4)
        set(ABYTEK_ENABLE_SSE3 ON)
    endif()
    if(ABYTEK_ENABLE_SSE3)
        set(ABYTEK_ENABLE_SSE2 ON)
    endif()
    if(ABYTEK_ENABLE_SSE2)
        set(ABYTEK_ENABLE_SSE ON)
    endif()

    # Add natvis files for boost
    set(ABYTEK_ENABLE_BOOST_NATVIS ON)
    if (ABYTEK_ENABLE_BOOST_NATVIS)
        if(MSVC)
            file(
                GLOB_RECURSE
                ${CurrentTarget}.BoostNatvisFiles_MSVC
                "${cpp_debugger_visualizers_SOURCE_DIR}/VS2019/Visualizers/*.natvis"
            )
            list(APPEND Minimal.AdditionalSourceFiles ${${CurrentTarget}.BoostNatvisFiles_MSVC})
        endif()
    endif()

    #
    set(ABYTEK_TEMPLATE_TRICKS_FILE "${Minimal.GeneratedSource.PublicDirectory}/Abytek/TemplateTricks.hpp")
    set(ABYTEK_TEMPLATE_TRICKS_TEMP_FILE "${Minimal.GeneratedSource.TempDirectory}/Public/Abytek/TemplateTricks.hpp")
    include(Abytek/TemplateTricks)
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    if(MSVC)
        target_compile_options(${CurrentTarget}
            PUBLIC 
                /Zc:preprocessor 
                /GT
        )
    endif()

    # instruction extensions
    if(ABYTEK_ENABLE_SSE)
        target_compile_options(${CurrentTarget} PUBLIC ${ABYTEK_COMPILER_FLAG_SSE})
    endif()
    if(ABYTEK_ENABLE_SSE2)
        target_compile_options(${CurrentTarget} PUBLIC ${ABYTEK_COMPILER_FLAG_SSE2})
    endif()
    if(ABYTEK_ENABLE_SSE3)
        target_compile_options(${CurrentTarget} PUBLIC ${ABYTEK_COMPILER_FLAG_SSE3})
    endif()
    if(ABYTEK_ENABLE_SSE4)
        target_compile_options(${CurrentTarget} PUBLIC ${ABYTEK_COMPILER_FLAG_SSE4})
    endif()
    if(ABYTEK_ENABLE_AVX)
        target_compile_options(${CurrentTarget} PUBLIC ${ABYTEK_COMPILER_FLAG_AVX})
    endif()
    if(ABYTEK_ENABLE_AVX2)
        target_compile_options(${CurrentTarget} PUBLIC ${ABYTEK_COMPILER_FLAG_AVX2})
    endif()
    if(ABYTEK_ENABLE_FMA3)
        target_compile_options(${CurrentTarget} PUBLIC ${ABYTEK_COMPILER_FLAG_FMA3})
    endif()
    if(ABYTEK_ENABLE_ARM_NEON)
        target_compile_options(${CurrentTarget} PUBLIC ${ABYTEK_COMPILER_FLAG_ARM_NEON})
    endif()

    list(APPEND Minimal.PublicDependencies
        boost_core
        boost_any
        boost_system
        boost_atomic
        boost_optional
        boost_unordered
        boost_random
        boost_iostreams
        boost_container
        boost_chrono
        boost_json
        boost_locale
        boost_context
    )
    # if(ANDROID)
    # else()
    #     list(APPEND Minimal.PublicDependencies
    #         boost_process
    #     )
    #     target_compile_definitions(${CurrentTarget}
    #         PUBLIC
    #             -DABYTEK_ENABLE_BOOST_PROCESS
    #     )
    # endif()

    if(ABYTEK_ENABLE_LOG_STREAM_V2)
        target_compile_definitions(${CurrentTarget}
            PUBLIC
                -DABYTEK_ENABLE_LOG_STREAM_V2
        )
    endif()

    if(ABYTEK_ENABLE_WIDE_CHAR_TEXT)
        target_compile_definitions(${CurrentTarget}
            PUBLIC
                -DUNICODE -D_UNICODE
        )
    else()
        if(WIN32)
            message(FATAL_ERROR "Wide-char text is required on Windows platforms")
        endif()
    endif()

    # Enable/disable threads
    if(NOT ABYTEK_DISABLE_THREADS)
        list(APPEND Minimal.PublicDependencies
            boost_thread
        )
        if(EMSCRIPTEN)
            target_compile_options(boost_thread
                PUBLIC
                    -pthread
            )
        endif()
    else()
        target_compile_definitions(boost_thread
            PUBLIC
                -DBOOST_DISABLE_THREADS
        )
        target_compile_definitions(${CurrentTarget}
            PUBLIC
                -DABYTEK_DISABLE_THREADS
        )
    endif()

    target_compile_definitions(${CurrentTarget}
        PUBLIC
            $<$<CONFIG:Debug>:ABYTEK_INTERNAL_CONFIG_DEBUG>
            $<$<CONFIG:RelWithDebInfo>:ABYTEK_INTERNAL_CONFIG_REL_WITH_DEB_INFO>
    )

    if (ANDROID)
        target_link_libraries(${CurrentTarget}
            PUBLIC
                # The game activity
                game-activity::game-activity_static
                jnigraphics
                android
                log
        )
    endif()

    if(WIN32)
        target_compile_definitions(${ModuleFullName}
            PUBLIC 
                NOMINMAX
                WIN32_LEAN_AND_MEAN
        )
    endif()

    target_include_directories(${ModuleFullName}
        PUBLIC 
            "${ABYTEK_EBT_GENERATED_SOURCE_DIRECTORY}"    
    )
endif()

if(ABYTEK_MODULE_PHASE_LATE_SETUP)
    Abytek_ApplyTempGeneratedFile(
        "${ABYTEK_UNIT_TEST_DEFINES_HPP_TEMP_FILE}"
        "${ABYTEK_UNIT_TEST_DEFINES_HPP_FILE}"
    )
    Abytek_ApplyTempGeneratedFile(
        "${ABYTEK_LOG_DEFINES_HPP_TEMP_FILE}"
        "${ABYTEK_LOG_DEFINES_HPP_FILE}"
    )
    Abytek_ApplyTempGeneratedFile(
        "${ABYTEK_ASSERTION_DEFINES_HPP_TEMP_FILE}"
        "${ABYTEK_ASSERTION_DEFINES_HPP_FILE}"
    )
endif()

