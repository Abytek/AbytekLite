# Getting Started with Abytek

Welcome to the Abytek project! This guide provides a structured learning path to help you get up to speed with development.

## Prerequisites

Before you begin, make sure you have:

- **CMake** (3.15 or higher)
- **C++ Compiler** (Visual Studio 2019/2022 on Windows, GCC 9+ on Linux, Clang 10+ on macOS)
- **Git** for version control
- **JavaScript** runtime (Node.js 14+) for build scripts

## Learning Path

### Step 1: Development Environment Setup

1. **Clone the repository**:
   ```bash
   git clone <repository-url>
   cd Abytek
   ```

2. **Initialize Git submodules**:
   ```bash
   # Windows
   Scripts\CheckSubmodules.bat
   
   # Unix/macOS
   Scripts/CheckSubmodules.sh
   ```

3. **Generate project files**:
   
   Abytek provides project generation scripts for various IDEs and build systems. Use these scripts instead of direct CMake commands for proper configuration:

   ```bash
   # Visual Studio 2022 (Windows)
   Scripts\JSRunTool.bat Tools/VS2022/GenerateProjectFiles Standalone
   
   # CLion
   Scripts\JSRunTool.bat Tools/CLion/GenerateProjectFiles Standalone
   
   # Unix Makefiles (Linux/macOS)
   Scripts\JSRunTool.bat Tools/UnixMakefiles/GenerateProjectFiles Standalone Debug
   ```

   See [Scripts Documentation](Scripts.md) for detailed information on project generation options.

4. **Open the project**:
   
   Once generated, you can open the project in your preferred IDE:
   
   - Visual Studio: Open `Build/Standalone/VS2022/Abytek.sln`
   - CLion: Open project from `Build/Standalone/CLion/`
   - Unix Makefiles: Navigate to the build directory and run make:
     ```bash
     cd Build/Standalone/UnixMakefiles/Debug
     make
     ```

5. **Build and run**:
   
   Build the project using your IDE or build system. The output binaries will be located in the build directory specific to your chosen project type and build system.

### Step 2: Project Structure Fundamentals

1. **Key directories**:
   - `/Modules` - Core modules and components
   - `/ThirdParty` - External dependencies
   - `/Scripts` - Build and automation scripts
   - `/Documentation` - Project documentation
   - `/GlobalSource` - Global headers and definitions

2. **Essential documentation**:
   - [Module System Overview](Module%20System/Overview.md)
   - [Module Structure](Module%20System/Structure.md)

3. **Reference modules**:
   - Simple: `Base.Minimal`
   - Intermediate: `OSBridge`
   - Advanced: Complex modules with dependencies and JavaScript integration

### Step 3: Creating Your First Module

1. **Module setup**:
   - Create directory structure: `Modules/Abytek/Modules/Public/MyFirstModule`
   - Add `Setup.cmake` with basic configuration
   - Create source directories

   ```cmake
   # Modules/Abytek/Modules/Public/MyFirstModule/Setup.cmake
   if(ABYTEK_MODULE_PHASE_INIT)
       set(MyFirstModule.Type STATIC)
       set(MyFirstModule.MacroName MY_FIRST_MODULE)
       set(MyFirstModule.PrivateDependencies "Base.Minimal")
   endif()
   ```

2. **Implementing basic functionality**:
   - Create header and implementation files
   - Use proper API macros for symbol visibility

   ```cpp
   // Source/Public/MyClass.h
   #pragma once
   
   class MY_FIRST_MODULE_API MyClass {
   public:
       void DoSomething();
   };
   ```

   ```cpp
   // Source/Private/MyClass.cpp
   #include "MyClass.h"
   #include <iostream>
   
   void MyClass::DoSomething() {
       std::cout << "Hello from MyFirstModule!" << std::endl;
   }
   ```

3. **Integration and testing**:
   - Rebuild the project
   - Create a simple test application
   - Verify module functionality

### Step 4: Module System Concepts

1. **Build phases**:
   - [Build Phases](Module%20System/Phases.md) documentation
   - When to use each phase (INIT, TARGET_CREATED, etc.)
   - Phase-specific operations

2. **Module properties**:
   - [Module Properties](Module%20System/Properties.md)
   - Module types (STATIC, SHARED, EXECUTABLE)
   - Configuration options

3. **Dependency management**:
   - Public vs. private dependencies
   - Include directory propagation
   - Dependency resolution order

4. **Configuration system**:
   - `.config` files and generated headers
   - CMake variable integration
   - Conditional compilation

### Step 5: Advanced Features

1. **JavaScript integration**:
   - [JavaScript Integration](Module%20System/JavaScript.md)
   - Build-time code generation
   - Custom build tools

2. **Cross-platform development**:
   - Platform-specific code
   - Conditional compilation
   - Handling different compilers

3. **API design**:
   - MacroName property and API macros
   - Symbol visibility in shared libraries
   - Interface design principles

4. **Architecture patterns**:
   - Facade pattern with interface modules
   - Plugin systems
   - Service locators and factories

## Reference: Common Tasks

### Adding a New Module

1. Create module directory with required structure
2. Add `Setup.cmake` with appropriate configuration
3. Add source files and tests
4. Update any dependent modules

### Adding a Third-Party Dependency

1. Place the dependency in `/ThirdParty` or a module's `/ThirdParty` directory
2. Create a `Setup.cmake` file for the dependency
3. Configure include paths and link libraries

### Creating a Shared Library

```cmake
if(ABYTEK_MODULE_PHASE_INIT)
    set(MyModule.Type SHARED)
    set(MyModule.MacroName MY_MODULE)
    # Set other properties
endif()
```

### Setting Up Module Config Files

1. Create `.config` files in your module's Source directory
2. Set CMake variables in the INIT phase
3. Include generated headers in your code

## Best Practices

### Module Organization

- Keep modules focused on a single responsibility
- Use clear, consistent naming conventions
- Prefer shallow hierarchies for better maintainability

### Code Style

- Follow the project's coding standards
- Use API macros consistently for shared libraries
- Keep public interfaces minimal and well-documented

### Build Performance

- Use precompiled headers for large modules
- Enable unity builds for appropriate modules
- Minimize include dependencies

### Documentation

- Document public APIs thoroughly
- Keep module READMEs up to date
- Include examples for complex features

## Advanced Topics to Explore

- Custom build rules and targets
- Reflection systems
- Code generation techniques
- Module system extensions
- Performance optimizations

## Getting Help

- Review the comprehensive documentation
- Check existing modules for examples and patterns
- Ask questions in the project's communication channels

Welcome to the Abytek project community, and happy coding! 