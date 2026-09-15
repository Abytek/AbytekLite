include(Abytek/Utilities/AddEnableAssertionsOption)
include(Abytek/Utilities/AddEnableLogOption)
include(Abytek/Utilities/AddEnableUnitTestsOption)
include(Abytek/Utilities/GitHelper)
include(Abytek/Utilities/SetGlobal)
include(Abytek/Utilities/Development)
include(Abytek/Utilities/RHIRuntime)
include(Abytek/Utilities/RHIToolchain)
include(Abytek/Utilities/RHIRuntime)
include(Abytek/Utilities/AddFileAsDependency)

include(FetchContent)


function(RHI_Utilities_SetupPix)
    set(PIX_BASE_URL "https://www.nuget.org/api/v2/package/WinPixEventRuntime/1.0.240308001")

    set(PIX_DOWNLOAD_PATH "${ABYTEK_ENGINE_RHI_PIX_DIR}/../Pix.nupkg")
    set(PIX_EXTRACT_DIR "${ABYTEK_ENGINE_RHI_PIX_DIR}")

    # Download PIX installer
    if(NOT EXISTS ${PIX_DOWNLOAD_PATH})
        message(STATUS "Downloading PIX...")
        file(DOWNLOAD
            ${PIX_BASE_URL}
            ${PIX_DOWNLOAD_PATH}
            SHOW_PROGRESS
        )
    endif()

    # Extract
    if(NOT EXISTS ${PIX_EXTRACT_DIR})
        message(STATUS "Extracting PIX...")
        file(MAKE_DIRECTORY "${PIX_EXTRACT_DIR}")
        file(ARCHIVE_EXTRACT INPUT ${PIX_DOWNLOAD_PATH} DESTINATION ${PIX_EXTRACT_DIR} VERBOSE)
    endif()

    # Guess paths
    set(PIX_INCLUDE_DIR "${PIX_EXTRACT_DIR}/Include")
    set(PIX_LIB "${PIX_EXTRACT_DIR}/bin/x64/WinPixEventRuntime.lib")
    set(PIX_DLL "${PIX_EXTRACT_DIR}/bin/x64/WinPixEventRuntime.dll")

    if(
        EXISTS "${PIX_INCLUDE_DIR}"
        AND EXISTS "${PIX_LIB}"
        AND EXISTS "${PIX_DLL}"
    )
        Abytek_SetGlobal(ABYTEK_ENGINE_RHI_PIX_INCLUDE_DIR "${PIX_INCLUDE_DIR}")
        Abytek_SetGlobal(ABYTEK_ENGINE_RHI_PIX_LIB "${PIX_LIB}")
        Abytek_SetGlobal(ABYTEK_ENGINE_RHI_PIX_DLL "${PIX_DLL}")

        message(STATUS "PIX setup OK")
    else()
        message(FATAL_ERROR "PIX setup failed ${PIX_EXTRACT_DIR}")
    endif()
endfunction()


function(RHI_Utilities_CheckDependencies)
    if(ABYTEK_ENGINE_RHI_ENABLE_SLANG)
        message(STATUS "Importing AbytekSlang")
        Abytek_SetGlobal(SLANG_LIB_TYPE STATIC)
        Abytek_SetGlobal(ABYTEK_SLANG_ENABLE_BUILTIN_SHADER_MODULE_TIMESTAMP_CHECKING OFF)
        Abytek_SetGlobal(SLANG_ENABLE_TESTS OFF)
        add_subdirectory("${ABYTEK_SLANG_DIR}" "${ABYTEK_DEPENDENCIES_DIR}/AbytekSlang/Build")
        Abytek_SetGlobal(ABYTEK_ENGINE_RHI_SLANG_DIR "${ABYTEK_SLANG_DIR}/source/slang")
        message(STATUS "Imported AbytekSlang")
    endif()
    
    if(ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12)
        #####################################################################################
        #   DirectX-Headers checking
        #####################################################################################
        if(NOT TARGET DirectX-Headers)
            if(NOT EXISTS "${ABYTEK_DEPENDENCIES_DIR}/DirectX-Headers")
                file(MAKE_DIRECTORY "${ABYTEK_DEPENDENCIES_DIR}/DirectX-Headers")
                Abytek_GitHelper_Clone(
                    PROJECT_NAME "DirectX-Headers"
                    GIT_URL "https://github.com/microsoft/DirectX-Headers"
                    GIT_COMMIT "e472d3be36858c5090fea7e1ae21806436b2ed32"
                    GIT_BRANCH "main"
                    DIRECTORY "${ABYTEK_DEPENDENCIES_DIR}"
                )
            endif()
        endif()
    endif()

    if(ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12_AGILITY)
        set(ABYTEK_DIRECTX12_AGILITY_SDK_VERSION_MAJOR "614" CACHE STRING "The version major of DirectX12 Agility SDK")
        set(ABYTEK_DIRECTX12_AGILITY_SDK_VERSION_MINOR "1" CACHE STRING "The version minor of DirectX12 Agility SDK")
        Abytek_SetGlobal(ABYTEK_DIRECTX12_AGILITY_SDK_NUGET_URL "https://www.nuget.org/api/v2/package/Microsoft.Direct3D.D3D12/1.${ABYTEK_DIRECTX12_AGILITY_SDK_VERSION_MAJOR}.${ABYTEK_DIRECTX12_AGILITY_SDK_VERSION_MINOR}")

        FetchContent_Declare(abytek_directx12_agility_sdk URL ${ABYTEK_DIRECTX12_AGILITY_SDK_NUGET_URL})
        FetchContent_MakeAvailable(abytek_directx12_agility_sdk)

        Abytek_SetGlobal(ABYTEK_DIRECTX12_AGILITY_SDK_D3D12_CORE_DLL      "${abytek_directx12_agility_sdk_SOURCE_DIR}/build/native/bin/x64/D3D12Core.dll")
        Abytek_SetGlobal(ABYTEK_DIRECTX12_AGILITY_SDK_D3D12_SDK_LAYERS_DLL "${abytek_directx12_agility_sdk_SOURCE_DIR}/build/native/bin/x64/d3d12SDKLayers.dll")

        Abytek_SetGlobal(ABYTEK_DIRECTX12_AGILITY_SDK_DIR ${abytek_directx12_agility_sdk_SOURCE_DIR})
    endif()

    if(ABYTEK_ENGINE_RHI_ENABLE_PIX)
        RHI_Utilities_SetupPix()
    endif()

    if(ABYTEK_ENGINE_RHI_ENABLE_NVTX)
        Abytek_GitHelper_Clone(
            PROJECT_NAME "NVTX"
            GIT_URL "https://github.com/NVIDIA/NVTX"
            GIT_COMMIT "ffc00b3ce65b1b276315ab1b3aeca18d153441a5"
            GIT_BRANCH "release-v3"
            DIRECTORY "${ABYTEK_DEPENDENCIES_DIR}"
        )
    endif()
endfunction()

function(RHI_Utilities_ConfigureDependencies)
    if(ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12)
        #####################################################################################
        #   Add DirectX-Headers subdirectory
        #####################################################################################
        if(NOT TARGET DirectX-Headers)
            add_subdirectory("${ABYTEK_DEPENDENCIES_DIR}/DirectX-Headers" "${ABYTEK_DEPENDENCIES_DIR}/DirectX-Headers/Build")
        endif()
    endif()

    if(ABYTEK_ENGINE_RHI_ENABLE_NVTX)   
        if(NOT TARGET nvtx3-cpp)
            add_subdirectory("${ABYTEK_DEPENDENCIES_DIR}/NVTX" "${ABYTEK_DEPENDENCIES_DIR}/NVTX/Build")
        endif()
    endif()
endfunction()


if(ABYTEK_MODULE_PHASE_INIT)
    set(RHI.MacroName ABYTEK_ENGINE_RHI)

    # This code may need to be used if RHI fully uses static API linking but it actually uses dynamic API linking => safe to enable dummy API
    # if(WIN32)
    # else()
    #     set(RHI.Enable OFF)
    # endif()

    RHI_Utilities_CheckDependencies()
    RHI_Utilities_ConfigureDependencies()
endif()

if(ABYTEK_MODULE_PHASE_GLOBAL_SHARE)
    # test
    include(Abytek/AddEnableUnitTestsOptionForCurrentModule)

    # assert
    include(Abytek/AddEnableAssertionsOptionForCurrentModule)
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    list(APPEND RHI.PublicDependencies
        Minimal
        Core
        Window
        Assets
    )

    if(ABYTEK_ENGINE_RHI_ENABLE_DXC)
        # list(APPEND RHI.PublicDependencies
        #     dxclib
        #     dxcompiler
        # )
    endif()

    if(ABYTEK_ENGINE_RHI_ENABLE_SLANG)
        if(ABYTEK_ENGINE_RHI_ENABLE_DXC)
            Abytek_AddFileAsDependency(${CurrentTarget} "${ABYTEK_SLANG_DXCOMPILER_DLL_FILE}")
            Abytek_AddFileAsDependency(${CurrentTarget} "${ABYTEK_SLANG_DXIL_DLL_FILE}")
        endif()
    endif()

    if(ABYTEK_ENGINE_RHI_ENABLE_SLANG)
        list(APPEND RHI.PublicDependencies
            slang-without-embedded-core-module
        )
    endif()

    if(ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12)
        target_link_libraries(${CurrentTarget}
            PUBLIC
                d3d12.lib
                DirectX-Headers
        )
    endif()
    if(ABYTEK_ENGINE_RHI_ENABLE_DIRECTX11)
        target_link_libraries(${CurrentTarget}
            PUBLIC
                d3d11.lib
        )
    endif()
    if(ABYTEK_ENGINE_RHI_ENABLE_DIRECTX)
        target_link_libraries(${CurrentTarget}
            PUBLIC
                d3dcompiler.lib
        )
    endif()
    if(ABYTEK_ENGINE_RHI_ENABLE_DXGI)
        target_link_libraries(${CurrentTarget}
            PUBLIC
                dxgi.lib
        )
        if(ABYTEK_ENGINE_RHI_ENABLE_DRIVER_DEBUGGER)
            target_link_libraries(${CurrentTarget}
                PUBLIC
                    dxguid.lib
            )
        endif()
    endif()

    if(ABYTEK_ENGINE_RHI_ENABLE_PIX)
        target_include_directories(${CurrentTarget}
            PUBLIC
                "${ABYTEK_ENGINE_RHI_PIX_INCLUDE_DIR}"
        )
        target_link_libraries(${CurrentTarget}
            PUBLIC
                "${ABYTEK_ENGINE_RHI_PIX_LIB}"
        )
        Abytek_AddRuntimeFile(
            "${ABYTEK_ENGINE_RHI_PIX_DLL}"
            "WinPixEventRuntime.dll"
        )
    endif()

    if(ABYTEK_ENGINE_RHI_ENABLE_NVTX)
        list(APPEND RHI.PublicDependencies
            nvtx3-cpp
        )
    endif()

    # MSVC compile options
    if(MSVC)
        target_compile_options(${CurrentTarget} PUBLIC /GT)
    endif()

    # MacOS libs
    if(APPLE)
        # Find the Cocoa lib
        find_library(COCOA_LIBRARY Cocoa)
        if (NOT COCOA_LIBRARY)
            message(FATAL_ERROR "Cocoa not found")
        endif()

        target_link_libraries(${CurrentTarget}
            PRIVATE
                ${COCOA_LIBRARY}
        )
    endif()

    # Setup DirectX12 Agility SDK
    if(ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12_AGILITY)
        Abytek_AddRuntimeFile(
            "${ABYTEK_DIRECTX12_AGILITY_SDK_D3D12_CORE_DLL}"
            "${ABYTEK_DIRECTX12_AGILITY_SDK_RUNTIME_RELATIVE_DIR}/D3D12Core.dll"
        )
        Abytek_AddRuntimeFile(
            "${ABYTEK_DIRECTX12_AGILITY_SDK_D3D12_SDK_LAYERS_DLL}"
            "${ABYTEK_DIRECTX12_AGILITY_SDK_RUNTIME_RELATIVE_DIR}/d3d12SDKLayers.dll"
        )
    endif()
endif()