
include(Abytek/Utilities/SetGlobal)



function(Abytek_ApplyGlobal)

    foreach(ARG ${ARGS})
        Abytek_SetGlobal(${ARG} ${${ARG}})
    endforeach()
    
endfunction()
