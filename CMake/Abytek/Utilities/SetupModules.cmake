
include(Abytek/Utilities/ApplyDependencies)
include(Abytek/Utilities/SetGlobal)
include(Abytek/Utilities/ApplyGlobal)
include(Abytek/Utilities/TempDependenciesDir)


Abytek_SetGlobal(ABYTEK_GLOBAL_IMPORT_JS_FILE "${ABYTEK_PROJECT_BUILD_DIR}/AbytekEBT/Import.js")

Abytek_SetGlobal(ABYTEK_EBT_GENERATED_SOURCE_DIRECTORY "${ABYTEK_PROJECT_BUILD_DIR}/AbytekEBT/GeneratedSource")
Abytek_SetGlobal(ABYTEK_GLOBAL_MODULES_INL_FILE "${ABYTEK_EBT_GENERATED_SOURCE_DIRECTORY}/Abytek/Modules.inl")


function(Abytek_GetModuleVSFolderNameAndLastName input_string moduleVSFolderName lastName)
    # Find the position of the last dot
    string(FIND "${input_string}" "/" LAST_DOT_POSITION REVERSE)

    # Check if a dot was found
    if(LAST_DOT_POSITION EQUAL -1)
        set(${moduleVSFolderName} "${input_string}" PARENT_SCOPE)
        set(${lastName} "" PARENT_SCOPE)
        return()
    endif()

    # Split the string into two parts
    math(EXPR PART1_LENGTH "${LAST_DOT_POSITION}")
    string(SUBSTRING "${input_string}" 0 ${PART1_LENGTH} PART1)
    math(EXPR PART2_START "${LAST_DOT_POSITION} + 1")
    string(SUBSTRING "${input_string}" ${PART2_START} -1 PART2)

    # Set the output variables
    set(${moduleVSFolderName} ${PART1} PARENT_SCOPE)
    set(${lastName} ${PART2} PARENT_SCOPE)
endfunction()



function(Abytek_ExcludeFilesInDirectory List ExcludeDirectory)
    set(TempList "")



    set(List ${TempList} PARENT_SCOPE)
endfunction()



function(Abytek_CalculateModuleFullName OutVar Namespace ModuleName)
    if(Namespace STREQUAL "")
        set(${OutVar} "${ModuleName}" PARENT_SCOPE)
    else()
        set(${OutVar} "${Namespace}.${ModuleName}" PARENT_SCOPE)
    endif()
endfunction()



option(ABYTEK_FORCE_DISABLE_UNITY_BUILD "" OFF)

function(Abytek_SetupCompilerConfigsForTarget Target)
    if(EMSCRIPTEN)
        if(ABYTEK_EMSCRIPTEN_EXECUTABLE_JS_ONLY)
            set_target_properties(${Target} PROPERTIES LINK_FLAGS "-s WASM=0 -s EXPORTED_FUNCTIONS='[_main]'")
        else()
            set_target_properties(${Target} PROPERTIES LINK_FLAGS "-s WASM=1 -s EXPORTED_FUNCTIONS='[_main]'")
        endif()
    endif()
    if(${${Target}.EnableUnityBuild})
        if(ABYTEK_FORCE_DISABLE_UNITY_BUILD)
            set_target_properties(${Target} PROPERTIES UNITY_BUILD OFF)
        else()
            set_target_properties(${Target} PROPERTIES UNITY_BUILD ON)
        endif()
    endif()
endfunction()



function(Abytek_ExecuteJS)
    cmake_parse_arguments(
        PARGS
        ""
        "RELATIVE_PATH"
        "CLI_ARGS"
        ${ARGN}
    )
    if(NOT PARGS_RELATIVE_PATH)
        message(FATAL_ERROR "RELATIVE_PATH is required")
    endif()
    if(NOT PARGS_CLI_ARGS)
        set(PARGS_CLI_ARGS "")
    endif()
    
    set(CLIArgsLog "")
    foreach(CLIArg ${PARGS_CLI_ARGS})
        set(CLIArgsLog "${CLIArgsLog}${CLIArg} ")
    endforeach()
        
    message(STATUS "Start executing JS script: ${ModuleFullName}::${PARGS_RELATIVE_PATH} ${CLIArgsLog}")
    get_filename_component(JSRunScriptFileDir "${PARGS_JS_RUN_SCRIPT_FILE}" DIRECTORY)
    get_filename_component(JSRunScriptFileName "${PARGS_JS_RUN_SCRIPT_FILE}" NAME)
    execute_process(
        COMMAND "${ABYTEK_NODEJS_EXECUTABLE}" "${JSRunScriptFileName}" "${ModuleFullName}" "${PARGS_RELATIVE_PATH}" ${PARGS_CLI_ARGS}
        RESULT_VARIABLE result
        ECHO_OUTPUT_VARIABLE
        WORKING_DIRECTORY "${JSRunScriptFileDir}"
    )
    if(NOT result EQUAL 0)
        message(FATAL_ERROR "Failed to execute JS script: ${ModuleFullName}::${PARGS_RELATIVE_PATH}")
    else()
        message(STATUS "JS script executed successfully: ${ModuleFullName}::${PARGS_RELATIVE_PATH}")
    endif()
endfunction()



function(Abytek_AddRuntimeFile SrcPath DstRelativePath)
    add_custom_command(TARGET ${CurrentTarget} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${SrcPath}"
        "${${CurrentTarget}.TempDependenciesDirectory}/${DstRelativePath}"
    )
endfunction()

function(Abytek_SetupThirdParty Stage)
    if(EXISTS "${${ModuleFullName}.ThirdParty.Directory}")
        set(ThirdPartyNames "")
        file(
            GLOB
            ThirdPartyNames
            RELATIVE
            "${${ModuleFullName}.ThirdParty.Directory}"
            "${${ModuleFullName}.ThirdParty.Directory}/*"
        )
        set(${Stage} ON)
        foreach(ThirdPartyName ${ThirdPartyNames})
            set(ThirdPartySetupFile "${${ModuleFullName}.ThirdParty.Directory}/${ThirdPartyName}/Setup.cmake")
            if(EXISTS "${ThirdPartySetupFile}")
                include("${ThirdPartySetupFile}")
            endif()
        endforeach()
        set(${Stage} OFF)
    endif()
endfunction()



function(Abytek_SetupModules)
    cmake_parse_arguments(
        PARGS
        "PHASE_ALL;"
        "BASE_NAMESPACE;MODULES_DIR;OUT_MODULES_VAR_NAME;OUT_TARGETS_VAR_NAME;OUT_ALL_MODULES;ADDITIONAL_INFO;DEFAULT_MODULE_TYPE;PHASE;JS_CONTEXT_FILE;JS_RUN_SCRIPT_FILE"
        "NAMESPACES"
        ${ARGN}
    )

    if(NOT PARGS_BASE_NAMESPACE)
        set(PARGS_BASE_NAMESPACE "")
    endif()
    if(NOT PARGS_MODULES_DIR)
        message(FATAL_ERROR "MODULES_DIR is required")
    endif()
    if(NOT PARGS_JS_CONTEXT_FILE)
        message(FATAL_ERROR "JS_CONTEXT_FILE is required")
    endif()
    if(NOT PARGS_JS_RUN_SCRIPT_FILE)
        message(FATAL_ERROR "JS_RUN_SCRIPT_FILE is required")
    endif()
    if(NOT PARGS_NAMESPACES)
        set(PARGS_NAMESPACES "")
    endif()
    if(PARGS_BASE_NAMESPACE)
        list(APPEND PARGS_NAMESPACES ${PARGS_BASE_NAMESPACE})
    endif()
    if(NOT PARGS_ADDITIONAL_INFO)
        set(PARGS_ADDITIONAL_INFO "")
    endif()
    if(NOT PARGS_DEFAULT_MODULE_TYPE)
        set(PARGS_DEFAULT_MODULE_TYPE STATIC)
    endif()



    #
    set(GlobalSource.Directory "${ABYTEK_ROOT_DIR}/GlobalSource")



    # Find modules
    set(MODULE_NAMES "")
    file(
        GLOB
        ItemNamesInModules
        RELATIVE
        "${PARGS_MODULES_DIR}"
        "${PARGS_MODULES_DIR}/*"
    )
    foreach(ItemNameInModules ${ItemNamesInModules})
        if(IS_DIRECTORY "${PARGS_MODULES_DIR}/${ItemNameInModules}")
            list(APPEND MODULE_NAMES "${ItemNameInModules}")

            # To later steps know that this module is actually exists
            set(MODULES.${ItemNameInModules} ON)
        endif()
    endforeach()

    if(PARGS_PHASE_ALL)
        message(STATUS "Found modules (BASE_NAMESPACE=${PARGS_BASE_NAMESPACE}${PARGS_ADDITIONAL_INFO}, PHASE_ALL):")
    else()
        message(STATUS "Found modules (BASE_NAMESPACE=${PARGS_BASE_NAMESPACE}${PARGS_ADDITIONAL_INFO}, PHASE=${PARGS_PHASE}):")
    endif()
    foreach(ModuleName ${MODULE_NAMES})
        Abytek_CalculateModuleFullName(ModuleFullName "${PARGS_BASE_NAMESPACE}" ${ModuleName})
        message(STATUS "   ${ModuleFullName}")
    endforeach()



    # Setup module list
    if(PARGS_OUT_MODULES_VAR_NAME)
        set(${PARGS_OUT_MODULES_VAR_NAME} "")
    endif()
    if(PARGS_OUT_TARGETS_VAR_NAME)
        set(${PARGS_OUT_TARGETS_VAR_NAME} "")
    endif()
    if(NOT PARGS_OUT_ALL_MODULES)
        set(PARGS_OUT_ALL_MODULES "DummyAbytekModules")
    endif()
    foreach(ModuleName ${MODULE_NAMES})
        Abytek_CalculateModuleFullName(ModuleFullName "${PARGS_BASE_NAMESPACE}" ${ModuleName})
        if(PARGS_OUT_MODULES_VAR_NAME)
            list(APPEND ${PARGS_OUT_MODULES_VAR_NAME} "${ModuleName}")
        endif()
        if(PARGS_OUT_TARGETS_VAR_NAME)
            list(APPEND ${PARGS_OUT_TARGETS_VAR_NAME} "${ModuleFullName}")
        endif()
        if(PARGS_OUT_ALL_MODULES)
            list(APPEND ${PARGS_OUT_ALL_MODULES} "${ModuleFullName}")
        endif()
    endforeach()
    if(PARGS_OUT_MODULES_VAR_NAME)
        set(${PARGS_OUT_MODULES_VAR_NAME} "${${PARGS_OUT_MODULES_VAR_NAME}}" PARENT_SCOPE)
    endif()
    if(PARGS_OUT_TARGETS_VAR_NAME)
        set(${PARGS_OUT_TARGETS_VAR_NAME} "${${PARGS_OUT_TARGETS_VAR_NAME}}" PARENT_SCOPE)
    endif()
    if(PARGS_OUT_ALL_MODULES)
        set(${PARGS_OUT_ALL_MODULES} "${${PARGS_OUT_ALL_MODULES}}" PARENT_SCOPE)
    endif()



    # Init modules
    string(REPLACE "\\" "/" PARSED_MODULES_DIR "${PARGS_MODULES_DIR}")
    string(REPLACE "\\" "/" PARSED_CMAKE_CURRENT_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}")
    if(PARGS_PHASE_ALL OR PARGS_PHASE STREQUAL INIT)
        foreach(ModuleName ${MODULE_NAMES})
            Abytek_CalculateModuleFullName(ModuleFullName "${PARGS_BASE_NAMESPACE}" ${ModuleName})

            Abytek_SetGlobal(${ModuleName}.Target "${ModuleFullName}")
            Abytek_SetGlobal(${ModuleName}.Directory "${PARSED_MODULES_DIR}/${ModuleName}")
            Abytek_SetGlobal(${ModuleName}.BuildDirectory "${PARSED_CMAKE_CURRENT_BINARY_DIR}/${ModuleFullName}")
            Abytek_SetGlobal(${ModuleName}.Modules.Directory "${${ModuleName}.Directory}/Modules")
            Abytek_SetGlobal(${ModuleName}.Modules.PrivateDirectory "${${ModuleName}.Modules.Directory}/Private")
            Abytek_SetGlobal(${ModuleName}.Modules.InterfaceDirectory "${${ModuleName}.Modules.Directory}/Interface")
            Abytek_SetGlobal(${ModuleName}.Modules.PublicDirectory "${${ModuleName}.Modules.Directory}/Public")
            Abytek_SetGlobal(${ModuleName}.Source.Directory "${${ModuleName}.Directory}/Source")
            Abytek_SetGlobal(${ModuleName}.Source.PrivateDirectory "${${ModuleName}.Source.Directory}/Private")
            Abytek_SetGlobal(${ModuleName}.Source.InterfaceDirectory "${${ModuleName}.Source.Directory}/Interface")
            Abytek_SetGlobal(${ModuleName}.Source.PublicDirectory "${${ModuleName}.Source.Directory}/Public")
            Abytek_SetGlobal(${ModuleName}.AutoCopy.Directory "${${ModuleName}.Directory}/AutoCopy")
            Abytek_SetGlobal(${ModuleName}.ThirdParty.Directory "${${ModuleName}.Directory}/ThirdParty")
            Abytek_SetGlobal(${ModuleName}.GeneratedSource.Directory "${PARSED_CMAKE_CURRENT_BINARY_DIR}/${ModuleFullName}/GeneratedSource")
            Abytek_SetGlobal(${ModuleName}.GeneratedSource.TempDirectory "${${ModuleName}.GeneratedSource.Directory}/Temp")
            Abytek_SetGlobal(${ModuleName}.GeneratedSource.PrivateDirectory "${${ModuleName}.GeneratedSource.Directory}/Private")
            Abytek_SetGlobal(${ModuleName}.GeneratedSource.InterfaceDirectory "${${ModuleName}.GeneratedSource.Directory}/Interface")
            Abytek_SetGlobal(${ModuleName}.GeneratedSource.PublicDirectory "${${ModuleName}.GeneratedSource.Directory}/Public")
            Abytek_SetGlobal(${ModuleName}.CMakeDirectory "${${ModuleName}.Directory}/CMake")
            Abytek_SetGlobal(${ModuleName}.JSDirectory "${${ModuleName}.Directory}/JS")
            Abytek_SetGlobal(${ModuleName}.GeneratedJSDirectory "${PARSED_CMAKE_CURRENT_BINARY_DIR}/${ModuleFullName}/GeneratedJS")
            Abytek_SetGlobal(${ModuleName}.JSModuleFile "${${ModuleName}.GeneratedJSDirectory}/${ModuleFullName}.js")
            Abytek_SetGlobal(${ModuleName}.JSModulesFile "${${ModuleName}.GeneratedJSDirectory}/${ModuleFullName}.Modules.js")
            Abytek_SetGlobal(${ModuleName}.JSPrivateModulesFile "${${ModuleName}.GeneratedJSDirectory}/${ModuleFullName}.Modules.Private.js")
            Abytek_SetGlobal(${ModuleName}.JSPublicModulesFile "${${ModuleName}.GeneratedJSDirectory}/${ModuleFullName}.Modules.Public.js")
            Abytek_SetGlobal(${ModuleName}.JSDependenciesFile "${${ModuleName}.GeneratedJSDirectory}/${ModuleFullName}.Dependencies.js")
            Abytek_SetGlobal(${ModuleName}.JSPrivateDependenciesFile "${${ModuleName}.GeneratedJSDirectory}/${ModuleFullName}.Dependencies.Private.js")
            Abytek_SetGlobal(${ModuleName}.JSPublicDependenciesFile "${${ModuleName}.GeneratedJSDirectory}/${ModuleFullName}.Dependencies.Public.js")

            Abytek_SetGlobal(${ModuleFullName}.Target "${${ModuleName}.Target}")
            Abytek_SetGlobal(${ModuleFullName}.Directory "${${ModuleName}.Directory}")
            Abytek_SetGlobal(${ModuleFullName}.BuildDirectory "${PARSED_CMAKE_CURRENT_BINARY_DIR}/${ModuleFullName}")
            Abytek_SetGlobal(${ModuleFullName}.Modules.Directory "${${ModuleName}.Modules.Directory}")
            Abytek_SetGlobal(${ModuleFullName}.Modules.PrivateDirectory "${${ModuleName}.Modules.PrivateDirectory}")
            Abytek_SetGlobal(${ModuleFullName}.Modules.InterfaceDirectory "${${ModuleName}.Modules.InterfaceDirectory}")
            Abytek_SetGlobal(${ModuleFullName}.Modules.PublicDirectory "${${ModuleName}.Modules.PublicDirectory}")
            Abytek_SetGlobal(${ModuleFullName}.Source.Directory "${${ModuleName}.Source.Directory}")
            Abytek_SetGlobal(${ModuleFullName}.Source.PrivateDirectory "${${ModuleName}.Source.PrivateDirectory}")
            Abytek_SetGlobal(${ModuleFullName}.Source.InterfaceDirectory "${${ModuleName}.Source.InterfaceDirectory}")
            Abytek_SetGlobal(${ModuleFullName}.Source.PublicDirectory "${${ModuleName}.Source.PublicDirectory}")
            Abytek_SetGlobal(${ModuleFullName}.AutoCopy.Directory "${${ModuleName}.AutoCopy.Directory}")
            Abytek_SetGlobal(${ModuleFullName}.ThirdParty.Directory "${${ModuleName}.ThirdParty.Directory}")
            Abytek_SetGlobal(${ModuleFullName}.GeneratedSource.Directory "${${ModuleName}.GeneratedSource.Directory}")
            Abytek_SetGlobal(${ModuleFullName}.GeneratedSource.TempDirectory "${${ModuleName}.GeneratedSource.TempDirectory}")
            Abytek_SetGlobal(${ModuleFullName}.GeneratedSource.PrivateDirectory "${${ModuleName}.GeneratedSource.PrivateDirectory}")
            Abytek_SetGlobal(${ModuleFullName}.GeneratedSource.InterfaceDirectory "${${ModuleName}.GeneratedSource.InterfaceDirectory}")
            Abytek_SetGlobal(${ModuleFullName}.GeneratedSource.PublicDirectory "${${ModuleName}.GeneratedSource.PublicDirectory}")
            Abytek_SetGlobal(${ModuleFullName}.CMakeDirectory "${${ModuleName}.CMakeDirectory}")
            Abytek_SetGlobal(${ModuleFullName}.JSDirectory "${${ModuleName}.JSDirectory}")
            Abytek_SetGlobal(${ModuleFullName}.GeneratedJSDirectory "${${ModuleName}.GeneratedJSDirectory}")
            Abytek_SetGlobal(${ModuleFullName}.JSModuleFile "${${ModuleName}.JSModuleFile}")
            Abytek_SetGlobal(${ModuleFullName}.JSModulesFile "${${ModuleName}.JSModulesFile}")
            Abytek_SetGlobal(${ModuleFullName}.JSPrivateModulesFile "${${ModuleName}.JSPrivateModulesFile}")
            Abytek_SetGlobal(${ModuleFullName}.JSPublicModulesFile "${${ModuleName}.JSPublicModulesFile}")
            Abytek_SetGlobal(${ModuleFullName}.JSDependenciesFile "${${ModuleName}.JSDependenciesFile}")
            Abytek_SetGlobal(${ModuleFullName}.JSPrivateDependenciesFile "${${ModuleName}.JSPrivateDependenciesFile}")
            Abytek_SetGlobal(${ModuleFullName}.JSPublicDependenciesFile "${${ModuleName}.JSPublicDependenciesFile}")

            set(CurrentModule "${ModuleName}")
            set(CurrentModuleFullName "${ModuleFullName}")
            set(CurrentTarget "${ModuleFullName}")
            
            # CMake module
            list(APPEND CMAKE_MODULE_PATH "${ModuleFullName}.CMakeDirectory")

            # Find config files
            file(
                GLOB_RECURSE
                ${ModuleName}.Source.PrivateConfigFiles
                "${${ModuleName}.Source.PrivateDirectory}/*.config"
            )
            file(
                GLOB_RECURSE
                ${ModuleName}.Source.InterfaceConfigFiles
                "${${ModuleName}.Source.InterfaceDirectory}/*.config"
            )
            file(
                GLOB_RECURSE
                ${ModuleName}.Source.PublicConfigFiles
                "${${ModuleName}.Source.PublicDirectory}/*.config"
            )

            set(${ModuleName}.Source.ConfigFiles "")
            list(APPEND ${ModuleName}.Source.ConfigFiles ${${ModuleName}.Source.PrivateConfigFiles})
            list(APPEND ${ModuleName}.Source.ConfigFiles ${${ModuleName}.Source.InterfaceConfigFiles})
            list(APPEND ${ModuleName}.Source.ConfigFiles ${${ModuleName}.Source.PublicConfigFiles})

            set(${ModuleName}.GeneratedSource.PrivateConfigFiles "")
            foreach(ConfigFile ${${ModuleName}.Source.PrivateConfigFiles})
                file(
                    RELATIVE_PATH
                    ConfigFileRelativePath
                    "${${ModuleName}.Source.Directory}"
                    "${ConfigFile}"
                )
                set(GeneratedConfigFile "${${ModuleName}.GeneratedSource.Directory}/${ConfigFileRelativePath}.hpp")
                list(APPEND ${ModuleName}.GeneratedSource.PrivateConfigFiles ${GeneratedConfigFile})
            endforeach()

            set(${ModuleName}.GeneratedSource.InterfaceConfigFiles "")
            foreach(ConfigFile ${${ModuleName}.Source.InterfaceConfigFiles})
                file(
                    RELATIVE_PATH
                    ConfigFileRelativePath
                    "${${ModuleName}.Source.Directory}"
                    "${ConfigFile}"
                )
                set(GeneratedConfigFile "${${ModuleName}.GeneratedSource.Directory}/${ConfigFileRelativePath}.hpp")
                list(APPEND ${ModuleName}.GeneratedSource.InterfaceConfigFiles ${GeneratedConfigFile})
            endforeach()

            set(${ModuleName}.GeneratedSource.PublicConfigFiles "")
            foreach(ConfigFile ${${ModuleName}.Source.PublicConfigFiles})
                file(
                    RELATIVE_PATH
                    ConfigFileRelativePath
                    "${${ModuleName}.Source.Directory}"
                    "${ConfigFile}"
                )
                set(GeneratedConfigFile "${${ModuleName}.GeneratedSource.Directory}/${ConfigFileRelativePath}.hpp")
                list(APPEND ${ModuleName}.GeneratedSource.PublicConfigFiles ${GeneratedConfigFile})
            endforeach()

            set(${ModuleName}.GeneratedSource.ConfigFiles ${${ModuleName}.GeneratedSource.PrivateConfigFiles})
            list(APPEND ${ModuleName}.GeneratedSource.ConfigFiles ${${ModuleName}.GeneratedSource.InterfaceConfigFiles})
            list(APPEND ${ModuleName}.GeneratedSource.ConfigFiles ${${ModuleName}.GeneratedSource.PublicConfigFiles})

            # Default configs
            set(${ModuleName}.Enable ON)
            set(${ModuleName}.EnableUnityBuild ON)
            set(${ModuleName}.MacroName "")
            set(${ModuleName}.Type ${PARGS_DEFAULT_MODULE_TYPE})
            set(${ModuleName}.CanParentDependOn ON)
            set(${ModuleName}.PrivateDependencies "")
            set(${ModuleName}.PublicDependencies "")
            set(${ModuleName}.Depends "")
            set(${ModuleName}.Commands "")
            set(${ModuleName}.PrivateDefinitions "")
            set(${ModuleName}.InterfaceDefinitions "")
            set(${ModuleName}.PublicDefinitions "")
            set(${ModuleName}.Modules.DefaultType ${PARGS_DEFAULT_MODULE_TYPE})
            set(${ModuleName}.AdditionalSourceFiles "")

            # Setup phase: ABYTEK_MODULE_PHASE_INIT
            set(ABYTEK_MODULE_PHASE_INIT ON)
            message(STATUS "ABYTEK_MODULE_PHASE_INIT::BEGIN(${ModuleFullName})")
            set(ModuleSetupPath "${${ModuleName}.Directory}/Setup.cmake")
            if(EXISTS "${ModuleSetupPath}")
                include("${ModuleSetupPath}")
            endif()
            message(STATUS "ABYTEK_MODULE_PHASE_INIT::END(${ModuleFullName})")
            set(ABYTEK_MODULE_PHASE_INIT OFF)

            Abytek_SetGlobal(ABYTEK_SETUP_PRE_SCRIPTS "${ABYTEK_SETUP_PRE_SCRIPTS}")

            #
            if(${${ModuleName}.Type} STREQUAL CUSTOM)
                set(${ModuleName}.IsCompilable OFF)
            else()
                set(${ModuleName}.IsCompilable ON)
            endif()

            # Find public header and inl files
            file(
                GLOB_RECURSE
                ${ModuleName}.PublicHeaderAndInlFiles
                "${${ModuleName}.Source.PublicDirectory}/*.hpp"
                "${${ModuleName}.Source.PublicDirectory}/*.h"
                "${${ModuleName}.Source.PublicDirectory}/*.inl"
                "${${ModuleName}.GeneratedSource.PublicDirectory}/*.hpp"
                "${${ModuleName}.GeneratedSource.PublicDirectory}/*.h"
                "${${ModuleName}.GeneratedSource.PublicDirectory}/*.inl"
            )
            message(STATUS "${${ModuleName}.PublicHeaderAndInlFiles}")
            list(APPEND ${ModuleName}.PublicHeaderAndInlFiles ${${ModuleName}.GeneratedSource.PublicConfigFiles})

            # Check if there is any public cpp files
            file(
                GLOB_RECURSE
                ${ModuleName}.PublicCPPFiles
                "${${ModuleName}.Source.PublicDirectory}/*.cpp"
                "${${ModuleName}.GeneratedSource.PublicDirectory}/*.cpp"
            )
            foreach(PublicCPPFile ${${ModuleName}.PublicCPPFiles})
                message(FATAL_ERROR ".cpp files are not allowed to be public")
            endforeach()

            # Check if there is any public c files
            file(
                GLOB_RECURSE
                ${ModuleName}.PublicCFiles
                "${${ModuleName}.Source.PublicDirectory}/*.c"
                "${${ModuleName}.GeneratedSource.PublicDirectory}/*.c"
            )
            foreach(PublicCFile ${${ModuleName}.PublicCFiles})
                message(FATAL_ERROR ".c files are not allowed to be public")
            endforeach()

            # Check if there is any interface cpp files
            file(
                GLOB_RECURSE
                ${ModuleName}.InterfaceCPPFiles
                "${${ModuleName}.Source.InterfaceDirectory}/*.cpp"
                "${${ModuleName}.GeneratedSource.InterfaceDirectory}/*.cpp"
            )
            foreach(InterfaceCPPFile ${${ModuleName}.InterfaceCPPFiles})
                message(FATAL_ERROR ".cpp files are not allowed to be interface")
            endforeach()

            # Check if there is any interface c files
            file(
                GLOB_RECURSE
                ${ModuleName}.InterfaceCFiles
                "${${ModuleName}.Source.InterfaceDirectory}/*.c"
                "${${ModuleName}.GeneratedSource.InterfaceDirectory}/*.c"
            )
            foreach(InterfaceCFile ${${ModuleName}.InterfaceCFiles})
                message(FATAL_ERROR ".c files are not allowed to be interface")
            endforeach()

            # Check if there is any public mm files
            file(
                GLOB_RECURSE
                ${ModuleName}.PublicMMFiles
                "${${ModuleName}.Source.PublicDirectory}/*.mm"
                "${${ModuleName}.GeneratedSource.PublicDirectory}/*.mm"
            )
            foreach(PublicMMFile ${${ModuleName}.PublicMMFiles})
                message(FATAL_ERROR ".mm files are not allowed to be public")
            endforeach()

            # Check if there is any interface mm files
            file(
                GLOB_RECURSE
                ${ModuleName}.InterfaceMMFiles
                "${${ModuleName}.Source.InterfaceDirectory}/*.mm"
                "${${ModuleName}.GeneratedSource.InterfaceDirectory}/*.mm"
            )
            foreach(InterfaceMMFile ${${ModuleName}.InterfaceMMFiles})
                message(FATAL_ERROR ".mm files are not allowed to be interface")
            endforeach()

            # Find PCH files
            file(
                GLOB_RECURSE
                ${ModuleName}.PublicPCHFiles
                "${${ModuleName}.Source.PublicDirectory}/*.pch.hpp"
                "${${ModuleName}.Source.PublicDirectory}/*.pch.h"
                "${${ModuleName}.GeneratedSource.PublicDirectory}/*.pch.hpp"
                "${${ModuleName}.GeneratedSource.PublicDirectory}/*.pch.h"
            )
            file(
                GLOB_RECURSE
                ${ModuleName}.PrivatePCHFiles
                "${${ModuleName}.Source.PrivateDirectory}/*.pch.hpp"
                "${${ModuleName}.Source.PrivateDirectory}/*.pch.h"
                "${${ModuleName}.GeneratedSource.PrivateDirectory}/*.pch.hpp"
                "${${ModuleName}.GeneratedSource.PrivateDirectory}/*.pch.h"
            )
            file(
                GLOB_RECURSE
                ${ModuleName}.InterfacePCHFiles
                "${${ModuleName}.Source.InterfaceDirectory}/*.pch.hpp"
                "${${ModuleName}.Source.InterfaceDirectory}/*.pch.h"
                "${${ModuleName}.GeneratedSource.InterfaceDirectory}/*.pch.hpp"
                "${${ModuleName}.GeneratedSource.InterfaceDirectory}/*.pch.h"
            )

            # Find private files
            file(
                GLOB_RECURSE
                ${ModuleName}.PrivateHeaderAndInlFiles
                "${${ModuleName}.Source.PrivateDirectory}/*.hpp"
                "${${ModuleName}.Source.PrivateDirectory}/*.h"
                "${${ModuleName}.Source.PrivateDirectory}/*.inl"
                "${${ModuleName}.GeneratedSource.PrivateDirectory}/*.hpp"
                "${${ModuleName}.GeneratedSource.PrivateDirectory}/*.h"
                "${${ModuleName}.GeneratedSource.PrivateDirectory}/*.inl"
            )
            list(APPEND ${ModuleName}.PrivateHeaderAndInlFiles ${${ModuleName}.GeneratedSource.PrivateConfigFiles})

            # Find interface files
            file(
                GLOB_RECURSE
                ${ModuleName}.InterfaceHeaderAndInlFiles
                "${${ModuleName}.Source.InterfaceDirectory}/*.hpp"
                "${${ModuleName}.Source.InterfaceDirectory}/*.h"
                "${${ModuleName}.Source.InterfaceDirectory}/*.inl"
                "${${ModuleName}.GeneratedSource.InterfaceDirectory}/*.hpp"
                "${${ModuleName}.GeneratedSource.InterfaceDirectory}/*.h"
                "${${ModuleName}.GeneratedSource.InterfaceDirectory}/*.inl"
            )
            list(APPEND ${ModuleName}.InterfaceHeaderAndInlFiles ${${ModuleName}.GeneratedSource.InterfaceConfigFiles})

            # Find cpp files
            file(
                GLOB_RECURSE
                ${ModuleName}.PrivateCPPFiles
                "${${ModuleName}.Source.PrivateDirectory}/*.cpp"
                "${${ModuleName}.GeneratedSource.PrivateDirectory}/*.cpp"
            )

            # Find c files
            file(
                GLOB_RECURSE
                ${ModuleName}.PrivateCFiles
                "${${ModuleName}.Source.PrivateDirectory}/*.c"
                "${${ModuleName}.GeneratedSource.PrivateDirectory}/*.c"
            )

            # Find mm files
            file(
                GLOB_RECURSE
                ${ModuleName}.PrivateMMFiles
                "${${ModuleName}.Source.PrivateDirectory}/*.mm"
                "${${ModuleName}.GeneratedSource.PrivateDirectory}/*.mm"
            )

            # Find source files
            file(
                GLOB_RECURSE
                ${ModuleName}.Source.Files
                "${${ModuleName}.Source.Directory}/*.cpp"
                "${${ModuleName}.Source.Directory}/*.mm"
                "${${ModuleName}.Source.Directory}/*.hpp"
                "${${ModuleName}.Source.Directory}/*.h"
                "${${ModuleName}.Source.Directory}/*.inl"
                "${${ModuleName}.Source.Directory}/*.config"
            )
            source_group(TREE ${${ModuleName}.Source.Directory} PREFIX Source/ FILES ${${ModuleName}.Source.Files})

            # Find generated source files
            file(
                GLOB_RECURSE
                ${ModuleName}.GeneratedSource.Files
                "${${ModuleName}.GeneratedSource.Directory}/*.cpp"
                "${${ModuleName}.GeneratedSource.Directory}/*.mm"
                "${${ModuleName}.GeneratedSource.Directory}/*.hpp"
                "${${ModuleName}.GeneratedSource.Directory}/*.h"
                "${${ModuleName}.GeneratedSource.Directory}/*.inl"
                "${${ModuleName}.GeneratedSource.Directory}/*.config"
            )
            source_group(TREE ${${ModuleName}.GeneratedSource.Directory} PREFIX GeneratedSource/ FILES ${${ModuleName}.GeneratedSource.Files})

            #
            message(STATUS ${ModuleFullName}.Enable: ${${ModuleName}.Enable})
            message(STATUS ${ModuleFullName}.EnableUnityBuild: ${${ModuleName}.EnableUnityBuild})
            message(STATUS ${ModuleFullName}.MacroName: ${${ModuleName}.MacroName})
            message(STATUS ${ModuleFullName}.Type: ${${ModuleName}.Type})
            message(STATUS ${ModuleFullName}.CanParentDependOn: ${${ModuleName}.CanParentDependOn})
            message(STATUS ${ModuleFullName}.PrivateDependencies:)
            foreach(PrivateDependency ${${ModuleName}.PrivateDependencies})
                message(STATUS "    ${PrivateDependency}")
            endforeach()
            message(STATUS ${ModuleFullName}.PublicDependencies:)
            foreach(PublicDependency ${${ModuleName}.PublicDependencies})
                message(STATUS "    ${PublicDependency}")
            endforeach()
            message(STATUS ${ModuleFullName}.Depends:)
            foreach(Depend ${${ModuleName}.Depends})
                message(STATUS "    ${Depend}")
            endforeach()
            message(STATUS ${ModuleFullName}.Commands:)
            foreach(Command ${${ModuleName}.Commands})
                message(STATUS "    ${Command}")
            endforeach()
            message(STATUS ${ModuleFullName}.PrivateDefinitions:)
            foreach(Definition ${${ModuleName}.PrivateDefinitions})
                message(STATUS "    ${Definition}")
            endforeach()
            message(STATUS ${ModuleFullName}.InterfaceDefinitions:)
            foreach(Definition ${${ModuleName}.InterfaceDefinitions})
                message(STATUS "    ${Definition}")
            endforeach()
            message(STATUS ${ModuleFullName}.PublicDefinitions:)
            foreach(Definition ${${ModuleName}.PublicDefinitions})
                message(STATUS "    ${Definition}")
            endforeach()
            message(STATUS ${ModuleFullName}.AdditionalSourceFiles:)
            foreach(AdditionalSourceFile ${${ModuleName}.AdditionalSourceFiles})
                message(STATUS "    ${AdditionalSourceFile}")
            endforeach()

            if(NOT ${ModuleName}.Enable)
                continue()
            endif()

            # Setup dependencies
            set(${ModuleName}.Dependencies "")
            list(APPEND ${ModuleName}.Dependencies ${${ModuleName}.PrivateDependencies})
            list(APPEND ${ModuleName}.Dependencies ${${ModuleName}.PublicDependencies})

            Abytek_ApplyGlobal(${ModuleName}.Enable)
            Abytek_ApplyGlobal(${ModuleName}.EnableUnityBuild)
            Abytek_ApplyGlobal(${ModuleName}.MacroName)
            Abytek_ApplyGlobal(${ModuleName}.Type)
            Abytek_ApplyGlobal(${ModuleName}.CanParentDependOn)
            Abytek_ApplyGlobal(${ModuleName}.PrivateDependencies)
            Abytek_ApplyGlobal(${ModuleName}.PublicDependencies)
            Abytek_ApplyGlobal(${ModuleName}.Depends)
            Abytek_ApplyGlobal(${ModuleName}.Commands)
            Abytek_ApplyGlobal(${ModuleName}.PrivateDefinitions)
            Abytek_ApplyGlobal(${ModuleName}.InterfaceDefinitions)
            Abytek_ApplyGlobal(${ModuleName}.PublicDefinitions)
            Abytek_ApplyGlobal(${ModuleName}.Modules.DefaultType)
            Abytek_ApplyGlobal(${ModuleName}.IsCompilable)
            Abytek_ApplyGlobal(${ModuleName}.Dependencies)
            Abytek_ApplyGlobal(${ModuleName}.GeneratedSource.PrivateConfigFiles)
            Abytek_ApplyGlobal(${ModuleName}.GeneratedSource.InterfaceConfigFiles)
            Abytek_ApplyGlobal(${ModuleName}.GeneratedSource.PublicConfigFiles)
            Abytek_ApplyGlobal(${ModuleName}.GeneratedSource.ConfigFiles)
            Abytek_ApplyGlobal(${ModuleName}.GeneratedSource.PrivateConfigFiles)
            Abytek_ApplyGlobal(${ModuleName}.GeneratedSource.InterfaceConfigFiles)
            Abytek_ApplyGlobal(${ModuleName}.GeneratedSource.PublicConfigFiles)
            Abytek_ApplyGlobal(${ModuleName}.PublicHeaderAndInlFiles)
            Abytek_ApplyGlobal(${ModuleName}.PublicPCHFiles)
            Abytek_ApplyGlobal(${ModuleName}.PrivatePCHFiles)
            Abytek_ApplyGlobal(${ModuleName}.InterfacePCHFiles)
            Abytek_ApplyGlobal(${ModuleName}.PrivateHeaderAndInlFiles)
            Abytek_ApplyGlobal(${ModuleName}.InterfaceHeaderAndInlFiles)
            Abytek_ApplyGlobal(${ModuleName}.AdditionalSourceFiles)
            Abytek_ApplyGlobal(${ModuleName}.PrivateCPPFiles)
            Abytek_ApplyGlobal(${ModuleName}.PrivateCFiles)
            Abytek_ApplyGlobal(${ModuleName}.PrivateMMFiles)
            Abytek_ApplyGlobal(${ModuleName}.Source.Files)
            Abytek_ApplyGlobal(${ModuleName}.Source.ConfigFiles)
            Abytek_ApplyGlobal(${ModuleName}.GeneratedSource.Files)

            Abytek_SetGlobal(${ModuleFullName}.Enable "${${ModuleName}.Enable}")
            Abytek_SetGlobal(${ModuleFullName}.EnableUnityBuild "${${ModuleName}.EnableUnityBuild}")
            Abytek_SetGlobal(${ModuleFullName}.MacroName "${${ModuleName}.MacroName}")
            Abytek_SetGlobal(${ModuleFullName}.Type "${${ModuleName}.Type}")
            Abytek_SetGlobal(${ModuleFullName}.CanParentDependOn "${${ModuleName}.CanParentDependOn}")
            Abytek_SetGlobal(${ModuleFullName}.PrivateDependencies "${${ModuleName}.PrivateDependencies}")
            Abytek_SetGlobal(${ModuleFullName}.PublicDependencies "${${ModuleName}.PublicDependencies}")
            Abytek_SetGlobal(${ModuleFullName}.Depends "${${ModuleName}.Depends}")
            Abytek_SetGlobal(${ModuleFullName}.Commands "${${ModuleName}.Commands}")
            Abytek_SetGlobal(${ModuleFullName}.PrivateDefinitions "${${ModuleName}.PrivateDefinitions}")
            Abytek_SetGlobal(${ModuleFullName}.InterfaceDefinitions "${${ModuleName}.InterfaceDefinitions}")
            Abytek_SetGlobal(${ModuleFullName}.PublicDefinitions "${${ModuleName}.PublicDefinitions}")
            Abytek_SetGlobal(${ModuleFullName}.Modules.DefaultType "${${ModuleName}.Modules.DefaultType}")
            Abytek_SetGlobal(${ModuleFullName}.IsCompilable "${${ModuleName}.IsCompilable}")
            Abytek_SetGlobal(${ModuleFullName}.Dependencies "${${ModuleName}.Dependencies}")
            Abytek_SetGlobal(${ModuleFullName}.GeneratedSource.PrivateConfigFiles "${${ModuleName}.GeneratedSource.PrivateConfigFiles}")
            Abytek_SetGlobal(${ModuleFullName}.GeneratedSource.InterfaceConfigFiles "${${ModuleName}.GeneratedSource.InterfaceConfigFiles}")
            Abytek_SetGlobal(${ModuleFullName}.GeneratedSource.PublicConfigFiles "${${ModuleName}.GeneratedSource.PublicConfigFiles}")
            Abytek_SetGlobal(${ModuleFullName}.GeneratedSource.ConfigFiles "${${ModuleName}.GeneratedSource.ConfigFiles}")
            Abytek_SetGlobal(${ModuleFullName}.GeneratedSource.PrivateConfigFiles "${${ModuleName}.GeneratedSource.PrivateConfigFiles}")
            Abytek_SetGlobal(${ModuleFullName}.GeneratedSource.InterfaceConfigFiles "${${ModuleName}.GeneratedSource.InterfaceConfigFiles}")
            Abytek_SetGlobal(${ModuleFullName}.GeneratedSource.PublicConfigFiles "${${ModuleName}.GeneratedSource.PublicConfigFiles}")
            Abytek_SetGlobal(${ModuleFullName}.PublicHeaderAndInlFiles "${${ModuleName}.PublicHeaderAndInlFiles}")
            Abytek_SetGlobal(${ModuleFullName}.PublicPCHFiles "${${ModuleName}.PublicPCHFiles}")
            Abytek_SetGlobal(${ModuleFullName}.PrivatePCHFiles "${${ModuleName}.PrivatePCHFiles}")
            Abytek_SetGlobal(${ModuleFullName}.InterfacePCHFiles "${${ModuleName}.InterfacePCHFiles}")
            Abytek_SetGlobal(${ModuleFullName}.PrivateHeaderAndInlFiles "${${ModuleName}.PrivateHeaderAndInlFiles}")
            Abytek_SetGlobal(${ModuleFullName}.InterfaceHeaderAndInlFiles "${${ModuleName}.InterfaceHeaderAndInlFiles}")
            Abytek_SetGlobal(${ModuleFullName}.AdditionalSourceFiles "${${ModuleName}.AdditionalSourceFiles}")
            Abytek_SetGlobal(${ModuleFullName}.PrivateCPPFiles "${${ModuleName}.PrivateCPPFiles}")
            Abytek_SetGlobal(${ModuleFullName}.PrivateCFiles "${${ModuleName}.PrivateCFiles}")
            Abytek_SetGlobal(${ModuleFullName}.PrivateMMFiles "${${ModuleName}.PrivateMMFiles}")
            Abytek_SetGlobal(${ModuleFullName}.Source.Files "${${ModuleName}.Source.Files}")
            Abytek_SetGlobal(${ModuleFullName}.Source.ConfigFiles "${${ModuleName}.Source.ConfigFiles}")
            Abytek_SetGlobal(${ModuleFullName}.GeneratedSource.Files "${${ModuleName}.GeneratedSource.Files}")

            # write JSModuleFile
            file(
                WRITE
                "${${ModuleFullName}.JSModuleFile}"
                "
                    const path = require(\"path\");
                    const fs = require(\"fs\");
                    
                    module.exports = (Context) => {
                        if(\"${ModuleFullName}\" in Context.Modules)
                        {
                            return Context.Modules[\"${ModuleFullName}\"];
                        }
                        class F_Module 
                        {
                            constructor()
                            {
                                this.Context = Context;
                            
                                this.Name = \"${ModuleFullName}\";
                                this.Enable = \"${${ModuleFullName}.Enable}\";
                                this.Enable = (this.Enable == 'ON') ? true : this.Enable;
                                this.Enable = (this.Enable == 'OFF') ? false : this.Enable;
                                this.Enable = (this.Enable == 'YES') ? true : this.Enable;
                                this.Enable = (this.Enable == 'NO') ? false : this.Enable;
                                this.Enable = (this.Enable != false);
                                this.EnableUnityBuild = \"${${ModuleFullName}.EnableUnityBuild}\";
                                this.EnableUnityBuild = (this.EnableUnityBuild == 'ON') ? true : this.EnableUnityBuild;
                                this.EnableUnityBuild = (this.EnableUnityBuild == 'OFF') ? false : this.EnableUnityBuild;
                                this.EnableUnityBuild = (this.EnableUnityBuild == 'YES') ? true : this.EnableUnityBuild;
                                this.EnableUnityBuild = (this.EnableUnityBuild == 'NO') ? false : this.EnableUnityBuild;
                                this.EnableUnityBuild = (this.EnableUnityBuild != false);
                                this.MacroName = \"${${ModuleFullName}.MacroName}\";
                                this.Type = \"${${ModuleFullName}.Type}\";
                                this.CanParentDependOn = \"${${ModuleFullName}.CanParentDependOn}\";
                                this.Directory = \"${${ModuleFullName}.Directory}\";
                                this.BuildDirectory = \"${${ModuleFullName}.BuildDirectory}\";
                                this.Modules = require(\"${${ModuleFullName}.JSModulesFile}\")(Context);
                                this.Modules.Private = require(\"${${ModuleFullName}.JSPrivateModulesFile}\")(Context);
                                this.Modules.Public = require(\"${${ModuleFullName}.JSPublicModulesFile}\")(Context);
                                this.Modules.Directory = \"${${ModuleFullName}.Modules.Directory}\";
                                this.Modules.InterfaceDirectory = \"${${ModuleFullName}.Modules.InterfaceDirectory}\";
                                this.Modules.PrivateDirectory = \"${${ModuleFullName}.Modules.PrivateDirectory}\";
                                this.Modules.PublicDirectory = \"${${ModuleFullName}.Modules.PublicDirectory}\";
                                this.Source = new Object();
                                this.Source.Directory = \"${${ModuleFullName}.Source.Directory}\";
                                this.Source.InterfaceDirectory = \"${${ModuleFullName}.Source.InterfaceDirectory}\";
                                this.Source.PrivateDirectory = \"${${ModuleFullName}.Source.PrivateDirectory}\";
                                this.Source.PublicDirectory = \"${${ModuleFullName}.Source.PublicDirectory}\";
                                this.AutoCopy = new Object();
                                this.AutoCopy.Directory = \"${${ModuleFullName}.AutoCopy.Directory}\";
                                this.ThirdParty = new Object();
                                this.ThirdParty.Directory = \"${${ModuleFullName}.ThirdParty.Directory}\";
                                this.GeneratedSource = new Object();
                                this.GeneratedSource.Directory = \"${${ModuleFullName}.GeneratedSource.Directory}\";
                                this.GeneratedSource.TempDirectory = \"${${ModuleFullName}.GeneratedSource.TempDirectory}\";
                                this.GeneratedSource.InterfaceDirectory = \"${${ModuleFullName}.GeneratedSource.InterfaceDirectory}\";
                                this.GeneratedSource.PrivateDirectory = \"${${ModuleFullName}.GeneratedSource.PrivateDirectory}\";
                                this.GeneratedSource.PublicDirectory = \"${${ModuleFullName}.GeneratedSource.PublicDirectory}\";
                                this.CMakeDirectory = \"${${ModuleFullName}.CMakeDirectory}\";
                                this.JSDirectory = \"${${ModuleFullName}.JSDirectory}\";
                                this.GeneratedJSDirectory = \"${${ModuleFullName}.GeneratedJSDirectory}\";
                                this.JSModuleFile = \"${${ModuleFullName}.JSModuleFile}\";
                                this.JSModulesFile = \"${${ModuleFullName}.JSModulesFile}\";
                                this.JSPrivateModulesFile = \"${${ModuleFullName}.JSPrivateModulesFile}\";
                                this.JSPublicModulesFile = \"${${ModuleFullName}.JSPublicModulesFile}\";
                                this.JSDependenciesFile = \"${${ModuleFullName}.JSDependenciesFile}\";
                                this.JSPrivateDependenciesFile = \"${${ModuleFullName}.JSPrivateDependenciesFile}\";
                                this.JSPublicDependenciesFile = \"${${ModuleFullName}.JSPublicDependenciesFile}\";
                                this.Dependencies = require(\"${${ModuleFullName}.JSDependenciesFile}\")(Context);
                                this.Dependencies.Private = require(\"${${ModuleFullName}.JSPrivateDependenciesFile}\")(Context);
                                this.Dependencies.Public = require(\"${${ModuleFullName}.JSPublicDependenciesFile}\")(Context);
                            }
                            
                            GetResolvedGeneratorExpression(Key, Config, Required = true)
                            {
                                let JSONFile = path.join(this.GeneratedJSDirectory, `${ModuleFullName}.ResolvedGeneratorExpressions.\$\{Key\}.\$\{Config\}.json`);
                                if (!fs.existsSync(JSONFile))
                                {
                                    if (Required)
                                    {
                                        throw new Error(`Not found \$\{JSONFile\}, require building module`);
                                    }
                                    return null;
                                }
                                return require(JSONFile);
                            }
                            
                            FindScript(RelativePath, IsTopLevel, History)
                            {
                                if (History == null)
                                {
                                    History = new Object();
                                }
                            
                                {
                                    const SamplePath = path.join(this.JSDirectory, RelativePath);
                                    if (fs.existsSync(SamplePath))
                                    {
                                        return require(SamplePath);
                                    }
                                }
                                {
                                    const SamplePath = path.join(this.GeneratedJSDirectory, RelativePath);
                                    if (fs.existsSync(SamplePath))
                                    {
                                        return require(SamplePath);
                                    }
                                }
                                if((IsTopLevel == true) || (IsTopLevel == null))
                                {
                                    for(let ModuleName of this.Modules.Private)
                                    {
                                        if(ModuleName in History)
                                        {
                                            continue;
                                        }
                                        History[ModuleName] = true;
                                        let Result = Context.RequireModule(ModuleName).FindScript(RelativePath, false, History);
                                        if(Result != null)
                                        {
                                            return Result;
                                        }
                                    }
                                    for(let ModuleName of this.Dependencies.Private)
                                    {
                                        if(ModuleName in History)
                                        {
                                            continue;
                                        }
                                        History[ModuleName] = true;
                                        let Result = Context.RequireModule(ModuleName).FindScript(RelativePath, false, History);
                                        if(Result != null)
                                        {
                                            return Result;
                                        }
                                    }
                                }
                                for(let ModuleName of this.Modules.Public)
                                {
                                    if(ModuleName in History)
                                    {
                                        continue;
                                    }
                                    History[ModuleName] = true;
                                    let Result = Context.RequireModule(ModuleName).FindScript(RelativePath, false, History);
                                    if(Result != null)
                                    {
                                        return Result;
                                    }
                                }
                                for(let ModuleName of this.Dependencies.Public)
                                {
                                    if(ModuleName in History)
                                    {
                                        continue;
                                    }
                                    History[ModuleName] = true;
                                    let Result = Context.RequireModule(ModuleName).FindScript(RelativePath, false, History);
                                    if(Result != null)
                                    {
                                        return Result;
                                    }
                                }
                                return null;
                            }
                            ExecuteScript(RelativePath, CLIArgs)
                            {
                                let Script = this.FindScript(RelativePath);
                                if(Script == null)
                                {
                                    throw new Error(`Not found script \${this.Name}::\${RelativePath}`);
                                }
                                return Script(this, CLIArgs);
                            }
                            ImportScript(RelativePath)
                            {
                                let Script = this.FindScript(RelativePath);
                                if(Script == null)
                                {
                                    throw new Error(`Not found script \${this.Name}::\${RelativePath}`);
                                }
                                return Script(this);
                            }
                            ScanScripts(RelativeDirectoryPath, IsTopLevel, History)
                            {
                                if (History == null)
                                {
                                    History = new Object();
                                }

                                let ItemFiles = [];
                                    
                                const DirectoryPath = path.join(this.JSDirectory, RelativeDirectoryPath);
                                if(fs.existsSync(DirectoryPath))
                                {
                                    ItemFiles = ItemFiles.concat(
                                        fs.readdirSync(
                                            DirectoryPath, 
                                            { withFileTypes: true }
                                        )
                                        .filter(dirent => dirent.isFile())
                                        .map(dirent => path.join(RelativeDirectoryPath, dirent.name))
                                    );
                                }
                                    
                                if((IsTopLevel == true) || (IsTopLevel == null))
                                {
                                    for(let ModuleName of this.Modules.Private)
                                    {
                                        if(ModuleName in History)
                                        {
                                            continue;
                                        }
                                        History[ModuleName] = true;
                                        ItemFiles = ItemFiles.concat(
                                            Context.RequireModule(ModuleName).ScanScripts(RelativeDirectoryPath, false, History)
                                        );
                                    }
                                    for(let ModuleName of this.Dependencies.Private)
                                    {
                                        if(ModuleName in History)
                                        {
                                            continue;
                                        }
                                        History[ModuleName] = true;
                                        ItemFiles = ItemFiles.concat(
                                            Context.RequireModule(ModuleName).ScanScripts(RelativeDirectoryPath, false, History)
                                        );
                                    }
                                }
                                for(let ModuleName of this.Modules.Public)
                                {
                                    if(ModuleName in History)
                                    {
                                        continue;
                                    }
                                    History[ModuleName] = true;
                                    ItemFiles = ItemFiles.concat(
                                        Context.RequireModule(ModuleName).ScanScripts(RelativeDirectoryPath, false, History)
                                    );
                                }
                                for(let ModuleName of this.Dependencies.Public)
                                {
                                    if(ModuleName in History)
                                    {
                                        continue;
                                    }
                                    History[ModuleName] = true;
                                    ItemFiles = ItemFiles.concat(
                                        Context.RequireModule(ModuleName).ScanScripts(RelativeDirectoryPath, false, History)
                                    );
                                }

                                return ItemFiles;
                            }
                            ScanAndImportScripts(RelativeDirectoryPath)
                            {
                                let ScriptPaths = this.ScanScripts(RelativeDirectoryPath);
                                let Result = [];
                                for(let ScriptPath of ScriptPaths)
                                {
                                    Result.push(
                                        this.ImportScript(ScriptPath)
                                    );
                                }
                                return Result;
                            }
                            
                            MakeAutoEnum(...Keys)
                            {
                                let Result = new Object();
                                for(let Key of Keys)
                                    Result[Key] = Key;
                                return Result;
                            }
                        }
                        const Module = new F_Module();
                        Context.Modules[\"${ModuleFullName}\"] = Module;
                        return Module;
                    };
                "
            )

            Abytek_SetupThirdParty(ABYTEK_MODULE_PHASE_INIT)

            message(STATUS "")
        endforeach()
    endif()



    # Late init phase
    if(PARGS_PHASE_ALL OR PARGS_PHASE STREQUAL LATE_INIT)
        foreach(ModuleName ${MODULE_NAMES})
            Abytek_CalculateModuleFullName(ModuleFullName "${PARGS_BASE_NAMESPACE}" ${ModuleName})

            set(CurrentModule "${ModuleName}")
            set(CurrentModuleFullName "${ModuleFullName}")
            set(CurrentTarget "${ModuleFullName}")

            if(NOT ${ModuleFullName}.Enable)
                continue()
            endif()

            # Phase: ABYTEK_MODULE_PHASE_LATE_INIT
            set(ABYTEK_MODULE_PHASE_LATE_INIT ON)
            message(STATUS "ABYTEK_MODULE_PHASE_LATE_INIT::BEGIN(${ModuleFullName})")
            foreach(PreScript ${ABYTEK_SETUP_PRE_SCRIPTS})
                include("${PreScript}")
            endforeach()
            set(ModuleSetupPath "${${ModuleFullName}.Directory}/Setup.cmake")
            if(EXISTS "${ModuleSetupPath}")
                include("${ModuleSetupPath}")
            endif()
            message(STATUS "ABYTEK_MODULE_PHASE_LATE_INIT::END(${ModuleFullName})")
            set(ABYTEK_MODULE_PHASE_LATE_INIT OFF)

            Abytek_SetupThirdParty(ABYTEK_MODULE_PHASE_LATE_INIT)

            message(STATUS "")
        endforeach()

        foreach(ModuleName ${MODULE_NAMES})
            Abytek_CalculateModuleFullName(ModuleFullName "${PARGS_BASE_NAMESPACE}" ${ModuleName})

            set(CurrentModule "${ModuleName}")
            set(CurrentModuleFullName "${ModuleFullName}")
            set(CurrentTarget "${ModuleFullName}")

            foreach(ConfigFile ${${CurrentModuleFullName}.Source.ConfigFiles})
                file(
                    RELATIVE_PATH
                        ConfigFileRelativePath
                        "${${CurrentModuleFullName}.Source.Directory}"
                        "${ConfigFile}"
                )

                file(READ "${ConfigFile}" ConfigFileContent)

                set(GeneratedConfigFile "${${CurrentModuleFullName}.GeneratedSource.Directory}/${ConfigFileRelativePath}.hpp")
                set(TempGeneratedConfigFile "${${CurrentModuleFullName}.GeneratedSource.TempDirectory}/${ConfigFileRelativePath}.hpp")

                file(CONFIGURE OUTPUT "${TempGeneratedConfigFile}" CONTENT "${ConfigFileContent}")
                file(READ "${TempGeneratedConfigFile}" GeneratedConfigFileContent)

                set(NeedToCopy YES)
                if(EXISTS "${GeneratedConfigFile}")
                    file(READ "${GeneratedConfigFile}" LastGeneratedConfigFileContent)
                    if("${GeneratedConfigFileContent}" STREQUAL "${LastGeneratedConfigFileContent}")
                        set(NeedToCopy NO)
                    endif()
                endif()

                if(${NeedToCopy})
                    file(WRITE "${GeneratedConfigFile}" "${GeneratedConfigFileContent}")
                endif()
            endforeach()
        endforeach()
    endif()



    # After targets created
    if(PARGS_PHASE_ALL OR PARGS_PHASE STREQUAL TARGET_CREATED)
        foreach(ModuleName ${MODULE_NAMES})
            Abytek_CalculateModuleFullName(ModuleFullName "${PARGS_BASE_NAMESPACE}" ${ModuleName})

            set(CurrentModule "${ModuleName}")
            set(CurrentModuleFullName "${ModuleFullName}")
            set(CurrentTarget "${ModuleFullName}")

            if(NOT ${ModuleFullName}.Enable)
                continue()
            endif()

            set(PrivateMMFilesSearch "")
            if(APPLE)
                set(PrivateMMFilesSearch ${${ModuleFullName}.PrivateMMFiles})
            endif()

            set(TargetFileExactor "")
            set(TargetFileDirectoryExactor "")
            
            # Create module target
            if("${${ModuleFullName}.Type}" STREQUAL EXECUTABLE)
                add_executable(
                    ${ModuleFullName}
                    ${${ModuleFullName}.PrivateCPPFiles}
                    ${${ModuleFullName}.PrivateCFiles}
                    ${PrivateMMFilesSearch}
                    ${${ModuleFullName}.PrivateHeaderAndInlFiles}
                    ${${ModuleFullName}.InterfaceHeaderAndInlFiles}
                    ${${ModuleFullName}.PublicHeaderAndInlFiles}
                    ${${ModuleFullName}.AdditionalSourceFiles}
                )
                set(TargetFileExactor "$<TARGET_FILE:${ModuleFullName}>")
                set(TargetFileDirectoryExactor "$<TARGET_FILE_DIR:${ModuleFullName}>")
            elseif("${${ModuleFullName}.Type}" STREQUAL STATIC)
                add_library(
                    ${ModuleFullName} STATIC
                    ${${ModuleFullName}.PrivateCPPFiles}
                    ${${ModuleFullName}.PrivateCFiles}
                    ${PrivateMMFilesSearch}
                    ${${ModuleFullName}.PrivateHeaderAndInlFiles}
                    ${${ModuleFullName}.InterfaceHeaderAndInlFiles}
                    ${${ModuleFullName}.PublicHeaderAndInlFiles}
                    ${${ModuleFullName}.AdditionalSourceFiles}
                )
                set(TargetFileExactor "$<TARGET_FILE:${ModuleFullName}>")
                set(TargetFileDirectoryExactor "$<TARGET_FILE_DIR:${ModuleFullName}>")
                if(NOT ${${ModuleFullName}.MacroName} STREQUAL "")
                    target_compile_definitions(${ModuleFullName}
                        PUBLIC
                            -D${${ModuleFullName}.MacroName}_API=
                            -D${${ModuleFullName}.MacroName}_LOCAL=
                    )
                endif()
            elseif("${${ModuleFullName}.Type}" STREQUAL OBJECT)
                add_library(
                    ${ModuleFullName} OBJECT
                    ${${ModuleFullName}.PrivateCPPFiles}
                    ${${ModuleFullName}.PrivateCFiles}
                    ${PrivateMMFilesSearch}
                    ${${ModuleFullName}.PrivateHeaderAndInlFiles}
                    ${${ModuleFullName}.InterfaceHeaderAndInlFiles}
                    ${${ModuleFullName}.PublicHeaderAndInlFiles}
                    ${${ModuleFullName}.AdditionalSourceFiles}
                )
                if(NOT ${${ModuleFullName}.MacroName} STREQUAL "")
                    target_compile_definitions(${ModuleFullName}
                        PUBLIC
                            -D${${ModuleFullName}.MacroName}_API=
                            -D${${ModuleFullName}.MacroName}_LOCAL=
                    )
                endif()
            elseif("${${ModuleFullName}.Type}" STREQUAL INTERFACE)
                add_library(
                    ${ModuleFullName} INTERFACE
                )
                target_sources(
                    ${ModuleFullName}
                    INTERFACE
                        ${${ModuleFullName}.InterfaceHeaderAndInlFiles}
                        ${${ModuleFullName}.AdditionalSourceFiles}
                )
                if(NOT ${${ModuleFullName}.MacroName} STREQUAL "")
                    target_compile_definitions(${ModuleFullName}
                        INTERFACE
                            -D${${ModuleFullName}.MacroName}_API=
                            -D${${ModuleFullName}.MacroName}_LOCAL=
                    )
                endif()
            elseif("${${ModuleFullName}.Type}" STREQUAL SHARED)
                add_library(
                    ${ModuleFullName} SHARED
                    ${${ModuleFullName}.PrivateCPPFiles}
                    ${${ModuleFullName}.PrivateCFiles}
                    ${PrivateMMFilesSearch}
                    ${${ModuleFullName}.PrivateHeaderAndInlFiles}
                    ${${ModuleFullName}.InterfaceHeaderAndInlFiles}
                    ${${ModuleFullName}.PublicHeaderAndInlFiles}
                    ${${ModuleFullName}.AdditionalSourceFiles}
                )
                set(TargetFileExactor "$<TARGET_FILE:${ModuleFullName}>")
                set(TargetFileDirectoryExactor "$<TARGET_FILE_DIR:${ModuleFullName}>")
                if(NOT ${${ModuleFullName}.MacroName} STREQUAL "")
                    if(MSVC)
                        target_compile_definitions(${ModuleFullName}
                            PRIVATE
                                -D${${ModuleFullName}.MacroName}_API=ABYTEK_BUILD_SHARED_API_EXPORT_MSVC
                                -D${${ModuleFullName}.MacroName}_LOCAL=ABYTEK_BUILD_SHARED_LOCAL_EXPORT_MSVC
                            INTERFACE
                                -D${${ModuleFullName}.MacroName}_API=ABYTEK_BUILD_SHARED_API_IMPORT_MSVC
                                -D${${ModuleFullName}.MacroName}_LOCAL=ABYTEK_BUILD_SHARED_LOCAL_IMPORT_MSVC
                        )
                    elseif(CYGWIN)
                        target_compile_definitions(${ModuleFullName}
                            PRIVATE
                                -D${${ModuleFullName}.MacroName}_API=ABYTEK_BUILD_SHARED_API_EXPORT_CYGWIN
                                -D${${ModuleFullName}.MacroName}_LOCAL=ABYTEK_BUILD_SHARED_LOCAL_EXPORT_CYGWIN
                            INTERFACE
                                -D${${ModuleFullName}.MacroName}_API=ABYTEK_BUILD_SHARED_API_IMPORT_CYGWIN
                                -D${${ModuleFullName}.MacroName}_LOCAL=ABYTEK_BUILD_SHARED_LOCAL_IMPORT_CYGWIN
                        )
                    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 4)
                        target_compile_definitions(${ModuleFullName}
                            PRIVATE
                                -D${${ModuleFullName}.MacroName}_API=ABYTEK_BUILD_SHARED_API_EXPORT_GNU
                                -D${${ModuleFullName}.MacroName}_LOCAL=ABYTEK_BUILD_SHARED_LOCAL_EXPORT_GNU
                            INTERFACE
                                -D${${ModuleFullName}.MacroName}_API=ABYTEK_BUILD_SHARED_API_IMPORT_GNU
                                -D${${ModuleFullName}.MacroName}_LOCAL=ABYTEK_BUILD_SHARED_LOCAL_IMPORT_GNU
                        )
                    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
                        target_compile_definitions(${ModuleFullName}
                            PRIVATE
                                -D${${ModuleFullName}.MacroName}_API=ABYTEK_BUILD_SHARED_API_EXPORT_CLANG
                                -D${${ModuleFullName}.MacroName}_LOCAL=ABYTEK_BUILD_SHARED_LOCAL_EXPORT_CLANG
                            INTERFACE
                                -D${${ModuleFullName}.MacroName}_API=ABYTEK_BUILD_SHARED_API_IMPORT_CLANG
                                -D${${ModuleFullName}.MacroName}_LOCAL=ABYTEK_BUILD_SHARED_LOCAL_IMPORT_CLANG
                        )
                    else()
                        message(FATAL_ERROR "Unknown compiler")
                    endif()
                endif()
            elseif("${${ModuleFullName}.Type}" STREQUAL CUSTOM)
                add_custom_target(
                    ${ModuleFullName}
                    COMMAND ${${ModuleFullName}.Commands}
                    DEPENDS ${${ModuleFullName}.Depends}
                    SOURCES
                        ${${ModuleFullName}.Source.Files}
                        ${${ModuleFullName}.GeneratedSource.Files}
                        ${${ModuleFullName}.AdditionalSourceFiles}
                )
            endif()

            if(${ModuleFullName}.IsCompilable)
                Abytek_SetupCompilerConfigsForTarget(${CurrentTarget})
            endif()

            Abytek_TempDependenciesDirBase(${ModuleFullName} ${ModuleFullName}.TempDependenciesDirectoryBase)
            Abytek_ApplyGlobal(${ModuleFullName}.TempDependenciesDirectoryBase)
            if(EXISTS "${${ModuleFullName}.TempDependenciesDirectoryBase}")
                file(REMOVE_RECURSE "${${ModuleFullName}.TempDependenciesDirectoryBase}")
            endif()

            Abytek_TempDependenciesDir(${ModuleFullName} ${ModuleFullName}.TempDependenciesDirectory)
            Abytek_ApplyGlobal(${ModuleFullName}.TempDependenciesDirectory)
            
            if(EXISTS "${${ModuleFullName}.AutoCopy.Directory}")
                add_custom_command(TARGET ${ModuleFullName} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory
                    "${${ModuleFullName}.AutoCopy.Directory}"
                    "${ModuleFullName}.TempDependenciesDirectory"
                )
            endif()

            set(RuntimeOutputDirectory "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/Runtime/${ModuleFullName}")
            set(LibraryOutputDirectory "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/Libraries/${ModuleFullName}")
            set(ArchiveOutputDirectory "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/Archive/${ModuleFullName}")
            
            add_custom_command(TARGET ${ModuleFullName} PRE_BUILD
                COMMAND ${CMAKE_COMMAND} -E echo "\\\"${RuntimeOutputDirectory}\\\"" > "${${ModuleFullName}.GeneratedJSDirectory}/${ModuleFullName}.ResolvedGeneratorExpressions.RuntimeOutputDirectory.$<CONFIG>.json"
            )
            add_custom_command(TARGET ${ModuleFullName} PRE_BUILD
                COMMAND ${CMAKE_COMMAND} -E echo "\\\"${LibraryOutputDirectory}\\\"" > "${${ModuleFullName}.GeneratedJSDirectory}/${ModuleFullName}.ResolvedGeneratorExpressions.LibraryOutputDirectory.$<CONFIG>.json"
            )
            add_custom_command(TARGET ${ModuleFullName} PRE_BUILD
                COMMAND ${CMAKE_COMMAND} -E echo "\\\"${ArchiveOutputDirectory}\\\"" > "${${ModuleFullName}.GeneratedJSDirectory}/${ModuleFullName}.ResolvedGeneratorExpressions.ArchiveOutputDirectory.$<CONFIG>.json"
            )
            add_custom_command(TARGET ${ModuleFullName} PRE_BUILD
                COMMAND ${CMAKE_COMMAND} -E echo "\\\"${TargetFileExactor}\\\"" > "${${ModuleFullName}.GeneratedJSDirectory}/${ModuleFullName}.ResolvedGeneratorExpressions.TargetFile.$<CONFIG>.json"
            )
            add_custom_command(TARGET ${ModuleFullName} PRE_BUILD
                COMMAND ${CMAKE_COMMAND} -E echo "\\\"${TargetFileDirectoryExactor}\\\"" > "${${ModuleFullName}.GeneratedJSDirectory}/${ModuleFullName}.ResolvedGeneratorExpressions.TargetFileDirectory.$<CONFIG>.json"
            )

            set_target_properties(
                ${ModuleFullName}
                PROPERTIES
                    RUNTIME_OUTPUT_DIRECTORY "${RuntimeOutputDirectory}"
            )
            set_target_properties(
                ${ModuleFullName}
                PROPERTIES
                    LIBRARY_OUTPUT_DIRECTORY "${LibraryOutputDirectory}"
            )
            set_target_properties(
                ${ModuleFullName}
                PROPERTIES
                    ARCHIVE_OUTPUT_DIRECTORY "${ArchiveOutputDirectory}"
            )

            string(REPLACE "." "/" ParsedModuleFullName ${ModuleFullName})
            Abytek_GetModuleVSFolderNameAndLastName(
                ${ParsedModuleFullName}
                ModuleVSFolderName
                ModuleLastName
            )
            set_target_properties(${ModuleFullName}
                PROPERTIES
                    FOLDER "${ModuleVSFolderName}"
                    PROJECT_LABEL "${ModuleLastName}"
            )

            if(${ModuleFullName}.IsCompilable)
                # if (ABYTEK_EBT_CMAKE_GENERATOR_NINJA)
                #     set_property(TARGET ${ModuleFullName} PROPERTY JOB_POOL_COMPILE compile_pool)
                #     set_property(TARGET ${ModuleFullName} PROPERTY JOB_POOL_LINK compile_pool)
                # endif()

                if(ABYTEK_EBT_CMAKE_GENERATOR_NINJA AND ABYTEK_EBT_CMAKE_CONFIG_DEBUG)
                    target_compile_options(${ModuleFullName}
                        PUBLIC
                            # -----------------------------
                            # Core compiler flags
                            # -----------------------------
                            -O1
                            -g1
                            -fno-omit-frame-pointer
                            -fno-limit-debug-info
                            -fno-inline

                            # -----------------------------
                            # Split DWARF (reduce RAM peak)
                            # -----------------------------
                            -gsplit-dwarf

                            # -----------------------------
                            # Optional: reduce template explosion pressure
                            # -----------------------------
                            -fno-standalone-debug
                    )
                endif()
                
                target_precompile_headers(${ModuleFullName}
                    PRIVATE
                        ${${ModuleFullName}.PrivatePCHFiles}
                        ${${ModuleFullName}.InterfacePCHFiles}
                        ${${ModuleFullName}.PublicPCHFiles}
                )

                target_include_directories(${ModuleFullName}
                    PRIVATE
                        "${${ModuleFullName}.Source.PrivateDirectory}"
                        "${${ModuleFullName}.GeneratedSource.PrivateDirectory}"
                    INTERFACE
                        "${${ModuleFullName}.Source.InterfaceDirectory}"
                        "${${ModuleFullName}.GeneratedSource.InterfaceDirectory}"
                    PUBLIC
                        "${${ModuleFullName}.Source.PublicDirectory}"
                        "${${ModuleFullName}.GeneratedSource.PublicDirectory}"
                        "${GlobalSource.Directory}"
                )

                target_compile_definitions(${ModuleFullName}
                    PRIVATE
                        ${${ModuleFullName}.PrivateDefinitions}
                        -DABYTEK_MODULE=\"${ModuleFullName}\"
                        -DABYTEK_MODULE_DIRECTORY=\"${${ModuleFullName}.Directory}\"
                        # -DABYTEK_PROJECT_DIR=\"${ABYTEK_PROJECT_DIR}\"
                        # -DABYTEK_PROJECT_BUILD_DIR=\"${ABYTEK_PROJECT_BUILD_DIR}\"
                    INTERFACE
                        ${${ModuleFullName}.InterfaceDefinitions}
                    PUBLIC
                        ${${ModuleFullName}.PublicDefinitions}
                )
            endif()

            set_target_properties(${ModuleFullName}
                PROPERTIES
                    EXCLUDE_FROM_ALL TRUE
            )

            # Phase: ABYTEK_MODULE_PHASE_TARGET_CREATED
            set(ABYTEK_MODULE_PHASE_TARGET_CREATED ON)
            message(STATUS "ABYTEK_MODULE_PHASE_TARGET_CREATED::BEGIN(${ModuleFullName})")
            foreach(PreScript ${ABYTEK_SETUP_PRE_SCRIPTS})
                include("${PreScript}")
            endforeach()
            set(ModuleSetupPath "${${ModuleFullName}.Directory}/Setup.cmake")
            if(EXISTS "${ModuleSetupPath}")
                include("${ModuleSetupPath}")
            endif()
            message(STATUS "ABYTEK_MODULE_PHASE_TARGET_CREATED::END(${ModuleFullName})")
            set(ABYTEK_MODULE_PHASE_TARGET_CREATED OFF)

            if(${ModuleName}.PrivateDependencies)
                Abytek_SetGlobal(${ModuleFullName}.PrivateDependencies "${${ModuleName}.PrivateDependencies}")
            endif()
            if(${ModuleName}.PublicDependencies)
                Abytek_SetGlobal(${ModuleFullName}.PublicDependencies "${${ModuleName}.PublicDependencies}")
            endif()

            Abytek_SetupThirdParty(ABYTEK_MODULE_PHASE_TARGET_CREATED)

            message(STATUS "")
        endforeach()
    endif()



    # Child modules recursive calls
    foreach(ModuleName ${MODULE_NAMES})
        Abytek_CalculateModuleFullName(ModuleFullName "${PARGS_BASE_NAMESPACE}" ${ModuleName})

        set(CurrentModule "${ModuleName}")
        set(CurrentModuleFullName "${ModuleFullName}")
        set(CurrentTarget "${ModuleFullName}")

        if(NOT ${ModuleFullName}.Enable)
            continue()
        endif()

        if(EXISTS "${${ModuleFullName}.Modules.PrivateDirectory}")
            if(PARGS_PHASE_ALL)
                Abytek_SetupModules(
                    BASE_NAMESPACE "${ModuleFullName}"
                    MODULES_DIR "${${ModuleFullName}.Modules.PrivateDirectory}"
                    OUT_MODULES_VAR_NAME ${ModuleFullName}.Modules.Private
                    OUT_TARGETS_VAR_NAME ${ModuleFullName}.Modules.PrivateTargets
                    OUT_ALL_MODULES ${PARGS_OUT_ALL_MODULES}
                    NAMESPACES ${PARGS_NAMESPACES}
                    ADDITIONAL_INFO "; PRIVATE"
                    DEFAULT_MODULE_TYPE ${${ModuleFullName}.Modules.DefaultType}
                    PHASE_ALL
                    JS_CONTEXT_FILE "${PARGS_JS_CONTEXT_FILE}"
                    JS_RUN_SCRIPT_FILE "${PARGS_JS_RUN_SCRIPT_FILE}"
                )
            else()
                Abytek_SetupModules(
                    BASE_NAMESPACE "${ModuleFullName}"
                    MODULES_DIR "${${ModuleFullName}.Modules.PrivateDirectory}"
                    OUT_MODULES_VAR_NAME ${ModuleFullName}.Modules.Private
                    OUT_TARGETS_VAR_NAME ${ModuleFullName}.Modules.PrivateTargets
                    OUT_ALL_MODULES ${PARGS_OUT_ALL_MODULES}
                    NAMESPACES ${PARGS_NAMESPACES}
                    ADDITIONAL_INFO "; PRIVATE"
                    DEFAULT_MODULE_TYPE ${${ModuleFullName}.Modules.DefaultType}
                    PHASE ${PARGS_PHASE}
                    JS_CONTEXT_FILE "${PARGS_JS_CONTEXT_FILE}"
                    JS_RUN_SCRIPT_FILE "${PARGS_JS_RUN_SCRIPT_FILE}"
                )
            endif()
        endif()

        if(EXISTS "${${ModuleFullName}.Modules.PublicDirectory}")
            if(PARGS_PHASE_ALL)
                Abytek_SetupModules(
                    BASE_NAMESPACE "${ModuleFullName}"
                    MODULES_DIR "${${ModuleFullName}.Modules.PublicDirectory}"
                    OUT_MODULES_VAR_NAME ${ModuleFullName}.Modules.Public
                    OUT_TARGETS_VAR_NAME ${ModuleFullName}.Modules.PublicTargets
                    OUT_ALL_MODULES ${PARGS_OUT_ALL_MODULES}
                    NAMESPACES ${PARGS_NAMESPACES}
                    ADDITIONAL_INFO "; PUBLIC"
                    DEFAULT_MODULE_TYPE ${${ModuleFullName}.Modules.DefaultType}
                    PHASE_ALL
                    JS_CONTEXT_FILE "${PARGS_JS_CONTEXT_FILE}"
                    JS_RUN_SCRIPT_FILE "${PARGS_JS_RUN_SCRIPT_FILE}"
                )
            else()
                Abytek_SetupModules(
                    BASE_NAMESPACE "${ModuleFullName}"
                    MODULES_DIR "${${ModuleFullName}.Modules.PublicDirectory}"
                    OUT_MODULES_VAR_NAME ${ModuleFullName}.Modules.Public
                    OUT_TARGETS_VAR_NAME ${ModuleFullName}.Modules.PublicTargets
                    OUT_ALL_MODULES ${PARGS_OUT_ALL_MODULES}
                    NAMESPACES ${PARGS_NAMESPACES}
                    ADDITIONAL_INFO "; PUBLIC"
                    DEFAULT_MODULE_TYPE ${${ModuleFullName}.Modules.DefaultType}
                    PHASE ${PARGS_PHASE}
                    JS_CONTEXT_FILE "${PARGS_JS_CONTEXT_FILE}"
                    JS_RUN_SCRIPT_FILE "${PARGS_JS_RUN_SCRIPT_FILE}"
                )
            endif()
        endif()
    endforeach()
    if(PARGS_OUT_ALL_MODULES)
        set(${PARGS_OUT_ALL_MODULES} "${${PARGS_OUT_ALL_MODULES}}" PARENT_SCOPE)
    endif()



    # Set-up target dependencies
    if(PARGS_PHASE_ALL OR PARGS_PHASE STREQUAL SETUP_TARGET_DEPENDENCIES)
        foreach(ModuleName ${MODULE_NAMES})
            Abytek_CalculateModuleFullName(ModuleFullName "${PARGS_BASE_NAMESPACE}" ${ModuleName})

            set(CurrentModule "${ModuleName}")
            set(CurrentModuleFullName "${ModuleFullName}")
            set(CurrentTarget "${ModuleFullName}")

            if(NOT ${ModuleFullName}.Enable)
                continue()
            endif()

            # begin write JSModulesFile
            file(
                WRITE
                "${${ModuleFullName}.JSModulesFile}"
                "
                    module.exports = (Context) => {
                        return [
                "
            )

            # begin write JSPrivateModulesFile
            file(
                WRITE
                "${${ModuleFullName}.JSPrivateModulesFile}"
                "
                    module.exports = (Context) => {
                        return [
                "
            )

            # begin write JSPublicModulesFile
            file(
                WRITE
                "${${ModuleFullName}.JSPublicModulesFile}"
                "
                    module.exports = (Context) => {
                        return [
                "
            )

            # begin write JSDependenciesFile
            file(
                WRITE
                "${${ModuleFullName}.JSDependenciesFile}"
                "
                    module.exports = (Context) => {
                        return [
                "
            )

            # begin write JSPrivateDependenciesFile
            file(
                WRITE
                "${${ModuleFullName}.JSPrivateDependenciesFile}"
                "
                    module.exports = (Context) => {
                        return [
                "
            )

            # begin write JSPublicDependenciesFile
            file(
                WRITE
                "${${ModuleFullName}.JSPublicDependenciesFile}"
                "
                    module.exports = (Context) => {
                        return [
                "
            )

            # For private child modules as dependencies
            foreach(ChildModule ${${ModuleFullName}.Modules.Private})
                set(ChildModuleFullName ${CurrentModuleFullName}.${ChildModule})
                set(ChildModuleTarget ${${ChildModuleFullName}.Target})

                if(NOT ${ChildModuleTarget}.Enable)
                    continue()
                endif()

                if(${ModuleFullName}.IsCompilable AND ${ChildModuleTarget}.IsCompilable)
                    if(NOT ${ChildModuleTarget}.Type STREQUAL EXECUTABLE)
                        if(${ChildModuleTarget}.CanParentDependOn)
                            target_link_libraries(${ModuleFullName}
                                PRIVATE
                                    ${ChildModuleTarget}
                            )
                        endif()
                    endif()
                endif()
                if(NOT ${ChildModuleTarget}.Type STREQUAL EXECUTABLE)
                    if(${ChildModuleTarget}.CanParentDependOn)
                        Abytek_ApplyDependencies(
                            TARGET ${ModuleFullName}
                            DEPENDENT_TARGETS ${ChildModuleTarget}
                        )
    
                        # try append JSDependenciesFile
                        if(${ChildModuleTarget}.JSDependenciesFile)
                            file(
                                APPEND
                                "${${ModuleFullName}.JSDependenciesFile}"
                                "
                                    \"${ChildModuleTarget}\",
                                "
                            )
                        endif()
    
                        # try append JSPrivateDependenciesFile
                        if(${ChildModuleTarget}.JSPrivateDependenciesFile)
                            file(
                                APPEND
                                "${${ModuleFullName}.JSPrivateDependenciesFile}"
                                "
                                    \"${ChildModuleTarget}\",
                                "
                            )
                        endif()
                    endif()
                elseif(${ModuleFullName}.Type STREQUAL CUSTOM)
                    Abytek_ApplyDependencies(
                        TARGET ${ModuleFullName}
                        DEPENDENT_TARGETS ${ChildModuleTarget}
                    )

                    # try append JSDependenciesFile
                    if(${ChildModuleTarget}.JSDependenciesFile)
                        file(
                            APPEND
                            "${${ModuleFullName}.JSDependenciesFile}"
                            "
                                \"${ChildModuleTarget}\",
                            "
                        )
                    endif()

                    # try append JSPrivateDependenciesFile
                    if(${ChildModuleTarget}.JSPrivateDependenciesFile)
                        file(
                            APPEND
                            "${${ModuleFullName}.JSPrivateDependenciesFile}"
                            "
                                \"${ChildModuleTarget}\",
                            "
                        )
                    endif()
                endif()

                # try append JSModulesFile
                file(
                    APPEND
                    "${${ModuleFullName}.JSModulesFile}"
                    "
                        \"${ChildModuleTarget}\",
                    "
                )

                # try append JSPrivateModulesFile
                file(
                    APPEND
                    "${${ModuleFullName}.JSPrivateModulesFile}"
                    "
                        \"${ChildModuleTarget}\",
                    "
                )
            endforeach()

            # For public child modules as dependencies
            foreach(ChildModule ${${ModuleFullName}.Modules.Public})
                set(ChildModuleFullName ${CurrentModuleFullName}.${ChildModule})
                set(ChildModuleTarget ${${ChildModuleFullName}.Target})

                if(NOT ${ChildModuleTarget}.Enable)
                    continue()
                endif()

                if(${ModuleFullName}.IsCompilable AND ${ChildModuleTarget}.IsCompilable)
                    if(NOT ${ChildModuleTarget}.Type STREQUAL EXECUTABLE)
                        if(${ChildModuleTarget}.CanParentDependOn)
                            target_link_libraries(${ModuleFullName}
                                PUBLIC
                                    ${ChildModuleTarget}
                            )
                        endif()
                    endif()
                endif()
                if(NOT ${ChildModuleTarget}.Type STREQUAL EXECUTABLE)
                    if(${ChildModuleTarget}.CanParentDependOn)
                        Abytek_ApplyDependencies(
                            TARGET ${ModuleFullName}
                            DEPENDENT_TARGETS ${ChildModuleTarget}
                        )
    
                        # try append JSDependenciesFile
                        if(${ChildModuleTarget}.JSDependenciesFile)
                            file(
                                APPEND
                                "${${ModuleFullName}.JSDependenciesFile}"
                                "
                                    \"${ChildModuleTarget}\",
                                "
                            )
                        endif()
    
                        # try append JSPublicDependenciesFile
                        if(${ChildModuleTarget}.JSPublicDependenciesFile)
                            file(
                                APPEND
                                "${${ModuleFullName}.JSPublicDependenciesFile}"
                                "
                                    \"${ChildModuleTarget}\",
                                "
                            )
                        endif()
                    endif()
                elseif(${ModuleFullName}.Type STREQUAL CUSTOM)
                    Abytek_ApplyDependencies(
                        TARGET ${ModuleFullName}
                        DEPENDENT_TARGETS ${ChildModuleTarget}
                    )

                    # try append JSDependenciesFile
                    if(${ChildModuleTarget}.JSDependenciesFile)
                        file(
                            APPEND
                            "${${ModuleFullName}.JSDependenciesFile}"
                            "
                                \"${ChildModuleTarget}\",
                            "
                        )
                    endif()

                    # try append JSPublicDependenciesFile
                    if(${ChildModuleTarget}.JSPublicDependenciesFile)
                        file(
                            APPEND
                            "${${ModuleFullName}.JSPublicDependenciesFile}"
                            "
                                \"${ChildModuleTarget}\",
                            "
                        )
                    endif()
                endif()

                # try append JSModulesFile
                file(
                    APPEND
                    "${${ModuleFullName}.JSModulesFile}"
                    "
                        \"${ChildModuleTarget}\",
                    "
                )

                # try append JSPublicModulesFile
                file(
                    APPEND
                    "${${ModuleFullName}.JSPublicModulesFile}"
                    "
                        \"${ChildModuleTarget}\",
                    "
                )
            endforeach()

            # For private manual dependencies
            foreach(PrivateDependency ${${ModuleFullName}.PrivateDependencies})
                set(PrivateDependentTarget ${PrivateDependency})
                foreach(Namespace ${PARGS_NAMESPACES})
                    if(TARGET ${Namespace}.${PrivateDependency})
                        set(PrivateDependentTarget ${Namespace}.${PrivateDependency})
                        break()
                    endif()
                endforeach()

                if(NOT TARGET ${PrivateDependentTarget})
                    message(FATAL_ERROR "Not found dependent target \"${PrivateDependency}\"")
                endif()

                if(${ModuleFullName}.IsCompilable)
                    get_target_property(PrivateDependentTargetType ${PrivateDependentTarget} TYPE)
                    if(NOT PrivateDependentTargetType STREQUAL "UTILITY")
                        target_link_libraries(${ModuleFullName}
                            PRIVATE
                                ${PrivateDependentTarget}
                        )
                    endif()
                endif()
                Abytek_ApplyDependencies(
                    TARGET ${ModuleFullName}
                    DEPENDENT_TARGETS ${PrivateDependentTarget}
                )

                # try append JSDependenciesFile
                if(${PrivateDependentTarget}.JSDependenciesFile)
                    file(
                        APPEND
                        "${${ModuleFullName}.JSDependenciesFile}"
                        "
                            \"${PrivateDependentTarget}\",
                        "
                    )
                endif()

                # try append JSPrivateDependenciesFile
                if(${PrivateDependentTarget}.JSPrivateDependenciesFile)
                    file(
                        APPEND
                        "${${ModuleFullName}.JSPrivateDependenciesFile}"
                        "
                            \"${PrivateDependentTarget}\",
                        "
                    )
                endif()
            endforeach()

            # For public manual dependencies
            foreach(PublicDependency ${${ModuleFullName}.PublicDependencies})
                set(PublicDependentTarget ${PublicDependency})
                foreach(Namespace ${PARGS_NAMESPACES})
                    if(TARGET ${Namespace}.${PublicDependency})
                        set(PublicDependentTarget ${Namespace}.${PublicDependency})
                        break()
                    endif()
                endforeach()

                if(NOT TARGET ${PublicDependentTarget})
                    message(FATAL_ERROR "Not found dependent target \"${PublicDependency}\"")
                endif()

                if(${ModuleFullName}.IsCompilable)
                    get_target_property(PublicDependentTargetType ${PublicDependentTarget} TYPE)
                    if(NOT PublicDependentTargetType STREQUAL "UTILITY")
                        target_link_libraries(${ModuleFullName}
                            PUBLIC
                                ${PublicDependentTarget}
                        )
                    endif()
                endif()
                Abytek_ApplyDependencies(
                    TARGET ${ModuleFullName}
                    DEPENDENT_TARGETS ${PublicDependentTarget}
                )

                # try append JSDependenciesFile
                if(${PublicDependentTarget}.JSDependenciesFile)
                    file(
                        APPEND
                        "${${ModuleFullName}.JSDependenciesFile}"
                        "
                            \"${PublicDependentTarget}\",
                        "
                    )
                endif()

                # try append JSPublicDependenciesFile
                if(${PublicDependentTarget}.JSPublicDependenciesFile)
                    file(
                        APPEND
                        "${${ModuleFullName}.JSPublicDependenciesFile}"
                        "
                            \"${PublicDependentTarget}\",
                        "
                    )
                endif()
            endforeach()

            # end write JSPublicDependenciesFile
            file(
                APPEND
                "${${ModuleFullName}.JSPublicDependenciesFile}"
                "
                        ];
                    }
                "
            )

            # end write JSPrivateDependenciesFile
            file(
                APPEND
                "${${ModuleFullName}.JSPrivateDependenciesFile}"
                "
                        ];
                    }
                "
            )

            # end write JSDependenciesFile
            file(
                APPEND
                "${${ModuleFullName}.JSDependenciesFile}"
                "
                        ];
                    }
                "
            )

            # end write JSPublicModulesFile
            file(
                APPEND
                "${${ModuleFullName}.JSPublicModulesFile}"
                "
                        ];
                    }
                "
            )

            # end write JSPrivateModulesFile
            file(
                APPEND
                "${${ModuleFullName}.JSPrivateModulesFile}"
                "
                        ];
                    }
                "
            )

            # end write JSModulesFile
            file(
                APPEND
                "${${ModuleFullName}.JSModulesFile}"
                "
                        ];
                    }
                "
            )
        endforeach()
    endif()



    # Phase: ABYTEK_MODULE_PHASE_GLOBAL_SHARE
    if(PARGS_PHASE_ALL OR PARGS_PHASE STREQUAL GLOBAL_SHARE)
        foreach(ModuleName ${MODULE_NAMES})
            Abytek_CalculateModuleFullName(ModuleFullName "${PARGS_BASE_NAMESPACE}" ${ModuleName})

            set(CurrentModule "${ModuleName}")
            set(CurrentModuleFullName "${ModuleFullName}")
            set(CurrentTarget "${ModuleFullName}")

            if(NOT ${ModuleFullName}.Enable)
                continue()
            endif()

            message(STATUS "ABYTEK_MODULE_PHASE_GLOBAL_SHARE::BEGIN(${ModuleFullName})")
            foreach(PreScript ${ABYTEK_SETUP_PRE_SCRIPTS})
                include("${PreScript}")
            endforeach()
            set(ModuleSetupPath "${${ModuleFullName}.Directory}/Setup.cmake")
            set(ABYTEK_MODULE_PHASE_GLOBAL_SHARE ON)
            if(EXISTS "${ModuleSetupPath}")
                include("${ModuleSetupPath}")
            endif()
            set(ABYTEK_MODULE_PHASE_GLOBAL_SHARE OFF)
            message(STATUS "ABYTEK_MODULE_PHASE_GLOBAL_SHARE::END(${ModuleFullName})")

            Abytek_SetupThirdParty(ABYTEK_MODULE_PHASE_GLOBAL_SHARE)

            message(STATUS "")
        endforeach()
    endif()



    # Phase: ABYTEK_MODULE_PHASE_LATE_SETUP
    if(PARGS_PHASE_ALL OR PARGS_PHASE STREQUAL LATE_SETUP)
        foreach(ModuleName ${MODULE_NAMES})
            Abytek_CalculateModuleFullName(ModuleFullName "${PARGS_BASE_NAMESPACE}" ${ModuleName})

            set(CurrentModule "${ModuleName}")
            set(CurrentModuleFullName "${ModuleFullName}")
            set(CurrentTarget "${ModuleFullName}")

            if(NOT ${ModuleFullName}.Enable)
                continue()
            endif()

            message(STATUS "ABYTEK_MODULE_PHASE_LATE_SETUP::BEGIN(${ModuleFullName})")
            foreach(PreScript ${ABYTEK_SETUP_PRE_SCRIPTS})
                include("${PreScript}")
            endforeach()
            set(ModuleSetupPath "${${ModuleFullName}.Directory}/Setup.cmake")
            set(ABYTEK_MODULE_PHASE_LATE_SETUP ON)
            if(EXISTS "${ModuleSetupPath}")
                include("${ModuleSetupPath}")
            endif()
            set(ABYTEK_MODULE_PHASE_LATE_SETUP OFF)
            message(STATUS "ABYTEK_MODULE_PHASE_LATE_SETUP::END(${ModuleFullName})")

            # Find cpp files
            file(
                GLOB_RECURSE
                NewGeneratedPrivateCPPFiles
                "${${ModuleFullName}.GeneratedSource.PrivateDirectory}/*.cpp"
            )

            # Find c files
            file(
                GLOB_RECURSE
                NewGeneratedPrivateCFiles
                "${${ModuleFullName}.GeneratedSource.PrivateDirectory}/*.c"
            )

            # Find mm files
            file(
                GLOB_RECURSE
                NewGeneratedPrivateMMFiles
                "${${ModuleFullName}.GeneratedSource.PrivateDirectory}/*.mm"
            )
            
            #
            target_sources(${CurrentTarget}
                PRIVATE
                    ${NewGeneratedPrivateCPPFiles}
                    ${NewGeneratedPrivateCFiles}
                    ${NewGeneratedPrivateMMFiles}
            )

            Abytek_SetupThirdParty(ABYTEK_MODULE_PHASE_LATE_SETUP)

            message(STATUS "")
        endforeach()
    endif()
endfunction()

function(Abytek_SetupModules_MultiplePhases RootName ModulesDir)
    Abytek_SetGlobal(${RootName}.JSImportFile "${CMAKE_CURRENT_BINARY_DIR}/Import.js")
    Abytek_SetGlobal(${RootName}.JSContextFile "${CMAKE_CURRENT_BINARY_DIR}/${RootName}/GeneratedJS/${RootName}.Context.js")
    Abytek_SetGlobal(${RootName}.JSRunScriptFile "${CMAKE_CURRENT_BINARY_DIR}/${RootName}/GeneratedJS/${RootName}.RunScript.js")
    
    Abytek_SetGlobal(${RootName}.JSListFile "${ABYTEK_PROJECT_BUILD_DIR}/AbytekEBT/JSListFiles/${RootName}.js")
    file(
        WRITE 
        "${${RootName}.JSListFile}"
        "
            module.exports = {
                JSContextFile: \"${${RootName}.JSContextFile}\"
            }
        "
    )

    file(
        WRITE 
        "${ABYTEK_GLOBAL_IMPORT_JS_FILE}"
        "
            const path = require('path');
            const fs = require('fs');

            // Directory to search
            const targetDir = path.join(__dirname, 'JSListFiles');

            // Array to collect exports
            const JSContextFiles = [];

            // Read all .js files from the directory
            fs.readdirSync(targetDir).forEach(file => {
                const fullPath = path.join(targetDir, file);

                // Only process JS files
                if (fs.statSync(fullPath).isFile() && path.extname(file) === '.js') {
                    JSContextFiles.push(fullPath);
                }
            });

            module.exports = JSContextFiles;
        "
    )

    set(ABYTEK_MODULES "")
    Abytek_SetupModules(
        MODULES_DIR "${ModulesDir}"
        PHASE INIT
        OUT_ALL_MODULES ABYTEK_MODULES
        JS_CONTEXT_FILE "${${RootName}.JSContextFile}"
        JS_RUN_SCRIPT_FILE "${${RootName}.JSRunScriptFile}"
    )

    foreach(Module ${ABYTEK_MODULES})
        if(${${Module}.Enable})
            file(
                APPEND 
                "${ABYTEK_GLOBAL_MODULES_INL_FILE}"
                "\"${Module}\","
            )
        endif()
    endforeach()

    # write JSImportFile
    file(
        WRITE
        "${${RootName}.JSImportFile}"
        "
            module.exports = {
                Name: \"${RootName}\",
                Context: require(\"${${RootName}.JSContextFile}\"),
            };
        "
    )

    # JSRunScriptFile
    file(
        WRITE
        "${${RootName}.JSRunScriptFile}"
        "
            const ModuleFullName = process.argv[2];
            if (!ModuleFullName) {
                console.error(\"Module full name is required as the first argument.\");
                process.exit(1);
            }
            const ScriptPath = process.argv[3];
            if (!ScriptPath) {
                console.error(\"Script path is required as the second argument.\");
                process.exit(1);
            }
            const CLIArgs = process.argv.slice(4);

            let Context = require(\"${${RootName}.JSContextFile}\");
            let Module = Context.RequireModule(ModuleFullName);
            Module.ExecuteScript(ScriptPath, CLIArgs);
        "
    )
    
    # begin JSContextFile
    file(
        WRITE 
        "${${RootName}.JSContextFile}"
        "
            var ModuleFiles = new Object();
        "
    )

    # add module into JSContextFile
    foreach(ModuleFullName ${ABYTEK_MODULES})
        string(REPLACE "\\" "/" ParsedJSModuleFile "${${ModuleFullName}.JSModuleFile}")
        if(NOT ${ModuleFullName}.Enable)
            continue()
        endif()
        file(
            APPEND
            "${${RootName}.JSContextFile}"
            "
                ModuleFiles[\"${ModuleFullName}\"] = \"${ParsedJSModuleFile}\";
            "
        )
    endforeach()

    # end JSContextFile
    set(Platform "")
    if(WIN32)
        set(Platform WINDOWS)
    endif()
    if(LINUX)
        set(Platform LINUX)
    endif()
    if(MACOS)
        set(Platform MACOS)
    endif()
    if(IOS)
        set(Platform IOS)
    endif()
    if(ANDROID)
        set(Platform ANDROID)
    endif()
    if(EMSCRIPTEN)
        set(Platform EMSCRIPTEN)
    endif()
    file(
        APPEND
        "${${RootName}.JSContextFile}"
        "
            class F_Context
            {
                constructor()
                {
                    this.ModuleFiles = ModuleFiles;    
                    
                    this.Modules = new Object();
                    
                    this.PlatformName = \"${Platform}\";
                }
                
                RequireModule(Name)
                {
                    if(Name in this.Modules)
                    {
                        return this.Modules[Name];
                    }
                    if(!(Name in this.ModuleFiles))
                    {
                        throw new Error(`not found module file \${Name}`);
                    }
                    let Module = require(this.ModuleFiles[Name])(this);
                    this.Modules[Name] = Module;
                    return Module;
                }
            }
        
            module.exports = new F_Context();
        "
    )
    
    Abytek_SetupModules(
        MODULES_DIR "${ModulesDir}"
        PHASE LATE_INIT
        JS_CONTEXT_FILE "${${RootName}.JSContextFile}"
        JS_RUN_SCRIPT_FILE "${${RootName}.JSRunScriptFile}"
    )
    Abytek_SetupModules(
        MODULES_DIR "${ModulesDir}"
        PHASE TARGET_CREATED
        JS_CONTEXT_FILE "${${RootName}.JSContextFile}"
        JS_RUN_SCRIPT_FILE "${${RootName}.JSRunScriptFile}"
    )
    Abytek_SetupModules(
        MODULES_DIR "${ModulesDir}"
        PHASE AFTER_CHILD_MODULES_SETUP
        JS_CONTEXT_FILE "${${RootName}.JSContextFile}"
        JS_RUN_SCRIPT_FILE "${${RootName}.JSRunScriptFile}"
    )
    Abytek_SetupModules(
        MODULES_DIR "${ModulesDir}"
        PHASE SETUP_TARGET_DEPENDENCIES
        JS_CONTEXT_FILE "${${RootName}.JSContextFile}"
        JS_RUN_SCRIPT_FILE "${${RootName}.JSRunScriptFile}"
    )
    Abytek_SetupModules(
        MODULES_DIR "${ModulesDir}"
        PHASE GLOBAL_SHARE
        JS_CONTEXT_FILE "${${RootName}.JSContextFile}"
        JS_RUN_SCRIPT_FILE "${${RootName}.JSRunScriptFile}"
    )
    Abytek_SetupModules(
        MODULES_DIR "${ModulesDir}"
        PHASE LATE_SETUP
        JS_CONTEXT_FILE "${${RootName}.JSContextFile}"
        JS_RUN_SCRIPT_FILE "${${RootName}.JSRunScriptFile}"
    )
endfunction()