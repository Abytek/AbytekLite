# Build Project

This project uses CMake as the build system with a set of JavaScript-based tools for generating project files.

---

## Dependencies
**Node.js** (required for running project generation scripts)

**CMake 3.14+** (required for build system)

**Doxygen** (required for generating documentation)

**Boost** (required, and automatically downloaded)

---

## Supported Platforms
+ **Windows**
+ **macOS**
+ **Unix Platforms**

---

## Generating Project Files

The project provides easy-to-use scripts for generating project files for different IDEs:

### Visual Studio 2022
```
Scripts\JSRunTool.bat Tools/VS2022/GenerateProjectFiles Standalone
```
Project files will be generated in `Build/Standalone/VS2022/`

### CLion
```
Scripts\JSRunTool.bat Tools/CLion/GenerateProjectFiles Standalone
```
Project files will be generated in `Build/Standalone/CLion/`

### Unix Makefiles
```
Scripts\JSRunTool.bat Tools/UnixMakefiles/GenerateProjectFiles Standalone [Debug|Release|RelWithDebInfo|MinSizeRel]
```
Project files will be generated in `Build/Standalone/UnixMakefiles/[Config]/`

---

## Project Types

The build system supports three project types:

1. **Standalone** - Default project type (sets ABYTEK_ENABLE_STANDALONE=ON)
2. **Operator** - Special configuration for Operator mode (automatically sets ABYTEK_ENABLE_OPERATOR=ON)
3. **ANode** - Special configuration for ANode mode (automatically sets ABYTEK_ENABLE_ANODE=ON)

Note: You cannot enable Operator and ANode simultaneously, which is why they are separate project types.

---

## Build Options

The following CMake options are automatically set based on your chosen project type:

- **ABYTEK_ENABLE_STANDALONE** (ON for Standalone project type, OFF otherwise)
- **ABYTEK_ENABLE_OPERATOR** (ON for Operator project type, OFF otherwise) 
- **ABYTEK_ENABLE_ANODE** (ON for ANode project type, OFF otherwise)

Other build options:

- **ABYTEK_ENABLE_EDITOR** (ON by default for Standalone mode, OFF otherwise)

These options are configured automatically when you select a project type using the script commands listed in the "Generating Project Files" section.

---

## Compiling

After generating project files, you can build the project:

- **Visual Studio 2022**: 
  - Open the solution file in `Build/Standalone/VS2022/`
  - Select a target and click Run or Build

- **CLion**: 
  - Open the project in `Build/Standalone/CLion/`
  - Select a target and click Run

- **Unix Makefiles**:
  - Navigate to the build directory
  - Run: `make [target_name]`
  
  Or you can create a build script:
  ```
  #!/bin/bash
  cd Build/Standalone/UnixMakefiles/Debug
  make $1
  ```