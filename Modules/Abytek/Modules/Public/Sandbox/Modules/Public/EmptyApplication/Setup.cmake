
if(ABYTEK_MODULE_PHASE_INIT)
    set(EmptyApplication.MacroName ABYTEK_SANDBOX_EMPTY_APPLICATION)
    set(EmptyApplication.Enable ${Abytek.Engine.Enable})
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    list(APPEND EmptyApplication.PublicDependencies
        Engine
    )
endif()
