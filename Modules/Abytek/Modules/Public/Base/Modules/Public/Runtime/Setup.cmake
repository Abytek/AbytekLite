
include(Abytek/Utilities/AddEnableAssertionsOption)
include(Abytek/Utilities/AddEnableLogOption)
include(Abytek/Utilities/AddEnableUnitTestsOption)
include(Abytek/Utilities/AddResolveReflectorInline)


if(ABYTEK_MODULE_PHASE_INIT)
    set(Runtime.MacroName ABYTEK_BASE_RUNTIME)
endif()

if(ABYTEK_MODULE_PHASE_GLOBAL_SHARE)
    # test
    include(Abytek/AddEnableUnitTestsOptionForCurrentModule)

    # assert
    include(Abytek/AddEnableAssertionsOptionForCurrentModule)
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    list(APPEND Runtime.PublicDependencies
        Minimal
        Core
        Object
        Reflection
        Platform
        Console
        Profiling
        Task
        Concurrency
        Serializable
        Math
    )
endif()