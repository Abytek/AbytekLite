
include(Abytek/Utilities/AddEnableAssertionsOption)
include(Abytek/Utilities/AddEnableLogOption)
include(Abytek/Utilities/AddEnableUnitTestsOption)


if(ABYTEK_MODULE_PHASE_INIT)
    set(Engine.MacroName ABYTEK_ENGINE)

    option(ABYTEK_ENGINE_BUILD_SHARED "" OFF)
    if(ABYTEK_ENGINE_BUILD_SHARED)
        set(Engine.Type SHARED)
        set(Engine.Modules.DefaultType SHARED)
    endif()

    if(ABYTEK_EBT_EXTERNAL_MODE)
        set(Engine.Enable OFF)
    else()
        set(Engine.Enable ON)
    endif()
endif()

if(ABYTEK_MODULE_PHASE_GLOBAL_SHARE)
    # test
    include(Abytek/AddEnableUnitTestsOptionForCurrentModule)

    # assert
    include(Abytek/AddEnableAssertionsOptionForCurrentModule)
endif()
