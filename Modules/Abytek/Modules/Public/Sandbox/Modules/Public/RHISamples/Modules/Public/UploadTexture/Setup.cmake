
include(Abytek/Utilities/AddEnableAssertionsOption)
include(Abytek/Utilities/AddEnableLogOption)
include(Abytek/Utilities/AddEnableUnitTestsOption)


if(ABYTEK_MODULE_PHASE_INIT)
    set(UploadTexture.MacroName ABYTEK_SANDBOX_RHI_SAMPLES_UPLOAD_TEXTURE)
    set(UploadTexture.Enable ${Abytek.Engine.Enable})
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    list(APPEND UploadTexture.PublicDependencies
        Engine
    )
endif()
