
include(Abytek/Utilities/AddEnableAssertionsOption)
include(Abytek/Utilities/AddEnableLogOption)
include(Abytek/Utilities/AddEnableUnitTestsOption)


if(ABYTEK_MODULE_PHASE_INIT)
    set(MLPDemo.MacroName ABYTEK_SANDBOX_MLP_DEMO)
    set(MLPDemo.Enable ${Abytek.Engine.Enable})
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    list(APPEND MLPDemo.PublicDependencies
        Engine
        MLUtilities
    )
endif()
