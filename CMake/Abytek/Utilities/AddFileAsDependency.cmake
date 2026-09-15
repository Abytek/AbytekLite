
include(Abytek/Utilities/TempDependenciesDir)



function(Abytek_AddFileAsDependency TARGET FILE)
    Abytek_TempDependenciesDir(${TARGET} TargetTempDependenciesDir)
    add_custom_command(TARGET ${TARGET} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
            "${TargetTempDependenciesDir}"
        COMMAND ${CMAKE_COMMAND} -E copy
            "${FILE}"
            "${TargetTempDependenciesDir}/"
    )
endfunction()

function(Abytek_AddFileAsDependencyWithOffset TARGET FILE OFFSET)
    Abytek_TempDependenciesDir(${TARGET} TargetTempDependenciesDir)
    add_custom_command(TARGET ${TARGET} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
            "${TargetTempDependenciesDir}/${OFFSET}"
        COMMAND ${CMAKE_COMMAND} -E copy
            "${FILE}"
            "${TargetTempDependenciesDir}/${OFFSET}/"
    )
endfunction()