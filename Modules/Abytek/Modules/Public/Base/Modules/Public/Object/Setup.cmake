
include(Abytek/Utilities/AddEnableAssertionsOption)
include(Abytek/Utilities/AddEnableLogOption)
include(Abytek/Utilities/AddEnableUnitTestsOption)
include(Abytek/Utilities/AddResolveReflectorInline)



if(ABYTEK_MODULE_PHASE_INIT)
    set(Object.MacroName ABYTEK_BASE_OBJECT)
    
    option(ABYTEK_OBJECT_VALIDATION_ENABLE_FAST_MODE "" ON)
endif()

if(ABYTEK_MODULE_PHASE_GLOBAL_SHARE)
    # test
    include(Abytek/AddEnableUnitTestsOptionForCurrentModule)

    # assert
    include(Abytek/AddEnableAssertionsOptionForCurrentModule)
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    list(APPEND Object.PublicDependencies
        Minimal
        Core
    )
endif()