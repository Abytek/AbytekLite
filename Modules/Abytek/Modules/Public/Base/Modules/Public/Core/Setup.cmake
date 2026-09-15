##
# @file Setup.cmake
# @brief Core module setup configuration
# 
# This file configures the Core module build settings, including memory allocation options,
# assertions, logging, and unit tests. It's processed through multiple module phases.
##

include(Abytek/Utilities/AddEnableAssertionsOption)
include(Abytek/Utilities/AddEnableLogOption)
include(Abytek/Utilities/AddEnableUnitTestsOption)
include(Abytek/Utilities/Development)


if(ABYTEK_MODULE_PHASE_INIT)
    # Define the Core module macro prefix
    set(Core.MacroName ABYTEK_BASE_CORE)

    # Memory allocation configuration options
    option(ABYTEK_BASE_CORE_ENABLE_MEMORY_ALLOCATION_INFO "Enable tracking of memory allocation information" ON)
    if(ABYTEK_AUTO_OPTIMIZATION_FLAGS AND ABYTEK_ENABLE_MAX_OPTIMIZATION)
        Abytek_SetGlobal(ABYTEK_BASE_CORE_ENABLE_MEMORY_ALLOCATION_INFO OFF)
    else()
        Abytek_SetGlobal(ABYTEK_BASE_CORE_ENABLE_MEMORY_ALLOCATION_INFO ${ABYTEK_BASE_CORE_ENABLE_MEMORY_ALLOCATION_INFO})
    endif()

    option(ABYTEK_BASE_CORE_ENABLE_STRING_ID_DEBUGGER "Enable string id debugger" OFF)
    if(ABYTEK_AUTO_OPTIMIZATION_FLAGS AND ABYTEK_ENABLE_MAX_OPTIMIZATION)
        Abytek_SetGlobal(ABYTEK_BASE_CORE_ENABLE_STRING_ID_DEBUGGER OFF)
    else()
        Abytek_SetGlobal(ABYTEK_BASE_CORE_ENABLE_STRING_ID_DEBUGGER ${ABYTEK_BASE_CORE_ENABLE_STRING_ID_DEBUGGER})
    endif()
    set(ABYTEK_BASE_CORE_NAME_DATABASE_HASH_SIZE 4096 CACHE STRING "Size of the name database hash table")
endif()

if(ABYTEK_MODULE_PHASE_GLOBAL_SHARE)
    # Configure unit testing support
    include(Abytek/AddEnableUnitTestsOptionForCurrentModule)

    # Configure assertion support
    include(Abytek/AddEnableAssertionsOptionForCurrentModule)
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    # Define module dependencies
    list(APPEND Core.PublicDependencies
        Minimal
    )
endif()
