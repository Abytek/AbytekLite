
include(Abytek/Utilities/AddEnableAssertionsOption)
include(Abytek/Utilities/AddEnableLogOption)
include(Abytek/Utilities/AddEnableUnitTestsOption)


if(ABYTEK_MODULE_PHASE_INIT)
    set(StaticMeshComponent.MacroName ABYTEK_SANDBOX_NFC_SAMPLES_STATIC_MESH_COMPONENT)
    set(StaticMeshComponent.Enable ${Abytek.Engine.Enable})
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    list(APPEND StaticMeshComponent.PublicDependencies
        Engine
    )
endif()
