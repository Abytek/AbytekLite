#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"
#include "Abytek/DirectX12/RHIResource.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class A_DirectX12RHIPassExtension;
    class A_RHIPassProxy;
    class F_DirectX12RHICommandQueue;
    class F_DirectX12RHICommandQueueProxy;
    
    static constexpr U32 DirectX12RHIMaxPassBatchSize = 256;
    
    enum class E_DirectX12RHIExecutionRangeType : U8
    {
        NONE,
        USE_COMMAND_LIST,
        USE_COMMAND_QUEUE,
        CPU_ACCESS,
        DEFAULT = USE_COMMAND_LIST
    };
    inline B8 DirectX12RHIIsCPUExecutionRange(E_DirectX12RHIExecutionRangeType ExecutionRangeType)
    {
        if (ExecutionRangeType == E_DirectX12RHIExecutionRangeType::CPU_ACCESS)
        {
            return true;
        }
        return false;
    }
    struct F_DirectX12RHIExecutionRange
    {
        E_DirectX12RHIExecutionRangeType Type = E_DirectX12RHIExecutionRangeType::DEFAULT;
        
        U32 BeginLocalPassExtensionIndex = 0;
        U32 EndLocalPassExtensionIndex = 0;
        U32 GetNumPassExtensions() const noexcept
        {
            return EndLocalPassExtensionIndex - BeginLocalPassExtensionIndex;
        }
        
        F_DirectX12RHIResourceBarrierProxySet D3D12ResourceBarrierProxies_Before;
        F_DirectX12RHIResourceBarrierProxySet D3D12ResourceBarrierProxies_After;
        
        B8 ShouldSignalFence = false;
        B8 ShouldSyncGPU = false;
        
        mutable B8 IsSubmitted = false;
        mutable U64 FenceValueOnCompletion = ~U64(0);
    
        AB8& GetAtomic_IsSubmitted() const
        {
            return *(AB8*)&IsSubmitted;
        }
        AU64& GetAtomic_FenceValueOnCompletion() const
        {
            return *(AU64*)&FenceValueOnCompletion;
        }
    };

    enum class E_DirectX12RHIPassBatchType : U8
    {
        NONE = 0,
        CPU_SYNC = 1,
        GPU = 2,
        DEFAULT = GPU
    };
    struct F_DirectX12RHIPassBatch
    {
        E_DirectX12RHIPassBatchType Type = E_DirectX12RHIPassBatchType::DEFAULT;

        TW<F_DirectX12RHICommandQueue> CommandQueue;
        TW<F_DirectX12RHICommandQueueProxy> CommandQueueProxy;
        
        TF_Vector<TW<A_DirectX12RHIPassExtension>> PassExtensions;
        TF_Vector<F_DirectX12RHIExecutionRange> ExecutionRanges;
        
        TF_Vector<TS<A_RHIPassProxy>> PassProxies;
        
        B8 HasCommands_Begin = false;
        B8 HasCommands_End = false;
        
        TF_SmallVector<U32, 16> DependencyIndices;
        TF_SmallVector<U32, 16> ReverseDependencyIndices;
    };
}
#endif