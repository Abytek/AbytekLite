
include(Abytek/Utilities/AddEnableAssertionsOption)
include(Abytek/Utilities/AddEnableLogOption)
include(Abytek/Utilities/AddEnableUnitTestsOption)


if(ABYTEK_MODULE_PHASE_INIT)
    set(Base.MacroName ABYTEK_BASE)

    option(ABYTEK_BASE_BUILD_SHARED "" OFF)
    if(ABYTEK_BASE_BUILD_SHARED)
        set(Base.Type SHARED)
        set(Base.Modules.DefaultType SHARED)
    endif()
endif()

if(ABYTEK_MODULE_PHASE_GLOBAL_SHARE)
    # test
    include(Abytek/AddEnableUnitTestsOptionForCurrentModule)

    # assert
    include(Abytek/AddEnableAssertionsOptionForCurrentModule)
endif()
