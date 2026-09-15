
include(Abytek/Utilities/AddEnableAssertionsOption)
include(Abytek/Utilities/AddEnableLogOption)
include(Abytek/Utilities/AddEnableUnitTestsOption)
include(Abytek/Utilities/SetGlobal)


if(ABYTEK_MODULE_PHASE_INIT)
    set(RenderCore.MacroName ABYTEK_ENGINE_RENDER_CORE)

    option(ABYTEK_ENABLE_INTERNAL_GLOBAL_RENDER_PACK_DEBUG OFF)
    option(ABYTEK_ENABLE_INTERNAL_GLOBAL_RENDER_BINDING_DEBUG OFF)
    option(ABYTEK_ENABLE_INTERNAL_GLOBAL_RENDER_PIPELINE_DEBUG OFF)
    if(ABYTEK_ENABLE_INTERNAL_GLOBAL_RENDER_PACK_DEBUG)
        Abytek_SetGlobal(ABYTEK_ENABLE_INTERNAL_GLOBAL_RENDER_BINDING_DEBUG ON)
        Abytek_SetGlobal(ABYTEK_ENABLE_INTERNAL_GLOBAL_RENDER_PIPELINE_DEBUG ON)
    endif()
endif()

if(ABYTEK_MODULE_PHASE_GLOBAL_SHARE)
    # test
    include(Abytek/AddEnableUnitTestsOptionForCurrentModule)

    # assert
    include(Abytek/AddEnableAssertionsOptionForCurrentModule)
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    list(APPEND RenderCore.PublicDependencies
        Minimal
        Core
        Window
        Assets
        RHI
        Resource
    )
endif()