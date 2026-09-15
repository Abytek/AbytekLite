
include(Abytek/Utilities/AddEnableAssertionsOption)
include(Abytek/Utilities/AddEnableLogOption)
include(Abytek/Utilities/AddEnableUnitTestsOption)


if(ABYTEK_MODULE_PHASE_INIT)
    set(UploadBuffer.MacroName ABYTEK_SANDBOX_RHI_SAMPLES_UPLOAD_BUFFER)
    set(UploadBuffer.Enable ${Abytek.Engine.Enable})
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    list(APPEND UploadBuffer.PublicDependencies
        Engine
    )
endif()
