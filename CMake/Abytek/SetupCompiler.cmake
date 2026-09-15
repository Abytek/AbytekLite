
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-fpermissive)
endif()

if(EMSCRIPTEN)
    option(ABYTEK_EMSCRIPTEN_EXECUTABLE_JS_ONLY "Compiles to native JS (No WASM)" OFF)
    if(ABYTEK_EMSCRIPTEN_EXECUTABLE_JS_ONLY)
        set(CMAKE_EXECUTABLE_SUFFIX ".js")
    else()
        set(CMAKE_EXECUTABLE_SUFFIX ".wasm.js")
    endif()

    add_compile_options(-fdeclspec)
    add_compile_options(-fms-extensions)
endif()

if(MSVC)
    # Enable parallel build on MSVC
    add_compile_options(/MP)
endif()

# if(ABYTEK_EBT_CMAKE_GENERATOR_NINJA AND ABYTEK_EBT_CMAKE_CONFIG_DEBUG)

#     message(STATUS ">>> Enabling SAFE DEBUG MODE (low memory clang++)")

#     # -----------------------------
#     # Core compiler flags
#     # -----------------------------
#     set(SAFE_DEBUG_FLAGS
#         -O1
#         -g1
#         -fno-omit-frame-pointer
#         -fno-limit-debug-info
#         -fno-inline
#     )

#     add_compile_options(${SAFE_DEBUG_FLAGS})

#     # -----------------------------
#     # Split DWARF (giảm RAM peak cực mạnh)
#     # -----------------------------
#     add_compile_options(-gsplit-dwarf)

#     # -----------------------------
#     # Optional: reduce template explosion pressure
#     # -----------------------------
#     add_compile_options(-fno-standalone-debug)

# endif()

# if (ABYTEK_EBT_CMAKE_GENERATOR_NINJA)
#     set_property(GLOBAL PROPERTY JOB_POOLS compile_pool=1)
# endif()