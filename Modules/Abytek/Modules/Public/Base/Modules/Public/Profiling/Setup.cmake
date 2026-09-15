
include(Abytek/Utilities/AddEnableAssertionsOption)
include(Abytek/Utilities/AddEnableLogOption)
include(Abytek/Utilities/AddEnableUnitTestsOption)
include(Abytek/Utilities/AddResolveReflectorInline)


function(Abytek_Base_Profiling_ImportTracy)
    message(STATUS "Importing Tracy")
    if(MSVC)
        add_compile_definitions(
            CONTROLTRACE_ID=TRACEHANDLE
            PROCESSTRACE_HANDLE=TRACEHANDLE
        )
    endif()
    if(ABYTEK_ENABLE_PROFILER)
        ABYTEK_SetGlobal(TRACY_ENABLE ON)
        ABYTEK_SetGlobal(TRACY_FIBERS ON)
    else()
        ABYTEK_SetGlobal(TRACY_ENABLE OFF)
        ABYTEK_SetGlobal(TRACY_FIBERS OFF)
    endif()

    # add_subdirectory("${ABYTEK_DEPENDENCIES_DIR}/tracy" "${ABYTEK_DEPENDENCIES_DIR}/tracy/Build")
    add_subdirectory("${ABYTEK_TRACY_DIR}" "${ABYTEK_DEPENDENCIES_DIR}/tracy/Build")

    if(WIN32)
        # add_subdirectory("${ABYTEK_DEPENDENCIES_DIR}/tracy/profiler" "${ABYTEK_DEPENDENCIES_DIR}/tracy/Build/profiler")
        add_subdirectory("${ABYTEK_TRACY_DIR}/profiler" "${ABYTEK_DEPENDENCIES_DIR}/tracy/Build/profiler")
        # add_dependencies(${CurrentTarget}
        #     tracy-profiler
        # )
    endif()

    target_link_libraries(${CurrentTarget}
        PUBLIC
            TracyClient
    )
    message(STATUS "Imported Tracy")
endfunction()


if(ABYTEK_MODULE_PHASE_INIT)
    set(Profiling.MacroName ABYTEK_BASE_PROFILING)

    option(ABYTEK_ENABLE_PROFILER "Enable profiler" ON)
    if(ABYTEK_AUTO_OPTIMIZATION_FLAGS AND ABYTEK_ENABLE_MAX_OPTIMIZATION)
        Abytek_SetGlobal(ABYTEK_ENABLE_PROFILER OFF)
    else()
        Abytek_SetGlobal(ABYTEK_ENABLE_PROFILER ${ABYTEK_ENABLE_PROFILER})
    endif()
endif()

if(ABYTEK_MODULE_PHASE_GLOBAL_SHARE)
    # test
    include(Abytek/AddEnableUnitTestsOptionForCurrentModule)

    # assert
    include(Abytek/AddEnableAssertionsOptionForCurrentModule)
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    Abytek_Base_Profiling_ImportTracy()

    list(APPEND Profiling.PublicDependencies
        Minimal
        Core
    )
endif()