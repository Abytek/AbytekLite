# Abytek RHI (Rendering Hardware Interface) Architecture

## Overview

The Abytek RHI is a rendering abstraction layer designed to provide a unified interface for GPU rendering operations across different graphics APIs (currently DirectX 12). The architecture follows a hierarchical, object-oriented design with clear separation of concerns between API-agnostic logic and API-specific implementations.

## Core Components

### 1. RHI Subsystem (`F_RHISubsystem`)
- **Role**: Central management system for the entire RHI
- **Responsibilities**:
  - Manages the active graphics API (DirectX 12, etc.)
  - Owns `A_RHIDeviceManager` - manages all GPU devices
  - Owns `A_RHIWorkerManager` - manages command execution workers
  - Lifetime management for all RHI resources
- **Key Methods**:
  - `PostInit()` - Initialize after engine startup
  - `PreRelease()` / `Release()` - Cleanup operations
  - `GetActiveAPI()` - Query currently active graphics API
  - `GetProxy()` - Access the API-specific proxy

### 2. RHI Graph (`A_RHIGraph`)
- **Role**: Command submission and routing engine
- **Pattern**: Singleton using Render Allocator
- **Responsibilities**:
  - Acts as the main submission point for GPU work
  - Routes submission items to appropriate workers/devices
  - Maintains registry of devices and workers
  - Supports multiple render APIs through polymorphism
- **Key Operations**:
  ```cpp
  // Submit various work types
  Submit(A_RHISubmissionItem)
  Submit(F_RHICompilePipelineState)
  Submit(F_RHISubmissionList)
  Submit(F_RHIClearPass)
  Submit(F_RHIDrawPass / F_RHIDrawIndexedPass)
  Submit(F_RHIDispatchComputePass)
  Submit(F_RHIRayTracePass)
  Submit(F_RHIReadbackPass)
  Submit(F_RHIUploadPass)
  Submit(F_RHIWorkGraphPass)
  ```

### 3. Device Management

#### A_RHIDevice
- **Role**: Represents a single GPU device
- **Contains**: Three specialized worker queues:
  - `MainWorker` - Primary graphics operations (E_RHIGPUWorkClass::DIRECT)
  - `AsyncComputeWorker` - Independent compute operations (E_RHIGPUWorkClass::COMPUTE)
  - `BlitWorker` - Transfer operations (E_RHIGPUWorkClass::BLIT)
- **Properties**:
  - Device info (capabilities, memory, etc.)
  - Activation state
- **Key Methods**:
  - `Build()` - Initialize device and workers
  - `Activate()` / `Deactivate()` - Enable/disable device
  - `Release()` - Cleanup resources

#### A_RHIDeviceManager
- **Role**: Collection manager for all GPU devices
- **Responsibilities**:
  - Creates and owns all RHI devices
  - Provides device enumeration
  - Handles device lifecycle

### 4. Worker Management

#### A_RHIWorker
- **Role**: Command queue abstraction
- **Responsibilities**:
  - Represents a GPU command queue of a specific type
  - Manages associated runnable/thread
  - Executes work from its queue
- **Configuration**:
  - GPU work class (DIRECT, COMPUTE, BLIT)
  - Optional dedicated runnable thread
  - Device association
- **Key Methods**:
  - `Build()` - Initialize worker and create/acquire runnable
  - `AcquireRunnable()` - Get or create execution thread
  - `Release()` - Cleanup

#### A_RHIWorkerManager
- **Role**: Worker collection manager
- **Responsibilities**:
  - Creates workers from device specifications
  - Manages worker lifecycle
  - Provides worker enumeration

### 5. Submission System

#### Submission Items (`A_RHISubmissionItem`)
Base class for all GPU work submissions. Provides:
- Type identification via `GetTypeHashCode()`
- `CreateRequest()` - Creates tracking request for submission

#### Submission Request (`A_RHISubmissionRequest`)
- **Role**: Track submission completion status
- **Properties**:
  - `_IsDone` flag - Set when GPU work completes
- **Key Methods**:
  - `IsDone()` - Query completion status
  - `Wait()` - Block until submission completes

#### Submission List (`F_RHISubmissionList`)
- **Role**: Group multiple submissions together
- **Execution Modes**:
  - `SEQUENTIAL` - Execute in order
  - `PARALLEL` - May execute concurrently
- **Container**: Vector of `F_RHISubmissionSlot` items

## Pass Types (GPU Work Units)

All passes inherit from `A_RHIPass` which extends `A_RHISubmissionItem`:

### 1. Graphics Passes
Inherit from `A_RHIGraphicsPass` → `A_RHIGPUPass`

- **F_RHIClearPass**: Clear render targets/depth
- **F_RHIDrawPass**: Unindexed draw call
- **F_RHIDrawIndexedPass**: Indexed draw call
- **A_RHIRasterPass**: Base for rasterization operations

### 2. Compute Passes
Inherit from `A_RHIComputePass` → `A_RHIGPUPass`

- **F_RHIDispatchComputePass**: Compute shader dispatch
- **F_RHIWorkGraphPass**: Work graph execution (newer compute model)

### 3. Data Transfer Passes
Inherit from `A_RHIPass` directly

- **F_RHIUploadPass**: CPU → GPU data transfer
- **F_RHIReadbackPass**: GPU → CPU data readback
- **F_RHIBlitPass**: GPU-to-GPU copy/blit operations

### 4. Ray Tracing Passes
- **F_RHIRayTracePass**: Ray tracing workload execution

## GPU Work Classes

```cpp
enum class E_RHIGPUWorkClass
{
    NONE,
    DIRECT,     // Main graphics queue (graphics + async compute)
    COMPUTE,    // Async compute queue (independent compute)
    BLIT,       // Transfer queue (copies, blits)
    DEFAULT = DIRECT
};
```

Each work class routes to corresponding device worker:
- DIRECT → Device's MainWorker
- COMPUTE → Device's AsyncComputeWorker
- BLIT → Device's BlitWorker

## Pipeline State Management

- **F_RHIPipelineState**: Encapsulates GPU pipeline configuration
- **F_RHICompilePipelineState**: Submission item for pipeline compilation

## Inheritance Hierarchy

```
A_Object
├── A_RHISubmissionItem (base for all submissions)
│   ├── A_RHIPass (all GPU work has GPU work class)
│   │   ├── A_RHIGPUPass (GPU-based operations)
│   │   │   ├── A_RHIGraphicsPass
│   │   │   │   ├── A_RHIRasterPass
│   │   │   │   │   ├── F_RHIDrawPass
│   │   │   │   │   └── F_RHIDrawIndexedPass
│   │   │   │   └── F_RHIClearPass
│   │   │   └── A_RHIComputePass
│   │   │       └── F_RHIDispatchComputePass
│   │   ├── F_RHIRayTracePass
│   │   ├── F_RHIBlitPass
│   │   ├── F_RHIUploadPass
│   │   └── F_RHIReadbackPass
│   ├── F_RHISubmissionList
│   └── F_RHICompilePipelineState
│
├── A_RAObject
│   ├── A_RHIGraph (singleton - command submission hub)
│   └── A_RHIDevice (represents one GPU)
│
└── A_RHIWorker (command queue for specific work class)
```

## Submission Flow

```
1. User Code Creates Pass/List
   ↓
2. Submit to A_RHIGraph::Submit()
   ↓
3. Base implementation calls submission.CreateRequest()
   ↓
4. API-specific override (F_DirectX12RHIGraph::Submit)
   ├─→ Route to appropriate worker based on GPU work class
   ├─→ Worker executes on device's command queue
   └─→ Returns A_RHISubmissionRequest for tracking
   ↓
5. Optional: User calls request->Wait() to block until complete
```

## Memory Management

- **Render Allocator Pattern**: Uses transient allocator for short-lived GPU work
- **Object Creation**: `RHICreateSubmissionItem<PassType>(...)` helper
- **Weak References**: Uses weak pointers for device/worker collections to avoid cycles

## API-Specific Implementation

### DirectX 12 Implementation
- **F_DirectX12RHIGraph**: Overrides `Submit()` for DX12 command submission
- **DirectX12 Workers**: Implement device command queue mapping
- **Pipeline State Compilation**: Converts to DX12 PSO format
- **Location**: `Source/Private/Abytek/DirectX12/`

## Design Patterns Used

1. **Singleton Pattern**: RHIGraph, RHISubsystem for global access
2. **Strategy Pattern**: Different pass types implement different GPU operations
3. **Factory Pattern**: Device/Worker creation through managers
4. **Template Method**: Pass base classes define structure
5. **Object Pool**: Render allocator manages GPU object lifetime
6. **Type Erasure**: Hash codes for runtime type dispatch

## Key Concepts

- **GPU Work Class**: Routes work to appropriate queue (DIRECT, COMPUTE, BLIT)
- **Transient Allocation**: Passes allocated for single submission, cleaned up after
- **Polymorphic Submission**: Unified Submit() interface for all work types
- **Device/Worker Separation**: Each device manages its own queues
- **Async Compute**: Enabled via separate AsyncComputeWorker

## Recent Changes (Feature: RHI DX12)

- Added `F_RHIDispatchComputePass` for compute shader execution
- Enhanced `A_RHIGraph` submission handling
- Improved `A_RHIPipelineState` management
- DirectX 12 specific optimizations to `F_DirectX12RHIGraph`

---

## Architecture Diagrams

### System Architecture
```
┌─────────────────────────────────────────────────────────────────┐
│                       F_RHISubsystem                            │
│           (Engine Subsystem - Central Management)               │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  ┌──────────────────┐     ┌──────────────────┐                 │
│  │ A_RHIGraphics    │     │ A_RHIWorker      │                 │
│  │    (Singleton)   │     │   Manager        │                 │
│  │                  │     │                  │                 │
│  │ • Routes work    │     │ • Creates        │                 │
│  │ • Manages        │     │   workers        │                 │
│  │   devices        │     │ • Manages        │                 │
│  │ • Owns workers   │     │   lifecycle      │                 │
│  └────────┬─────────┘     └──────────────────┘                 │
│           │                                                    │
│           │ Provides Device List                              │
│           ▼                                                    │
│  ┌──────────────────┐                                          │
│  │ A_RHIDevice      │  (Can be multiple)                       │
│  │   Manager        │                                          │
│  │                  │                                          │
│  │ • Enumerates     │                                          │
│  │   GPUs           │                                          │
│  │ • Creates        │                                          │
│  │   devices        │                                          │
│  └──────────────────┘                                          │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

### Device and Worker Architecture
```
┌─────────────────────────────────────────┐
│         A_RHIDevice (GPU)               │
├─────────────────────────────────────────┤
│                                         │
│ ┌──────────────────────────────────┐   │
│ │ MainWorker (DIRECT)              │   │
│ │ • Graphics rendering             │   │
│ │ • Async compute allowed          │   │
│ │ • Routes F_RHIDrawPass           │   │
│ │ • Routes F_RHIDispatchCompute... │   │
│ └──────────────────────────────────┘   │
│                                         │
│ ┌──────────────────────────────────┐   │
│ │ AsyncComputeWorker (COMPUTE)     │   │
│ │ • Independent compute queue      │   │
│ │ • Can run parallel to graphics   │   │
│ │ • Routes COMPUTE work class      │   │
│ └──────────────────────────────────┘   │
│                                         │
│ ┌──────────────────────────────────┐   │
│ │ BlitWorker (BLIT)                │   │
│ │ • Copy/transfer operations       │   │
│ │ • Dedicated for I/O efficiency   │   │
│ │ • Routes F_RHIUploadPass         │   │
│ │ • Routes F_RHIReadbackPass       │   │
│ └──────────────────────────────────┘   │
│                                         │
│ Device Info (capabilities, memory, etc)│
└─────────────────────────────────────────┘
```

### Submission and Pass Type Hierarchy
```
┌─────────────────────────────────────────────────────────────┐
│              A_RHISubmissionItem (Abstract)                 │
│  (All GPU work items inherit from this)                     │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│ ┌──────────────────┐  ┌──────────────────┐  ┌─────────────┐│
│ │   A_RHIPass      │  │F_RHISub...List   │  │F_RHICompile││
│ │                  │  │                  │  │PipelineState││
│ │ Base for all     │  │ Container for    │  │             ││
│ │ GPU work         │  │ multiple items   │  │ Pipeline    ││
│ │ (has GPU class)  │  │                  │  │ compilation ││
│ └────────┬─────────┘  └──────────────────┘  └─────────────┘│
│          │                                                  │
│          ├─ Graphics Operations                            │
│          │  ├─ F_RHIClearPass (Clear RT/Depth)            │
│          │  ├─ F_RHIDrawPass (Unindexed draw)             │
│          │  └─ F_RHIDrawIndexedPass (Indexed draw)        │
│          │                                                 │
│          ├─ Compute Operations                             │
│          │  ├─ F_RHIDispatchComputePass                    │
│          │  └─ F_RHIWorkGraphPass                          │
│          │                                                 │
│          ├─ Data Transfer Operations                       │
│          │  ├─ F_RHIUploadPass (CPU→GPU)                  │
│          │  ├─ F_RHIReadbackPass (GPU→CPU)                │
│          │  └─ F_RHIBlitPass (GPU→GPU)                    │
│          │                                                 │
│          └─ Ray Tracing Operations                         │
│             └─ F_RHIRayTracePass                           │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

### Submission Flow Diagram
```
┌──────────────────────────────────────────────────────────────┐
│  Application Code                                            │
│  auto Pass = RHICreateSubmissionItem<F_RHIDrawPass>(params)  │
└──────────────┬───────────────────────────────────────────────┘
               │
               ▼
┌──────────────────────────────────────────────────────────────┐
│  A_RHIGraph::Submit(A_RHISubmissionItem)                      │
│  (Base implementation - routes through polymorphism)          │
└──────────────┬───────────────────────────────────────────────┘
               │
               ▼
┌──────────────────────────────────────────────────────────────┐
│  F_DirectX12RHIGraph::Submit() Override                       │
│  (API-specific implementation)                                │
├──────────────────────────────────────────────────────────────┤
│                                                              │
│  1. Inspect submission item type                             │
│  2. Determine GPU work class                                 │
│  3. Select appropriate worker:                               │
│     ├─ DIRECT      → Device->GetMainWorker()                 │
│     ├─ COMPUTE     → Device->GetAsyncComputeWorker()         │
│     └─ BLIT        → Device->GetBlitWorker()                 │
│  4. Translate to DirectX 12 format                           │
│  5. Submit to device command queue                           │
│                                                              │
└──────────────┬───────────────────────────────────────────────┘
               │
               ▼
┌──────────────────────────────────────────────────────────────┐
│  A_RHIWorker                                                 │
│  (Executes on dedicated GPU command queue)                   │
├──────────────────────────────────────────────────────────────┤
│  • Runs on Runnable thread (if dedicated)                   │
│  • GPU hardware executes commands                            │
│  • Sets completion flag on A_RHISubmissionRequest            │
└──────────────┬───────────────────────────────────────────────┘
               │
               ▼
┌──────────────────────────────────────────────────────────────┐
│  A_RHISubmissionRequest Returned to Application              │
│  • User can call request->IsDone()                           │
│  • User can call request->Wait() to block until complete     │
└──────────────────────────────────────────────────────────────┘
```

### Work Class Routing
```
Pass Type                          GPU Work Class    Routes To
─────────────────────────────────────────────────────────────
F_RHIDrawPass                      DIRECT        → MainWorker
F_RHIDrawIndexedPass               DIRECT        → MainWorker
F_RHIClearPass                     DIRECT        → MainWorker
F_RHIDispatchComputePass           COMPUTE       → AsyncComputeWorker
F_RHIWorkGraphPass                 COMPUTE       → AsyncComputeWorker
F_RHIUploadPass                    BLIT          → BlitWorker
F_RHIReadbackPass                  BLIT          → BlitWorker
F_RHIBlitPass                      BLIT          → BlitWorker
F_RHIRayTracePass                  DIRECT/COMPUTE → MainWorker
```

---

## Implementation Notes

### Object Lifecycle
1. **Creation**: Objects allocated via render allocator (transient)
2. **Submission**: Submit to RHI Graph
3. **Execution**: Worker thread processes
4. **Completion**: Request marked as done
5. **Cleanup**: Render allocator reclaims memory

### Synchronization Points
- `A_RHISubmissionRequest::Wait()` - Spin-lock until GPU complete
- Multiple submission lists can execute in parallel with PARALLEL mode
- SEQUENTIAL lists maintain order within themselves

### DirectX 12 Mapping
- `A_RHIDevice` ↔ `ID3D12Device`
- `A_RHIWorker` ↔ `ID3D12CommandQueue`
- `Pass` ↔ `ID3D12GraphicsCommandList` recording
- Pipeline state compilation ↔ `ID3D12PipelineState`

### Thread Safety
- Render allocator handles allocation synchronization
- Workers may run on dedicated threads
- Command lists built on submission thread
- GPU execution happens asynchronously

---

## Current Implementation Status

### Completed Components ✓

**Core Infrastructure:**
- [x] `A_RHIGraph` - Submission routing engine
- [x] `F_RHISubsystem` - Central management
- [x] `A_RHIDevice` - GPU device abstraction
- [x] `A_RHIDeviceManager` - Device enumeration and management
- [x] `A_RHIWorker` - Command queue abstraction
- [x] `A_RHIWorkerManager` - Worker lifecycle management

**Submission System:**
- [x] `A_RHISubmissionItem` - Base class for all work items
- [x] `A_RHISubmissionRequest` - Completion tracking
- [x] `F_RHISubmissionList` - Batch submission with ordering modes

**Pass Types:**
- [x] Graphics passes: Draw, DrawIndexed, Clear
- [x] Compute passes: ComputeDispatch (NEW), WorkGraph
- [x] Transfer passes: Upload, Readback, Blit
- [x] Ray tracing pass: RayTrace

**DirectX 12 Support:**
- [x] `F_DirectX12RHIGraph` - DX12-specific submission handling
- [x] Work class routing to appropriate command queues
- [x] Device/Worker mapping to DX12 concepts
- [x] Pipeline state compilation

### Recent Changes (feature/rhi_dx12)

**Files Modified:**
1. `RHIGraph.hpp` / `RHIGraph.cpp` - Base submission routing
2. `DirectX12/RHIGraph.hpp` / `DirectX12/RHIGraph.cpp` - DX12 implementation
3. `RHIPipelineState.hpp` / `RHIPipelineState.cpp` - Pipeline state handling
4. `DirectX12/RHIPipelineState.hpp` / `DirectX12/RHIPipelineState.cpp` - DX12 PSO
5. `RHISubmissionItem.hpp` - Enhanced submission interface
6. `RHISubmissionList.hpp` - Improved list handling

**New Files Added:**
1. `RHIDispatchComputePass.hpp` - Compute shader dispatch submission item
2. `RHIDispatchComputePass.cpp` - Implementation

**Key Enhancements:**
- Refined polymorphic submit() method for all pass types
- Improved pipeline state compilation workflow
- Added explicit compute pass dispatch with `F_RHIDispatchComputePass`
- Better separation between compute and graphics work
- Enhanced DX12 backend optimization

### Architecture Strengths

1. **Clean Separation of Concerns**
   - API-agnostic base classes with virtual overrides
   - DirectX 12 specifics isolated to DirectX12 namespace

2. **Type Safety**
   - Template-based object creation prevents allocation errors
   - Type hashing for runtime dispatch

3. **Performance**
   - Dedicated workers for different work classes enable parallelism
   - Transient allocation minimizes overhead
   - Async compute fully independent from graphics

4. **Extensibility**
   - Easy to add new pass types (inherit from appropriate base)
   - Simple to add new graphics APIs (inherit from A_RHIGraph)
   - GPU work class system scales naturally

5. **Resource Management**
   - Render allocator handles cleanup automatically
   - No manual memory management needed
   - Weak references prevent reference cycles

### Known Design Decisions

1. **Spin-Lock Synchronization**: `Wait()` uses busy-wait rather than kernel events
   - Trade-off: CPU usage vs simplicity
   - Consider event-based sync for future optimization

2. **Type Erasure via Hash Codes**: Runtime type identification
   - Alternative: RTTI could be used but hash codes avoid vtable overhead

3. **Dedicated Workers Optional**: Workers can share threads or have dedicated threads
   - Configurable via `UseDedicatedRunnable` flag

4. **Sequential vs Parallel Lists**: User must specify submission list ordering
   - Gives explicit control over GPU scheduling

### Extension Points for Future Enhancement

**Potential Additions:**
1. **Video Encoding/Decoding** - New pass type for media operations
2. **Hardware Scheduling** - Leverage DX12 GPU scheduling improvements
3. **Asynchronous Compilation** - Pipeline state pre-compilation
4. **Shader Model 6.8+** - Support newer DirectX features
5. **VK/Metal Backends** - Add Vulkan and Metal support via derived graphs
6. **Performance Monitoring** - Built-in GPU performance counters
7. **Descriptor Heap Management** - Centralized resource binding
8. **Raytracing Optimization** - Dedicated RT command recording

### Quality Metrics

**File Organization:**
- 50+ public headers defining clear interfaces
- 50+ private implementations
- API-specific implementations in DirectX12 subdirectory
- Clear naming conventions (A_ for abstract, F_ for final, E_ for enum)

**Code Structure:**
- Consistent with Abytek codebase patterns
- RAObject inheritance for render allocator integration
- Singleton pattern for graph and subsystem
- Virtual methods for polymorphism

**Testing Recommendations:**
1. Unit tests for submission item creation
2. Integration tests for submission routing
3. Performance benchmarks for work queue throughput
4. Stress tests for concurrent submissions
5. Validation of GPU synchronization correctness

---

## Quick Reference: Common Patterns

### Creating and Submitting Work

```cpp
// Create a draw pass
auto DrawPass = RHICreateSubmissionItem<F_RHIDrawPass>(
    F_RHIDrawPassBuildParams{ /* params */ }
);

// Submit it
auto Request = A_RHIGraph::GetInstance()->Submit(DrawPass);

// Wait for completion (optional)
Request->Wait();
```

### Creating a Batch

```cpp
auto List = RHICreateSubmissionItem<F_RHISubmissionList>(
    E_RHISubmissionListOrder::SEQUENTIAL
);

List->Add(pass1);
List->Add(pass2);
List->Add(pass3);

A_RHIGraph::GetInstance()->Submit(List);
```

### Async Compute Pattern

```cpp
// Create compute pass with COMPUTE work class
auto ComputePass = RHICreateSubmissionItem<F_RHIDispatchComputePass>(
    F_RHIDispatchComputePassBuildParams{
        .GPUWorkClass = E_RHIGPUWorkClass::COMPUTE  // Routes to AsyncComputeWorker
    }
);

A_RHIGraph::GetInstance()->Submit(ComputePass);
```

### Multi-Device Pattern

```cpp
auto Devices = A_RHIGraph::GetInstance()->GetDevices();
for (const auto& Device : Devices)
{
    // Can submit work to specific device via Worker selection
}
```
