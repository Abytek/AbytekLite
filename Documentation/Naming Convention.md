# Naming Convention

This document outlines the standard naming conventions used throughout the Abytek framework, particularly in the RHI (Rendering Hardware Interface) module.

## Class Naming Conventions

### Class Prefixes

- `A_` - Abstract base classes (e.g., `A_RHIDevice`, `A_RHIPipelineState`, `A_RHICommandList`)
- `F_` - Concrete implementation classes (e.g., `F_DirectX12RHIDevice`, `F_RHIPipelineStateDesc`)
- `TA_` - Abstract class templates (e.g., `TA_OSBObjectWithAlternative`)
- `TF_` - Concrete implementation class templates (e.g., `TF_Foo`)

### File Naming

- Base abstract classes should have "Base" in their filename (e.g., `RHIPipelineStateBase.hpp`, `RHIDeviceBase.hpp`)
- Implementation files should match header names (e.g., `RHIPipelineStateBase.cpp` for `RHIPipelineStateBase.hpp`)
- API-specific implementation files should be in their own subdirectory (e.g., `DirectX12/RHIPipelineState.hpp`)

## Enumeration Naming

- `E_` - Regular enumerations (e.g., `E_RHIAPI`)
- `ED_` - Dynamic enumerations (e.g., `ED_RHIBlendFactor`, `ED_RHIPipelineStateType`)
- Enum values are typically ALL_CAPS (e.g., `ZERO`, `ONE`, `SRC_COLOR`)

### Dynamic Enum Usage

When handling dynamic enums, use the special macros:
- `ABYTEK_RHI_ENUM_DYSWITCH` - For switch-like behavior on dynamic enums
- `ABYTEK_RHI_ENUM_DYCASE` - For case statements within the dynamic switch
- `ABYTEK_RHI_ENUM_DYDEFAULT` - For default case in the dynamic switch

Example:
```cpp
ABYTEK_RHI_ENUM_DYSWITCH(
    enumValue,
    ABYTEK_RHI_ENUM_DYCASE(
        ED_RHIEnumType::SOME_VALUE,
        // Handle this case
    )
    ABYTEK_RHI_ENUM_DYDEFAULT(
        // Handle default case
    )
);
```

## Pointer Naming Conventions

- Single object pointers are suffixed with `P` (e.g., `DeviceP`, `ResourceP`, `RootSignatureP`)
- Collections of pointers are suffixed with `Ps` (e.g., `ViewPs`, `BufferPs`, `DescriptorHeapPs`)
- Class member variables that are pointers are prefixed with underscore (e.g., `_DeviceP`, `_D3D12PipelineStateP`)

## Common Variable Naming

- Boolean values often start with `Is` or `Has` (e.g., `IsEnabled`, `HasValue`)
- Class member variables are typically prefixed with underscore (e.g., `_Desc`, `_Type`)
- Local variables use camelCase starting with lowercase letter (e.g., `deviceCount`, `indexBuffer`)
- Function parameters use camelCase with capital first letter (e.g., `DeviceP`, `Desc`)

## Type Wrapping

- `TW` - Wrapped pointer (e.g., `WithCrtAllocator::TW<A_RHIDevice>`)
- `TW_Valid` - Wrapped valid pointer (e.g., `WithCrtAllocator::TW_Valid<A_RHIDevice>`)
- `TU` - Unique pointer (e.g., `WithCrtAllocator::TU<A_RHIDevice>`)
- `TS` - Shared pointer (e.g., `WithCrtAllocator::TS<F_Foo>`)
- `TF_Vector` - Vector container (e.g., `WithCrtAllocator::TF_Vector<T>`)
- `TF_List` - List container (e.g., `WithCrtAllocator::TF_List<T>`)

## Constant Values

- Constants are typically ALL_CAPS with underscore separators (e.g., `MAX_COUNT`, `DEFAULT_SIZE`)
- Member constants may follow the same pattern as variables with underscore prefix (e.g., `_MAX_COUNT`)

## Documentation

- Documentation follows Doxygen-style comments
- Class documentation precedes the class declaration
- Method documentation precedes the method declaration
- Brief descriptions use `@brief` tag
- Parameter descriptions use `@param` tag
- Return value descriptions use `@return` tag