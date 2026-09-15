
include(Abytek/Utilities/AddEnableAssertionsOption)
include(Abytek/Utilities/AddEnableLogOption)
include(Abytek/Utilities/AddEnableUnitTestsOption)


if(ABYTEK_MODULE_PHASE_INIT)
    set(Operator.MacroName ABYTEK_OPERATOR)
    set(Operator.Type EXECUTABLE)
    set(Operator.Enable ${ABYTEK_ENABLE_OPERATOR})
endif()

if(ABYTEK_MODULE_PHASE_GLOBAL_SHARE)
    # test
    include(Abytek/AddEnableUnitTestsOptionForCurrentModule)

    # assert
    include(Abytek/AddEnableAssertionsOptionForCurrentModule)
endif()
