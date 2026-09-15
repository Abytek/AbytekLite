
include(Abytek/Utilities/AddEnableAssertionsOption)
include(Abytek/Utilities/AddEnableLogOption)
include(Abytek/Utilities/AddEnableUnitTestsOption)


if(ABYTEK_MODULE_PHASE_INIT)
    set(RHISamples.MacroName ABYTEK_SANDBOX_RHI_SAMPLES)
    set(RHISamples.Type CUSTOM)
endif()