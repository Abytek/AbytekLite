
include(Abytek/Utilities/SetGlobal)
include(Abytek/Utilities/CommonFlags)


function(Abytek_AddEnableAssertionsOption Prefix Suffix Description DefaultValue)
    set(MacroName ${Prefix}ENABLE_ASSERTIONS${Suffix})

    option(${MacroName} "${Description}" ${DefaultValue})
    if(ABYTEK_DISABLE_ALL_ASSERTIONS)
        Abytek_SetGlobal(${MacroName} OFF)
    endif()
    if(ABYTEK_ENABLE_ALL_ASSERTIONS)
        Abytek_SetGlobal(${MacroName} ON)
    endif()

    if(ABYTEK_AUTO_OPTIMIZATION_FLAGS AND ABYTEK_ENABLE_MAX_OPTIMIZATION)
        Abytek_SetGlobal(${MacroName} OFF)
    else()
        Abytek_SetGlobal(${MacroName} ${${MacroName}})
    endif()

    if(${MacroName})
        message(STATUS "Enabled assertion option \"${MacroName}\"")
        file(APPEND "${ABYTEK_ASSERTION_DEFINES_HPP_TEMP_FILE}" "#define ${MacroName}\n")
        file(APPEND "${ABYTEK_ASSERTION_DEFINES_HPP_TEMP_FILE}" "#define ${Prefix}ASSERT${Suffix}(...) ABYTEK_ASSERT_BASE(${Prefix},${Suffix}, __VA_ARGS__)\n")
    else()
        message(STATUS "Disabled assertion option \"${MacroName}\"")
        file(APPEND "${ABYTEK_ASSERTION_DEFINES_HPP_TEMP_FILE}" "#define ${Prefix}ASSERT${Suffix}(...) ABYTEK_ASSERT_BASE_DISABLED(${Prefix},${Suffix}, __VA_ARGS__)\n")
    endif()
endfunction()