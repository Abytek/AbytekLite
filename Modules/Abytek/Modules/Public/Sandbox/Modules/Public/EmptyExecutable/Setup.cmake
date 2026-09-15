
if(ABYTEK_MODULE_PHASE_INIT)
    set(EmptyExecutable.MacroName ABYTEK_SANDBOX_EMPTY_EXECUTABLE)
    set(EmptyExecutable.Type EXECUTABLE)
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    list(APPEND EmptyExecutable.PublicDependencies
        Base
    )
endif()
