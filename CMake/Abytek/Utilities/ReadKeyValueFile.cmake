
include(Abytek/Utilities/SetGlobal)

function(Abytek_ReadKeyValueFile FILE_PATH)
    if(NOT EXISTS "${FILE_PATH}")
        message(FATAL_ERROR "File not found: ${FILE_PATH}")
    endif()

    # Đọc file
    file(READ "${FILE_PATH}" FILE_CONTENTS)
    if(FILE_CONTENTS STREQUAL "")
        return()
    endif()
    if(FILE_CONTENTS STREQUAL "\r\n")
        return()
    endif()
    if(FILE_CONTENTS STREQUAL "\r")
        return()
    endif()

    string(REPLACE "\r\n" "\n" FILE_CONTENTS "${FILE_CONTENTS}")
    string(REPLACE "\r" "\n" FILE_CONTENTS "${FILE_CONTENTS}")
    string(REGEX REPLACE "\n$" "" FILE_CONTENTS "${FILE_CONTENTS}")

    # Chia thành list các dòng
    string(REGEX REPLACE "\n" ";" FILE_LINES "${FILE_CONTENTS}")

    list(LENGTH FILE_LINES NUM_LINES)
    math(EXPR MAX_INDEX "${NUM_LINES}-1")

    # Lặp qua từng cặp key/value
    foreach(INDEX RANGE 0 ${MAX_INDEX} 2)
        list(GET FILE_LINES ${INDEX} KEY)
        math(EXPR VALUE_INDEX "${INDEX}+1")
        if(${VALUE_INDEX} LESS ${NUM_LINES})
            list(GET FILE_LINES ${VALUE_INDEX} VALUE)
            Abytek_SetGlobal(${KEY} "${VALUE}")
        endif()
    endforeach()
endfunction()