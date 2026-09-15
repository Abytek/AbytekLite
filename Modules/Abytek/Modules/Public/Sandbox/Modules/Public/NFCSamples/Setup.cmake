
include(Abytek/Utilities/AddEnableAssertionsOption)
include(Abytek/Utilities/AddEnableLogOption)
include(Abytek/Utilities/AddEnableUnitTestsOption)


if(ABYTEK_MODULE_PHASE_INIT)
    set(NFCSamples.MacroName ABYTEK_SANDBOX_NFC_SAMPLES)
    set(NFCSamples.Type CUSTOM)
endif()