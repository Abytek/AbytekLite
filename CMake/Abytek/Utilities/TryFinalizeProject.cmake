
include(Abytek/Utilities/SetGlobal)
include(Abytek/Utilities/SAFToolchain)

function(Abytek_TryFinalizeProject)    
    get_filename_component(CMAKE_CURRENT_SOURCE_DIR_REAL "${CMAKE_CURRENT_SOURCE_DIR}" REALPATH)
    get_filename_component(ABYTEK_PROJECT_DIR_REAL "${ABYTEK_PROJECT_DIR}" REALPATH)

    if(NOT CMAKE_CURRENT_SOURCE_DIR_REAL STREQUAL ABYTEK_PROJECT_DIR_REAL)
        return()
    endif()
    
    Abytek_SAF_AutoSetupApplications()
endfunction()
