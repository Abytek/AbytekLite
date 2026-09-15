
if(ABYTEK_MODULE_PHASE_INIT)
    set(${ModuleName}.MacroName ABYTEK_TEMPLATES_DEMO_PACKAGE)
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    list(APPEND ${ModuleName}.PublicDependencies
        Abytek.Engine
    )
endif()