include(Abytek/Utilities/AddEnableAssertionsOption)
include(Abytek/Utilities/AddEnableLogOption)
include(Abytek/Utilities/AddEnableUnitTestsOption)
include(Abytek/Utilities/SetGlobal)

if(ABYTEK_MODULE_PHASE_INIT)
    set(Serializable.MacroName ABYTEK_BASE_SERIALIZABLE)
endif()

if(ABYTEK_MODULE_PHASE_GLOBAL_SHARE)
    # test
    include(Abytek/AddEnableUnitTestsOptionForCurrentModule)

    # assert
    include(Abytek/AddEnableAssertionsOptionForCurrentModule)
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    list(APPEND Serializable.PublicDependencies
        Core
        Object
        Reflection
        Concurrency
    )
endif()
