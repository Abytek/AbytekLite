
include(Abytek/Utilities/AddEnableAssertionsOption)
include(Abytek/Utilities/AddEnableLogOption)
include(Abytek/Utilities/AddEnableUnitTestsOption)
include(Abytek/Utilities/ImGuiRuntime)
include(Abytek/Utilities/RHIRuntime)


function(Abytek_Engine_ImGui_ConfigureImGui)
    message(STATUS "Configure ImGui")
    Abytek_SetGlobal(IMGUI_ENABLE_DIRECTX12 ${ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12})
    if(WIN32)
        Abytek_SetGlobal(IMGUI_ENABLE_WIN32 ON)
    else()
        Abytek_SetGlobal(IMGUI_ENABLE_WIN32 OFF)
    endif()

    Abytek_SetGlobal(ABYTEK_ENGINE_IMGUI_ENABLE_DIRECTX12 ${IMGUI_ENABLE_DIRECTX12})
    Abytek_SetGlobal(ABYTEK_ENGINE_IMGUI_ENABLE_WIN32 ${IMGUI_ENABLE_WIN32})
    add_subdirectory("${ABYTEK_IMGUI_DIR}" "${ABYTEK_DEPENDENCIES_DIR}/ImGui/Build")
    message(STATUS "Configured ImGui")
endfunction()


if(ABYTEK_MODULE_PHASE_INIT)
    set(ImGui.MacroName ABYTEK_ENGINE_IMGUI)
    set(ImGui.Enable ${ABYTEK_ENGINE_IMGUI_ENABLE})

    if(${ABYTEK_ENGINE_IMGUI_ENABLE})
        Abytek_Engine_ImGui_ConfigureImGui()
    endif()
endif()

if(ABYTEK_MODULE_PHASE_GLOBAL_SHARE)
    # test
    include(Abytek/AddEnableUnitTestsOptionForCurrentModule)

    # assert
    include(Abytek/AddEnableAssertionsOptionForCurrentModule)
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    list(APPEND ImGui.PublicDependencies
        Core
        RHI
        RenderCore
    )
    if(ABYTEK_ENGINE_IMGUI_ENABLE)
        list(APPEND ImGui.PublicDependencies
            ImGuiCore
        )
    endif()
endif()
