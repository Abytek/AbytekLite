# OSBCodeGenerator.cmake
#
# Provides functions for generating code using the OSBridge code generator system.
# This is a general-purpose system that can be used by any OSBridge module.

include(CMakeParseArguments)

# Function to generate code for an OSBridge module using the OSBCodeGenerator
#
# Parameters:
#   MODULE_NAME - The name of the module (required)
#   DEFAULT_SPECIFICATOR - The default specificator to use
#   SPECIFICATORS - List of specificators to enable
#   ITEM_DIRS - List of directories containing module-specific items
#   ITEM_SUPPORT_DIRS - List of directories containing module-specific item supports
#   SPECIFICATOR_DIRS - List of directories containing module-specific specificators
#
function(Abytek_OSBCodeGenerator_Generate)
    # Parse arguments
    set(options)
    set(oneValueArgs MODULE_NAME ROOT_HELPER_CLASS DEFAULT_SPECIFICATOR)
    set(multiValueArgs SPECIFICATORS ITEM_DIRS ITEM_SUPPORT_DIRS SPECIFICATOR_DIRS INCLUDES)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    
    # Validate required arguments
    if(NOT ARG_MODULE_NAME)
        message(FATAL_ERROR "MODULE_NAME is required for Abytek_OSBCodeGenerator_Generate")
    endif()

    # Validate required arguments
    if(NOT ARG_ROOT_HELPER_CLASS)
        message(FATAL_ERROR "ROOT_HELPER_CLASS is required for Abytek_OSBCodeGenerator_Generate")
    endif()
    
    # Set defaults if not provided
    if(NOT ARG_SPECIFICATORS)
        set(ARG_SPECIFICATORS "Dummy")
    endif()
    
    if(NOT ARG_DEFAULT_SPECIFICATOR)
        set(ARG_DEFAULT_SPECIFICATOR "Dummy")
    endif()
    
    # Prepare CLI arguments for the JS script
    set(CLI_ARGS "")
    
    list(APPEND CLI_ARGS "--ModuleName" "${ARG_MODULE_NAME}")
    list(APPEND CLI_ARGS "--RootHelperClass" "${ARG_ROOT_HELPER_CLASS}")
    
    list(APPEND CLI_ARGS "--Specificators")
    foreach(SPECIFICATOR ${ARG_SPECIFICATORS})
        list(APPEND CLI_ARGS "${SPECIFICATOR}")
    endforeach()
    
    list(APPEND CLI_ARGS "--DefaultSpecificator" "${ARG_DEFAULT_SPECIFICATOR}")
    
    if(ARG_ITEM_DIRS)
        list(APPEND CLI_ARGS "--ItemDirectoryPaths")
        foreach(DIR ${ARG_ITEM_DIRS})
            list(APPEND CLI_ARGS "${DIR}")
        endforeach()
    endif()
    
    if(ARG_ITEM_SUPPORT_DIRS)
        list(APPEND CLI_ARGS "--ItemSupportDirectoryPaths")
        foreach(DIR ${ARG_ITEM_SUPPORT_DIRS})
            list(APPEND CLI_ARGS "${DIR}")
        endforeach()
    endif()
    
    if(ARG_SPECIFICATOR_DIRS)
        list(APPEND CLI_ARGS "--SpecificatorDirectoryPaths")
        foreach(DIR ${ARG_SPECIFICATOR_DIRS})
            list(APPEND CLI_ARGS "${DIR}")
        endforeach()
    endif()

    if(ARG_INCLUDES)
        list(APPEND CLI_ARGS "--Includes")
        foreach(DIR ${ARG_INCLUDES})
            list(APPEND CLI_ARGS "${DIR}")
        endforeach()
    endif()
    
    # Execute the generator script
    Abytek_ExecuteJS(
        RELATIVE_PATH "Abytek/Tools/OSBGenerate.js"
        CLI_ARGS ${CLI_ARGS}
    )
endfunction() 