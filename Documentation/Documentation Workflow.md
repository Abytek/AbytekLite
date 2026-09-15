# Documentation Workflow

## Things that are placeable onto Documentation Website

Currently, we use Doxygen to generate our documentation website, but we will move to another tool in the future.

- For C++:
  - All the C++ files in the directory `<project_root>/Abytek/` are documentated automatically.
  - Use javadoc comment convention.

- For additional markdown files:
  - Just add them into `<project_root>/Documentation/`

## Documentation Organization

We maintain separate documentation directories for different types of documentation to better serve different teams and use cases:

- `<project_root>/Documentation/Engineering/` - Technical documentation for engineers working on the codebase
- `<project_root>/Documentation/Design/` - Design concepts, patterns, and architecture overviews
- `<project_root>/Documentation/Usage/` - How-to guides and usage documentation for consumers of the library
- `<project_root>/Documentation/API/` - API reference documentation

This separation ensures that each sub-team can easily find the documentation most relevant to their role and responsibilities.

## AI-Based Documentation

### Documentation Location Priority
- Documentation should be placed in the following order of preference:
  - Directly inside source code where the feature/component is defined
  - In markdown files within the appropriate Documentation subdirectory

###	Not just for developer but also for AI Agents
- Should keep file size small
- Should provide examples
- AI Agents should have the ability to directly answer the correct answer at the first replication.

### AI Integration
- This project leverages AI for documentation in two primary ways:
  - **AI Documentation Reading**: AIAgents in IDEs/TextEditors such as Cursor can read and interpret existing documentation to assist developers
  - **AI Documentation Generation**: AIAgents can automatically generate documentation based on code structure and implementation