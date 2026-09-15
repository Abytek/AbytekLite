# Scripts Documentation

## Overview
The Scripts directory contains a collection of tools to help with project setup, building, and maintenance. The scripts system is built primarily using JavaScript (Node.js) with wrapper batch (.bat) and shell (.sh) files for platform compatibility.

## Structure
- **Scripts/** - Root scripts directory
  - **JS/** - JavaScript implementation files
    - **Tools/** - Individual tool implementations
      - **CLion/** - CLion project generation tools
      - **VS2022/** - Visual Studio 2022 project generation tools
      - **UnixMakefiles/** - Unix Makefiles generation tools
      - **Emscripten/** - Emscripten-related tools
    - **Utilities/** - Helper utilities for scripts
    - **Project.js** - Project metadata and configuration
    - **RunTool.js** - Tool runner implementation
  - **JSRunTool.bat** - Windows batch wrapper for running JS tools
  - **CheckSubmodules.bat/.sh** - Git submodule initialization scripts
  - **UpdateSubmodulesToLatest.bat/.sh** - Git submodule update scripts

## Usage

### Running Tools
To run a tool, use the JSRunTool script with the tool path and any arguments:

#### Windows
```
Scripts\JSRunTool.bat <tool_path> [arguments]
```

#### Unix/macOS
```
Scripts/JSRunTool.sh <tool_path> [arguments]
```

### Available Tools

#### Project Generation

1. **Generate Visual Studio 2022 Project**
   ```
   Scripts\JSRunTool.bat VS2022/GenerateProjectFiles <project_type> [extra_args]
   ```
   - `<project_type>`: Standalone, Operator, or ANode
   - Project files will be generated in `Build/<project_type>/VS2022/`

2. **Generate CLion Project**
   ```
   Scripts\JSRunTool.bat CLion/GenerateProjectFiles <project_type>
   ```
   - `<project_type>`: Standalone, Operator, or ANode
   - Project files will be generated in `Build/<project_type>/CLion/`

3. **Generate Unix Makefiles Project**
   ```
   Scripts\JSRunTool.bat UnixMakefiles/GenerateProjectFiles <project_type> [config] [extra_args]
   ```
   - `<project_type>`: Standalone, Operator, or ANode
   - `[config]`: Debug (default), Release, RelWithDebInfo, or MinSizeRel
   - Project files will be generated in `Build/<project_type>/UnixMakefiles/<config>/`

#### Git Submodule Management

1. **Initialize Submodules**
   ```
   Scripts\CheckSubmodules.bat    # Windows
   Scripts/CheckSubmodules.sh     # Unix/macOS
   ```

2. **Update Submodules to Latest Version**
   ```
   Scripts\UpdateSubmodulesToLatest.bat    # Windows
   Scripts/UpdateSubmodulesToLatest.sh     # Unix/macOS
   ```

## Tool Development

### Creating New Tools
To create a new tool:

1. Create a new JavaScript file in an appropriate subdirectory under `Scripts/JS/Tools/`
2. Export a function that accepts `AbytekJS` and `CLIArgs` parameters
3. Implement your tool functionality

Example:
```javascript
// Scripts/JS/Tools/MyTool/DoSomething.js
module.exports = (AbytekJS, CLIArgs) => {
    // Tool implementation
    console.log("Running my tool with args:", CLIArgs);
    
    // Return any result if needed
    return "Tool execution successful";
};
```

### Utilities
The script system provides these utilities:

- **GetBuildPath**: Generates standardized build paths based on project type and platform
- **RunTool**: Loads and runs a specified tool with arguments

## Conventions
- Tool paths use forward slashes regardless of platform
- Project types are case-sensitive (Standalone, Operator, ANode)
- Build outputs go to `Build/<project_type>/<build_system>/` 