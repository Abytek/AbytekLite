
function(Abytek_SetupExecutable Target)

    # Setup LongPath manifest for windows
    if(MSVC)
        target_link_options(${Target} PRIVATE
            "/MANIFEST:EMBED"
            "/MANIFESTINPUT:${ABYTEK_ROOT_DIR}/GlobalSource/Abytek/Windows/LongPath.manifest"
        )
    endif()
endfunction()