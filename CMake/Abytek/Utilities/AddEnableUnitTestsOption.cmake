
include(Abytek/Utilities/SetGlobal)
include(Abytek/Utilities/CommonFlags)


function(Abytek_AddEnableUnitTestsOption Prefix Suffix Description DefaultValue)
    set(MacroName ${Prefix}ENABLE_UNIT_TESTS${Suffix})

    option(${MacroName} "${Description}" ${DefaultValue})
    if(ABYTEK_DISABLE_ALL_UNIT_TESTS)
        Abytek_SetGlobal(${MacroName} OFF)
    endif()
    if(ABYTEK_ENABLE_ALL_UNIT_TESTS)
        Abytek_SetGlobal(${MacroName} ON)
    endif()

    if(ABYTEK_AUTO_OPTIMIZATION_FLAGS AND ABYTEK_ENABLE_MAX_OPTIMIZATION)
        Abytek_SetGlobal(${MacroName} OFF)
    else()
        Abytek_SetGlobal(${MacroName} ${${MacroName}})
    endif()

    if(${MacroName})
        message(STATUS "Enabled unit test option \"${MacroName}\"")
        file(APPEND "${ABYTEK_UNIT_TEST_DEFINES_HPP_TEMP_FILE}" "#define ${MacroName}\n")
    else()
        message(STATUS "Disabled unit test option \"${MacroName}\"")
    endif()
endfunction()