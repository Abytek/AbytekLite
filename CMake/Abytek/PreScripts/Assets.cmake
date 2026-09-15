
if(ABYTEK_MODULE_PHASE_LATE_INIT)
    Abytek_SetGlobal(${CurrentTarget}.AssetsDirectory "${${CurrentTarget}.Directory}/Assets")
    if(NOT EXISTS "${${CurrentTarget}.AssetsDirectory}")
        file(MAKE_DIRECTORY "${${CurrentTarget}.AssetsDirectory}")
    endif()

    Abytek_SetGlobal(${CurrentTarget}.CookedAssetsDirectory "${${CurrentTarget}.BuildDirectory}/CookedAssets")
    if(NOT EXISTS "${${CurrentTarget}.CookedAssetsDirectory}")
        file(MAKE_DIRECTORY "${${CurrentTarget}.CookedAssetsDirectory}")
    endif()

    Abytek_SetGlobal(${CurrentTarget}.AssetCacheDirectory "${${CurrentTarget}.BuildDirectory}/AssetCache")
    if(NOT EXISTS "${${CurrentTarget}.AssetCacheDirectory}")
        file(MAKE_DIRECTORY "${${CurrentTarget}.AssetCacheDirectory}")
    endif()

    Abytek_SetGlobal(${CurrentTarget}.AssetDirectorFile "${ABYTEK_ASSET_DIRECTORS_DIR}/${CurrentTarget}.json")
    file(WRITE "${${CurrentTarget}.AssetDirectorFile}" "
        {
            \"AssetsDirectory\": \"${${CurrentTarget}.AssetsDirectory}\",
            \"CookedAssetsDirectory\": \"${${CurrentTarget}.CookedAssetsDirectory}\",
            \"AssetCacheDirectory\": \"${${CurrentTarget}.AssetCacheDirectory}\"
        }
    ")
    message(STATUS "Generated asset director file for ${CurrentTarget} at ${${CurrentTarget}.AssetDirectorFile}")
endif()

if(ABYTEK_MODULE_PHASE_GLOBAL_SHARE)
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
endif()
