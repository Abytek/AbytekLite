
include(Abytek/Utilities/AddEnableAssertionsOption)
include(Abytek/Utilities/AddEnableLogOption)
include(Abytek/Utilities/AddEnableUnitTestsOption)


if(ABYTEK_MODULE_PHASE_INIT)
    set(TextureImportFile.MacroName ABYTEK_SANDBOX_NFC_SAMPLES_TEXTURE_IMPORT_FILE)
    set(TextureImportFile.Enable ${Abytek.Engine.Enable})
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    list(APPEND TextureImportFile.PublicDependencies
        Engine
    )
endif()
