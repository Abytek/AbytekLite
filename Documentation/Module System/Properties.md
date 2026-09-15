# Module Properties

Modules have a rich set of properties that control their behavior, dependencies, and configuration. These properties are accessed through variables in CMake.

## Property Access Patterns

Each property of a module is exported/copied to two variables:
- `${ModuleName}.<property>`: Using short module name (for writing/modifying)
- `${ModuleFullName}.<property>`: Using full module name with namespace (for reading)

### Read-only vs. Writable Properties

- **Read-only properties**: Require full module name when accessing `${ModuleFullName}.<property>`
- **Writable properties**: Require short module name when accessing `${ModuleName}.<property>`

Most properties are writable during the INIT phase, and become read-only in later phases.

## Basic Properties

- **`Target`** (read-only): 
  - The name of its target.
  - Available starting from phase `ABYTEK_MODULE_PHASE_TARGET_CREATED`.
  
- **`Directory`** (read-only): 
  - Its own directory.
  - Available for all phases.
  
- **`Enable`**: 
  - For enabling/disabling the module.
  - Read-write in phase `ABYTEK_MODULE_PHASE_INIT`, read-only afterwards.
  - Example: `set(MyModule.Enable ON)`
  
- **`MacroName`**: 
  - Its macro name that could be used for source generation.
  - Read-write in phase `ABYTEK_MODULE_PHASE_INIT`, read-only afterwards.
  - Example: `set(MyModule.MacroName "MY_MODULE")`

- **`EnableUnityBuild`**:
  - Whether to enable unity build for the module.
  - Read-write in phase `ABYTEK_MODULE_PHASE_INIT`.
  - Example: `set(MyModule.EnableUnityBuild ON)`

## Module Type

- **`Type`**: 
  - The module type (STATIC, SHARED, etc.)
  - Read-write in phase `ABYTEK_MODULE_PHASE_INIT`, read-only afterwards.
  - Example: `set(MyModule.Type STATIC)`
  
  Supported types:
  - **STATIC** (default): Static library
  - **SHARED**: Shared library
  - **MODULE**: Module library
  - **OBJECT**: Object library
  - **INTERFACE**: Interface library
  - **EXECUTABLE**: Executable
  - **CUSTOM**: Special module type that doesn't produce binary output

## Dependency Properties

- **`PrivateDependencies`**: 
  - Its private dependencies.
  - Read-write in phase `ABYTEK_MODULE_PHASE_INIT`, modifiable in phase `ABYTEK_MODULE_PHASE_TARGET_CREATED`.
  - Example: `set(MyModule.PrivateDependencies "Base.Core" "OSBridge.Minimal")`
  
- **`PublicDependencies`**: 
  - Its public dependencies (also available to dependent modules).
  - Read-write in phase `ABYTEK_MODULE_PHASE_INIT`, modifiable in phase `ABYTEK_MODULE_PHASE_TARGET_CREATED`.
  - Example: `set(MyModule.PublicDependencies "Base.Math")`
  
- **`Dependencies`** (read-only): 
  - Combined list of its dependencies.
  - Available after phase `ABYTEK_MODULE_PHASE_INIT`.

## Directory Properties

### Module Directories

- **`Modules.Directory`** (read-only): The directory containing its child modules.
- **`Modules.PrivateDirectory`** (read-only): The directory containing its private child modules.
- **`Modules.PublicDirectory`** (read-only): The directory containing its public child modules.

### Source Directories

- **`Source.Directory`** (read-only): Its own source directory.
- **`Source.PrivateDirectory`** (read-only): The directory containing its private source.
- **`Source.InterfaceDirectory`** (read-only): The directory containing its interface source.
- **`Source.PublicDirectory`** (read-only): The directory containing its public source.

### Additional Directories

- **`ThirdParty.Directory`** (read-only): The directory containing its third-party dependencies.
- **`CMakeDirectory`** (read-only): The directory containing additional CMake scripts.
- **`JSDirectory`** (read-only): The directory containing JavaScript utilities.
- **`AutoCopy.Directory`** (read-only): The directory containing files to be automatically copied.

### Generated Directories

- **`GeneratedSource.Directory`** (read-only): The directory containing its generated source.
- **`GeneratedSource.PrivateDirectory`** (read-only): The directory containing its private generated source.
- **`GeneratedSource.InterfaceDirectory`** (read-only): The directory containing its interface generated source.
- **`GeneratedSource.PublicDirectory`** (read-only): The directory containing its public generated source.
- **`GeneratedSource.TempDirectory`** (read-only): The directory containing temporary generated files.

### JavaScript Directories and Files

- **`GeneratedJSDirectory`** (read-only): The directory for generated JavaScript files.
- **`JSModuleFile`** (read-only): The path to the main JavaScript module file.
- **`JSModulesFile`** (read-only): The path to the JavaScript modules file.
- **`JSPrivateModulesFile`** (read-only): The path to the private JavaScript modules file.
- **`JSPublicModulesFile`** (read-only): The path to the public JavaScript modules file.
- **`JSDependenciesFile`** (read-only): The path to the JavaScript dependencies file.
- **`JSPrivateDependenciesFile`** (read-only): The path to the private JavaScript dependencies file.
- **`JSPublicDependenciesFile`** (read-only): The path to the public JavaScript dependencies file.

## Configuration and Definition Properties

### Configuration Files

- **`Source.ConfigFiles`** (read-only): The list of its config files.
- **`Source.PrivateConfigFiles`** (read-only): The list of its private config files.
- **`Source.InterfaceConfigFiles`** (read-only): The list of its interface config files.
- **`Source.PublicConfigFiles`** (read-only): The list of its public config files.
- **`GeneratedSource.ConfigFiles`** (read-only): The list of its generated config files.
- **`GeneratedSource.PrivateConfigFiles`** (read-only): The list of its generated private config files.
- **`GeneratedSource.InterfaceConfigFiles`** (read-only): The list of its generated interface config files.
- **`GeneratedSource.PublicConfigFiles`** (read-only): The list of its generated public config files.

### Definitions

- **`PrivateDefinitions`**: 
  - Its private preprocessor definitions.
  - Read-write in phase `ABYTEK_MODULE_PHASE_INIT`, read-only afterwards.
  - Example: `set(MyModule.PrivateDefinitions "MY_PRIVATE_DEFINE=1")`
  
- **`InterfaceDefinitions`**: 
  - Its interface preprocessor definitions.
  - Read-write in phase `ABYTEK_MODULE_PHASE_INIT`, read-only afterwards.
  - Example: `set(MyModule.InterfaceDefinitions "MY_INTERFACE_DEFINE=1")`
  
- **`PublicDefinitions`**: 
  - Its public preprocessor definitions.
  - Read-write in phase `ABYTEK_MODULE_PHASE_INIT`, read-only afterwards.
  - Example: `set(MyModule.PublicDefinitions "MY_PUBLIC_DEFINE=1")`

## Custom Target Properties

- **`Depends`**: 
  - The files that the module depends on.
  - Usable for CUSTOM type.
  - Read-write in phase `ABYTEK_MODULE_PHASE_INIT`, read-only afterwards.
  - Example: `set(MyModule.Depends "${CMAKE_CURRENT_SOURCE_DIR}/data.txt")`
  
- **`Commands`**: 
  - The commands that will be executed when the module is being built.
  - Usable for CUSTOM type.
  - Read-write in phase `ABYTEK_MODULE_PHASE_INIT`, read-only afterwards.
  - Example: `set(MyModule.Commands "python ${CMAKE_CURRENT_SOURCE_DIR}/script.py")`

## Source File Properties

- **`AdditionalSourceFiles`**: 
  - Additional source files to include in the build.
  - Read-write in phase `ABYTEK_MODULE_PHASE_INIT`, read-only afterwards.
  - Example: `set(MyModule.AdditionalSourceFiles "${CMAKE_CURRENT_SOURCE_DIR}/external/src.cpp")`

### Automatically Detected Source Files

The following properties are automatically populated based on the module's directory structure:

- **`PublicHeaderAndInlFiles`** (read-only): Its public `.hpp`, `.h`, `.inl` files.
- **`PrivateHeaderAndInlFiles`** (read-only): Its private `.hpp`, `.h`, `.inl` files.
- **`InterfaceHeaderAndInlFiles`** (read-only): Its interface `.hpp`, `.h`, `.inl` files.
- **`PrivateCPPFiles`** (read-only): Its private `.cpp` files.
- **`PrivateCFiles`** (read-only): Its private `.c` files.
- **`PrivateMMFiles`** (read-only): Its private `.mm` files.
- **`PrivatePCHFiles`** (read-only): Its private `.pch.hpp`, `.pch.h` files (pre-compiled header files).
- **`InterfacePCHFiles`** (read-only): Its interface `.pch.hpp`, `.pch.h` files (pre-compiled header files).
- **`PublicPCHFiles`** (read-only): Its public `.pch.hpp`, `.pch.h` files (pre-compiled header files).
- **`Source.Files`** (read-only): Combined list of its source files.
- **`GeneratedSource.Files`** (read-only): Combined list of its generated source files.

## Pre-defined CMake Variables

These CMake variables are automatically available in `Setup.cmake`:

- **`CurrentModule`**: The name of the current module (short name). Available to all phases.
- **`CurrentModuleFullName`**: The full name of the current module. Available to all phases.
- **`CurrentTarget`**: The name of the current target. Available to all phases. 

Note: The `CurrentTarget` variable only tells you the target name but does not ensure that the target was already created! It should generally be used only after the `ABYTEK_MODULE_PHASE_TARGET_CREATED` phase. 