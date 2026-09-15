
include(Abytek/Utilities/AddEnableUnitTestsOption)



function(Abytek_AddEnableUnitTestsOptionForModule ModuleName Description DefaultValue)
    Abytek_AddEnableUnitTestsOption(${${ModuleName}.MacroName}_ "" "${Description}" ${DefaultValue})
endfunction()