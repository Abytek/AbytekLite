
include(Abytek/Utilities/AddEnableAssertionsOption)
include(Abytek/Utilities/AddEnableLogOption)
include(Abytek/Utilities/AddEnableUnitTestsOption)


if(ABYTEK_MODULE_PHASE_INIT)
    set(ReadbackBuffer.MacroName ABYTEK_SANDBOX_RHI_SAMPLES_READBACK_BUFFER)
    set(ReadbackBuffer.Enable ${Abytek.Engine.Enable})
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    list(APPEND ReadbackBuffer.PublicDependencies
        Engine
    )
endif()
