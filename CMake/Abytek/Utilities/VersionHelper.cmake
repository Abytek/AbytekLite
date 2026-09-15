
include(Abytek/Utilities/SetGlobal)
include(Abytek/Utilities/ApplyGlobal)
include(Abytek/Utilities/MacroHelper)



function(Abytek_VersionHelper_CreateVersionMacros)
    cmake_parse_arguments(
        PARGS
        "QUIET"
        "PROJECT_NAME;OUTPUT_FILE;MAJOR;MINOR;PATCH;MAX_COMPONENT_VALUE"
        ""
        ${ARGN}
    )

    if(NOT PARGS_MAJOR)
        set(PARGS_MAJOR 0)
    endif()
    if(NOT PARGS_MINOR)
        set(PARGS_MINOR 0)
    endif()
    if(NOT PARGS_PATCH)
        set(PARGS_PATCH 0)
    endif()
    if(NOT PARGS_MAX_COMPONENT_VALUE)
        set(PARGS_MAX_COMPONENT_VALUE 99)
    endif()

    math(EXPR version_number "${PARGS_MAJOR} * (${PARGS_MAX_COMPONENT_VALUE} + 1) * (${PARGS_MAX_COMPONENT_VALUE} + 1) + ${PARGS_MINOR} * (${PARGS_MAX_COMPONENT_VALUE} + 1) + ${PARGS_PATCH}")    

    if(PARGS_PROJECT_NAME)
        if(PARGS_OUTPUT_FILE)
            file(WRITE "${PARGS_OUTPUT_FILE}" "")

            # Apply variables
            Abytek_SetGlobal(${PARGS_PROJECT_NAME}_VERSION_MAJOR ${PARGS_MAJOR})
            Abytek_SetGlobal(${PARGS_PROJECT_NAME}_VERSION_MAINOR ${PARGS_MINOR})
            Abytek_SetGlobal(${PARGS_PROJECT_NAME}_VERSION_PATCH ${PARGS_PATCH})
            Abytek_SetGlobal(${PARGS_PROJECT_NAME}_VERSION ${PARGS_MAJOR}.${PARGS_MINOR}.${PARGS_PATCH})
            Abytek_SetGlobal(${PARGS_PROJECT_NAME}_VERSION_STR "\"${PARGS_MAJOR}.${PARGS_MINOR}.${PARGS_PATCH}\"")
            Abytek_SetGlobal(${PARGS_PROJECT_NAME}_VERSION_NUMBER ${version_number})
            Abytek_SetGlobal(${PARGS_PROJECT_NAME}_VERSION_MAX_COMPONENT_VALUE ${PARGS_MAX_COMPONENT_VALUE})

            # Create macros
            Abytek_MacroHelper_CreateObjectMacro(
                NAME ${PARGS_PROJECT_NAME}_VERSION_MAJOR
                OUTPUT_FILE "${PARGS_OUTPUT_FILE}"
                BODY ${PARGS_MAJOR}
            )
            Abytek_MacroHelper_CreateObjectMacro(
                NAME ${PARGS_PROJECT_NAME}_VERSION_MAINOR
                OUTPUT_FILE "${PARGS_OUTPUT_FILE}"
                BODY ${PARGS_MINOR}
            )
            Abytek_MacroHelper_CreateObjectMacro(
                NAME ${PARGS_PROJECT_NAME}_VERSION_PATCH
                OUTPUT_FILE "${PARGS_OUTPUT_FILE}"
                BODY ${PARGS_PATCH}
            )
            Abytek_MacroHelper_CreateObjectMacro(
                NAME ${PARGS_PROJECT_NAME}_VERSION
                OUTPUT_FILE "${PARGS_OUTPUT_FILE}"
                BODY ${${PARGS_PROJECT_NAME}_VERSION}
            )
            Abytek_MacroHelper_CreateObjectMacro(
                NAME ${PARGS_PROJECT_NAME}_VERSION_STR
                OUTPUT_FILE "${PARGS_OUTPUT_FILE}"
                BODY ${${PARGS_PROJECT_NAME}_VERSION_STR}
            )
            Abytek_MacroHelper_CreateObjectMacro(
                NAME ${PARGS_PROJECT_NAME}_VERSION_NUMBER
                OUTPUT_FILE "${PARGS_OUTPUT_FILE}"
                BODY ${version_number}
            )
            Abytek_MacroHelper_CreateFunctionMacro(
                NAME ${PARGS_PROJECT_NAME}_ANOTHER_VERSION_NUMBER
                OUTPUT_FILE "${PARGS_OUTPUT_FILE}"
                PARAM_NAMES 
                    "MAJOR"
                    "MINOR"
                    "PATCH"
                BODY 
                    "((MAJOR * (${PARGS_MAX_COMPONENT_VALUE} + 1) * (${PARGS_MAX_COMPONENT_VALUE} + 1)) + MINOR * ((${PARGS_MAX_COMPONENT_VALUE} + 1)) + PATCH)"
            )
        else()
            message(FATAL_ERROR "Cant create version macros without OUTPUT_FILE")
        endif()  
    else()
        message(FATAL_ERROR "Cant create version macros without PROJECT_NAME")
    endif()
endfunction()
