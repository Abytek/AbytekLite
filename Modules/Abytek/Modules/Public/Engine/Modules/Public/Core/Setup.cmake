
include(Abytek/Utilities/AddEnableAssertionsOption)
include(Abytek/Utilities/AddEnableLogOption)
include(Abytek/Utilities/AddEnableUnitTestsOption)
include(Abytek/Utilities/AddFileAsDependency)
include(Abytek/Utilities/CommonFlags)


function(Abytek_Engine_Core_CheckSocketIOClient)
    message(STATUS "Checking SocketIOClient")
    if(NOT EXISTS "${ABYTEK_DEPENDENCIES_DIR}/socket.io-client-cpp")
        Abytek_GitHelper_Clone(
            PROJECT_NAME "socket.io-client-cpp"
            GIT_URL "https://github.com/socketio/socket.io-client-cpp"
            GIT_COMMIT "3b7be7e4173b5bdeed393966e3274f65d513a280"
            GIT_BRANCH "master"
            DIRECTORY "${ABYTEK_DEPENDENCIES_DIR}"
            UPDATE_SUBMODULES
        )
    endif()
    message(STATUS "Checked SocketIOClient")
endfunction()

function(Abytek_Engine_Core_CheckNodeJS)
    find_program(ABYTEK_NODEJS_EXECUTABLE
        NAMES node nodejs
        HINTS $ENV{PATH}
    )
    if(NOT ABYTEK_NODEJS_EXECUTABLE)
      message(FATAL_ERROR "node executable not found; please install Node.js or set ABYTEK_NODEJS_EXECUTABLE")
    endif()    
    Abytek_SetGlobal(ABYTEK_NODEJS_EXECUTABLE "${ABYTEK_NODEJS_EXECUTABLE}")
endfunction()

function(Abytek_Engine_Core_CheckNPM)
    if(ABYTEK_NPM_EXECUTABLE)
        unset(ABYTEK_NPM_EXECUTABLE CACHE)
    endif()
    find_program(ABYTEK_NPM_EXECUTABLE
        NAMES npm
        HINTS $ENV{PATH}
    )
    if(NOT ABYTEK_NPM_EXECUTABLE)
      message(FATAL_ERROR "npm executable not found; please install Node.js or set ABYTEK_NPM_EXECUTABLE")
    endif()    
    if(WIN32)
        set(ABYTEK_NPM_EXECUTABLE "${ABYTEK_NPM_EXECUTABLE}.cmd")
    endif()
    Abytek_SetGlobal(ABYTEK_NPM_EXECUTABLE "${ABYTEK_NPM_EXECUTABLE}")
endfunction()

function(Abytek_Engine_Core_ImportSocketIOClient)
    message(STATUS "Importing SocketIOClient")
    Abytek_SetGlobal(RAPIDJSON_HAS_STDSTRING 1)
    Abytek_SetGlobal(RAPIDJSON_NOMEMBERITERATORCLASS 1)
    add_subdirectory("${ABYTEK_DEPENDENCIES_DIR}/socket.io-client-cpp" "${ABYTEK_DEPENDENCIES_DIR}/socket.io-client-cpp/Build")
    target_link_libraries(${CurrentTarget}
        PUBLIC
            sioclient
    )
    message(STATUS "Imported SocketIOClient")
endfunction()



if(ABYTEK_MODULE_PHASE_INIT)
    set(Core.MacroName ABYTEK_ENGINE_CORE)

    list(APPEND ABYTEK_SETUP_PRE_SCRIPTS "Abytek/PreScripts/Core")

    Abytek_SetGlobal(ABYTEK_DEVELOPMENT_DATA_SERVER_JS_FILE "${Core.JSDirectory}/Abytek/Apps/DevelopmentDataServer/Main.js")
    Abytek_SetGlobal(ABYTEK_DEVELOPMENT_DATA_SERVER_DIR "${Core.JSDirectory}/Abytek/Apps/DevelopmentDataServer")

    Abytek_Engine_Core_CheckNodeJS()
    Abytek_Engine_Core_CheckNPM()
    if(ABYTEK_ENABLE_SOCKET_IO_CLIENT)
        Abytek_Engine_Core_CheckSocketIOClient()
    endif()
endif()

if(ABYTEK_MODULE_PHASE_GLOBAL_SHARE)
    # test
    include(Abytek/AddEnableUnitTestsOptionForCurrentModule)

    # assert
    include(Abytek/AddEnableAssertionsOptionForCurrentModule)
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    if(ABYTEK_ENABLE_SOCKET_IO_CLIENT)
        Abytek_Engine_Core_ImportSocketIOClient()
    endif()

    list(APPEND Core.PublicDependencies
        Minimal
    )
endif()
