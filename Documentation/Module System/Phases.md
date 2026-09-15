# Build Phases

The module system executes through a series of well-defined phases for each module. These phases allow for a structured and organized configuration process.

## Phase Execution Order

The module system executes the following phases sequentially through the `Abytek_SetupModules_MultiplePhases` function:

1. **ABYTEK_MODULE_PHASE_INIT**
2. **ABYTEK_MODULE_PHASE_LATE_INIT**
3. **ABYTEK_MODULE_PHASE_TARGET_CREATED**
4. **ABYTEK_MODULE_PHASE_AFTER_CHILD_MODULES_SETUP**
5. **ABYTEK_MODULE_PHASE_SETUP_TARGET_DEPENDENCIES**
6. **ABYTEK_MODULE_PHASE_GLOBAL_SHARE**
7. **ABYTEK_MODULE_PHASE_LATE_SETUP**

Each phase serves a specific purpose in the module configuration workflow.

## Phase Details

### 1. ABYTEK_MODULE_PHASE_INIT

**Purpose**: Initial module configuration

**Key Activities**:
- Configure basic module properties (Type, MacroName, etc.)
- Define dependencies (PrivateDependencies, PublicDependencies)
- Set up module-specific options
- Configure preprocessor definitions

**Example**:
```cmake
if(ABYTEK_MODULE_PHASE_INIT)
    set(MyModule.Type STATIC)
    set(MyModule.MacroName MY_MODULE)
    set(MyModule.PrivateDependencies "Base.Minimal")
    
    option(MY_MODULE_ENABLE_FEATURE "Enable a special feature" OFF)
endif()
```

This is the only phase where most properties can be initially set.

### 2. ABYTEK_MODULE_PHASE_LATE_INIT

**Purpose**: Finalize module configuration before target creation

**Key Activities**:
- Adjust options based on other modules' settings
- Make final decisions before targets are created
- Configure additional properties based on initial options

**Example**:
```cmake
if(ABYTEK_MODULE_PHASE_LATE_INIT)
    if(SOME_GLOBAL_OPTION)
        # Adjust module settings based on global options
    endif()
endif()
```

### 3. ABYTEK_MODULE_PHASE_TARGET_CREATED

**Purpose**: Configure the actual CMake target

**Key Activities**:
- Add include directories
- Set compiler flags
- Configure target properties
- Modify dependencies if needed

**Example**:
```cmake
if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    target_include_directories(${CurrentTarget} 
        PRIVATE ${SOME_INCLUDE_DIR}
    )
    
    if(MY_MODULE_ENABLE_FEATURE)
        target_compile_definitions(${CurrentTarget}
            PRIVATE MY_FEATURE_ENABLED=1
        )
    endif()
endif()
```

The `${CurrentTarget}` variable is available and points to the actual CMake target.

### 4. ABYTEK_MODULE_PHASE_AFTER_CHILD_MODULES_SETUP

**Purpose**: Handle configuration that depends on child modules

**Key Activities**:
- Process information from child modules
- Configure the module based on its children
- Set up parent-child relationships in code

**Example**:
```cmake
if(ABYTEK_MODULE_PHASE_AFTER_CHILD_MODULES_SETUP)
    # Process information from child modules
    foreach(ChildModule ${ChildModules})
        # Do something with each child module
    endforeach()
endif()
```

### 5. ABYTEK_MODULE_PHASE_SETUP_TARGET_DEPENDENCIES

**Purpose**: Set up complex dependency relationships

**Key Activities**:
- Configure complex inter-target dependencies
- Handle circular references
- Set up build order dependencies

**Example**:
```cmake
if(ABYTEK_MODULE_PHASE_SETUP_TARGET_DEPENDENCIES)
    # Configure complex dependencies
    add_dependencies(${CurrentTarget} ${SomeGeneratorTarget})
endif()
```

### 6. ABYTEK_MODULE_PHASE_GLOBAL_SHARE

**Purpose**: Access global configuration and finalize settings

**Key Activities**:
- Access global variables and settings
- Share module information globally
- Set up shared resources
- Process global options

**Example**:
```cmake
if(ABYTEK_MODULE_PHASE_GLOBAL_SHARE)
    # Access global settings
    include(Abytek/AddEnableUnitTestsOptionForCurrentModule)
    
    # Process global options
    if(ABYTEK_ENABLE_DOCUMENTATION)
        # Set up documentation for this module
    endif()
endif()
```

### 7. ABYTEK_MODULE_PHASE_LATE_SETUP

**Purpose**: Final configuration after all other phases

**Key Activities**:
- Handle final configurations
- Process generated files
- Final cleanup
- Create custom commands that depend on all previous phases

**Example**:
```cmake
if(ABYTEK_MODULE_PHASE_LATE_SETUP)
    # Final configurations
    if(MY_MODULE_ENABLE_FEATURE)
        add_custom_command(
            TARGET ${CurrentTarget} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${SomeFile} ${Destination}
        )
    endif()
endif()
```

## Using Phases in Setup.cmake

The proper way to use phases in your `Setup.cmake` file is to wrap code in conditional blocks:

```cmake
if(ABYTEK_MODULE_PHASE_INIT)
    # INIT phase code
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    # TARGET_CREATED phase code
endif()

# Additional phase blocks as needed
```

This ensures that code is executed only during the appropriate phase. 