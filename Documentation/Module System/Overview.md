# Module System Overview

The Abytek project uses a modular architecture where modules placed in the `Modules` directory are automatically set up by the module system. This document provides an overview of the module system.

## Introduction

The module system is designed to:

1. Organize code into logical, reusable components
2. Manage dependencies between modules
3. Provide consistent build configurations
4. Support cross-platform development
5. Enable JavaScript integration for build-time tasks

## Core Concepts

### Modules and Namespaces

Like modern languages, the module system supports module names and namespaces:

- A module has a full name (target name): `<module_namespace>.<module_name>`
- The module namespace of a child module is: `<parent_module_namespace>.<parent_module_name>`
- Therefore, the full name of a child module is: `<parent_module_namespace>.<parent_module_name>.<child_module_name>`

For example: The full name of the RHI module inside OSBridge would be `Abytek.OSBridge.RHI`.

### Setup.cmake

Each module must have a `Setup.cmake` file that configures the module. This file is executed multiple times during different phases of the build system. To determine which phase is currently active, use conditionals:

```cmake
if(ABYTEK_MODULE_PHASE_INIT)
    # Code to run during initialization phase
    set(MyModule.Type STATIC)
    set(MyModule.PrivateDependencies "OtherModule")
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    # Code to run after targets are created
    target_include_directories(${CurrentTarget} PRIVATE ...)
endif()
```

### Build Phases

The module system processes modules through multiple phases:

1. **INIT**: Basic module configuration
2. **LATE_INIT**: Finalize configuration before target creation
3. **TARGET_CREATED**: Configure targets after creation
4. **AFTER_CHILD_MODULES_SETUP**: Handle configuration after child modules
5. **SETUP_TARGET_DEPENDENCIES**: Configure dependency relationships
6. **GLOBAL_SHARE**: Access global configuration 
7. **LATE_SETUP**: Final setup steps

Each phase addresses specific aspects of module configuration, allowing for a structured and organized build process.

### Documentation Index

- [Module Structure](Structure.md): Module directory structure and components
- [Build Phases](Phases.md): Detailed explanation of build phases
- [Module Properties](Properties.md): Available module properties and their usage
- [JavaScript Integration](JavaScript.md): Using JavaScript in the build system
- [C++ Features](C++%20Features.md): Configuration files, include directories, and C++ language features
- [Examples](Examples.md): Real-world examples of module setup 