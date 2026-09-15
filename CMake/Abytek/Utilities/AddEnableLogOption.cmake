
include(Abytek/Utilities/SetGlobal)
include(Abytek/Utilities/CommonFlags)


function(Abytek_AddEnableLogOption Prefix Suffix LogType Description DefaultValue)
    set(MacroName ${Prefix}ENABLE_LOG${Suffix})

    option(${MacroName} "${Description}" ${DefaultValue})
    if(ABYTEK_DISABLE_ALL_LOG)
        Abytek_SetGlobal(${MacroName} OFF)
    endif()
    if(ABYTEK_ENABLE_ALL_LOG)
        Abytek_SetGlobal(${MacroName} ON)
    endif()

    if(ABYTEK_AUTO_OPTIMIZATION_FLAGS AND ABYTEK_ENABLE_MAX_OPTIMIZATION)
        Abytek_SetGlobal(${MacroName} OFF)
    else()
        Abytek_SetGlobal(${MacroName} ${${MacroName}})
    endif()

    if(${MacroName})
        message(STATUS "Enabled log option \"${MacroName}\"")
        file(APPEND "${ABYTEK_LOG_DEFINES_HPP_TEMP_FILE}" "#define ${MacroName}\n")
        file(APPEND "${ABYTEK_LOG_DEFINES_HPP_TEMP_FILE}" "#define ${Prefix}LOG${Suffix}() ABYTEK_LOG_BASE(${Prefix},${Suffix}, ${LogType})\n")
    else()
        message(STATUS "Disabled log option \"${MacroName}\"")
        file(APPEND "${ABYTEK_LOG_DEFINES_HPP_TEMP_FILE}" "#define ${Prefix}LOG${Suffix}() ABYTEK_LOG_BASE_DISABLED(${Prefix},${Suffix}, ${LogType})\n")
    endif()
endfunction()