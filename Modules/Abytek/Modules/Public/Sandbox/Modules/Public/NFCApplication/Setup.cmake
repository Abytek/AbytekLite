
include(Abytek/Utilities/AddEnableAssertionsOption)
include(Abytek/Utilities/AddEnableLogOption)
include(Abytek/Utilities/AddEnableUnitTestsOption)


if(ABYTEK_MODULE_PHASE_INIT)
    set(NFCApplication.MacroName ABYTEK_SANDBOX_NFC_APPLICATION)
    set(NFCApplication.Enable ${Abytek.Engine.Enable})
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    list(APPEND NFCApplication.PublicDependencies
        Engine
    )
endif()
