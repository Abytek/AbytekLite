
include(Abytek/Utilities/AddEnableAssertionsOption)
include(Abytek/Utilities/AddEnableLogOption)
include(Abytek/Utilities/AddEnableUnitTestsOption)


if(ABYTEK_MODULE_PHASE_INIT)
    set(Assets.MacroName ABYTEK_ENGINE_ASSETS)

    list(APPEND ABYTEK_SETUP_PRE_SCRIPTS "Abytek/PreScripts/Assets")

    Abytek_SetGlobal(ABYTEK_ASSET_DIRECTORS_DIR "${ABYTEK_DEVELOPMENT_DATA_DIR}/AssetDirectors")
    if (EXISTS "${ABYTEK_ASSET_DIRECTORS_DIR}")
        file(REMOVE_RECURSE "${ABYTEK_ASSET_DIRECTORS_DIR}")
    endif()
    file(MAKE_DIRECTORY "${ABYTEK_ASSET_DIRECTORS_DIR}")
endif()

if(ABYTEK_MODULE_PHASE_GLOBAL_SHARE)
    # test
    include(Abytek/AddEnableUnitTestsOptionForCurrentModule)

    # assert
    include(Abytek/AddEnableAssertionsOptionForCurrentModule)
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    list(APPEND Assets.PublicDependencies
        Core
    )
endif()
