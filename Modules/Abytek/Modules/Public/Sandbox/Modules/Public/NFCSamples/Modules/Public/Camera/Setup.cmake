
include(Abytek/Utilities/AddEnableAssertionsOption)
include(Abytek/Utilities/AddEnableLogOption)
include(Abytek/Utilities/AddEnableUnitTestsOption)


if(ABYTEK_MODULE_PHASE_INIT)
    set(Camera.MacroName ABYTEK_SANDBOX_NFC_SAMPLES_CAMERA)
    set(Camera.Enable ${Abytek.Engine.Enable})
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    list(APPEND Camera.PublicDependencies
        Engine
    )
endif()
