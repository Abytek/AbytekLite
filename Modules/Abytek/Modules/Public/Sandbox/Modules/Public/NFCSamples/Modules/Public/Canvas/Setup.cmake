
include(Abytek/Utilities/AddEnableAssertionsOption)
include(Abytek/Utilities/AddEnableLogOption)
include(Abytek/Utilities/AddEnableUnitTestsOption)


if(ABYTEK_MODULE_PHASE_INIT)
    set(Canvas.MacroName ABYTEK_SANDBOX_NFC_SAMPLES_CANVAS)
    set(Canvas.Enable ${Abytek.Engine.Enable})
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    list(APPEND Canvas.PublicDependencies
        Engine
    )
endif()
