# RHI Module Documentation

## Module Overview

The **RHI (Rendering Hardware Interface)** module is a core Abytek Engine component that provides a high-level abstraction layer for GPU programming and command submission across different graphics APIs.

## Module Path

```
Modules/Abytek/Modules/Public/Engine/Modules/Public/RHI
├── Source/
│   ├── Public/          (Public interfaces and classes)
│   ├── Private/         (Implementation)
│   │   └── DirectX12/   (API-specific backend)
│   └── Interface/       (Engine interface)
└── Setup.cmake
```

## Module CMake Configuration

The RHI module is configured through `Setup.cmake`:

```cmake
# Core public headers
- Engine.RHI.prerequisites.hpp
- Engine.RHI.config
- RHIAPI.hpp

# Public API
- RHIGraph.hpp
- RHIDevice.hpp
- RHIWorker.hpp
- RHIPass.hpp (and variants)
- RHISubmissionItem.hpp
- RHISubmissionList.hpp

# Implementation backends
- DirectX12/RHIGraph.hpp
- DirectX12/RHIDevice.hpp
```

## Key Classes

### Management Classes

| Class | Purpose | Pattern |
|-------|---------|---------|
| `F_RHISubsystem` | Central RHI management | Engine Subsystem |
| `A_RHIGraph` | Command submission hub | Singleton (RAObject) |
| `A_RHIDeviceManager` | GPU enumeration | Manager |
| `A_RHIWorkerManager` | Worker lifecycle | Manager |

### Device & Execution

| Class | Purpose | Relationship |
|-------|---------|--------------|
| `A_RHIDevice` | Single GPU abstraction | Owns 3 workers |
| `A_RHIWorker` | Command queue | Device-specific |
| `F_Runnable` | Execution thread | Optional per worker |

### GPU Work Items

| Class | Base | GPU Work Class |
|-------|------|-----------------|
| `F_RHIDrawPass` | A_RHIRasterPass | DIRECT |
| `F_RHIDispatchComputePass` | A_RHIComputePass | COMPUTE |
| `F_RHIUploadPass` | A_RHIPass | BLIT |
| `F_RHIRayTracePass` | A_RHIPass | DIRECT |

## Module Features

### 1. Multi-API Support
- Abstract base classes for API-agnostic code
- DirectX 12 implementation fully integrated
- Extensible design for future APIs (Vulkan, Metal)

### 2. Async Compute
- Separate `AsyncComputeWorker` on each device
- Independent compute queue parallel to graphics
- Configurable via `E_RHIGPUWorkClass`

### 3. Transient Allocation
- All GPU work items allocated via render allocator
- Automatic cleanup after submission
- Zero-copy submission where possible

### 4. Command Batching
- `F_RHISubmissionList` for grouping work
- Sequential or parallel execution modes
- Atomic submission with ordering guarantees

### 5. GPU Synchronization
- `A_RHISubmissionRequest` for tracking completion
- Optional blocking wait on requests
- Status querying without blocking

## Integration Points

### With Engine Core
- Registered as `F_RHISubsystem` in engine subsystems
- Lifetime managed by engine initialization
- Access via singleton pattern

### With Rendering Module
- Provides GPU execution backend for renderers
- Used by Material system for shader compilation
- Utilized by Resource system for GPU uploads

### With Editor Module
- Viewport rendering uses RHI submissions
- Debug visualization uses draw passes
- Performance profiling hooks

## Configuration

### Environment Setup
- DirectX 12 SDK required (Windows)
- GPU drivers supporting DirectX 12.0+
- Feature level 11_0 minimum

### CMake Options
```cmake
# Enable DirectX 12 support
ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12 ON

# Configure worker threads
RHI_USE_DEDICATED_WORKERS ON
```

### Runtime Configuration
Via `Config/Environment/` JSON files:
```json
{
  "RHI": {
    "EnableAsyncCompute": true,
    "WorkerThreadCount": 4,
    "EnablePerformanceProfiling": false
  }
}
```

## File Structure

### Public Headers (80+ files)
```
RHIGraph.hpp              - Main submission interface
RHIDevice.hpp             - GPU device abstraction
RHIWorker.hpp             - Command queue
RHIPass.hpp               - Base for all GPU work
RHIPass*.hpp              - Specific pass types
RHISubmissionItem.hpp     - Submission base class
RHISubmissionList.hpp     - Batch container
RHIPipelineState.hpp      - GPU pipeline configuration
RHI*.hpp                  - Supporting types and enums
```

### Private Implementation (50+ files)
```
RHIGraph.cpp              - Submission routing logic
RHIDevice.cpp             - Device lifecycle
RHIWorker.cpp             - Worker execution
RHIPass*.cpp              - Pass implementations
RHISubsystem.cpp          - Subsystem management
DirectX12/                - DX12-specific backends
  RHIGraph.cpp            - DX12 submission
  RHIDevice.cpp           - DX12 device
  RHIPipelineState.cpp    - PSO compilation
  ...
```

## Usage Examples

### Basic Rendering
```cpp
// Get the singleton graph
auto RHIGraph = A_RHIGraph::GetInstance();

// Submit a draw pass
auto DrawPass = RHICreateSubmissionItem<F_RHIDrawPass>(params);
auto Request = RHIGraph->Submit(DrawPass);

// Optional: wait for GPU completion
Request->Wait();
```

### Batch Operations
```cpp
auto Batch = RHICreateSubmissionItem<F_RHISubmissionList>(
    E_RHISubmissionListOrder::SEQUENTIAL
);

// Add multiple passes in order
Batch->Add(ClearPass);
Batch->Add(DrawPass);
Batch->Add(ReadbackPass);

RHIGraph->Submit(Batch);
```

### Compute Pipeline
```cpp
// Submit compute work to dedicated queue
auto ComputePass = RHICreateSubmissionItem<F_RHIDispatchComputePass>(
    F_RHIDispatchComputePassBuildParams{
        .GPUWorkClass = E_RHIGPUWorkClass::COMPUTE
    }
);

RHIGraph->Submit(ComputePass);
// Can run simultaneously with graphics work!
```

### Data Transfers
```cpp
// Upload to GPU
auto UploadPass = RHICreateSubmissionItem<F_RHIUploadPass>(params);
RHIGraph->Submit(UploadPass);

// Download from GPU
auto ReadbackPass = RHICreateSubmissionItem<F_RHIReadbackPass>(params);
auto Request = RHIGraph->Submit(ReadbackPass);
Request->Wait();  // Block until ready to read
```

## Performance Considerations

### Best Practices
1. **Batch Submissions**: Group related work in `F_RHISubmissionList`
2. **Use Async Compute**: Put independent compute on separate queue
3. **Minimize Synchronization**: Only wait when necessary
4. **Pipeline Optimization**: Pre-compile pipeline states

### Known Limitations
1. **Spin-Lock Wait**: CPU will busy-wait during `Request->Wait()`
2. **Single-threaded Graph**: Submit only from one thread for safety
3. **Immediate Execution**: No command list recording/playback yet

## Testing & Debugging

### Built-in Validation
- Device capability checking
- Work class validation
- GPU memory tracking

### Recommended Testing
1. Submit various pass types and verify execution
2. Test async compute running parallel to graphics
3. Verify worker queue ordering with SEQUENTIAL lists
4. Stress test with high-frequency submissions

### Debugging Aids
- Request status query: `Request->IsDone()`
- Device enumeration: `RHIGraph->GetDevices()`
- Worker inspection: `Device->GetMainWorker()`

## Extending the Module

### Adding a New Pass Type

1. Create header: `RHIMyPass.hpp`
```cpp
class F_RHIMyPass : public A_RHIGPUPass {
    // ...
};
```

2. Implement: `RHIMyPass.cpp`

3. Add to RHIGraph.hpp `Submit()` overloads

4. Implement DirectX 12 handling in `F_DirectX12RHIGraph::Submit()`

### Adding a New Graphics API

1. Create `A_RHIGraph` subclass (e.g., `F_VulkanRHIGraph`)
2. Implement all `Submit()` virtual methods
3. Register in `RHIGraph.cpp` object construction
4. Create corresponding device/worker backends

## Related Modules

- **Engine.Base** - Core types and allocators
- **Engine.Rendering** - Higher-level rendering abstraction
- **Engine.Editor** - Viewport rendering

## References

- [RHI Architecture](../RHI%20Architecture.md) - Detailed architecture documentation
- DirectX 12 API Documentation
- GPU command submission best practices
