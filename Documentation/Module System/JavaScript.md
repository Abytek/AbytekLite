# JavaScript Integration

The module system integrates with Node.js to extend the build process with JavaScript capabilities, providing more flexibility and power for complex build operations.

## JavaScript Architecture

The module system automatically creates a JavaScript context that represents the entire module hierarchy. This allows for cross-module JavaScript operations during the build process.

Key components:

1. **JavaScript Context File**: Contains information about all modules
2. **Module JavaScript Files**: Generated for each module
3. **JavaScript Run Script**: Executes module-specific JavaScript code

## JavaScript Directory Structure

Each module can have JavaScript utilities:

- **JS/**: Contains JavaScript source files for the module
- **GeneratedJS/**: Contains automatically generated JavaScript files

## Generated JavaScript Files

For each module, the system generates several JavaScript files:

- **`JSModuleFile`**: Main JavaScript file for the module
- **`JSModulesFile`**: Information about child modules
- **`JSPrivateModulesFile`**: Information about private child modules
- **`JSPublicModulesFile`**: Information about public child modules
- **`JSDependenciesFile`**: Information about dependencies
- **`JSPrivateDependenciesFile`**: Information about private dependencies
- **`JSPublicDependenciesFile`**: Information about public dependencies

## Executing JavaScript During Build

### The Abytek_ExecuteJS Function

The `Abytek_ExecuteJS` CMake function allows executing JavaScript code during the build process:

```cmake
Abytek_ExecuteJS(
    RELATIVE_PATH "path/to/script.js"
    CLI_ARGS "arg1" "arg2" "arg3"
)
```

Parameters:
- **RELATIVE_PATH**: Path to the JavaScript file, relative to the module's JS directory
- **CLI_ARGS**: Command-line arguments to pass to the script

### JavaScript Runtime Context

When a JavaScript file is executed, it has access to:

1. **Module Information**: Properties of the current module
2. **Dependencies**: Information about module dependencies
3. **Platform Information**: Details about the build platform
4. **Child Modules**: Access to child module information

## Creating JavaScript Utilities

### Basic JavaScript Module

Here's a simple example of a JavaScript file that can be called from CMake:

```javascript
// MyModule/JS/GenerateCode.js
module.exports = (context, args) => {
    // Access module information
    const moduleDir = context.Directory;
    
    // Process arguments
    const outputFile = args[0];
    
    // Generate some code
    const fs = require('fs');
    fs.writeFileSync(outputFile, '// Generated code\n');
    
    console.log('Code generation complete!');
    return true; // Success
};
```

### Calling the Script from CMake

```cmake
if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    Abytek_ExecuteJS(
        RELATIVE_PATH "GenerateCode.js"
        CLI_ARGS "${CMAKE_CURRENT_BINARY_DIR}/generated.cpp"
    )
endif()
```

## Advanced JavaScript Features

### Cross-Module JavaScript Operations

JavaScript scripts can interact with other modules:

```javascript
module.exports = (context, args) => {
    // Access another module
    const otherModule = context.RequireModule("Abytek.OtherModule");
    
    // Use information from that module
    const otherModuleDir = otherModule.Directory;
    
    // Do something with the information
    // ...
    
    return true;
};
```

### Platform-Specific Operations

You can perform platform-specific operations in JavaScript:

```javascript
module.exports = (context, args) => {
    if (context.PlatformName === "WINDOWS") {
        // Windows-specific code
    } else if (context.PlatformName === "MACOS") {
        // macOS-specific code
    }
    
    return true;
};
```

### Working with Files and Directories

JavaScript utilities can manipulate files and directories:

```javascript
module.exports = (context, args) => {
    const fs = require('fs');
    const path = require('path');
    
    // Source directory
    const sourceDir = context.Source.Directory;
    
    // Generate a file list
    const files = fs.readdirSync(sourceDir);
    
    // Process files
    for (const file of files) {
        if (path.extname(file) === '.h') {
            // Process header files
        }
    }
    
    return true;
};
```

## JavaScript Integration Examples

### Code Generation Example

```cmake
# In Setup.cmake
if(ABYTEK_MODULE_PHASE_INIT)
    set(MyModule.Type STATIC)
endif()

if(ABYTEK_MODULE_PHASE_TARGET_CREATED)
    # Generate code using JavaScript
    Abytek_ExecuteJS(
        RELATIVE_PATH "Generators/GenerateBindings.js"
        CLI_ARGS 
            "${MyModule.Source.PublicDirectory}/API.h"
            "${MyModule.GeneratedSource.PrivateDirectory}/Bindings.cpp"
    )
    
    # Add generated file to sources
    target_sources(${CurrentTarget} 
        PRIVATE "${MyModule.GeneratedSource.PrivateDirectory}/Bindings.cpp"
    )
endif()
```

```javascript
// In JS/Generators/GenerateBindings.js
module.exports = (context, args) => {
    const fs = require('fs');
    const headerFile = args[0];
    const outputFile = args[1];
    
    // Read header file
    const headerContent = fs.readFileSync(headerFile, 'utf8');
    
    // Process header and generate bindings
    // ...
    
    // Write output file
    fs.writeFileSync(outputFile, generatedBindings);
    
    return true;
};
```

### Dependency Analysis Example

```cmake
# In Setup.cmake
if(ABYTEK_MODULE_PHASE_LATE_SETUP)
    # Analyze dependencies
    Abytek_ExecuteJS(
        RELATIVE_PATH "Tools/AnalyzeDependencies.js"
        CLI_ARGS "${CMAKE_CURRENT_BINARY_DIR}/dependency_report.json"
    )
endif()
```

```javascript
// In JS/Tools/AnalyzeDependencies.js
module.exports = (context, args) => {
    const fs = require('fs');
    const outputFile = args[0];
    
    // Collect dependency information
    const dependencies = [];
    
    // For each dependency
    for (const dep of context.Dependencies) {
        const depModule = context.RequireModule(dep);
        dependencies.push({
            name: dep,
            type: depModule.Type,
            directory: depModule.Directory
        });
    }
    
    // Write report
    fs.writeFileSync(outputFile, JSON.stringify(dependencies, null, 2));
    
    return true;
};
```

## Best Practices

1. **Use JavaScript for Complex Operations**: Use JavaScript for tasks that would be difficult or verbose in CMake.
2. **Keep Scripts Modular**: Create small, focused JavaScript utilities.
3. **Handle Errors**: Always validate inputs and handle errors in JavaScript code.
4. **Return Status**: Return `true` for success and `false` for failure.
5. **Log Actions**: Use `console.log()` to report progress and results.
6. **Separate Concerns**: Use different scripts for different tasks.
7. **Call Scripts at Appropriate Phases**: Execute JavaScript only in the appropriate build phase. 