# Module System Examples

This document provides real-world examples of module configuration using the module system.

## Basic Static Library Module

This example shows a simple static library module with minimal configuration:

```cmake
# MyLibrary/Setup.cmake

if(ABYTEK_MODULE_PHASE_INIT)
    # Basic module configuration
    set(MyLibrary.Type STATIC)
    set(MyLibrary.MacroName MY_LIBRARY)
    
    # Dependencies
    set(MyLibrary.PrivateDependencies "Base.Minimal")
    
    # Module options
    option(MY_LIBRARY_ENABLE_LOGGING "Enable library logging" ON)
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    # Configure the target
    if(MY_LIBRARY_ENABLE_LOGGING)
        target_compile_definitions(${CurrentTarget}
            PRIVATE
                MY_LIBRARY_LOGGING_ENABLED=1
        )
    endif()
endif()
```

## Executable Module Example

This example shows how to create an executable module:

```cmake
# MyExecutable/Setup.cmake

if(ABYTEK_MODULE_PHASE_INIT)
    # Set as executable
    set(MyExecutable.Type EXECUTABLE)
    
    # Dependencies
    set(MyExecutable.PrivateDependencies "Base.Minimal" "OSBridge.Core" "MyLibrary")
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    # Set executable properties
    if(WIN32)
        # Set Win32 properties
        set_target_properties(${CurrentTarget} PROPERTIES
            WIN32_EXECUTABLE TRUE
        )
    endif()
    
    # Set output name
    set_target_properties(${CurrentTarget} PROPERTIES
        OUTPUT_NAME "MyApp"
    )
endif()

if(ABYTEK_MODULE_PHASE_LATE_SETUP)
    # Copy resources after build
    add_custom_command(
        TARGET ${CurrentTarget} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/Resources
            $<TARGET_FILE_DIR:${CurrentTarget}>/Resources
    )
endif()
```

## Module with Platform-Specific Code

This example shows a module that handles platform-specific configuration:

```cmake
# OSBridge.Thread/Setup.cmake

if(ABYTEK_MODULE_PHASE_INIT)
    set(Thread.Type STATIC)
    set(Thread.MacroName ABYTEK_OS_BRIDGE_THREAD)
    
    # Base dependencies
    set(Thread.PrivateDependencies "OSBridge.Core")
endif()

if(ABYTEK_MODULE_PHASE_GLOBAL_SHARE)
    # Platform-specific configuration
    if(WIN32)
        target_compile_definitions(${CurrentTarget}
            PRIVATE
                ABYTEK_PLATFORM_WINDOWS=1
        )
    elseif(APPLE)
        target_compile_definitions(${CurrentTarget}
            PRIVATE
                ABYTEK_PLATFORM_APPLE=1
        )
    elseif(UNIX)
        target_compile_definitions(${CurrentTarget}
            PRIVATE
                ABYTEK_PLATFORM_UNIX=1
        )
    endif()
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    # Platform-specific libraries
    if(WIN32)
        target_link_libraries(${CurrentTarget}
            PRIVATE
                winmm.lib
        )
    elseif(UNIX)
        find_package(Threads REQUIRED)
        target_link_libraries(${CurrentTarget}
            PRIVATE
                Threads::Threads
        )
    endif()
endif()
```

## Module with JavaScript Integration

This example shows a module that uses JavaScript for code generation:

```cmake
# CodeGenerator/Setup.cmake

if(ABYTEK_MODULE_PHASE_INIT)
    set(CodeGenerator.Type STATIC)
    set(CodeGenerator.MacroName CODE_GENERATOR)
    
    # Dependencies
    set(CodeGenerator.PrivateDependencies "Base.Minimal")
    
    # Input files for code generation
    set(CODE_GEN_SCHEMA_FILE "${CMAKE_CURRENT_SOURCE_DIR}/Source/Private/Schema.json" CACHE FILEPATH "Schema file")
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    # Output file paths
    set(GENERATED_HEADER "${CodeGenerator.GeneratedSource.PublicDirectory}/GeneratedTypes.h")
    set(GENERATED_SOURCE "${CodeGenerator.GeneratedSource.PrivateDirectory}/GeneratedTypes.cpp")
    
    # Run JavaScript code generator
    Abytek_ExecuteJS(
        RELATIVE_PATH "Generators/TypeGenerator.js"
        CLI_ARGS 
            "${CODE_GEN_SCHEMA_FILE}"
            "${GENERATED_HEADER}"
            "${GENERATED_SOURCE}"
    )
    
    # Add generated files to the build
    target_sources(${CurrentTarget}
        PRIVATE
            "${GENERATED_SOURCE}"
    )
endif()
```

JavaScript file (`CodeGenerator/JS/Generators/TypeGenerator.js`):

```javascript
module.exports = (context, args) => {
    const fs = require('fs');
    const path = require('path');
    
    const schemaFile = args[0];
    const headerFile = args[1];
    const sourceFile = args[2];
    
    console.log(`Generating types from schema: ${schemaFile}`);
    
    // Read schema
    const schema = JSON.parse(fs.readFileSync(schemaFile, 'utf8'));
    
    // Generate header content
    const headerContent = generateHeader(schema);
    
    // Generate source content
    const sourceContent = generateSource(schema, path.basename(headerFile));
    
    // Write files
    fs.writeFileSync(headerFile, headerContent);
    fs.writeFileSync(sourceFile, sourceContent);
    
    console.log('Code generation complete.');
    return true;
};

function generateHeader(schema) {
    // Header generation logic here
    return `// Generated header\n#pragma once\n\n// Types...\n`;
}

function generateSource(schema, headerName) {
    // Source generation logic here
    return `// Generated source\n#include "${headerName}"\n\n// Implementation...\n`;
}
```

## Custom Module Type Example

This example shows a module with CUSTOM type, which is useful for modules that don't produce binary output:

```cmake
# Documentation/Setup.cmake

if(ABYTEK_MODULE_PHASE_INIT)
    set(Documentation.Type CUSTOM)
    
    # Define input files
    set(Documentation.Depends
        "${CMAKE_CURRENT_SOURCE_DIR}/Docs/API.md"
        "${CMAKE_CURRENT_SOURCE_DIR}/Docs/Overview.md"
        "${CMAKE_CURRENT_SOURCE_DIR}/Docs/Examples.md"
    )
    
    # Define the command to generate documentation
    set(Documentation.Commands
        "${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/html"
        "doxygen ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile"
    )
    
    # Option to control documentation building
    option(BUILD_DOCUMENTATION "Build documentation" ON)
    
    # Disable if documentation is not needed
    if(NOT BUILD_DOCUMENTATION)
        set(Documentation.Enable OFF)
    endif()
endif()
```

## Module with ThirdParty Dependencies

This example shows a module that manages third-party dependencies:

```cmake
# RHI.Vulkan/Setup.cmake

if(ABYTEK_MODULE_PHASE_INIT)
    set(Vulkan.Type STATIC)
    set(Vulkan.MacroName ABYTEK_RHI_VULKAN)
    
    # Dependencies
    set(Vulkan.PrivateDependencies "RHI.Core")
    
    # Options
    option(RHI_VULKAN_ENABLE_VALIDATION "Enable Vulkan validation layers" ON)
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    # Find Vulkan package
    find_package(Vulkan REQUIRED)
    
    # Add Vulkan include directories
    target_include_directories(${CurrentTarget}
        PRIVATE
            ${Vulkan_INCLUDE_DIRS}
    )
    
    # Link against Vulkan
    target_link_libraries(${CurrentTarget}
        PRIVATE
            ${Vulkan_LIBRARIES}
    )
    
    # Add compile definitions
    if(RHI_VULKAN_ENABLE_VALIDATION)
        target_compile_definitions(${CurrentTarget}
            PRIVATE
                ABYTEK_VULKAN_VALIDATION_ENABLED=1
        )
    endif()
endif()
```

## Complete Module Hierarchy Example

This example shows how to set up a module hierarchy with parent and child modules:

Parent module (`Engine/Setup.cmake`):

```cmake
if(ABYTEK_MODULE_PHASE_INIT)
    set(Engine.Type CUSTOM)
    set(Engine.MacroName ABYTEK_ENGINE)
    
    # This is just a container module for engine components
endif()

if(ABYTEK_MODULE_PHASE_GLOBAL_SHARE)
    # Global engine options
    option(ENGINE_ENABLE_EDITOR "Enable editor support" ON)
    option(ENGINE_ENABLE_PROFILING "Enable engine profiling" OFF)
    
    # Propagate options to all engine modules
    if(ENGINE_ENABLE_PROFILING)
        add_compile_definitions(ABYTEK_ENGINE_PROFILING_ENABLED=1)
    endif()
endif()
```

Child module (`Engine/Modules/Public/Core/Setup.cmake`):

```cmake
if(ABYTEK_MODULE_PHASE_INIT)
    set(Core.Type STATIC)
    set(Core.MacroName ABYTEK_ENGINE_CORE)
    
    # Dependencies
    set(Core.PrivateDependencies "Base.Minimal" "OSBridge.Core")
endif()

if(ABYTEK_MODULE_PHASE_GLOBAL_SHARE)
    # Access parent module settings
    if(ENGINE_ENABLE_EDITOR)
        target_compile_definitions(${CurrentTarget}
            PRIVATE
                ABYTEK_ENGINE_EDITOR_ENABLED=1
        )
    endif()
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    # Additional Core-specific settings
    # ...
endif()
```

## Module with Configuration Files

This example shows a module that uses configuration files:

```cmake
# ConfigExample/Setup.cmake

if(ABYTEK_MODULE_PHASE_INIT)
    set(ConfigExample.Type STATIC)
    
    # Define configuration values
    set(CONFIG_EXAMPLE_VERSION "1.2.3" CACHE STRING "Module version")
    set(CONFIG_EXAMPLE_MAX_ITEMS 1000 CACHE STRING "Maximum items")
    option(CONFIG_EXAMPLE_ENABLE_FEATURE_A "Enable feature A" ON)
    option(CONFIG_EXAMPLE_ENABLE_FEATURE_B "Enable feature B" OFF)
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    # Additional configuration for the target
    # ...
endif()
```

Configuration file (`ConfigExample/Source/Public/ModuleConfig.config`):

```
// Configuration for ConfigExample module
#pragma once

// Version information
#define CONFIG_EXAMPLE_VERSION "@CONFIG_EXAMPLE_VERSION@"
#define CONFIG_EXAMPLE_VERSION_MAJOR @CONFIG_EXAMPLE_VERSION_MAJOR@
#define CONFIG_EXAMPLE_VERSION_MINOR @CONFIG_EXAMPLE_VERSION_MINOR@
#define CONFIG_EXAMPLE_VERSION_PATCH @CONFIG_EXAMPLE_VERSION_PATCH@

// Compile-time constants
#define CONFIG_EXAMPLE_MAX_ITEMS @CONFIG_EXAMPLE_MAX_ITEMS@

// Feature flags
#cmakedefine01 CONFIG_EXAMPLE_ENABLE_FEATURE_A
#cmakedefine01 CONFIG_EXAMPLE_ENABLE_FEATURE_B
```

Usage in C++ code:

```cpp
// ConfigExample/Source/Private/Module.cpp
#include "ModuleConfig.config.hpp"

void PrintConfig() {
    printf("ConfigExample v%s\n", CONFIG_EXAMPLE_VERSION);
    printf("Max items: %d\n", CONFIG_EXAMPLE_MAX_ITEMS);
    
    #if CONFIG_EXAMPLE_ENABLE_FEATURE_A
    printf("Feature A is enabled\n");
    #endif
    
    #if CONFIG_EXAMPLE_ENABLE_FEATURE_B
    printf("Feature B is enabled\n");
    #endif
}
``` 