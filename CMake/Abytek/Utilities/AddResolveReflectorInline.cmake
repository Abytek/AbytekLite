
include(Abytek/Utilities/SetGlobal)



function(Abytek_AddResolveReflectorInline Path)
    list(APPEND Abytek.Base.Reflection.ResolveReflectorFiles "${Path}")
    Abytek_SetGlobal(Abytek.Base.Reflection.ResolveReflectorFiles "${Abytek.Base.Reflection.ResolveReflectorFiles}")
endfunction()