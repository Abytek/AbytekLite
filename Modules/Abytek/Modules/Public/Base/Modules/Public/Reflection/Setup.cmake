
include(Abytek/Utilities/AddEnableAssertionsOption)
include(Abytek/Utilities/AddEnableLogOption)
include(Abytek/Utilities/AddEnableUnitTestsOption)
include(Abytek/Utilities/AddResolveReflectorInline)



if(ABYTEK_MODULE_PHASE_INIT)
    set(Reflection.MacroName ABYTEK_BASE_REFLECTION)

    option(ABYTEK_ENABLE_REFLECTION_LOG OFF)
    
    Abytek_SetGlobal(Abytek.Base.Reflection.ResolveReflectorFiles "")
    Abytek_SetGlobal(Abytek.Base.Reflection.GeneratedSource.AllResolveReflectorsFile "${Abytek.Base.Reflection.GeneratedSource.PublicDirectory}/Abytek/AllResolveReflectors.inl")
endif()

if(ABYTEK_MODULE_PHASE_GLOBAL_SHARE)
    # test
    include(Abytek/AddEnableUnitTestsOptionForCurrentModule)

    # assert
    include(Abytek/AddEnableAssertionsOptionForCurrentModule)

    Abytek_AddResolveReflectorInline(
        "${Abytek.Base.Reflection.Source.PublicDirectory}/Abytek/ResolveCoreContainerReflectors.inl"
    )
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    list(APPEND Reflection.PublicDependencies
        Minimal
        Core
        Object
    )

    if(MSVC)
        target_compile_options(${CurrentTarget}
            PUBLIC
                "/bigobj"
        )
    endif()
endif()

if(ABYTEK_MODULE_PHASE_LATE_SETUP)
    if(
        NOT EXISTS "${Abytek.Base.Reflection.GeneratedSource.AllResolveReflectorsFile}" 
        OR NOT "${Abytek.Base.Reflection.ResolveReflectorFiles}" STREQUAL "${Abytek.Base.Reflection.LastResolveReflectorFiles}"
    )
        set(Content "")
        foreach(ResolveReflectorFile ${Abytek.Base.Reflection.ResolveReflectorFiles})
            set(Content "${Content}\n
                #include \"${ResolveReflectorFile}\"
            ")
        endforeach()
        file(WRITE "${Abytek.Base.Reflection.GeneratedSource.AllResolveReflectorsFile}" "${Content}")
    endif()
    Abytek_SetGlobal(Abytek.Base.Reflection.LastResolveReflectorFiles "${Abytek.Base.Reflection.ResolveReflectorFiles}")
endif()
