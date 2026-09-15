
function(Abytek_TempDependenciesDirBase TARGET OUT_VAR)
    get_target_property(BinaryDir ${TARGET} BINARY_DIR)
    set(${OUT_VAR} "${BinaryDir}/___temp_dependencies_${TARGET}___" PARENT_SCOPE)
endfunction()

function(Abytek_TempDependenciesDir TARGET OUT_VAR)
    get_target_property(BinaryDir ${TARGET} BINARY_DIR)
    set(${OUT_VAR} "${BinaryDir}/___temp_dependencies_${TARGET}___/$<CONFIG>" PARENT_SCOPE)
endfunction()
