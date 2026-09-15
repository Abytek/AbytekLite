# Project Structure

## Overview
The Abytek project follows a modular architecture with clear separation between components. Here's the current project structure:

- **Modules/**: Contains modular components of the project
  - **Abytek/**: Main project module
    - **Modules/**: Submodules of Abytek
      - **Public/**: Modules that are accessible to external components
      - **Private/**: Modules that are only accessible within Abytek
    - **Setup.cmake**: CMake configuration for the Abytek module

- **GlobalSource/**: Contains global source files accessible to all modules
  - **Abytek/**: Global source specific to Abytek

- **CMake/**: Contains CMake scripts and utilities
  - **Abytek/**: CMake scripts specific to Abytek
    - **Utilities/**: Contains non-executable utility and helper CMake scripts
    - **Files/**: Contains executable CMake scripts

- **Documentation/**: Contains markdown files for project documentation

- **Scripts/**: Contains tools and scripts for project setup, building, and maintenance
  - **JS/**: JavaScript implementation of tools
    - **Tools/**: Individual tools (project generation, etc.)
    - **Utilities/**: Helper functions for tools
  - Various batch (.bat) and shell (.sh) script files

- **Build/**: Generated build artifacts and project files
  - Organized by project type, platform, and build system
  - For example: `Build/Standalone/VS2022/`

## Module Structure
Each module follows a standard structure:

- **Source/**: (optional) Contains source code
  - **Interface/**: Header files (.h, .hpp, .inl) accessible to dependent modules
  - **Private/**: Implementation files (.cpp, .c, .mm) and private headers
  - **Public/**: Headers accessible to the module and dependent modules

- **Modules/**: (optional) Contains child modules
  - **Private/**: Child modules only accessible to the module
  - **Public/**: Child modules accessible to dependent modules

- **Content/**: (optional) Contains content assets

- **Setup.cmake**: CMake script for configuring the module