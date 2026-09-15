# C++ Language Features in Abytek

The Abytek module system provides several C++ language integration features to make development easier, more consistent, and more portable across platforms.

## Configuration Files

Configuration files (`.config` files) allow you to export CMake cached variables to C/C++ code.

### How Config Files Work

1. Place a `.config` file in your module's Source directory (in Private, Interface, or Public subdirectory)
2. During the build, a generated header file is created for each config file
3. You can include these generated headers in your C/C++ code

### Config File Structure

Config files use CMake's `configure_file` mechanism. Variables in the format `@VARIABLE@` or `${VARIABLE}` will be replaced with their CMake values.

Example config file (`MyModule/Source/Public/Config.config`):
```
// MyModule configuration
#define MY_MODULE_VERSION "@MY_MODULE_VERSION@"
#define MY_MODULE_DEBUG @MY_MODULE_DEBUG_ENABLED@
#define MY_MODULE_PLATFORM "${CMAKE_SYSTEM_NAME}"
```

Generated header (`GeneratedSource/Public/Config.config.hpp`):
```cpp
// MyModule configuration
#define MY_MODULE_VERSION "1.0.0"
#define MY_MODULE_DEBUG 1
#define MY_MODULE_PLATFORM "Windows"
```

### Generated Config File Paths

For a config file at path `<source_directory>/<Public|Interface|Private>/<n>.config`, the generated header file is created at path `<generated_source_directory>/<Public|Interface|Private>/<n>.config.hpp`.

### Using Config Files in CMake

To define values that will be used in config files:

```cmake
if(ABYTEK_MODULE_PHASE_INIT)
    set(MY_MODULE_VERSION "1.0.0" CACHE STRING "Module version")
    option(MY_MODULE_DEBUG_ENABLED "Enable debug mode" ON)
endif()
```

### Using Generated Config Headers in C++

```cpp
#include "Config.config.hpp"

void printVersion() {
    printf("Version: %s\n", MY_MODULE_VERSION);
    
    #if MY_MODULE_DEBUG
    printf("Debug mode enabled\n");
    #endif
}
```

## Include Directories

The module system automatically sets up include directories based on the module's directory structure and dependencies.

### Automatic Include Directories

For each module, these include directories are automatically set up:

- **Private Includes**: Only accessible within the module
  - `${ModuleName}.Source.PrivateDirectory`
  - `${ModuleName}.GeneratedSource.PrivateDirectory`
  
- **Interface Includes**: Accessible to dependent modules
  - `${ModuleName}.Source.InterfaceDirectory`
  - `${ModuleName}.GeneratedSource.InterfaceDirectory`
  
- **Public Includes**: Accessible to the module and dependent modules
  - `${ModuleName}.Source.PublicDirectory`
  - `${ModuleName}.GeneratedSource.PublicDirectory`

### Dependency Include Propagation

- **Private Dependencies**: Their public and interface includes are added to your module's private includes
- **Public Dependencies**: Their public and interface includes are added to your module's public includes

### Adding Custom Include Directories

You can add additional include directories in the `ABYTEK_MODULE_PHASE_TARGET_CREATED` phase:

```cmake
if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    # Add private include directory
    target_include_directories(${CurrentTarget}
        PRIVATE
            ${SOME_EXTERNAL_INCLUDE_DIR}
    )
    
    # Add public include directory (propagates to dependents)
    target_include_directories(${CurrentTarget}
        PUBLIC
            ${SOME_PUBLIC_INCLUDE_DIR}
    )
    
    # Add interface include directory (only for dependents)
    target_include_directories(${CurrentTarget}
        INTERFACE
            ${SOME_INTERFACE_INCLUDE_DIR}
    )
endif()
```

## Precompiled Headers

The module system supports precompiled headers to improve build times.

### Precompiled Header Files

All source files with the extension `.pch.hpp` or `.pch.h` will be built as pre-compiled headers.

### Using Precompiled Headers

1. Create a precompiled header file:

```cpp
// MyModule/Source/Private/MyModule.pch.hpp
#pragma once

// Standard headers
#include <vector>
#include <string>
#include <unordered_map>

// Common project headers
#include "Common/Types.h"
#include "Utils/Logging.h"
```

2. The module system will automatically set up this file as a precompiled header for your module.

## Module API Macros

The module system provides a mechanism for automatically handling symbol visibility in libraries through API macros. This is a separate feature from config files and is particularly important for shared libraries.

### Setting Module Macro Name

Each module can define a `MacroName` property in the `ABYTEK_MODULE_PHASE_INIT` phase:

```cmake
if(ABYTEK_MODULE_PHASE_INIT)
    set(MyModule.MacroName MY_MODULE)
endif()
```

### The `${ModuleMacroName}_API` Pattern

When a `MacroName` property is set (e.g., `MY_MODULE`), the system automatically defines corresponding macros:

- `MY_MODULE_API`: For symbols that should be exported/imported
- `MY_MODULE_LOCAL`: For symbols that should remain internal to the module

These macros are used to mark classes, functions, and variables:

```cpp
// In a header file:
class MY_MODULE_API MyClass {
public:
    void DoSomething();
};

MY_MODULE_API void MyGlobalFunction();
```

### API Macro Implementation

The API macro is implemented through CMake's code generation capabilities. The build system automatically creates appropriate compiler definitions based on the module type.

#### Base Macros in GlobalSource

The actual implementation relies on base definitions defined in `GlobalSource/Abytek/BuildSharedDefines.hpp`:

```cpp
// From GlobalSource/Abytek/BuildSharedDefines.hpp
#define ABYTEK_BUILD_SHARED_API_EXPORT_MSVC __declspec(dllexport)
#define ABYTEK_BUILD_SHARED_LOCAL_EXPORT_MSVC

#define ABYTEK_BUILD_SHARED_API_IMPORT_MSVC __declspec(dllimport)
#define ABYTEK_BUILD_SHARED_LOCAL_IMPORT_MSVC

#define ABYTEK_BUILD_SHARED_API_EXPORT_GNU __attribute__((visibility("default")))
#define ABYTEK_BUILD_SHARED_LOCAL_EXPORT_GNU __attribute__((visibility("hidden")))

#define ABYTEK_BUILD_SHARED_API_IMPORT_GNU __attribute__((visibility("default")))
#define ABYTEK_BUILD_SHARED_LOCAL_IMPORT_GNU __attribute__((visibility("hidden")))
```

#### Code Generation via CMake Target Definitions

For `STATIC` and `OBJECT` libraries:
```cmake
target_compile_definitions(${ModuleFullName}
    PUBLIC
        -D${${ModuleFullName}.MacroName}_API=
        -D${${ModuleFullName}.MacroName}_LOCAL=
)
```

For `SHARED` libraries (with MSVC compiler):
```cmake
target_compile_definitions(${ModuleFullName}
    PRIVATE
        -D${${ModuleFullName}.MacroName}_API=ABYTEK_BUILD_SHARED_API_EXPORT_MSVC
        -D${${ModuleFullName}.MacroName}_LOCAL=ABYTEK_BUILD_SHARED_LOCAL_EXPORT_MSVC
    INTERFACE
        -D${${ModuleFullName}.MacroName}_API=ABYTEK_BUILD_SHARED_API_IMPORT_MSVC
        -D${${ModuleFullName}.MacroName}_LOCAL=ABYTEK_BUILD_SHARED_LOCAL_IMPORT_MSVC
)
```

This approach automates the correct handling of symbol visibility across different platforms and compilers without requiring manual configuration.

### Using API Macros in Your Code

Best practices for using API macros:

1. Apply the API macro to all classes and functions that need to be accessible outside the module
2. Do not apply the API macro to private implementation details

Example:

```cpp
// MyModule/Source/Public/MyClass.h
#pragma once

class MY_MODULE_API MyClass {
public:
    void PublicMethod();
    
private:
    // Private methods don't need the API macro
    void PrivateMethod();
};
```

## Example: Complete Configuration Workflow

Here's an example showing how to set up configuration files, include directories, and API macros:

```cmake
# Setup.cmake
if(ABYTEK_MODULE_PHASE_INIT)
    # Set library type
    set(MyFeature.Type SHARED)
    
    # Set API macro name
    set(MyFeature.MacroName MY_FEATURE)
    
    # Define configuration variables
    set(FEATURE_VERSION "2.1.0" CACHE STRING "Feature version")
    option(FEATURE_ENABLE_ADVANCED "Enable advanced features" OFF)
    
    # Set dependencies
    set(MyFeature.PrivateDependencies "Base.Minimal")
    set(MyFeature.PublicDependencies "Base.Math")
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    # Add extra include directories if needed
    if(FEATURE_ENABLE_ADVANCED)
        target_include_directories(${CurrentTarget}
            PRIVATE
                ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/AdvancedLib/include
        )
        
        target_compile_definitions(${CurrentTarget}
            PRIVATE
                ADVANCED_FEATURES_ENABLED=1
        )
    endif()
endif()
```

```
# MyFeature/Source/Public/FeatureConfig.config
#pragma once

#define FEATURE_VERSION "@FEATURE_VERSION@"

#cmakedefine01 FEATURE_ENABLE_ADVANCED
```

```cpp
// MyFeature/Source/Public/Feature.h
#pragma once

#include "FeatureConfig.config.hpp"

namespace MyFeature {

class MY_FEATURE_API API {
public:
    static const char* GetVersion() {
        return FEATURE_VERSION;
    }
    
    static int GetSupportLevel() {
        return FEATURE_SUPPORT_LEVEL;
    }
};

} // namespace MyFeature
```
