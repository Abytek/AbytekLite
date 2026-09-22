#pragma once

#include "RHIBindGroup.hpp"
#include "Abytek/RHIProcess.hpp"
#include "Abytek/RHIResourceProxy.hpp"
#include "Abytek/RHIResourceViewProxy.hpp"
#include "Abytek/RHISamplerProxy.hpp"
#include "Abytek/RHIViewportProxy.hpp"
#include "Abytek/RHIViewport.hpp"
#include "Abytek/RHIBindGroup.hpp"
#include "Abytek/RHIConstantDataManager.hpp"
#include "Abytek/DirectX12/RHIDescriptorManager.hpp"
#include "Abytek/DirectX12/RHIDescriptorManagerProxy.hpp"
#include "Abytek/DirectX12/RHICommandQueue.hpp"
#include "Abytek/DirectX12/RHICommandQueueProxy.hpp"
#include "Abytek/DirectX12/RHIRootSignatureTemplateRuntimeProxy.hpp"
#include "Abytek/DirectX12/RHIPipelineStateTemplateRuntimeProxy.hpp"
#include "Abytek/DirectX12/RHIPipelineStateProxy.hpp"
#include "Abytek/DirectX12/RHIPassBatch.hpp"
#include "Abytek/DirectX12/RHISubmissionItemExtension.hpp"
#include "Abytek/DirectX12/RHISubmissionListExtension.hpp"
#include "Abytek/DirectX12/RHIPassExtension.hpp"
#include "Abytek/DirectX12/RHIResource.hpp"
#include "Abytek/DirectX12/RHIResourceView.hpp"
#include "Abytek/DirectX12/RHIPassBatch.hpp"
#include "Abytek/RHIBindGroupProxy.hpp"
#include "Abytek/RHITransientUploadBuffer.hpp"
#include "Abytek/RHITransientReadbackBuffer.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    namespace DirectX12RHIProcessQueries
    {
        namespace Compile
        {
            using F_DeallocateDescriptors = DirectX12RHIContextQueries::F_DeallocateDescriptors;
            
            struct F_UploadBuffer
            {
                TS<A_RHIResource> Resource;
                F_RHIBufferDataView BufferDataView;
            };
            struct F_UploadTexture
            {
                TS<A_RHIResource> Resource;
                F_RHITextureDataView TextureDataView;
            }; 
            struct F_UploadRTAS
            {
                TS<A_RHIResource> Resource;
                F_RHIRTASDataView RTASDataView;
            };
            
            struct F_ResourceStaticTransition
            {
                TS<A_RHIResource> Resource;
            };
            
            struct F_UploadConstantData
            {
                F_RHIConstantDataRange ConstantDataRange;
                F_RHIBufferDataView BufferDataView;
            };
        }
        namespace Execution
        {
            struct F_InitCommandQueue
            {
                TS<F_DirectX12RHICommandQueueProxy> CommandQueueProxy;
            };
            
            struct F_InitRootSignature
            {
                TS<F_DirectX12RHIRootSignatureTemplateRuntimeProxy> RootSignatureTemplateRuntimeProxy;
            };
            struct F_InitPipelineState
            {
                TS<F_DirectX12RHIPipelineStateTemplateRuntimeProxy> PipelineStateTemplateRuntimeProxy;
            };
            
            struct F_InitViewport
            {
                TS<A_RHIViewportProxy> ViewportProxy;
            };
            struct F_ResizeViewport
            {
                TS<A_RHIViewportProxy> ViewportProxy;
                F_Vector2_U32 NewSize;
            };
            struct F_AssignViewportFrame
            {
                TS<A_RHIViewportProxy> ViewportProxy;
                TS<A_RHIResourceProxy> ResourceProxy;
                U32 FrameIndex = 0;
            };
            
            struct F_AssignExternalD3D12Resource
            {
                TS<A_RHIResourceProxy> ResourceProxy;
                Microsoft::WRL::ComPtr<ID3D12Resource> D3D12Resource;
            };
            struct F_CreateCommittedResource
            {
                TS<A_RHIResourceProxy> ResourceProxy;
                D3D12_HEAP_PROPERTIES D3D12HeapProperties;
                D3D12_RESOURCE_STATES InitialD3D12ResourceState;
                D3D12_RESOURCE_DESC D3D12ResourceDesc;
                TF_Optional<D3D12_CLEAR_VALUE> D3D12ClearValue;
            };
            struct F_CreatePlacedResource
            {
                TS<A_RHIResourceProxy> ResourceProxy;
                D3D12_RESOURCE_STATES InitialD3D12ResourceState;
                D3D12_RESOURCE_DESC D3D12ResourceDesc;
                TF_Optional<D3D12_CLEAR_VALUE> D3D12ClearValue;
                F_DirectX12RHIResourcePlacementProxy ResourcePlacementProxy;
            };
            
            struct F_SendCommitToBindGroupProxy
            {
                TS<A_RHIBindGroupProxy> BindGroupProxy;
                F_RHIBindGroupSlotDataProxy SlotDataProxy;
                F_DirectX12RHIBindGroupAllocationProxy AllocationProxy;
            };
            struct F_CopyDescriptorsForBindGroupProxy
            {
                // <Dest, Src>
                TF_SmallVector<std::pair<F_DirectX12RHIDescriptorAllocation, F_DirectX12RHIDescriptorAllocation>, 4> DescriptorAllocationPairs;
            };
            
            struct F_CopyDescriptors
            {
                DirectX12RHIDescriptorManagerQueries::F_Copy Payload;
            };
            struct F_InitResourceViewDescriptor
            { 
                TS<A_RHIResourceViewProxy> ResourceViewProxy;
                F_DirectX12RHIDescriptorAllocation DescriptorAllocation;
            };
            struct F_InitSamplerDescriptor
            { 
                TS<A_RHISamplerProxy> SamplerProxy;
                F_DirectX12RHIDescriptorAllocation DescriptorAllocation;
            };
        }
    }
    
    namespace DirectX12RHIProcessData
    {
        namespace Compile
        {
        }
        namespace Execution
        {
            struct F_TransientUploadBuffer
            {
                TS<A_RHIResourceProxy> BufferProxy;
                TF_Vector<F_RHITransientUploadBufferCandidate_V2> Candidates;
            };
            struct F_TransientReadbackBuffer
            {
                TS<A_RHIResourceProxy> BufferProxy;
                TF_Vector<F_RHITransientReadbackBufferCandidate_V2> Candidates;
            };
        }
    }
    
    
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIProcess : public A_RHIProcess
    {
    private:
        
    public:
        struct F_Queues
        {
            struct F_Compile
            {
                TF_ConcurrentQueue<DirectX12RHIProcessQueries::Compile::F_DeallocateDescriptors> DeallocateDescriptors;
                
                TF_ConcurrentQueue<TS<A_RHIResource>> AutoPlacedResourcesToAllocate;
                TF_ConcurrentQueue<TS<A_RHIResource>> AutoPlacedResourcesToDeallocate;
                
                TF_ConcurrentQueue<DirectX12RHIProcessQueries::Compile::F_UploadBuffer> UploadBuffer;
                TF_ConcurrentQueue<DirectX12RHIProcessQueries::Compile::F_UploadTexture> UploadTexture;
                TF_ConcurrentQueue<DirectX12RHIProcessQueries::Compile::F_UploadRTAS> UploadRTAS;
                
                TF_ConcurrentQueue<DirectX12RHIProcessQueries::Compile::F_ResourceStaticTransition> ResourceStaticTransition;
                
                TF_ConcurrentQueue<DirectX12RHIProcessQueries::Compile::F_UploadConstantData> UploadConstantData;
            } Compile;
            struct F_Execution
            {
                TF_ConcurrentQueue<DirectX12RHIProcessQueries::Execution::F_InitCommandQueue> InitCommandQueue;
                
                TF_ConcurrentQueue<DirectX12RHIProcessQueries::Execution::F_InitRootSignature> InitRootSignature;
                TF_ConcurrentQueue<DirectX12RHIProcessQueries::Execution::F_InitPipelineState> InitPipelineState;
                
                TF_ConcurrentQueue<DirectX12RHIProcessQueries::Execution::F_InitViewport> InitViewport;
                TF_ConcurrentQueue<DirectX12RHIProcessQueries::Execution::F_ResizeViewport> ResizeViewport;
                TF_ConcurrentQueue<DirectX12RHIProcessQueries::Execution::F_AssignViewportFrame> AssignViewportFrame;
                
                TF_ConcurrentQueue<DirectX12RHIProcessQueries::Execution::F_AssignExternalD3D12Resource> AssignExternalD3D12Resource;
                TF_ConcurrentQueue<DirectX12RHIProcessQueries::Execution::F_CreateCommittedResource> CreateCommittedResource;
                TF_ConcurrentQueue<DirectX12RHIProcessQueries::Execution::F_CreatePlacedResource> CreatePlacedResource;
                
                TF_ConcurrentQueue<DirectX12RHIProcessQueries::Execution::F_SendCommitToBindGroupProxy> SendCommitToBindGroupProxy;
                TF_ConcurrentQueue<DirectX12RHIProcessQueries::Execution::F_CopyDescriptorsForBindGroupProxy> CopyDescriptorsForBindGroupProxy;
            
                TF_ConcurrentQueue<DirectX12RHIProcessQueries::Execution::F_InitResourceViewDescriptor> InitResourceViewDescriptor;
                TF_ConcurrentQueue<DirectX12RHIProcessQueries::Execution::F_InitSamplerDescriptor> InitSamplerDescriptor;
                TF_ConcurrentQueue<DirectX12RHIProcessQueries::Execution::F_CopyDescriptors> CopyDescriptors;
            } Execution;
        } Queues;
        
        struct F_CompileSectionData
        {
            TF_Vector<TW<A_DirectX12RHIPassExtension>> PassExtensions; // Graph-based order
            TF_Vector<TW<A_DirectX12RHIPassExtension>> PassExtensions_SortedByDependencyScore;
            TF_Vector<TW<A_RHIViewport>> Viewports;
            TF_Vector<F_DirectX12RHISubresourceReference> SubresourceReferences;
            TF_Vector<TW<A_RHIResource>> Resources;
            
            TF_Vector<TS<A_RHIResource>> AutoPlacedResourcesToAllocate;
            TF_Vector<TS<A_RHIResource>> AutoPlacedResourcesToDeallocate;
            
            TF_Vector<TF_Vector<TW<A_RHIResource>>> LevelsOfResourceAllocations;
            TF_Vector<TF_Vector<TW<A_RHIResource>>> LevelsOfResourceDeallocations;
            
            TF_Vector<TW<A_RHIResource>> AutoPlacedResources;
        } CompileSectionData;
    
        struct F_CompileData
        {
            TF_Vector<TW<A_DirectX12RHISubmissionItemExtension>> SubmissionItemExtensions; // Graph-based order
            TF_Vector<TW<A_DirectX12RHISubmissionListExtension>> SubmissionListExtensions; // Graph-based order
            TF_Vector<TW<A_DirectX12RHIPassExtension>> PassExtensions; // Graph-based order
            TF_Vector<TW<A_RHIViewport>> Viewports; // Graph-based order
            
            TF_Vector<F_DirectX12RHISubresourceReference> SubresourceReferences;
            TF_Vector<TS<A_RHIResource>> Resources;
            
            TF_Vector<F_DirectX12RHIPassBatch> PassBatches;
            
            U32 CurrentSection_BeginSubmissionListExtensionIndex = 0;
            U32 CurrentSection_EndSubmissionListExtensionIndex = 0;
            U32 CurrentSection_BeginPassExtensionIndex = 0;
            U32 CurrentSection_EndPassExtensionIndex = 0;
            U32 CurrentSection_BeginViewportIndex = 0;
            U32 CurrentSection_EndViewportIndex = 0;
            
            U32 CurrentSection_BeginSubresourceReferenceIndex = 0;
            U32 CurrentSection_EndSubresourceReferenceIndex = 0;
            
            U32 CurrentSection_BeginResourceIndex = 0;
            U32 CurrentSection_EndResourceIndex = 0;
            
            U32 CurrentSection_BeginPassBatchIndex = 0;
            U32 CurrentSection_EndPassBatchIndex = 0;
            
            U32 CurrentSectionIndex = 0;
            
            TF_Vector<TW<A_RHIResource>> SortedAutoPlacedResourcesToDeallocate;
        } CompileData;
        
        struct F_ExecutionData
        {
            TF_Vector<F_DirectX12RHIPassBatch> PassBatches;
            TS_Unmanaged<F_TaskPromise> PassBatchesPromise;
                
            TF_Vector<DirectX12RHIProcessData::Execution::F_TransientUploadBuffer> TransientUploadBuffers;
            TF_Vector<DirectX12RHIProcessData::Execution::F_TransientReadbackBuffer> TransientReadbackBuffers;
        } ExecutionData;
        
        struct F_LateExecutionData
        {
            TF_Vector<TS<A_RHIViewportProxy>> ViewportProxies;
            TF_Vector<U64> ViewportPresentFenceValuesOnCompletion;
        } LateExecutionData;
        
    public:
        
    public:
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHIProcess);
        virtual void Build(const F_RHIProcessBuildParams& BuildParams) override;
        virtual void Release() override;
        
    protected:
        virtual void Compile(E_RHIProcessFlushFlag Flags) override;
        virtual void CleanCompile() override;
        virtual void Execute() override;
        virtual void CleanExecute() override;
        virtual void BeginLateExecute() override;
        virtual void EndLateExecute() override;
        
    private:
        void _DeallocateDescriptors();
        void _DeallocateResourcePlacements();
        
        void _EarlyAnalyze();
        void _EarlyAnalyze(const TS<A_RHISubmissionItem>& SubmissionItem);
        
        void _UploadResources(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer
        );
        void _UploadBuffer(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
            const DirectX12RHIProcessQueries::Compile::F_UploadBuffer& Query
        );
        void _UploadTexture(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
            const DirectX12RHIProcessQueries::Compile::F_UploadTexture& Query
        );
        void _UploadRTAS(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
            const DirectX12RHIProcessQueries::Compile::F_UploadRTAS& Query
        );
        
        void _UploadConstantData(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer
        );
        
        void _ResourceStaticTransitions();
        void _AddBackBufferTransitions();
        
        void _MainAnalyze();
        void _MainAnalyze(const TS<A_RHISubmissionItem>& SubmissionItem, F_DirectX12RHISubmissionItemGraphData& GraphData);
        
        void _GatherSubresourceBindings();
        void _GatherSubresourceReferences();
        void _GatherSubresourcePassTrackings();
        
        void _GatherResourceUses();
        void _GatherResources();
        void _GatherResourcePassTrackings();
        
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DRIVER_DEBUGGER
        void _ValidateSubresources();
#endif
        
        void _AssignCommandQueueForPasses();
        
        void _GatherSubresourceStateDependencies();
        void _CalculateSubresourceD3D12States();
        void _GatherPrologueStateDependencies();
        void _GatherReverseStateJoinDependencies();
        void _GatherWriteDependencies();
        void _ResolvePassDependencies_WithoutAliasing();
        void _ResolvePassDependencyLevel();
        void _PrepareForAutoPlacedResources();
        void _GatherAliasingDependencies();
        void _ResolvePassDependencyScore();
        void _SortPassesByDependencyScore();
        
        void _CreatePassBatches();
        
        void _UpdateLastSubresourceD3D12States();
        
        void _EndCompileSection();
        void _EndCurrentSectionForSubresources();
        void _EndCurrentSectionForResources();
        
        void _CreateResourceBarriersForPasses();
        void _CreateResourceUAVBarriersForPasses();
        void _CreateResourceTransitionBarriersForPasses();
        void _CreateResourceAliasingBarriersForPasses();
        
#ifdef ABYTEK_ENGINE_RHI_ENABLE_CAPTURE
        void _CreateCaptureDataForPassBatches();
#endif
        
        void _TransferCompileDataToExecutionData();
        void _TransferCompileDataToTransientUploadBuffers();
        void _TransferCompileDataToTransientReadbackBuffers();
        void _TransferCompileDataToLateExecutionData();
        void _UpdateViewports();
        
        void _TransferDataToContextQueues();
        
    private:
        void _InitCommandQueues();
        
        void _InitRootSignatures();
        void _InitPipelineStates();
        
        void _InitViewports();
        void _ResizeViewports();
        void _AssignViewportFrames();
        
        void _AssignD3D12ExternalResources();
        
        void _CreateResources();
        void _CreateCommittedResources();
        void _CreatePlacedResources();
        
        void _SendCommitToBindGroupProxies();
        void _CopyDescriptorsForBindGroupProxies();
        
        void _InitResourceViewDescriptors();
        void _InitSamplerDescriptors();
        void _CopyDescriptors();
        
        void _FlushDescriptorManagers();
        
        void _TransientUploadBuffers();
        
        void _ExecutePassBatches();
        void _JoinPassBatches();
        
        void _TransientReadbackBuffers();
        
        void _CleanCommandListManagers();
        
    private:
        void _ExecutePassBatch(U32 PassBatchIndex);
        
    private:
        void _BeginLateExecuteForViewportProxies();
        void _EndLateExecuteForViewportProxies();
    };
}
#endif