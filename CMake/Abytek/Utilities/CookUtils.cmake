
include(Abytek/Utilities/SetGlobal)


Abytek_SetGlobal(ABYTEK_DEFAULT_COOK_PIPELINE "SAF")


function(Abytek_AddCookTarget)   
    if(NOT ABYTEK_ENABLE_DEVELOPMENT_BUILD)
        return()
    endif()

    cmake_parse_arguments(
        PARGS
        ""
        "APPLICATION_NAME;PIPELINE;PLATFORM;PREFIX"
        "EXTRA_ARGUMENTS;ENVIRONMENTS;CONFIGS;PROCESSOR_ARCHS"
        ${ARGN}
    )

    if(NOT PARGS_APPLICATION_NAME)
        if(PARGS_APPLICATION_NAME)
            set(PARGS_APPLICATION_NAME ${PARGS_APPLICATION_NAME})
        else()
            message(FATAL_ERROR "requires APPLICATION_NAME")
        endif()
    endif()

    if(NOT PARGS_PIPELINE)
        set(PARGS_PIPELINE ${ABYTEK_DEFAULT_COOK_PIPELINE})
    endif()
    if(NOT PARGS_PLATFORM)
        message(FATAL_ERROR "Requires PLATFORM")
    endif()
    if(NOT PARGS_PREFIX)
        set(PARGS_PREFIX "")
    endif()

    if(NOT PARGS_EXTRA_ARGUMENTS)
        set(PARGS_EXTRA_ARGUMENTS "")
    endif()
    if(NOT PARGS_ENVIRONMENTS)
        set(PARGS_ENVIRONMENTS "Cooked;CookedMaxOptimized")
    endif()
    if(NOT PARGS_CONFIGS)
        set(PARGS_CONFIGS "Debug;Release;RelWithDebInfo;MinSizeRel")
    endif()
    if(NOT PARGS_PROCESSOR_ARCHS)
        set(PARGS_PROCESSOR_ARCHS "X86_32;X86_64;ARM_32;ARM_64")
    endif()

    foreach(Environment ${PARGS_ENVIRONMENTS})
        foreach(Config ${PARGS_CONFIGS})
            set(ProcessorArchSetName "")
            set(ShouldAddDot OFF)
            foreach(ProcessorArch ${PARGS_PROCESSOR_ARCHS})
                if (ShouldAddDot)
                    set(ProcessorArchSetName "${ProcessorArchSetName}.")
                    set(ShouldAddDot ON)
                endif()
                set(ProcessorArchSetName "${ProcessorArchSetName}${ProcessorArch}")
            endforeach()
            
            set(RelativeCookTargetName "${PARGS_PIPELINE}.${Environment}.${PARGS_PLATFORM}.${Config}.${ProcessorArchSetName}${PARGS_PREFIX}")
            set(CookTargetName "${PARGS_APPLICATION_NAME}.${RelativeCookTargetName}")
            set(CookCommandName "${CookTargetName}.Command")
            set(CookTrickFilesDir "${ABYTEK_PROJECT_BUILD_DIR}/Abytek/CookTrickFile")
            set(CookTrickFile "${CookTrickFilesDir}/${PARGS_APPLICATION_NAME}")
            if(NOT EXISTS "${CookTrickFilesDir}")
                file(MAKE_DIRECTORY "${CookTrickFilesDir}")
            endif()
            file(WRITE "${CookTrickFile}" "TRICK")

            set(ProcessorArchArguments "")
            foreach(ProcessorArch ${PARGS_PROCESSOR_ARCHS})
                list(APPEND ProcessorArchArguments --CookProcessorArch ${ProcessorArch})
            endforeach()

            add_custom_command(
                OUTPUT ${CookCommandName}
                COMMAND 
                    $<TARGET_FILE:${PARGS_APPLICATION_NAME}> 
                    --CookMode
                    --CookProjectPath "${ABYTEK_PROJECT_DIR}" 
                    --CookApplicationTarget "${PARGS_APPLICATION_NAME}" 
                    --CookPipeline "${PARGS_PIPELINE}"
                    --CookEnvironment "${Environment}"
                    --CookPlatform "${PARGS_PLATFORM}" 
                    --CookCMakeConfig "${Config}"
                    ${ProcessorArchArguments}
                    ${PARGS_EXTRA_ARGUMENTS}
                COMMAND 
                    ${CMAKE_COMMAND} 
                    -E touch "${CookTrickFile}"
                WORKING_DIRECTORY $<TARGET_FILE_DIR:${PARGS_APPLICATION_NAME}>
                DEPENDS ${PARGS_APPLICATION_NAME} "${CookTrickFile}"
            )
            add_custom_target(${CookTargetName} ALL
                DEPENDS ${CookCommandName} "${CookTrickFile}"
            )    

            get_target_property(ModuleVSFolderName ${PARGS_APPLICATION_NAME} FOLDER)
            set_target_properties(${CookTargetName}
                PROPERTIES
                    FOLDER "${ModuleVSFolderName}/Cook"
                    PROJECT_LABEL ${PARGS_APPLICATION_NAME}.${RelativeCookTargetName}
            )
            add_dependencies(${CookTargetName}
                ${PARGS_APPLICATION_NAME}
            )
        endforeach()
    endforeach()
endfunction()