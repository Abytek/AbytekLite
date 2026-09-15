
include(Abytek/Utilities/AddEnableAssertionsOption)
include(Abytek/Utilities/AddEnableLogOption)
include(Abytek/Utilities/AddEnableUnitTestsOption)


if(ABYTEK_MODULE_PHASE_INIT)
    set(StaticMeshImportFile.MacroName ABYTEK_SANDBOX_NFC_SAMPLES_STATIC_MESH_IMPORT_FILE)
    set(StaticMeshImportFile.Enable ${Abytek.Engine.Enable})
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    list(APPEND StaticMeshImportFile.PublicDependencies
        Engine
    )
endif()
