
if(ABYTEK_MODULE_PHASE_INIT)
    set(EmptyApplication2.MacroName ABYTEK_SANDBOX_EMPTY_APPLICATION2)
    set(EmptyApplication2.Enable ${Abytek.Engine.Enable})
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    list(APPEND EmptyApplication2.PublicDependencies
        Engine
    )
endif()
