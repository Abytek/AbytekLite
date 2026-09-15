
include(Abytek/Utilities/AddEnableAssertionsOption)
include(Abytek/Utilities/AddEnableLogOption)
include(Abytek/Utilities/AddEnableUnitTestsOption)


function(Abytek_PureDOD_CheckFlecs)
    if(ABYTEK_AUTO_OPTIMIZATION_FLAGS AND ABYTEK_ENABLE_MAX_OPTIMIZATION)
        Abytek_SetGlobal(FLECS_REST OFF)
    else()
        Abytek_SetGlobal(FLECS_REST ON)
    endif()

    include(FetchContent)
    FetchContent_Declare(
        flecs
        GIT_REPOSITORY https://github.com/SanderMertens/flecs.git
        GIT_TAG 1e19bae2d3207926092c96b3ce4869cb2f3ee132
        GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(flecs)
endfunction()


if(ABYTEK_MODULE_PHASE_INIT)
    set(PureDOD.MacroName ABYTEK_BASE_PURE_DOD)

    Abytek_PureDOD_CheckFlecs()
endif()

if(ABYTEK_MODULE_PHASE_GLOBAL_SHARE)
    # test
    include(Abytek/AddEnableUnitTestsOptionForCurrentModule)

    # assert
    include(Abytek/AddEnableAssertionsOptionForCurrentModule)
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    list(APPEND PureDOD.PublicDependencies
        Minimal
        Core
        Object
    )

    target_link_libraries(${ModuleFullName}
        PUBLIC 
            flecs::flecs_static
    )
endif()
