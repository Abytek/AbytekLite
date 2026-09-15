include(Abytek/Utilities/AddEnableAssertionsOption)
include(Abytek/Utilities/AddEnableLogOption)
include(Abytek/Utilities/AddEnableUnitTestsOption)
include(Abytek/Utilities/SetGlobal)

if(ABYTEK_MODULE_PHASE_INIT)
    set(Platform.MacroName ABYTEK_BASE_PLATFORM)

    if(WIN32)
        Abytek_SetGlobal(ABYTEK_BASE_PLATFORM_ENABLE_FILE_SYSTEM ON)
        Abytek_SetGlobal(ABYTEK_BASE_PLATFORM_ENABLE_LOW_LEVEL_MEMORY ON)
        Abytek_SetGlobal(ABYTEK_BASE_PLATFORM_ENABLE_IPC ON)
        Abytek_SetGlobal(ABYTEK_BASE_PLATFORM_ENABLE_IPC_PIPE ON)
    elseif(EMSCRIPTEN)
        Abytek_SetGlobal(ABYTEK_BASE_PLATFORM_ENABLE_FILE_SYSTEM ON)
        Abytek_SetGlobal(ABYTEK_BASE_PLATFORM_ENABLE_LOW_LEVEL_MEMORY OFF)
        Abytek_SetGlobal(ABYTEK_BASE_PLATFORM_ENABLE_IPC OFF)
        Abytek_SetGlobal(ABYTEK_BASE_PLATFORM_ENABLE_IPC_PIPE OFF)
    else()
        Abytek_SetGlobal(ABYTEK_BASE_PLATFORM_ENABLE_FILE_SYSTEM ON)
        Abytek_SetGlobal(ABYTEK_BASE_PLATFORM_ENABLE_LOW_LEVEL_MEMORY OFF)
        Abytek_SetGlobal(ABYTEK_BASE_PLATFORM_ENABLE_IPC ON)
        Abytek_SetGlobal(ABYTEK_BASE_PLATFORM_ENABLE_IPC_PIPE OFF)
    endif()
endif()

if(ABYTEK_MODULE_PHASE_GLOBAL_SHARE)
    # test
    include(Abytek/AddEnableUnitTestsOptionForCurrentModule)

    # assert
    include(Abytek/AddEnableAssertionsOptionForCurrentModule)
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    list(APPEND Platform.PublicDependencies
        Core
        Object
    )

    if(WIN32)
        target_link_libraries(${CurrentTarget}
            PRIVATE
                shlwapi.lib
                advapi32.lib
                ole32.lib
        )
    endif()
endif()
