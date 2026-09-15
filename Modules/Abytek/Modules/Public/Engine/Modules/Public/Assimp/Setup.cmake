
include(Abytek/Utilities/AddEnableAssertionsOption)
include(Abytek/Utilities/AddEnableLogOption)
include(Abytek/Utilities/AddEnableUnitTestsOption)
include(Abytek/Utilities/AddResolveReflectorInline)


function(Abytek_Engine_CheckAssimp)
    message(STATUS "Checking assimp")
    if(NOT EXISTS "${ABYTEK_DEPENDENCIES_DIR}/assimp")
        Abytek_GitHelper_Clone(
            PROJECT_NAME "assimp"
            GIT_URL "https://github.com/Abytek/assimp"
            GIT_COMMIT "a5687d7fff7674880c5a58ef58793ccc71438362"
            GIT_BRANCH "master"
            DIRECTORY "${ABYTEK_DEPENDENCIES_DIR}"
            UPDATE_SUBMODULES
        )
    endif()
    message(STATUS "Checked assimp")
endfunction()
function(Abytek_Engine_ImportAssimp)
    message(STATUS "Importing assimp")
    Abytek_SetGlobal(RAPIDJSON_HAS_STDSTRING 1)
    Abytek_SetGlobal(RAPIDJSON_NOMEMBERITERATORCLASS 1)
    Abytek_SetGlobal(ASSIMP_BUILD_GLTF_IMPORTER OFF)
    add_subdirectory("${ABYTEK_DEPENDENCIES_DIR}/assimp" "${ABYTEK_DEPENDENCIES_DIR}/assimp/Build")
    target_link_libraries(${CurrentTarget}
        PRIVATE
            assimp
    )
    message(STATUS "Imported assimp")
endfunction()

if(ABYTEK_MODULE_PHASE_INIT)
    set(Assimp.MacroName ABYTEK_ENGINE_ASSIMP)
    if(NOT EMSCRIPTEN)
        Abytek_SetGlobal(Assimp.Enable ON)
    else()
        Abytek_SetGlobal(Assimp.Enable OFF)
    endif()
    if(Assimp.Enable)
        Abytek_Engine_CheckAssimp()
    endif()
endif()

if(ABYTEK_MODULE_PHASE_GLOBAL_SHARE)
    # test
    include(Abytek/AddEnableUnitTestsOptionForCurrentModule)

    # assert
    include(Abytek/AddEnableAssertionsOptionForCurrentModule)
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    Abytek_Engine_ImportAssimp()
    list(APPEND Assimp.PublicDependencies
        Base
    )
endif()