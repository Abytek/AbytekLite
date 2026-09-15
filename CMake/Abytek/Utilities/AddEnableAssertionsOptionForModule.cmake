
include(Abytek/Utilities/AddEnableAssertionsOption)



function(Abytek_AddEnableAssertionsOptionForModule ModuleName Description DefaultValue)
    Abytek_AddEnableAssertionsOption(${${ModuleName}.MacroName}_ "" "${Description}" ${DefaultValue})
endfunction()