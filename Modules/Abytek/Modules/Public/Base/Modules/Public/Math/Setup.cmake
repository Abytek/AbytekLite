
include(Abytek/Utilities/AddEnableAssertionsOption)
include(Abytek/Utilities/AddEnableLogOption)
include(Abytek/Utilities/AddEnableUnitTestsOption)


if(ABYTEK_MODULE_PHASE_INIT)
    set(Math.MacroName ABYTEK_BASE_MATH)
    set(ABYTEK_DEFAULT_FP_TYPE float CACHE STRING "Default floating point type")
    set(ABYTEK_DEFAULT_FP_TOLERANCE 0.00001 CACHE STRING "Default floating point type")
endif()

if(ABYTEK_MODULE_PHASE_GLOBAL_SHARE)
    # test
    include(Abytek/AddEnableUnitTestsOptionForCurrentModule)

    # assert
    include(Abytek/AddEnableAssertionsOptionForCurrentModule)
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    list(APPEND Math.PublicDependencies
        Minimal
        Core
        Reflection
        Serializable
    )
endif()
