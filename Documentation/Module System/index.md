# Module System Documentation

Welcome to the Abytek Module System documentation. This documentation is split into multiple sections to make it easier to navigate and understand the system.

## Documentation Sections

- [Overview](Overview.md): Introduction to the module system and core concepts
- [Module Structure](Structure.md): Module directory structure and components
- [Build Phases](Phases.md): Detailed explanation of build phases
- [Module Properties](Properties.md): Available module properties and their usage
- [JavaScript Integration](JavaScript.md): Using JavaScript in the build system
- [C++ Features](C++%20Features.md): Configuration files, include directories, and C++ language features
- [Examples](Examples.md): Real-world examples of module setup

## Getting Started

If you're new to the module system, we recommend starting with the [Overview](Overview.md) document, which provides a high-level introduction to the system and its main concepts.

For detailed documentation on specific aspects, see the appropriate section above.

## Quick References

### Common Module Types

- **STATIC**: Static library (default)
- **SHARED**: Shared library
- **MODULE**: Module library
- **EXECUTABLE**: Executable 
- **CUSTOM**: Module that does not produce binary output

### Key Build Phases

1. **ABYTEK_MODULE_PHASE_INIT**: Initial module configuration
2. **ABYTEK_MODULE_PHASE_TARGET_CREATED**: Configure targets after creation
3. **ABYTEK_MODULE_PHASE_GLOBAL_SHARE**: Access global configuration 
4. **ABYTEK_MODULE_PHASE_LATE_SETUP**: Final setup steps

### Basic Module Template

```cmake
# MyModule/Setup.cmake

if(ABYTEK_MODULE_PHASE_INIT)
    # Basic configuration
    set(MyModule.Type STATIC)
    set(MyModule.PrivateDependencies "Base.Minimal")
    
    # Module options
    option(MY_MODULE_OPTION "Description" ON)
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    # Target configuration
    target_include_directories(${CurrentTarget} PRIVATE ${SOME_INCLUDE_DIR})
    
    if(MY_MODULE_OPTION)
        target_compile_definitions(${CurrentTarget} PRIVATE MY_OPTION_ENABLED=1)
    endif()
endif()
``` 