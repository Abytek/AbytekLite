
include(Abytek/Utilities/AddEnableAssertionsOption)
include(Abytek/Utilities/AddEnableLogOption)
include(Abytek/Utilities/AddEnableUnitTestsOption)
include(Abytek/Utilities/AddFileAsDependency)


if(ABYTEK_MODULE_PHASE_INIT)
    set(Python.MacroName ABYTEK_ENGINE_PLUGINS_PYTHON)

    if(EMSCRIPTEN)
        set(Python.Enable OFF)
    elseif(ANDROID)
        set(Python.Enable OFF)
    else()
        # Add rpath entries for Xcode frameworks.
        if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
            set(CMAKE_BUILD_RPATH "/Library/Developer/CommandlineTools/Library/Frameworks" CACHE STRING "")
            set(CMAKE_INSTALL_RPATH "/Library/Developer/CommandlineTools/Library/Frameworks" CACHE STRING "")
        endif()

        find_package(Python REQUIRED COMPONENTS Development)
        set(ParsedPythonLibraries "")
        foreach(lib ${Python_LIBRARIES})
            if(${lib} STREQUAL "debug")
                list(FIND Python_LIBRARIES "debug" debug_index)
                math(EXPR debug_lib_index "${debug_index}+1")
                list(GET Python_LIBRARIES ${debug_lib_index} debug_lib)
                list(APPEND ParsedPythonLibraries "$<$<CONFIG:Debug>:${debug_lib}>")
            endif()
            if(${lib} STREQUAL "optimized")
                list(FIND Python_LIBRARIES "optimized" optimized_index)
                math(EXPR optimized_lib_index "${optimized_index}+1")
                list(GET Python_LIBRARIES ${optimized_lib_index} optimized_lib)
                list(APPEND ParsedPythonLibraries "$<$<CONFIG:Release>:${optimized_lib}>")
            endif()
        endforeach()
        # Check if ParsedPythonLibraries is still empty and set it to Python_LIBRARIES if it is
        if("${ParsedPythonLibraries}" STREQUAL "")
            set(ParsedPythonLibraries ${Python_LIBRARIES})
        endif()
        Abytek_SetGlobal(${CurrentTarget}.PythonLibraries "${ParsedPythonLibraries}")
        Abytek_SetGlobal(${CurrentTarget}.PythonIncludeDirectories "${Python_INCLUDE_DIRS}")
    endif()
endif()

if(ABYTEK_MODULE_PHASE_GLOBAL_SHARE)
    # test
    include(Abytek/AddEnableUnitTestsOptionForCurrentModule)

    # assert
    include(Abytek/AddEnableAssertionsOptionForCurrentModule)
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    list(APPEND Python.PublicDependencies
        Engine.Core
    )

    target_link_libraries(${CurrentTarget}
        PUBLIC
            ${${CurrentTarget}.PythonLibraries}
    )
    target_include_directories(${CurrentTarget}
        PUBLIC
            ${${CurrentTarget}.PythonIncludeDirectories}
    )
    target_compile_definitions(${CurrentTarget}
        PUBLIC
            ABYTEK_ENABLE_PYTHON=1
    )
endif()
