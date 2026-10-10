
include(Abytek/Utilities/AddEnableAssertionsOption)
include(Abytek/Utilities/AddEnableLogOption)
include(Abytek/Utilities/AddEnableUnitTestsOption)


if(ABYTEK_MODULE_PHASE_INIT)
    set(StandardMaterial.MacroName ABYTEK_SANDBOX_NFC_SAMPLES_STANDARD_MATERIAL)
    set(StandardMaterial.Enable ${Abytek.Engine.Enable})
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    list(APPEND StandardMaterial.PublicDependencies
        Engine
    )
endif()
