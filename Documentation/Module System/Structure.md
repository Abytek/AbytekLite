# Module Structure

Each module in the Abytek project follows a standardized directory structure that determines how the module is built and integrated with other modules.

## Root CMakeLists.txt Integration

The module system is initialized in the root CMakeLists.txt file using:

```cmake
Abytek_SetupModules_MultiplePhases(
    Abytek 
    "${ABYTEK_ROOT_MODULES_DIR}"
)
```

This call sets up all modules in the specified directory and runs them through all phases.

## Module Directory Structure

A module can have these components:

### Required Components

- **Setup.cmake**: The script describing how to build the module. This file is executed during different phases of the build process.

### Optional Components

- **Modules/**: Contains child modules.
  - **Private/**: Contains child modules that are only accessible to the module.
  - **Public/**: Contains child modules that are accessible to the module and other dependent modules.

- **Source/**: Contains source code.
  - **Interface/**: Contains `.config`, `.hpp`, `.h`, `.inl` files that are accessible to other dependent modules.
  - **Private/**: Contains `.cpp`, `.c`, `.mm`, `.config`, `.hpp`, `.h`, `.inl` files that are only accessible to the module.
  - **Public/**: Contains `.config`, `.hpp`, `.h`, `.inl` files that are accessible to the module and other dependent modules.

- **Content/**: Contains content assets (meshes, textures, etc.).

- **ThirdParty/**: Contains third-party dependencies specific to the module.
  - Each third-party dependency typically has its own directory with a `Setup.cmake` file.

- **CMake/**: Contains additional CMake scripts used by the module.

- **JS/**: Contains JavaScript utilities for build-time operations.

## Generated Directories

During the build process, these directories are automatically generated for each module:

- **GeneratedSource/**: Contains generated source files.
  - **Private/**: Generated private source.
  - **Interface/**: Generated interface source.
  - **Public/**: Generated public source.
  - **Temp/**: Temporary generated files.

- **GeneratedJS/**: Contains generated JavaScript files.

## Module Types

The module system supports different types of modules, which can be set in the `Setup.cmake` file:

```cmake
if(ABYTEK_MODULE_PHASE_INIT)
    set(MyModule.Type STATIC)  # Set module type
endif()
```

Available module types:

- **STATIC** (default): Static library
- **SHARED**: Shared library
- **MODULE**: Module library
- **OBJECT**: Object library
- **INTERFACE**: Interface library
- **EXECUTABLE**: Executable
- **CUSTOM**: Special module type that doesn't produce binary output (or just contains child modules)

## File Conventions

### Source Files

- **C++ Headers**: `.h`, `.hpp`, `.inl`
- **C++ Source**: `.cpp`
- **C Headers**: `.h`
- **C Source**: `.c`
- **Objective-C++**: `.mm`
- **Precompiled Headers**: `.pch.h`, `.pch.hpp`
- **Configuration Files**: `.config`

### Generated Files

- **Config Headers**: `<config_file>.config.hpp`
- **JavaScript Module Files**: Various JS files for module representation

## Access Patterns

The module structure enforces these access patterns:

- **Private** members are only accessible within the module
- **Interface** members are accessible to dependent modules
- **Public** members are accessible to the module and dependent modules

This structure ensures clean separation of concerns and promotes good architecture. 