
include(Abytek/Utilities/AddEnableAssertionsOption)
include(Abytek/Utilities/AddEnableLogOption)
include(Abytek/Utilities/AddEnableUnitTestsOption)
include(Abytek/Utilities/NFCRuntime)


function(Abytek_Engine_NFC_CheckSTB)
    message(STATUS "Checking STB")
    if(NOT EXISTS "${ABYTEK_DEPENDENCIES_DIR}/stb")
        Abytek_GitHelper_Clone(
            PROJECT_NAME "stb"
            GIT_URL "https://github.com/Abytek/stb"
            GIT_COMMIT "2c980bb59875b0d32144a71867fbdebb2f77cd20"
            GIT_BRANCH "master"
            DIRECTORY "${ABYTEK_DEPENDENCIES_DIR}"
            UPDATE_SUBMODULES
        )
    endif()
    message(STATUS "Checked STB")
endfunction()
function(Abytek_Engine_NFC_ImportSTB)
    message(STATUS "Importing STB")
    target_include_directories(${CurrentTarget}
        PUBLIC
            "${ABYTEK_DEPENDENCIES_DIR}/stb"
    )
    message(STATUS "Imported STB")
endfunction()

function(Abytek_Engine_NFC_CheckFP16)
    message(STATUS "Checking FP16")
    if(NOT EXISTS "${ABYTEK_DEPENDENCIES_DIR}/FP16")
        Abytek_GitHelper_Clone(
            PROJECT_NAME "FP16"
            GIT_URL "https://github.com/Abytek/FP16"
            GIT_COMMIT "782eea126dc5c755827be751a099eb01826175cf"
            GIT_BRANCH "master"
            DIRECTORY "${ABYTEK_DEPENDENCIES_DIR}"
            UPDATE_SUBMODULES
        )
    endif()
    message(STATUS "Checked FP16")
endfunction()
function(Abytek_Engine_NFC_ImportFP16)
    message(STATUS "Importing FP16")
    add_subdirectory("${ABYTEK_DEPENDENCIES_DIR}/FP16" "${ABYTEK_DEPENDENCIES_DIR}/FP16/Build")
    target_link_libraries(${CurrentTarget}
        PUBLIC
            fp16
    )
    message(STATUS "Imported FP16")
endfunction()

function(Abytek_Engine_NFC_CheckMeshoptimizer)
    message(STATUS "Checking meshoptimizer")
    if(NOT EXISTS "${ABYTEK_DEPENDENCIES_DIR}/meshoptimizer")
        Abytek_GitHelper_Clone(
            PROJECT_NAME "meshoptimizer"
            GIT_URL "https://github.com/Abytek/meshoptimizer"
            GIT_COMMIT "9e1f07b159d3cb777f1c67ed31fc11fd117986f4"
            GIT_BRANCH "master"
            DIRECTORY "${ABYTEK_DEPENDENCIES_DIR}"
            UPDATE_SUBMODULES
        )
    endif()
    message(STATUS "Checked meshoptimizer")
endfunction()
function(Abytek_Engine_NFC_ImportMeshoptimizer)
    message(STATUS "Importing meshoptimizer")
    add_subdirectory("${ABYTEK_DEPENDENCIES_DIR}/meshoptimizer" "${ABYTEK_DEPENDENCIES_DIR}/meshoptimizer/Build")
    target_link_libraries(${CurrentTarget}
        PUBLIC
            meshoptimizer
    )
    message(STATUS "Imported meshoptimizer")
endfunction()


if(ABYTEK_MODULE_PHASE_INIT)
    set(NFC.MacroName ABYTEK_ENGINE_NFC)

    Abytek_Engine_NFC_CheckSTB()
    Abytek_Engine_NFC_CheckFP16()
    Abytek_Engine_NFC_CheckMeshoptimizer()
endif()

if(ABYTEK_MODULE_PHASE_GLOBAL_SHARE)
    # test
    include(Abytek/AddEnableUnitTestsOptionForCurrentModule)

    # assert
    include(Abytek/AddEnableAssertionsOptionForCurrentModule)
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    Abytek_Engine_NFC_ImportSTB()
    Abytek_Engine_NFC_ImportFP16()
    Abytek_Engine_NFC_ImportMeshoptimizer()

    list(APPEND NFC.PublicDependencies
        MFC
    )
    if(Abytek.Engine.Assimp.Enable)
        target_compile_definitions(${CurrentTarget}
            PUBLIC
                -DABYTEK_ENGINE_NFC_ENABLE_ASSIMP
        )
        list(APPEND NFC.PrivateDependencies
            Assimp
        )
    endif()
endif()
