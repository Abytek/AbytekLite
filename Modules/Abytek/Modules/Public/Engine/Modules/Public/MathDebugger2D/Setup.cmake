
include(Abytek/Utilities/AddEnableAssertionsOption)
include(Abytek/Utilities/AddEnableLogOption)
include(Abytek/Utilities/AddEnableUnitTestsOption)
include(Abytek/Utilities/ImGuiRuntime)


if(ABYTEK_MODULE_PHASE_INIT)
    set(MathDebugger2D.MacroName ABYTEK_ENGINE_MATH_DEBUGGER_2D)
endif()

if(ABYTEK_MODULE_PHASE_GLOBAL_SHARE)
    # test
    include(Abytek/AddEnableUnitTestsOptionForCurrentModule)

    # assert
    include(Abytek/AddEnableAssertionsOptionForCurrentModule)
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    list(APPEND MathDebugger2D.PublicDependencies
        Core
        IHI
        RHI
        RenderCore
    )
    if(ABYTEK_ENGINE_IMGUI_ENABLE)
        list(APPEND MathDebugger2D.PublicDependencies
            ImGui
        )
    endif()
endif()
