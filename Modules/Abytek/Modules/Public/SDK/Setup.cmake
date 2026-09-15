
include(Abytek/Utilities/AddEnableAssertionsOption)
include(Abytek/Utilities/AddEnableLogOption)
include(Abytek/Utilities/AddEnableUnitTestsOption)


if(ABYTEK_MODULE_PHASE_INIT)
    set(SDK.MacroName ABYTEK_SDK)

    option(ABYTEK_SDK_BUILD_SHARED "" OFF)
    if(ABYTEK_SDK_BUILD_SHARED)
        set(SDK.Type SHARED)
        set(SDK.Modules.DefaultType SHARED)
    endif()
endif()

if(ABYTEK_MODULE_PHASE_GLOBAL_SHARE)
    # test
    include(Abytek/AddEnableUnitTestsOptionForCurrentModule)

    # assert
    include(Abytek/AddEnableAssertionsOptionForCurrentModule)
endif()
