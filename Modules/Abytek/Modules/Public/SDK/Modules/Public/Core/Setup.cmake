
include(Abytek/Utilities/AddEnableAssertionsOption)
include(Abytek/Utilities/AddEnableLogOption)
include(Abytek/Utilities/AddEnableUnitTestsOption)


if(ABYTEK_MODULE_PHASE_INIT)
    set(Core.MacroName ABYTEK_SDK_CORE)
endif()

if(ABYTEK_MODULE_PHASE_GLOBAL_SHARE)
    # Configure unit testing support
    include(Abytek/AddEnableUnitTestsOptionForCurrentModule)

    # Configure assertion support
    include(Abytek/AddEnableAssertionsOptionForCurrentModule)
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    list(APPEND Core.PublicDependencies
        Base
    )
endif()
