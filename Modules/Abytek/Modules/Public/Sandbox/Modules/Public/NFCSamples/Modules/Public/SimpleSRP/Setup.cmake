
include(Abytek/Utilities/AddEnableAssertionsOption)
include(Abytek/Utilities/AddEnableLogOption)
include(Abytek/Utilities/AddEnableUnitTestsOption)


if(ABYTEK_MODULE_PHASE_INIT)
    set(SimpleSRP.MacroName ABYTEK_SANDBOX_NFC_SAMPLES_SIMPLE_SRP)
    set(SimpleSRP.Enable ${Abytek.Engine.Enable})
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    list(APPEND SimpleSRP.PublicDependencies
        Engine
    )
endif()
