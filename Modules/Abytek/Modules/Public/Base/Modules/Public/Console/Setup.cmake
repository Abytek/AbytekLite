
include(Abytek/Utilities/AddEnableAssertionsOption)
include(Abytek/Utilities/AddEnableLogOption)
include(Abytek/Utilities/AddEnableUnitTestsOption)
include(Abytek/Utilities/AddResolveReflectorInline)


function(Abytek_Base_Console_CheckCXXOpts)
    message(STATUS "Checking cxxopts")
    if(NOT EXISTS "${ABYTEK_DEPENDENCIES_DIR}/cxxopts")
        Abytek_GitHelper_Clone(
            PROJECT_NAME "cxxopts"
            GIT_URL "https://github.com/jarro2783/cxxopts"
            GIT_COMMIT "929bf2de375dba6f3a24a3a8fe681389b2b58a5b"
            GIT_BRANCH "master"
            DIRECTORY "${ABYTEK_DEPENDENCIES_DIR}"
            UPDATE_SUBMODULES
        )
    endif()
    message(STATUS "Checked cxxopts")
endfunction()
function(Abytek_Base_Console_ImportCXXOpts)
    message(STATUS "Importing cxxopts")
    add_subdirectory("${ABYTEK_DEPENDENCIES_DIR}/cxxopts" "${ABYTEK_DEPENDENCIES_DIR}/cxxopts/Build")
    target_link_libraries(${CurrentTarget}
        PUBLIC
            cxxopts
    )
    message(STATUS "Imported cxxopts")
endfunction()

function(Abytek_Base_Console_ImportTOMLPlusPlus)
    message(STATUS "Importing TOML++")
    FetchContent_Declare(
        tomlplusplus
        GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
        GIT_TAG        v3.4.0
        GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(tomlplusplus)
    target_link_libraries(${CurrentTarget}
        PUBLIC
            tomlplusplus::tomlplusplus
    )
    message(STATUS "Imported TOML++")
endfunction()


if(ABYTEK_MODULE_PHASE_INIT)
    set(Console.MacroName ABYTEK_BASE_CONSOLE)

    Abytek_Base_Console_CheckCXXOpts()
endif()

if(ABYTEK_MODULE_PHASE_GLOBAL_SHARE)
    # test
    include(Abytek/AddEnableUnitTestsOptionForCurrentModule)

    # assert
    include(Abytek/AddEnableAssertionsOptionForCurrentModule)
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    Abytek_Base_Console_ImportCXXOpts()
    Abytek_Base_Console_ImportTOMLPlusPlus()

    list(APPEND Console.PublicDependencies
        Minimal
        Core
        Object
        Reflection
        Platform
    )
endif()