#include "Abytek/DirectX12/RHIProcess.hpp"
#include "Abytek/RHISubmissionUtilities.hpp"
#include "Abytek/RHISubmissionList.hpp"
#include "Abytek/DirectX12/RHIPlacedResourceManagerProxy.hpp"
#include "Abytek/DirectX12/RHIBindGroupProxy.hpp"
#include "Abytek/DirectX12/RHICommandListManager.hpp"
#include "Abytek/DirectX12/RHISynchronizer.hpp"
#include "Abytek/DirectX12/RHIFence.hpp"
#include "Abytek/DirectX12/RHICommandQueueProxy.hpp"
#include "Abytek/DirectX12/RHIPlacedResourceManager.hpp"
#include "Abytek/DirectX12/RHIDevice.hpp"
#include "Abytek/DirectX12/RHIContext.hpp"
#include "Abytek/DirectX12/RHIContextProxy.hpp"
#include "Abytek/DirectX12/RHIPassProxyExtension.hpp"
#include "Abytek/DirectX12/RHIResourceProxy.hpp"
#include "Abytek/DirectX12/RHIResourceViewProxy.hpp"
#include "Abytek/DirectX12/RHISamplerProxy.hpp"
#include "Abytek/DirectX12/RHIViewportProxy.hpp"
#include "Abytek/DirectX12/RHIViewport.hpp"
#include "Abytek/DirectX12/RHIPipelineStateProxy.hpp"
#include "Abytek/DirectX12/RHIResourceSyncPass.hpp"
#include "Abytek/DirectX12/RHIEventStack.hpp"
#include "Abytek/RHITransientUploadBufferManager.hpp"
#include "Abytek/DirectX12/RHIViewportPresentation.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void F_DirectX12RHIProcess::Build(const F_RHIProcessBuildParams& BuildParams)
    {
        A_RHIProcess::Build(BuildParams);
    }
    void F_DirectX12RHIProcess::Release()
    {
        ABYTEK_ENGINE_RHI_ASSERT(Queues.Execution.CopyDescriptors.GetSize() == 0);
        ABYTEK_ENGINE_RHI_ASSERT(Queues.Execution.InitSamplerDescriptor.GetSize() == 0);
        ABYTEK_ENGINE_RHI_ASSERT(Queues.Execution.InitResourceViewDescriptor.GetSize() == 0);
        
        ABYTEK_ENGINE_RHI_ASSERT(Queues.Execution.CopyDescriptorsForBindGroupProxy.GetSize() == 0);
        ABYTEK_ENGINE_RHI_ASSERT(Queues.Execution.SendCommitToBindGroupProxy.GetSize() == 0);
        
        ABYTEK_ENGINE_RHI_ASSERT(Queues.Execution.CreatePlacedResource.GetSize() == 0);
        ABYTEK_ENGINE_RHI_ASSERT(Queues.Execution.CreateCommittedResource.GetSize() == 0);
        ABYTEK_ENGINE_RHI_ASSERT(Queues.Execution.AssignExternalD3D12Resource.GetSize() == 0);
        
        ABYTEK_ENGINE_RHI_ASSERT(Queues.Execution.AssignViewportFrame.GetSize() == 0);
        ABYTEK_ENGINE_RHI_ASSERT(Queues.Execution.ResizeViewport.GetSize() == 0);
        ABYTEK_ENGINE_RHI_ASSERT(Queues.Execution.InitViewport.GetSize() == 0);
        
        ABYTEK_ENGINE_RHI_ASSERT(Queues.Execution.InitPipelineState.GetSize() == 0);
        ABYTEK_ENGINE_RHI_ASSERT(Queues.Execution.InitRootSignature.GetSize() == 0);
        
        ABYTEK_ENGINE_RHI_ASSERT(Queues.Execution.InitCommandQueue.GetSize() == 0);
        
        ABYTEK_ENGINE_RHI_ASSERT(Queues.Compile.UploadConstantData.GetSize() == 0);
        
        ABYTEK_ENGINE_RHI_ASSERT(Queues.Compile.ResourceStaticTransition.GetSize() == 0);
        
        ABYTEK_ENGINE_RHI_ASSERT(Queues.Compile.UploadRTAS.GetSize() == 0);
        ABYTEK_ENGINE_RHI_ASSERT(Queues.Compile.UploadTexture.GetSize() == 0);
        ABYTEK_ENGINE_RHI_ASSERT(Queues.Compile.UploadBuffer.GetSize() == 0);
        
        ABYTEK_ENGINE_RHI_ASSERT(Queues.Compile.AutoPlacedResourcesToDeallocate.GetSize() == 0);
        ABYTEK_ENGINE_RHI_ASSERT(Queues.Compile.AutoPlacedResourcesToAllocate.GetSize() == 0);
        
        ABYTEK_ENGINE_RHI_ASSERT(Queues.Compile.DeallocateDescriptors.GetSize() == 0);
        A_RHIProcess::Release();
    }

    void F_DirectX12RHIProcess::Compile(E_RHIProcessFlushFlag Flags)
    {
        ABYTEK_PROFILER_EVENT();
        A_RHIProcess::Compile(Flags);
        
        // Clean data from the last process
        {
            _DeallocateDescriptors();
            H_TaskUtilities::Switch();
            _DeallocateResourcePlacements();
            H_TaskUtilities::Switch();
        }
        
        // Early analyze (for the required data of Postprocessing phase)
        {
            _EarlyAnalyze();
            H_TaskUtilities::Switch();
        }
        
        // Postprocess
        {
            // Push front 
            {
                // Upload
                {
                    auto SubmissionList = RACreateAndBuildShared<A_RHISubmissionList>(ABYTEK_WTHIS());
#ifdef ABYTEK_DEBUG_INFO
                    SubmissionList->SetDebugName(ABYTEK_DEBUG_NAME("Abytek::RHIUpload"));   
#endif
                    _UploadResources(SubmissionList);
                    H_TaskUtilities::Switch();
                    _UploadConstantData(SubmissionList);
                    H_TaskUtilities::Switch();
                    AddFrontSubmissionList(SubmissionList);
                }
            }
            
            // Push back
            {
                if (FlagHas(Flags, E_RHIProcessFlushFlag::EXECUTE))
                {
                    _ResourceStaticTransitions();
                    H_TaskUtilities::Switch();
                    _AddBackBufferTransitions();
                    H_TaskUtilities::Switch();
                }
            }
        }
        
        // sectioned compile
        {
            _MainAnalyze();
            H_TaskUtilities::Switch();
            
            _GatherSubresourceBindings();
            H_TaskUtilities::Switch();
            _GatherSubresourceReferences();
            H_TaskUtilities::Switch();
            _GatherSubresourcePassTrackings();
            H_TaskUtilities::Switch();
            
            _GatherResourceUses();
            H_TaskUtilities::Switch();
            _GatherResources();
            H_TaskUtilities::Switch();
            _GatherResourcePassTrackings();
            H_TaskUtilities::Switch();
            
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DRIVER_DEBUGGER
            if (GetEnableDriverDebugger())
            {
                _ValidateSubresources();
            }
#endif
            
            _AssignCommandQueueForPasses();
            H_TaskUtilities::Switch();
            
            _GatherSubresourceStateDependencies();
            H_TaskUtilities::Switch();
            _CalculateSubresourceD3D12States();
            H_TaskUtilities::Switch();
            
            /*
             *  Setup non-aliasing subresource dependencies:
             *  - A subresource has the following D3D12 states:
             *      + Last d3d12 state from the last process: d3d12 state X 
             *      + [Compile section 0]
             *          + Pass 1: d3d12 state A
             *          + Pass 2: d3d12 state A
             *          + Pass 3: d3d12 state B
             *          + Pass 4: d3d12 state B
             *          + Pass 5: d3d12 state B
             *      + [Compile section 1]
             *          + Pass 6: d3d12 state B
             *          + Pass 7: d3d12 state C
             *          + Pass 8: d3d12 state C
             *          + Pass 9: d3d12 state C
             *          + Pass 10: d3d12 state D
             *          + Pass 11: d3d12 state D
             *  - For each compile section:
             *      + Set up dependencies for D3D12 state transitions.
             *          A state transition must be synchronized at the pass where the
             *             transition occurs.
             *          => Every continuous range of passes using the same subresources in the same d3d12 state must be synchronized at its end. 
             *             These are called state-joint passes.
             *          => This introduces the concept of reverse state-joint dependencies:
             *             an earlier pass depends on a later state-joint pass.
             *          => Since future compile sections are not available at this point,
             *             reverse state-joint dependencies must be limited to the current compile section.
             *          => In compile section 0:
             *              + Pass 2 depends on Pass 1 due to reverse state-joint dependency.
             *              + Pass 5 depends on Pass 3, 4 due to reverse state-joint dependency.
             *          If d3d12 state transitions only occur at state-joint passes, we will miss some kinds of state transitions:
             *              + From the last d3d12 state to the prologue d3d12 state
             *              + From the last d3d12 state of the previous section to the prologue d3d12 of current section
             *          => Need prologue state dependencies: 
             *              + The first pass that accesses a subresource in every compile section broadcasts the required d3d12 state transitions before executing its commands.
             *              => We must sync on those prologue passes.
             *              => In compile section 0:
             *                  + Pass 2 depends on Pass 1 due to prologue state dependency.
             *      + Cross-section prologue state dependencies:
             *          Ensure that pass batches produced by different compile sections are executed in sequential order.
             *          In every continuous range of passes using the same subresources in the same d3d12 state: 
             *              + The last pass in the range is always a synchronization point.
             *              => Therefore, to synchronize a prologue pass with the entire range, we just need to add a dependency on the last pass.
             */
            {
                _GatherPrologueStateDependencies();
                H_TaskUtilities::Switch();
                _GatherReverseStateJoinDependencies();
                H_TaskUtilities::Switch();
                _GatherWriteDependencies();
                H_TaskUtilities::Switch();
            }
            
            // Resolve non-aliasing pass dependencies using the subresource tracking data above
            // We cannot account aliasing as dependencies here because we need the info of non-aliasing pass dependencies for allocating auto-placed resource allocations.
            _ResolvePassDependencies_WithoutAliasing();
            H_TaskUtilities::Switch();
            
            // This information is for allocating auto-placed resources.
            _ResolvePassDependencyLevel();
            H_TaskUtilities::Switch();
            
            // Allocate auto-placed resources
            _PrepareForAutoPlacedResources();
            H_TaskUtilities::Switch();
            
            // Gather aliasing dependencies for the final pass dependency graph + aliasing barriers
            _GatherAliasingDependencies();
            H_TaskUtilities::Switch();
            
            // May be used in the future, but it is necessary to re-calculate pass-dependency level for correctness.
            _ResolvePassDependencyLevel();
            
            // Calculate dependency score and sort passes for pass batch creations
            H_TaskUtilities::Switch();
            _ResolvePassDependencyScore();
            H_TaskUtilities::Switch();
            _SortPassesByDependencyScore();
            H_TaskUtilities::Switch();
            
            // 
            _CreatePassBatches();
            H_TaskUtilities::Switch();
            
            //
            _UpdateLastSubresourceD3D12States();
            H_TaskUtilities::Switch();
        
            // Final compile 
            if (FlagHas(Flags, E_RHIProcessFlushFlag::EXECUTE))
            {
                _TransferCompileDataToExecutionData();
                H_TaskUtilities::Switch();
                _TransferCompileDataToLateExecutionData();
                H_TaskUtilities::Switch();
                _UpdateViewports();
                H_TaskUtilities::Switch();
            }
            
            _EndCompileSection();
        }
    } 
    void F_DirectX12RHIProcess::CleanCompile() 
    {
        ABYTEK_PROFILER_EVENT();
        CompileData = {};
        A_RHIProcess::CleanCompile(); 
    }

    void F_DirectX12RHIProcess::Execute()
    {
        ABYTEK_PROFILER_EVENT();
        
        A_RHIProcess::Execute();
        
        _InitCommandQueues();
        H_TaskUtilities::Switch();
        
        _InitRootSignatures();
        H_TaskUtilities::Switch();
        _InitPipelineStates();
        H_TaskUtilities::Switch();
        
        _InitViewports();
        H_TaskUtilities::Switch();
        _ResizeViewports();
        H_TaskUtilities::Switch();
        _AssignViewportFrames();
        H_TaskUtilities::Switch();
        
        _AssignD3D12ExternalResources();
        H_TaskUtilities::Switch();
        _CreateResources();
        H_TaskUtilities::Switch();
        
        _SendCommitToBindGroupProxies();
        H_TaskUtilities::Switch();
        _CopyDescriptorsForBindGroupProxies();
        H_TaskUtilities::Switch();
        
        _InitResourceViewDescriptors();
        H_TaskUtilities::Switch();
        _InitSamplerDescriptors();
        H_TaskUtilities::Switch();
        _CopyDescriptors();
        H_TaskUtilities::Switch();
        
        _TransientUploadBuffers();
        H_TaskUtilities::Switch();
        
        _ExecutePassBatches();
        H_TaskUtilities::Switch();
        _JoinPassBatches();
        H_TaskUtilities::Switch();
        
        _TransientReadbackBuffers();
        H_TaskUtilities::Switch();
        
        _CleanCommandListManagers();
    }
    void F_DirectX12RHIProcess::CleanExecute()
    {
        ABYTEK_PROFILER_EVENT();
        ExecutionData = {};
        A_RHIProcess::CleanExecute();
        _TransferDataToContextQueues();
    }
    void F_DirectX12RHIProcess::BeginLateExecute()
    {
        ABYTEK_PROFILER_EVENT();
        A_RHIProcess::BeginLateExecute();
        _BeginLateExecuteForViewportProxies();
    }
    void F_DirectX12RHIProcess::EndLateExecute()
    {
        ABYTEK_PROFILER_EVENT();
        _EndLateExecuteForViewportProxies();
        LateExecutionData = {};
        A_RHIProcess::EndLateExecute();
    }

    void F_DirectX12RHIProcess::_DeallocateDescriptors()
    {
        ABYTEK_PROFILER_EVENT();
        for (const auto& Context : GetContexts())
        {
            auto CastedContext = Context.FastCast<F_DirectX12RHIContext>();
            DirectX12RHIProcessQueries::Compile::F_DeallocateDescriptors Query;
            while (CastedContext->Queues.DeallocateDescriptors.TryPop(Query))
            {
                Query.DescriptorAllocation.Manager->Deallocate(Query.DescriptorAllocation);
            }
        }
    }
    void F_DirectX12RHIProcess::_DeallocateResourcePlacements()
    {
        ABYTEK_PROFILER_EVENT();
        for (const auto& Context : GetContexts())
        {
            auto CastedContext = Context.FastCast<F_DirectX12RHIContext>();
            F_DirectX12RHIResourcePlacement Query;
            while (CastedContext->Queues.ResourcePlacementsToDeallocate.TryPop(Query))
            {
                Query.Page->Deallocate(Query);
            }
        }
    }

    void F_DirectX12RHIProcess::_EarlyAnalyze()
    {
        ABYTEK_PROFILER_EVENT();
        const auto& RootSubmissionItems = GetRootSubmissionItems();
        auto CurrentSection_BeginRootSubmissionItemIndex = GetCurrentSection_BeginRootSubmissionItemIndex();
        auto CurrentSection_EndRootSubmissionItemIndex = GetCurrentSection_EndRootSubmissionItemIndex();
        
        for (
            auto RootSubmissionItemIndex = CurrentSection_BeginRootSubmissionItemIndex;
            RootSubmissionItemIndex < CurrentSection_EndRootSubmissionItemIndex;
            ++RootSubmissionItemIndex
        )
        {
            const auto& RootSubmissionItem = RootSubmissionItems[RootSubmissionItemIndex];
            _EarlyAnalyze(RootSubmissionItem);
        }
    }
    void F_DirectX12RHIProcess::_EarlyAnalyze(const TS<A_RHISubmissionItem>& SubmissionItem)
    {
        TW<F_DirectX12RHIViewportPresentation> ViewportPresentation;
        if (SubmissionItem.TryDynamicCast<F_DirectX12RHIViewportPresentation>(ViewportPresentation))
        {
            auto Viewport = ViewportPresentation->GetViewport().Weak();
            if (
                std::find(
                    CompileData.Viewports.begin(),
                    CompileData.Viewports.end(),
                    Viewport
                )
                == CompileData.Viewports.end()
            )
            {
                CompileData.Viewports.push_back(Viewport);
                CompileSectionData.Viewports.push_back(Viewport);
                ++CompileData.CurrentSection_EndViewportIndex;
            }
        }
        
        TW<A_RHISubmissionList> SubmissionList;
        if (SubmissionItem.TryDynamicCast<A_RHISubmissionList>(SubmissionList))
        {
            for (const auto& Child : *SubmissionList)
            {
                _EarlyAnalyze(Child);
            }
        }
    }

    void F_DirectX12RHIProcess::_UploadResources(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer
    )
    {
        ABYTEK_PROFILER_EVENT();
        {
            DirectX12RHIProcessQueries::Compile::F_UploadBuffer Query;
            while (Queues.Compile.UploadBuffer.TryPop(Query))
            {
                _UploadBuffer(SubmissionItemContainer, Query);
            }
        }
        {
            DirectX12RHIProcessQueries::Compile::F_UploadTexture Query;
            while (Queues.Compile.UploadTexture.TryPop(Query))
            {
                _UploadTexture(SubmissionItemContainer, Query);
            }
        }
        {
            DirectX12RHIProcessQueries::Compile::F_UploadRTAS Query;
            while (Queues.Compile.UploadRTAS.TryPop(Query))
            {
                _UploadRTAS(SubmissionItemContainer, Query);
            }
        }
    }
    void F_DirectX12RHIProcess::_UploadBuffer(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const DirectX12RHIProcessQueries::Compile::F_UploadBuffer& Query
    )
    {
        H_RHISubmissionUtilities::UploadBuffer(
            SubmissionItemContainer,
            Query.BufferDataView,
            Query.Resource,
            0,
#ifdef ABYTEK_DEBUG_INFO
            F_Name(ToText(*Query.Resource->GetDebugName()) + ABYTEK_TEXT(".UploadInitialBufferData"))
#else
            {}
#endif
        );
    }
    void F_DirectX12RHIProcess::_UploadTexture(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const DirectX12RHIProcessQueries::Compile::F_UploadTexture& Query
    )
    {
        H_RHISubmissionUtilities::UploadTexture(
            SubmissionItemContainer,
            Query.TextureDataView,
            Query.Resource,
#ifdef ABYTEK_DEBUG_INFO
            F_Name(ToText(*Query.Resource->GetDebugName()) + ABYTEK_TEXT(".UploadInitialTextureData"))
#else
            {}
#endif
        );
    }
    void F_DirectX12RHIProcess::_UploadRTAS(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const DirectX12RHIProcessQueries::Compile::F_UploadRTAS& Query
    )
    {
        ABYTEK_ENGINE_RHI_ASSERT(false) << "RTAS upload was not supported";
    }

    void F_DirectX12RHIProcess::_UploadConstantData(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer
    )
    {
        ABYTEK_PROFILER_EVENT();
        DirectX12RHIProcessQueries::Compile::F_UploadConstantData Query;
        while (Queues.Compile.UploadConstantData.TryPop(Query))
        {
            Query.ConstantDataRange.Upload(
                SubmissionItemContainer,
                Query.BufferDataView
            );
        }
    }

    void F_DirectX12RHIProcess::_ResourceStaticTransitions()
    {
        ABYTEK_PROFILER_EVENT();
        auto SubmissionList = RACreateAndBuildShared<A_RHISubmissionList>(ABYTEK_WTHIS());
#ifdef ABYTEK_DEBUG_INFO
        SubmissionList->SetDebugName(ABYTEK_DEBUG_NAME("Abytek::RHIResourceStaticTransitions"));   
#endif
        DirectX12RHIProcessQueries::Compile::F_ResourceStaticTransition Query;
        while (Queues.Compile.ResourceStaticTransition.TryPop(Query))
        {
            auto& AdditionalFlags = Query.Resource->InjectAdditionalFlags();
            if (FlagHas(AdditionalFlags, E_RHIResourceAdditionalFlag::STATIC_AFTER_SUBMIT))
            {
                auto Context = Query.Resource->GetContext();
                
                F_RHIResourceSyncPassBuildParams PassBuildParams;
                PassBuildParams.Context = Context;
                PassBuildParams.Resource = Query.Resource;
                PassBuildParams.Access = *Query.Resource->GetStaticAccess();
                auto Pass = RACreateAndBuildShared<A_RHIResourceSyncPass>(PassBuildParams);
#ifdef ABYTEK_DEBUG_INFO
                Pass->SetDebugName(Query.Resource->GetDebugName());       
#endif
                
                SubmissionList->Add(Pass);
                        
                EnqueuePostCompileCommand(
                    [Resource = Query.Resource]()
                    {
                        auto& AdditionalFlags = Resource->InjectAdditionalFlags();
                        AdditionalFlags = FlagRemove(AdditionalFlags, E_RHIResourceAdditionalFlag::STATIC_AFTER_SUBMIT);
                        AdditionalFlags |= E_RHIResourceAdditionalFlag::STATIC;
                    }
                );
            }
        }
        AddSubmissionItem(SubmissionList);
    }
    void F_DirectX12RHIProcess::_AddBackBufferTransitions()
    {
        ABYTEK_PROFILER_EVENT();
        auto SubmissionList = RACreateAndBuildShared<A_RHISubmissionList>(ABYTEK_WTHIS());
#ifdef ABYTEK_DEBUG_INFO
        SubmissionList->SetDebugName(ABYTEK_DEBUG_NAME("Abytek::RHIBackBufferTransitions"));   
#endif
        for (const auto& Viewport : CompileSectionData.Viewports)
        {
            F_RHIResourceSyncPassBuildParams PassBuildParams;
            PassBuildParams.Context = Viewport->GetContext();
            PassBuildParams.Resource = Viewport->GetCurrentBackFrame();
            PassBuildParams.Access = F_RHIResourceAccess::MakeNone();
            auto Pass = RACreateAndBuildShared<A_RHIResourceSyncPass>(PassBuildParams);
#ifdef ABYTEK_DEBUG_INFO
            Pass->SetDebugName(Viewport->GetDebugName());
#endif
            SubmissionList->Add(Pass);
        }
        AddSubmissionItem(SubmissionList);
    }

    void F_DirectX12RHIProcess::_MainAnalyze()
    {
        ABYTEK_PROFILER_EVENT();
        const auto& RootSubmissionItems = GetRootSubmissionItems();
        auto CurrentSection_BeginRootSubmissionItemIndex = GetCurrentSection_BeginRootSubmissionItemIndex();
        auto CurrentSection_EndRootSubmissionItemIndex = GetCurrentSection_EndRootSubmissionItemIndex();
        
        for (
            auto RootSubmissionItemIndex = CurrentSection_BeginRootSubmissionItemIndex;
            RootSubmissionItemIndex < CurrentSection_EndRootSubmissionItemIndex;
            ++RootSubmissionItemIndex
        )
        {
            const auto& RootSubmissionItem = RootSubmissionItems[RootSubmissionItemIndex];
            
            F_DirectX12RHISubmissionItemGraphData GraphData;
            _MainAnalyze(RootSubmissionItem, GraphData);
        }
    }
    void F_DirectX12RHIProcess::_MainAnalyze(const TS<A_RHISubmissionItem>& SubmissionItem, F_DirectX12RHISubmissionItemGraphData& GraphData)
    {
        TW<A_DirectX12RHISubmissionItemExtension> SubmissionItemExtension;
        if (SubmissionItem.TryDynamicCast<A_DirectX12RHISubmissionItemExtension>(SubmissionItemExtension))
        {
            CompileData.SubmissionItemExtensions.push_back(SubmissionItemExtension);
            SubmissionItemExtension->GetProcessData_SubmissionItemExtension()->GraphData = GraphData;
        }
        
        ++GraphData.Offset;
        
        TW<A_DirectX12RHISubmissionListExtension> SubmissionListExtension;
        if (SubmissionItem.TryDynamicCast<A_DirectX12RHISubmissionListExtension>(SubmissionListExtension))
        {
            CompileData.SubmissionListExtensions.push_back(SubmissionListExtension);
            ++CompileData.CurrentSection_EndSubmissionListExtensionIndex;
        }
        
        TW<A_DirectX12RHIPassExtension> PassExtension;
        if (SubmissionItem.TryDynamicCast<A_DirectX12RHIPassExtension>(PassExtension))
        {
            auto Pass = PassExtension.DynamicCast<A_RHIPass>();
            
            auto ProcessData_PassExtension = PassExtension->GetProcessData_PassExtension();
            
            ProcessData_PassExtension->GlobalIndex = CompileData.PassExtensions.size();
            CompileData.PassExtensions.push_back(PassExtension);
            ++CompileData.CurrentSection_EndPassExtensionIndex;
            
            ProcessData_PassExtension->IndexInSection = CompileSectionData.PassExtensions.size();
            CompileSectionData.PassExtensions.push_back(PassExtension);
            
            ProcessData_PassExtension->IndexInSection = CompileData.CurrentSectionIndex;
        }
        
        TW<A_RHISubmissionList> SubmissionList;
        if (SubmissionItem.TryDynamicCast<A_RHISubmissionList>(SubmissionList))
        {
            for (const auto& Child : *SubmissionList)
            {
                auto ChildGraphData = GraphData;
                ChildGraphData.ListExtensions.push_back(SubmissionListExtension);
                _MainAnalyze(Child, ChildGraphData);
                GraphData.Offset = ChildGraphData.Offset;
            }
        }
    }

    void F_DirectX12RHIProcess::_GatherSubresourceBindings()
    {
        ABYTEK_PROFILER_EVENT();
        for (const auto& PassExtension : CompileSectionData.PassExtensions)
        {
            PassExtension->GatherSubresourceBindings(
                PassExtension->GetProcessData_PassExtension()->SubresourceBindingSet
            );
        }
    }
    void F_DirectX12RHIProcess::_GatherSubresourceReferences()
    {
        ABYTEK_PROFILER_EVENT();
        auto& CurrentSection_SubresourceReferences = CompileSectionData.SubresourceReferences;
        
        auto& SubresourceReferences = CompileData.SubresourceReferences;
        auto& CurrentSection_EndSubresourceReferenceIndex = CompileData.CurrentSection_EndSubresourceReferenceIndex;
        
        for (const auto& PassExtension : CompileSectionData.PassExtensions)
        {
            auto ProcessData_PassExtension = PassExtension->GetProcessData_PassExtension();
            auto& SubresourceBindingSet = ProcessData_PassExtension->SubresourceBindingSet;
            for (auto& SubresourceBinding : SubresourceBindingSet)
            {
                auto SubresourceReference = SubresourceBinding.SubresourceReference;
                auto CompileData_Subresource = SubresourceReference->GetCompileData();
                
                if (CompileData_Subresource->GlobalIndex == ~U32(0))
                {
                    CompileData_Subresource->GlobalIndex = static_cast<U32>(SubresourceReferences.size());
                    SubresourceReferences.push_back(SubresourceReference);
                    ++CurrentSection_EndSubresourceReferenceIndex;
                }
                if (CompileData_Subresource->IndexInSection == ~U32(0))
                {
                    CompileData_Subresource->IndexInSection = static_cast<U32>(CurrentSection_SubresourceReferences.size());
                    CurrentSection_SubresourceReferences.push_back(SubresourceReference);
                }
            }
        }
    }
    void F_DirectX12RHIProcess::_GatherSubresourcePassTrackings()
    {
        ABYTEK_PROFILER_EVENT();
        for (const auto& PassExtension : CompileSectionData.PassExtensions)
        {
            auto ProcessData_PassExtension = PassExtension->GetProcessData_PassExtension();
            auto& SubresourceBindingSet = ProcessData_PassExtension->SubresourceBindingSet;
            
            U32 NumSubresourceBindings = static_cast<U32>(SubresourceBindingSet.size());
            
            for (U32 SubresourceBindingIndex = 0; SubresourceBindingIndex < NumSubresourceBindings; SubresourceBindingIndex++)
            {
                auto& SubresourceBinding = ProcessData_PassExtension->SubresourceBindingSet[SubresourceBindingIndex];
                auto CompileData_Subresource = SubresourceBinding.SubresourceReference->GetCompileData();
                auto& OutPassTrackings = CompileData_Subresource->PassTrackings;
                
                SubresourceBinding.PassTrackingReference = F_DirectX12RHISubresourcePassTrackingReference::Make(
                    SubresourceBinding.SubresourceReference,
                    static_cast<U32>(OutPassTrackings.size())
                );
                
                F_DirectX12RHISubresourcePassTracking PassTracking;
                static_cast<F_DirectX12RHISubresourceBindingReference&>(PassTracking) = F_DirectX12RHISubresourceBindingReference::Make(
                    PassExtension, 
                    SubresourceBindingIndex
                );
                OutPassTrackings.push_back(PassTracking);
                ++(CompileData_Subresource->CurrentSection_EndPassTrackingIndex);
            }
        }
    }

    void F_DirectX12RHIProcess::_GatherResourceUses()
    {
        ABYTEK_PROFILER_EVENT();
        for (const auto& PassExtension : CompileSectionData.PassExtensions)
        {
            auto ProcessData_PassExtension = PassExtension->GetProcessData_PassExtension();
            auto& ResourceUseSet = ProcessData_PassExtension->ResourceUseSet;
            auto& SubresourcBindingSet = ProcessData_PassExtension->SubresourceBindingSet;
            for (auto& SubresourceBinding : SubresourcBindingSet)
            {
                const auto& SubresourceReference = SubresourceBinding.SubresourceReference;
                auto CastedResource = SubresourceReference.Resource.FastCast<F_DirectX12RHIResource>();
                
                // Try add resource use
                {
                    B8 AddedResourceUse = false;
                    U32 NumResourceUses = static_cast<U32>(ResourceUseSet.size());
                    for (U32 ResourceUseIndex = 0; ResourceUseIndex < NumResourceUses; ++ResourceUseIndex)
                    {
                        const auto& ResourceUse = ResourceUseSet[ResourceUseIndex];
                        if (ResourceUse.Resource == CastedResource)
                        {
                            SubresourceBinding.ResourceUseReference = F_DirectX12RHIResourceUseReference::Make(PassExtension, ResourceUseIndex);
                            AddedResourceUse = true;
                            break;
                        }
                    }
                    if (!AddedResourceUse)
                    {
                        F_DirectX12RHIResourceUse ResourceUse = F_DirectX12RHIResourceUse::MakeCore(CastedResource);
                        SubresourceBinding.ResourceUseReference = F_DirectX12RHIResourceUseReference::Make(PassExtension, ResourceUseSet.size());
                        ResourceUseSet.push_back(ResourceUse);
                    }
                }
                
                // Combine access
                SubresourceBinding.ResourceUseReference->Access |= SubresourceBinding.Access;
                SubresourceBinding.ResourceUseReference->AccessCategories |= SubresourceBinding.AccessCategories;
            }
        }
    }
    void F_DirectX12RHIProcess::_GatherResources()
    {
        ABYTEK_PROFILER_EVENT();
        auto& CurrentSection_Resources = CompileSectionData.Resources;
        
        auto& Resources = CompileData.Resources;
        auto& CurrentSection_EndResourceIndex = CompileData.CurrentSection_EndResourceIndex;
        
        for (const auto& PassExtension : CompileSectionData.PassExtensions)
        {
            auto ProcessData_PassExtension = PassExtension->GetProcessData_PassExtension();
            auto& ResourceUseSet = ProcessData_PassExtension->ResourceUseSet;
            for (auto& ResourceUse : ResourceUseSet)
            {
                auto CastedResource = ResourceUse.Resource.FastCast<F_DirectX12RHIResource>();
                auto CompileData_Resource = CastedResource->GetCompileData();
                
                if (CompileData_Resource->GlobalIndex == ~U32(0))
                {
                    CompileData_Resource->GlobalIndex = static_cast<U32>(Resources.size());
                    Resources.push_back(ShareObject(CastedResource));
                    ++CurrentSection_EndResourceIndex;
                }
                
                if (CompileData_Resource->IndexInSection == ~U32(0))
                {
                    CompileData_Resource->IndexInSection = static_cast<U32>(CurrentSection_Resources.size());
                    CurrentSection_Resources.push_back(CastedResource);
                }
            }
        }
    }
    void F_DirectX12RHIProcess::_GatherResourcePassTrackings()
    {
        ABYTEK_PROFILER_EVENT();
        for (const auto& PassExtension : CompileSectionData.PassExtensions)
        {
            auto ProcessData_PassExtension = PassExtension->GetProcessData_PassExtension();
            auto& ResourceUseSet = ProcessData_PassExtension->ResourceUseSet;
            
            U32 NumResourceUses = static_cast<U32>(ResourceUseSet.size());
            
            for (U32 ResourceUseIndex = 0; ResourceUseIndex < NumResourceUses; ResourceUseIndex++)
            {
                auto& ResourceUse = ResourceUseSet[ResourceUseIndex];
                auto CompileData_Resource = ResourceUse.Resource.FastCast<F_DirectX12RHIResource>()->GetCompileData();
                auto& OutPassTrackings = CompileData_Resource->PassTrackings;
                
                ResourceUse.PassTrackingReference = F_DirectX12RHIResourcePassTrackingReference::Make(
                    ResourceUse.Resource,
                    static_cast<U32>(OutPassTrackings.size())
                );
                
                F_DirectX12RHIResourcePassTracking PassTracking;
                static_cast<F_DirectX12RHIResourceUseReference&>(PassTracking) = F_DirectX12RHIResourceUseReference::Make(
                    PassExtension, 
                    ResourceUseIndex
                );
                OutPassTrackings.push_back(PassTracking);
                ++(CompileData_Resource->CurrentSection_EndPassTrackingIndex);
            }
        }
    }

#ifdef ABYTEK_ENGINE_RHI_ENABLE_DRIVER_DEBUGGER
    void F_DirectX12RHIProcess::_ValidateSubresources()
    {
        ABYTEK_PROFILER_EVENT();
        
        const auto& SubmissionItemExtensions = CompileData.SubmissionItemExtensions;
        const auto& PassExtensions = CompileData.PassExtensions;
        const auto& SubmissionListExtensions = CompileData.SubmissionListExtensions;
        
        U32 NumSubmissionItemExtensions = static_cast<U32>(SubmissionItemExtensions.size());
        U32 NumPassExtensions = static_cast<U32>(PassExtensions.size());
        U32 NumSubmissionListExtensions = static_cast<U32>(SubmissionListExtensions.size());
        
        // Phase 1: gathering all the subresource validation data
        for (const auto& PassExtension : PassExtensions)
        {
            auto ProcessData_SubmissionItemExtension = PassExtension->GetProcessData_SubmissionItemExtension();
            auto ProcessData_PassExtension = PassExtension->GetProcessData_PassExtension();
            
            const auto& SubresourceBindingSet = ProcessData_PassExtension->SubresourceBindingSet;
            U32 NumSubresourceBindings = static_cast<U32>(SubresourceBindingSet.size());
            for (U32 SubresourceBindingIndex = 0; SubresourceBindingIndex < NumSubresourceBindings; ++SubresourceBindingIndex)
            {
                const auto& SubresourceBinding = SubresourceBindingSet[SubresourceBindingIndex];
                
                F_DirectX12RHISubmissionItemExtensionProcessData::F_SubresourceAccessValidationData ValidationData;
                ValidationData.BindingReference = F_DirectX12RHISubresourceBindingReference::Make(PassExtension, SubresourceBindingIndex);
            
                ProcessData_SubmissionItemExtension->SubresourceAccessValidationData.insert({
                    SubresourceBinding.SubresourceReference,  
                    ValidationData
                });
            }
        }
        
        // Phase 2: accumulating subresource access validation data for lists
        for (U32 SubmissionListIndex = NumSubmissionListExtensions - 1; SubmissionListIndex != ~U32(0); --SubmissionListIndex)
        {
            const auto& SubmissionListExtension = SubmissionListExtensions[SubmissionListIndex];
            
            auto ProcessData_SubmissionItemExtension = SubmissionListExtension->GetProcessData_SubmissionItemExtension();
            auto& SubresourceAccessValidationData = ProcessData_SubmissionItemExtension->SubresourceAccessValidationData;
            
            for (const auto& Child : *SubmissionListExtension->GetSubmissionList())
            {
                auto ProcessData_SubmissionItem_Child = Child.DynamicCast<A_DirectX12RHISubmissionItemExtension>()->GetProcessData_SubmissionItemExtension();
                auto& SubresourceAccessValidationData_Child = ProcessData_SubmissionItem_Child->SubresourceAccessValidationData;
                
                for (const auto& Pair : SubresourceAccessValidationData_Child)
                {
                    SubresourceAccessValidationData.insert(Pair);
                }
            }
        }
        
        // Phase 3: build subresource access categories
        for (const auto& SubmissionItemExtension : SubmissionItemExtensions)
        {
            auto ProcessData_SubmissionItemExtension = SubmissionItemExtension->GetProcessData_SubmissionItemExtension();
            auto& SubresourceAccessValidationData = ProcessData_SubmissionItemExtension->SubresourceAccessValidationData;
            auto& SubresourceAccessCategories = ProcessData_SubmissionItemExtension->SubresourceAccessCategories;
            
            for (auto It = SubresourceAccessValidationData.begin(); It != SubresourceAccessValidationData.end();)
            {
                auto Resource = It->first;
                
                E_RHIResourceAccessCategory Flags = E_RHIResourceAccessCategory::NONE;
                
                auto UpperBoundIt = SubresourceAccessValidationData.upper_bound(It->first);
                for (; It != UpperBoundIt; ++It)
                {
                    Flags |= RHIParseResourceAccessCategory(It->second.BindingReference->Access);
                }
                
                SubresourceAccessCategories[Resource] = Flags;
                
                It = UpperBoundIt;
            }
        }
        
        // Phase 4: final
        for (const auto& SubmissionListExtension : SubmissionListExtensions)
        {
            auto SubmissionList = SubmissionListExtension->GetSubmissionList();
            
            auto ProcessData_SubmissionItemExtension = SubmissionListExtension->GetProcessData_SubmissionItemExtension();
            const auto& SubresourceAccessCategories = ProcessData_SubmissionItemExtension->SubresourceAccessCategories;
            
            if (SubmissionList->GetOrder() != E_RHISubmissionListOrder::CONCURRENT)
            {
                continue;
            }
            
            for (const auto& Pair : SubresourceAccessCategories)
            {
                E_RHIResourceAccessCategory CombinedFlags = E_RHIResourceAccessCategory::NONE;
            
                for (const auto& Child : *SubmissionList)
                {
                    auto ProcessData_SubmissionItemExtension_Child = Child.DynamicCast<A_DirectX12RHISubmissionItemExtension>()->GetProcessData_SubmissionItemExtension();
                    const auto& SubresourceAccessCategories_Child = ProcessData_SubmissionItemExtension_Child->SubresourceAccessCategories;
                
                    auto It = SubresourceAccessCategories_Child.find(Pair.first);
                    if (It != SubresourceAccessCategories_Child.end())
                    {
                        if (CombinedFlags == E_RHIResourceAccessCategory::NONE)
                        {
                            CombinedFlags = It->second;
                        }
                        else
                        {
                            if (CombinedFlags != It->second)
                            {
                                F_Text ErrorMessage = ABYTEK_TEXT("Subresource access conflicts in concurrent submission list: ") + ToText(F_NewLine {});
                                ErrorMessage += ABYTEK_TEXT("\tSubresource: ") + ToText(F_NewLine {}) + Pair.first.GetInfoText(2) + ToText(F_NewLine {});
                                ErrorMessage += ABYTEK_TEXT("\tSubmission list: ") + ToText(*SubmissionList->GetDebugName()) + ToText(F_NewLine {});
                                ErrorMessage += ABYTEK_TEXT("\tConflict submission items: ") + ToText(F_NewLine {});
                                for (U32 Idx = 0; Idx < SubmissionList->GetSize(); ++Idx)
                                {
                                    const auto& ChildToCheckConflict = (*SubmissionList)[Idx];
                                    const auto& SubresourceAccessValidationDataToCheckConflict = ChildToCheckConflict.DynamicCast<A_DirectX12RHISubmissionItemExtension>()
                                        ->GetProcessData_SubmissionItemExtension()
                                        ->SubresourceAccessValidationData;
                                    
                                    auto ItToCheckConflict = SubresourceAccessValidationDataToCheckConflict.find(Pair.first);
                                    if (ItToCheckConflict != SubresourceAccessValidationDataToCheckConflict.end())
                                    {
                                        auto UpperBoundItToCheckConflict = SubresourceAccessValidationDataToCheckConflict.upper_bound(Pair.first);
                                        for (; ItToCheckConflict != UpperBoundItToCheckConflict; ++ItToCheckConflict)
                                        {
                                            auto& BindingReference = ItToCheckConflict->second.BindingReference;
                                            auto PassExtensionToCheckConflict = BindingReference.PassExtension;
                                            ErrorMessage += ABYTEK_TEXT("\t\t[") + ToText(Idx) + ABYTEK_TEXT("] -> ") + ToText(*PassExtensionToCheckConflict->GetSubmissionItem()->GetDebugName()) + ABYTEK_TEXT(":") + ToText(F_NewLine {});
                                            ErrorMessage += BindingReference->Access.GetInfoText(3);
                                            ErrorMessage += ToText(F_NewLine {});
                                        }
                                    }
                                }
                                ABYTEK_ENGINE_RHI_ASSERT(false) << ErrorMessage;
                            }
                        }
                    }
                }
            }
        }
    }
#endif

    void F_DirectX12RHIProcess::_AssignCommandQueueForPasses()
    {
        ABYTEK_PROFILER_EVENT();
        for (const auto& PassExtension : CompileSectionData.PassExtensions)
        {
            auto ProcessData_PassExtension = PassExtension->GetProcessData_PassExtension();
            
            auto Pass = PassExtension->GetPass();
            auto GPUWorkClass = Pass->GetGPUWorkClass();
            auto CastedContext = Pass->GetContext().FastCast<F_DirectX12RHIContext>();
            
            ABYTEK_ENGINE_RHI_ASSERT(GPUWorkClass != E_RHIGPUWorkClass::NONE);
            
            ProcessData_PassExtension->CommandQueue = CastedContext->QueryCommandQueue(
                DirectX12SharedAPIWrapper::Conversions::RHIGPUWorkClassToCommandListType(GPUWorkClass)
            );
        }
    }

    void F_DirectX12RHIProcess::_GatherSubresourceStateDependencies()
    {
        ABYTEK_PROFILER_EVENT();
        for (const auto& PassExtension : CompileSectionData.PassExtensions)
        {
            auto ProcessData_PassExtension = PassExtension->GetProcessData_PassExtension();
            auto& SubresourceBindingSet = ProcessData_PassExtension->SubresourceBindingSet;
            for (auto& SubresourceBinding : SubresourceBindingSet)
            {
                auto CompileData_Subresource = SubresourceBinding.SubresourceReference->GetCompileData();
                
                const auto& PassTrackingReference = SubresourceBinding.PassTrackingReference;
                auto& StateDependency = SubresourceBinding.StateDependency;
                for (
                    U32 PassTrackingIndex = PassTrackingReference.Index - 1;
                    PassTrackingIndex != ~U32(0);
                    --PassTrackingIndex
                )
                {
                    const auto& PassTracking = CompileData_Subresource->PassTrackings[PassTrackingIndex];
                    if (PassTracking.PassExtension->IsBeforeIfHasLowerOffset(PassExtension))
                    {
                        static_cast<F_DirectX12RHISubresourceBindingReference&>(StateDependency) = static_cast<const F_DirectX12RHISubresourceBindingReference&>(
                            PassTracking
                        );
                        break;
                    }
                }
            }
        }
    }
    void F_DirectX12RHIProcess::_CalculateSubresourceD3D12States()
    {
        ABYTEK_PROFILER_EVENT();
        
        // Initial states
        for (const auto& SubresourceReference : CompileSectionData.SubresourceReferences)
        {
            auto Resource = SubresourceReference.Resource;
            auto CompileData_Subresource = SubresourceReference->GetCompileData();
            auto& PassTrackings = CompileData_Subresource->PassTrackings;
            auto& D3D12States = CompileData_Subresource->D3D12States;
            
            U32 NumPassTrackings = static_cast<U32>(PassTrackings.size());
            auto CurrentSection_BeginPassTrackingIndex = CompileData_Subresource->CurrentSection_BeginPassTrackingIndex;
            auto CurrentSection_EndPassTrackingIndex = CompileData_Subresource->CurrentSection_EndPassTrackingIndex;
            
            // [prologue state, states from pass trackings]
            U32 NumD3D12States = 1 + NumPassTrackings;
            D3D12States.resize(NumD3D12States);
            
            D3D12States.front() = SubresourceReference->LastD3D12State;
            for (
                U32 PassTrackingIndex = CurrentSection_BeginPassTrackingIndex; 
                PassTrackingIndex < CurrentSection_EndPassTrackingIndex; 
                ++PassTrackingIndex
            )
            {
                const auto& PassTracking = PassTrackings[PassTrackingIndex];
                D3D12States[
                    F_DirectX12RHISubresourceCompileData::PassTrackingIndexToD3D12StateIndex(PassTrackingIndex)    
                ] = PassTracking->BaseD3D12State;
            }
        }
        
        // Merge states
        {
            const auto& PassExtensions = CompileSectionData.PassExtensions;
            U32 NumPassExtensions = static_cast<U32>(PassExtensions.size());
            for (U32 PassIndex = NumPassExtensions - 1; PassIndex != ~U32(0); --PassIndex)
            {
                const auto& PassExtension = PassExtensions[PassIndex];
                auto ProcessData_PassExtension = PassExtension->GetProcessData_PassExtension();
                auto& SubresourceBindingSet = ProcessData_PassExtension->SubresourceBindingSet;
                for (auto& SubresourceBinding : SubresourceBindingSet)
                {
                    const auto& SubresourceReference = SubresourceBinding.SubresourceReference;
                    const auto& PassTrackingReference = SubresourceBinding.PassTrackingReference;
                    const auto& StateDependency = SubresourceBinding.StateDependency;
                
                    auto CompileData_Subresource = SubresourceReference->GetCompileData();
                
                    auto& D3D12State = CompileData_Subresource->D3D12States[
                        F_DirectX12RHISubresourceCompileData::PassTrackingIndexToD3D12StateIndex(PassTrackingReference.Index)    
                    ];
                
                    if (StateDependency)
                    {
                        auto ProcessData_PassExtension_StateDependency = StateDependency.PassExtension->GetProcessData_PassExtension();
                        if (ProcessData_PassExtension_StateDependency->SectionIndex != ProcessData_PassExtension->SectionIndex)
                        {
                            continue;
                        }
                        
                        const auto& PassTrackingReference_StateDependency = StateDependency->PassTrackingReference;

                        auto& D3D12State_StateDependency = CompileData_Subresource->D3D12States[
                            F_DirectX12RHISubresourceCompileData::PassTrackingIndexToD3D12StateIndex(PassTrackingReference_StateDependency.Index)    
                        ];
                    
                        if (Internal::DirectX12RHIResource::CanCombineD3D12ResourceStates(D3D12State_StateDependency, D3D12State))
                        {
                            D3D12State_StateDependency = D3D12State_StateDependency | D3D12State;
                        }
                    }
                }
            }
        
            for (U32 PassIndex = 0; PassIndex < NumPassExtensions; ++PassIndex)
            {
                const auto& PassExtension = PassExtensions[PassIndex];
                auto ProcessData_PassExtension = PassExtension->GetProcessData_PassExtension();
                auto& SubresourceBindingSet = ProcessData_PassExtension->SubresourceBindingSet;
                for (auto& SubresourceBinding : SubresourceBindingSet)
                {
                    const auto& SubresourceReference = SubresourceBinding.SubresourceReference;
                    const auto& PassTrackingReference = SubresourceBinding.PassTrackingReference;
                    const auto& StateDependency = SubresourceBinding.StateDependency;
                
                    auto CompileData_Subresource = SubresourceReference->GetCompileData();
                
                    auto& D3D12State = CompileData_Subresource->D3D12States[
                        F_DirectX12RHISubresourceCompileData::PassTrackingIndexToD3D12StateIndex(PassTrackingReference.Index)    
                    ];
                
                    if (StateDependency)
                    {
                        auto ProcessData_PassExtension_StateDependency = StateDependency.PassExtension->GetProcessData_PassExtension();
                        if (ProcessData_PassExtension_StateDependency->SectionIndex != ProcessData_PassExtension->SectionIndex)
                        {
                            continue;
                        }
                        
                        const auto& PassTrackingReference_StateDependency = StateDependency->PassTrackingReference;

                        auto& D3D12State_StateDependency = CompileData_Subresource->D3D12States[
                            F_DirectX12RHISubresourceCompileData::PassTrackingIndexToD3D12StateIndex(PassTrackingReference_StateDependency.Index)    
                        ];
                    
                        if (Internal::DirectX12RHIResource::CanCombineD3D12ResourceStates(D3D12State, D3D12State_StateDependency))
                        {
                            D3D12State = D3D12State | D3D12State_StateDependency;
                        }
                    }
                }
            }
        }
    }
    void F_DirectX12RHIProcess::_GatherPrologueStateDependencies()
    {
        ABYTEK_PROFILER_EVENT();
        for (const auto& SubresourceReference : CompileSectionData.SubresourceReferences)
        {
            auto CompileData_Subresource = SubresourceReference->GetCompileData();
            auto& PassTrackings = CompileData_Subresource->PassTrackings;
            auto CurrentSection_BeginPassTrackingIndex = CompileData_Subresource->CurrentSection_BeginPassTrackingIndex;
            auto CurrentSection_EndPassTrackingIndex = CompileData_Subresource->CurrentSection_EndPassTrackingIndex;
            auto& D3D12States = CompileData_Subresource->D3D12States;
            
            auto PrologueD3D12State = D3D12States[CurrentSection_BeginPassTrackingIndex];
            
            const auto& ProloguePassTracking = PassTrackings[CurrentSection_BeginPassTrackingIndex];
            ProloguePassTracking->NeedPrologueStateTransitionBarrier = true;
            for (
                U32 PassTrackingIndex = CurrentSection_BeginPassTrackingIndex + 1; 
                PassTrackingIndex < CurrentSection_EndPassTrackingIndex; 
                ++PassTrackingIndex
            )
            {
                const auto& PassTracking = PassTrackings[PassTrackingIndex];
                if (
                    D3D12States[
                        F_DirectX12RHISubresourceCompileData::PassTrackingIndexToD3D12StateIndex(PassTrackingIndex)
                    ] 
                    != PrologueD3D12State
                )
                {
                    break;
                }
                
                static_cast<F_DirectX12RHISubresourceBindingReference&>(
                    PassTracking->PrologueStateDependency
                )= static_cast<const F_DirectX12RHISubresourceBindingReference&>(ProloguePassTracking);
            }
        }
    }
    void F_DirectX12RHIProcess::_GatherReverseStateJoinDependencies()
    {
        ABYTEK_PROFILER_EVENT();
        for (const auto& SubresourceReference : CompileSectionData.SubresourceReferences)
        {
            auto CompileData_Subresource = SubresourceReference->GetCompileData();
            auto& PassTrackings = CompileData_Subresource->PassTrackings;
            auto& D3D12States = CompileData_Subresource->D3D12States;
            
            auto CurrentSection_BeginPassTrackingIndex = CompileData_Subresource->CurrentSection_BeginPassTrackingIndex;
            auto CurrentSection_EndPassTrackingIndex = CompileData_Subresource->CurrentSection_EndPassTrackingIndex;
            
            for (U32 BeginPassTrackingIndex = CurrentSection_BeginPassTrackingIndex; BeginPassTrackingIndex < CurrentSection_EndPassTrackingIndex; )
            {
                auto BeginD3D12State = D3D12States[
                    F_DirectX12RHISubresourceCompileData::PassTrackingIndexToD3D12StateIndex(BeginPassTrackingIndex)
                ];
                
                U32 EndPassTrackingIndex = BeginPassTrackingIndex + 1;
                for (; EndPassTrackingIndex < CurrentSection_EndPassTrackingIndex; ++EndPassTrackingIndex)
                {
                    auto EndD3D12State = D3D12States[
                        F_DirectX12RHISubresourceCompileData::PassTrackingIndexToD3D12StateIndex(EndPassTrackingIndex)
                    ];
                    if (EndD3D12State != BeginD3D12State)
                    {
                        break;
                    }
                }
                
                for (U32 PassTrackingIndex = BeginPassTrackingIndex; (PassTrackingIndex + 1) < EndPassTrackingIndex; ++PassTrackingIndex)
                {
                    auto& PassTracking = PassTrackings[PassTrackingIndex];
                    static_cast<F_DirectX12RHISubresourcePassTrackingReference&>(PassTracking.ReverseStateJoinDependency) = F_DirectX12RHISubresourcePassTrackingReference::Make(
                        SubresourceReference,
                        EndPassTrackingIndex - 1
                    );
                }
                
                PassTrackings[EndPassTrackingIndex - 1]->IsStateJoinPoint = true;
                    
                BeginPassTrackingIndex = EndPassTrackingIndex;
            }
        }
    }
    void F_DirectX12RHIProcess::_GatherWriteDependencies()
    {
        ABYTEK_PROFILER_EVENT();
        for (const auto& PassExtension : CompileSectionData.PassExtensions)
        {
            auto ProcessData_PassExtension = PassExtension->GetProcessData_PassExtension();
            auto& SubresourceBindingSet = ProcessData_PassExtension->SubresourceBindingSet;
            for (auto& SubresourceBinding : SubresourceBindingSet)
            {
                auto CompileData_Subresource = SubresourceBinding.SubresourceReference->GetCompileData();
                auto& WriteDependencies = SubresourceBinding.WriteDependencies;
                const auto& PassTrackingReference_Subresource = SubresourceBinding.PassTrackingReference;
                const auto& D3D12States = CompileData_Subresource->D3D12States;
                auto& PassTrackings = CompileData_Subresource->PassTrackings;
                
                U32 EndPassTrackingIndex_Subresource = CompileData_Subresource->CurrentSection_BeginPassTrackingIndex - U32(1);
                
                TF_Optional<D3D12_RESOURCE_STATES> LastD3D12State;
                
                for (
                    U32 PassTrackingIndex_Subresource = PassTrackingReference_Subresource.Index - 1;
                    PassTrackingIndex_Subresource != EndPassTrackingIndex_Subresource;
                    --PassTrackingIndex_Subresource
                )
                {
                    const auto& PassTracking_Subresource = PassTrackings[PassTrackingIndex_Subresource];
                    
                    // Early stop
                    auto D3D12State = D3D12States[
                        F_DirectX12RHISubresourceCompileData::PassTrackingIndexToD3D12StateIndex(PassTrackingIndex_Subresource)    
                    ];
                    if (LastD3D12State)
                    {
                        if (LastD3D12State != D3D12State)
                        {
                            break;
                        }
                    }
                    LastD3D12State = D3D12State;
                    
                    // Skip read-read but not read-write, write-read and write-write
                    if (
                        (PassTracking_Subresource->AccessCategories == E_RHIResourceAccessCategory::READ)
                        && (SubresourceBinding.AccessCategories == E_RHIResourceAccessCategory::READ)
                    )
                    {
                        continue;
                    }
                    
                    // Check if actual dependency
                    if (!(PassTracking_Subresource.PassExtension->IsBeforeIfHasLowerOffset(PassExtension))) 
                    {
                        continue;
                    } 
                    
                    F_DirectX12RHISubresourceWriteDependency WriteDependency;
                    static_cast<F_DirectX12RHISubresourceBindingReference&>(WriteDependency) = static_cast<const F_DirectX12RHISubresourceBindingReference&>(
                        PassTracking_Subresource
                    );
                    WriteDependencies.push_back(WriteDependency);
                    
                    // Note that write dependencies always happen on state-joint points!
                    // => Just stop here
                    break;
                }
            }
        } 
    }
    void F_DirectX12RHIProcess::_ResolvePassDependencies_WithoutAliasing()
    {
        ABYTEK_PROFILER_EVENT();
        for (const auto& PassExtension : CompileSectionData.PassExtensions)
        {
            auto ProcessData_PassExtension = PassExtension->GetProcessData_PassExtension();
            auto& SubresourceBindingSet = ProcessData_PassExtension->SubresourceBindingSet;
            
            // Write dependencies
            for (auto& SubresourceBinding : SubresourceBindingSet)
            {
                for (const auto& WriteDependency : SubresourceBinding.WriteDependencies)
                {
                    ABYTEK_ENGINE_RHI_ASSERT(WriteDependency.PassExtension != PassExtension);
                    if (
                        std::find(
                            ProcessData_PassExtension->Dependencies.begin(),
                            ProcessData_PassExtension->Dependencies.end(),
                            WriteDependency.PassExtension
                        )   
                        == ProcessData_PassExtension->Dependencies.end()
                    )
                    {
                        ProcessData_PassExtension->Dependencies.push_back(WriteDependency.PassExtension);
                    }
                }
            }
            
            // Reverse state join dependencies
            for (auto& SubresourceBinding : SubresourceBindingSet)
            {
                auto& PassTrackingReference = SubresourceBinding.PassTrackingReference;
                auto& ReverseStateJoinDependency = PassTrackingReference->ReverseStateJoinDependency;
                if (ReverseStateJoinDependency)
                {
                    auto StateJoinPassExtension = ReverseStateJoinDependency->PassExtension;
                    auto ProcessData_PassExtension_StateJoinPassExtension = StateJoinPassExtension->GetProcessData_PassExtension();
                    
                    ABYTEK_ENGINE_RHI_ASSERT(StateJoinPassExtension != PassExtension);
                    if (
                        std::find(
                            ProcessData_PassExtension_StateJoinPassExtension->Dependencies.begin(),
                            ProcessData_PassExtension_StateJoinPassExtension->Dependencies.end(),
                            PassExtension
                        )   
                        == ProcessData_PassExtension_StateJoinPassExtension->Dependencies.end()
                    )
                    {
                        ProcessData_PassExtension_StateJoinPassExtension->Dependencies.push_back(PassExtension);
                    }
                }
            }
            
            // State transition dependencies
            for (auto& SubresourceBinding : SubresourceBindingSet)
            {
                auto& StateDependency = SubresourceBinding.StateDependency;
                if (!StateDependency)
                {
                    continue;
                }
                
                auto PassTrackingReference_StateDependency = StateDependency->PassTrackingReference;
                const auto& ReverseStateJoinDependency_StateDependency = PassTrackingReference_StateDependency->ReverseStateJoinDependency;
                
                TW<A_DirectX12RHIPassExtension> StateJoinPassExtension;
                
                // If state dependency has no reverse-state-join dependency => it should be a state joint point.
                if (ReverseStateJoinDependency_StateDependency)
                {
                    continue;
                }
                ABYTEK_ENGINE_RHI_ASSERT(StateDependency->IsStateJoinPoint);
                StateJoinPassExtension = StateDependency.PassExtension;
                
                // Skip if has not state join pass proxy
                if (!StateJoinPassExtension)
                {
                    continue;
                }
                
                // Add dependency
                ABYTEK_ENGINE_RHI_ASSERT(StateJoinPassExtension != PassExtension);
                if (
                    std::find(
                        ProcessData_PassExtension->Dependencies.begin(),
                        ProcessData_PassExtension->Dependencies.end(),
                        StateJoinPassExtension
                    )   
                    == ProcessData_PassExtension->Dependencies.end()
                )
                {
                    ProcessData_PassExtension->Dependencies.push_back(StateJoinPassExtension);
                }
            }
            
            // Prologue state dependencies (for first resource state transition)
            for (auto& SubresourceBinding : SubresourceBindingSet)
            {
                if (auto& PrologueStateDependency = SubresourceBinding.PrologueStateDependency)
                {
                    auto PrologueStatePassExtension = PrologueStateDependency.PassExtension;
                    
                    ABYTEK_ENGINE_RHI_ASSERT(PrologueStatePassExtension != PassExtension);
                    if (
                        std::find(
                            ProcessData_PassExtension->Dependencies.begin(),
                            ProcessData_PassExtension->Dependencies.end(),
                            PrologueStatePassExtension
                        )   
                        == ProcessData_PassExtension->Dependencies.end()
                    )
                    {
                        ProcessData_PassExtension->Dependencies.push_back(PrologueStatePassExtension);
                    }
                }
            }
            
            // Cross-section prologue state dependencies
            for (auto& SubresourceBinding : SubresourceBindingSet)
            {
                if (SubresourceBinding.NeedPrologueStateTransitionBarrier)
                {
                    if (SubresourceBinding.PassTrackingReference.Index == 0)
                    {
                        continue;    
                    }
                    
                    auto CompileData_Subresource = SubresourceBinding.SubresourceReference->GetCompileData();
                    
                    auto PassTrackinIndex_LastSection = SubresourceBinding.PassTrackingReference.Index - 1;
                    auto& PassTracking_LastSection = CompileData_Subresource->PassTrackings[PassTrackinIndex_LastSection];
                    auto PassExtension_LastSection = PassTracking_LastSection.PassExtension;
                    
                    ABYTEK_ENGINE_RHI_ASSERT(PassExtension_LastSection != PassExtension);
                    if (
                        std::find(
                            ProcessData_PassExtension->Dependencies.begin(),
                            ProcessData_PassExtension->Dependencies.end(),
                            PassExtension_LastSection
                        )   
                        == ProcessData_PassExtension->Dependencies.end()
                    )
                    {
                        ProcessData_PassExtension->Dependencies.push_back(PassExtension_LastSection);
                    }
                }
            }
        }
    }
    void F_DirectX12RHIProcess::_ResolvePassDependencyLevel()
    {
        ABYTEK_PROFILER_EVENT();
        
        for (const auto& PassExtension : CompileSectionData.PassExtensions)
        {
            auto ProcessData_PassExtension = PassExtension->GetProcessData_PassExtension();
            ProcessData_PassExtension->DependencyLevel = 0;
        }
        
        for (const auto& PassExtension : CompileSectionData.PassExtensions)
        {
            auto ProcessData_PassExtension = PassExtension->GetProcessData_PassExtension();
            auto& Dependencies = ProcessData_PassExtension->Dependencies;
            
            for (auto& Dependency : Dependencies)
            {
                auto ProcessData_PassExtension_Dependency = Dependency->GetProcessData_PassExtension();
                
                ProcessData_PassExtension->DependencyLevel = Max(
                    ProcessData_PassExtension->DependencyLevel,
                    ProcessData_PassExtension_Dependency->DependencyLevel + 1
                );
            }
        }
    }
    void F_DirectX12RHIProcess::_PrepareForAutoPlacedResources()
    {
        ABYTEK_PROFILER_EVENT();
        
        auto& AutoPlacedResourcesToAllocate = CompileSectionData.AutoPlacedResourcesToAllocate;
        auto& AutoPlacedResourcesToDeallocate = CompileSectionData.AutoPlacedResourcesToDeallocate;
        auto& AutoPlacedResources = CompileSectionData.AutoPlacedResources;
            
        auto& LevelsOfResourceAllocations = CompileSectionData.LevelsOfResourceAllocations;
        auto& LevelsOfResourceDeallocations = CompileSectionData.LevelsOfResourceDeallocations;
        
        auto& SortedAutoPlacedResourcesToDeallocate = CompileData.SortedAutoPlacedResourcesToDeallocate;
        
        // Pop queues
        {
            TS<A_RHIResource> Resource;
            while (Queues.Compile.AutoPlacedResourcesToAllocate.TryPop(Resource))
            {
                auto CompileData_Resource = Resource.FastCast<F_DirectX12RHIResource>()->GetCompileData();
                auto& AutoPlacedData = CompileData_Resource->AutoPlacedData;
                AutoPlacedData.NeedAllocate = true;
                
                AutoPlacedResourcesToAllocate.push_back(Resource);
                if (AutoPlacedData.IndexInSection == ~U32(0))
                {
                    AutoPlacedData.IndexInSection = static_cast<U32>(AutoPlacedResources.size());
                    AutoPlacedResources.push_back(Resource.Weak());
                }
            }
        }
        {
            TS<A_RHIResource> Resource;
            while (Queues.Compile.AutoPlacedResourcesToDeallocate.TryPop(Resource))
            {
                auto CompileData_Resource = Resource.FastCast<F_DirectX12RHIResource>()->GetCompileData();
                auto& AutoPlacedData = CompileData_Resource->AutoPlacedData;
                AutoPlacedData.NeedAllocate = false;
                
                AutoPlacedResourcesToDeallocate.push_back(Resource);
                if (AutoPlacedData.IndexInSection == ~U32(0))
                {
                    AutoPlacedData.IndexInSection = static_cast<U32>(AutoPlacedResources.size());
                    AutoPlacedResources.push_back(Resource.Weak());
                }
            }
        }
        
        // Calculate min/max dependency level
        for (const auto& Resource : AutoPlacedResources)
        {
            auto CompileData_Resource = Resource.FastCast<F_DirectX12RHIResource>()->GetCompileData();
            const auto& PassTrackings = CompileData_Resource->PassTrackings;
            auto& AutoPlacedData = CompileData_Resource->AutoPlacedData;
        
            for (const auto& PassTracking : PassTrackings)
            {
                auto ProcessData_PassExtension = PassTracking.PassExtension->GetProcessData_PassExtension();
                auto DependencyLevel = ProcessData_PassExtension->DependencyLevel;
            
                if (AutoPlacedData.MinPassDependencyLevel == ~U32(0))
                {
                    AutoPlacedData.MinPassDependencyLevel = DependencyLevel;
                }
                else
                {
                    AutoPlacedData.MinPassDependencyLevel = Min<U32>(AutoPlacedData.MinPassDependencyLevel, DependencyLevel);
                }
            
                if (AutoPlacedData.MaxPassDependencyLevel == ~U32(0))
                {
                    AutoPlacedData.MaxPassDependencyLevel = DependencyLevel;
                }
                else
                {
                    AutoPlacedData.MaxPassDependencyLevel = Max<U32>(AutoPlacedData.MinPassDependencyLevel, DependencyLevel);
                }
            }
        }

        // Push to levels of allocations/deallocations
        for (const auto& Resource : AutoPlacedResources)
        {
            auto CompileData_Resource = Resource.FastCast<F_DirectX12RHIResource>()->GetCompileData();
            auto& AutoPlacedData = CompileData_Resource->AutoPlacedData;
            
            auto CastedContext = Resource->GetContext().FastCast<F_DirectX12RHIContext>();

            auto AllocationLevel = AutoPlacedData.MinPassDependencyLevel;
            auto DeallocationLevel = AutoPlacedData.MaxPassDependencyLevel;
            
            // Culling
            if (
                AutoPlacedData.NeedAllocate
                && AutoPlacedData.NeedDeallocate
                && (DeallocationLevel == ~U32(0))
                
            )
            {
                continue;
            }
            
            // Just want to allocate, no uses
            if (AutoPlacedData.NeedAllocate && (AllocationLevel == ~U32(0)))
            {
                AllocationLevel = 0;
            }
            
            // Just want to deallocate, no uses
            if (AutoPlacedData.NeedDeallocate && (DeallocationLevel == ~U32(0)))
            {
                DeallocationLevel = 0;
            }
            
            if (AutoPlacedData.NeedAllocate)
            {
                if (AllocationLevel >= LevelsOfResourceAllocations.size())
                {
                    LevelsOfResourceAllocations.resize(AllocationLevel + 1);
                }
                LevelsOfResourceAllocations[AllocationLevel].push_back(Resource);
                
                AutoPlacedData.AllocationLevel = AllocationLevel;
            }
            if (AutoPlacedData.NeedDeallocate)
            {
                if (DeallocationLevel >= LevelsOfResourceDeallocations.size())
                {
                    LevelsOfResourceDeallocations.resize(DeallocationLevel + 1);
                }
                LevelsOfResourceDeallocations[DeallocationLevel].push_back(Resource);
                
                AutoPlacedData.DeallocationLevel = AllocationLevel;
            }
        }
        
        // Allocate + deallocate
        U32 MaxNumLevels = Max<U32>(
            static_cast<U32>(LevelsOfResourceAllocations.size()) + 1,    
            static_cast<U32>(LevelsOfResourceDeallocations.size()) + 1    
        );
        for (U32 LevelIndex = 0; LevelIndex < MaxNumLevels; ++LevelIndex)
        {
            // Allocate
            if (LevelIndex < LevelsOfResourceAllocations.size())
            {
                auto& ResourcesToAllocate = LevelsOfResourceAllocations[LevelIndex];
                for (const auto& Resource : ResourcesToAllocate)
                {
                    auto CastedResource = Resource.FastCast<F_DirectX12RHIResource>();
                    auto CastedContext = Resource->GetContext().FastCast<F_DirectX12RHIContext>();
                    auto& ResourcePlacement = CastedResource->InjectPlacement();
                    auto CompileData_Resource = CastedResource->GetCompileData();
                    
                    auto& AutoPlacedProcessData = CompileData_Resource->AutoPlacedData;
                    
                    const auto& D3D12ResourceAllocationInfo = CastedResource->GetD3D12ResourceAllocationInfo();
                    
                    auto PlacedResourceManager = CastedContext->QueryPlacedResourceManager(Resource);
                    ResourcePlacement = PlacedResourceManager->Allocate(
                        D3D12ResourceAllocationInfo.SizeInBytes,
                        D3D12ResourceAllocationInfo.Alignment
                    );
                    AutoPlacedProcessData.Placement = ResourcePlacement;
                    AutoPlacedProcessData.EndDeallocationIndexToCheck = static_cast<U32>(SortedAutoPlacedResourcesToDeallocate.size());
                    
                    ABYTEK_ENGINE_RHI_ASSERT(ResourcePlacement);
                    
                    // enqueue init placed resources
                    CastedResource->D3D12CreatePlaced();
                }
            }
            
            // Deallocate
            if (LevelIndex < LevelsOfResourceDeallocations.size())
            {
                auto& ResourcesToDeallocate = LevelsOfResourceDeallocations[LevelIndex];
                for (const auto& Resource : ResourcesToDeallocate)
                {
                    auto CastedResource = Resource.FastCast<F_DirectX12RHIResource>();
                    auto& ResourcePlacement = CastedResource->InjectPlacement();
                    
                    ABYTEK_ENGINE_RHI_ASSERT(ResourcePlacement);
                    
                    SortedAutoPlacedResourcesToDeallocate.push_back(Resource);
                    
                    ResourcePlacement.Page->Deallocate(ResourcePlacement);
                    ResourcePlacement = {};
                }
            }
        }
    }
    void F_DirectX12RHIProcess::_GatherAliasingDependencies()
    {
        ABYTEK_PROFILER_EVENT();
        const auto& AutoPlacedResourcesToAllocate = CompileSectionData.AutoPlacedResourcesToAllocate;
        const auto& SortedAutoPlacedResourcesToDeallocate = CompileData.SortedAutoPlacedResourcesToDeallocate;
        
        for (const auto& Resource_Allocated : AutoPlacedResourcesToAllocate)
        {
            auto CastedResource_Allocated = Resource_Allocated.FastCast<F_DirectX12RHIResource>();
            auto CompileData_Resource_Allocated = CastedResource_Allocated->GetCompileData();
            auto& AutoPlacedProcessData_Resource_Allocated = CompileData_Resource_Allocated->AutoPlacedData;
            auto& ResourcePlacement_Allocated = AutoPlacedProcessData_Resource_Allocated.Placement;
            auto& PassTrackings_Allocated = CompileData_Resource_Allocated->PassTrackings;
            
            if (PassTrackings_Allocated.size() == 0)
            {
                continue;
            }
            
            F_DirectX12RHIResourceAliasingDependency AliasingDependency;
            
            for (
                U32 DeallocationIdx = CompileData_Resource_Allocated->AutoPlacedData.EndDeallocationIndexToCheck - 1; 
                DeallocationIdx != ~U32(0);
                --DeallocationIdx
            )
            {
                auto CastedResource_Deallocated = SortedAutoPlacedResourcesToDeallocate[
                    DeallocationIdx
                ].FastCast<F_DirectX12RHIResource>();
                auto CompileData_Resource_Deallocated = CastedResource_Deallocated->GetCompileData();
                auto& AutoPlacedProcessData_Resource_Deallocated = CompileData_Resource_Deallocated->AutoPlacedData;
                auto& ResourcePlacement_Deallocated = AutoPlacedProcessData_Resource_Deallocated.Placement;
                auto& PassTrackings_Deallocated = CompileData_Resource_Deallocated->PassTrackings;
                
                if (ResourcePlacement_Allocated.IsOverlap(ResourcePlacement_Deallocated))
                {
                    U32 NumPassTrackings_Deallocated = static_cast<U32>(PassTrackings_Deallocated.size());
                    if (NumPassTrackings_Deallocated > 0)
                    {
                        auto& PassTracking_Deallocated = PassTrackings_Deallocated.back();
                        static_cast<F_DirectX12RHIResourceUseReference&>(AliasingDependency) = PassTracking_Deallocated;
                    }
                    break;
                }
            }
            
            if (AliasingDependency)
            {
                auto& FirstPassTracking = PassTrackings_Allocated.front();
                auto& ResourceUse = *FirstPassTracking;
                auto ProcessData_PassExtension = FirstPassTracking.PassExtension->GetProcessData_PassExtension();
                
                ResourceUse.AliasingDependency = AliasingDependency;
                
                if (
                    std::find(
                        ProcessData_PassExtension->Dependencies.begin(),
                        ProcessData_PassExtension->Dependencies.end(),
                        AliasingDependency.PassExtension
                    )   
                    == ProcessData_PassExtension->Dependencies.end()
                )
                {
                    ProcessData_PassExtension->Dependencies.push_back(AliasingDependency.PassExtension);
                }
            }
        }
    }
    void F_DirectX12RHIProcess::_ResolvePassDependencyScore()
    {
        ABYTEK_PROFILER_EVENT();
        const auto& PassExtensions = CompileSectionData.PassExtensions;
        U32 NumPassExtensions = static_cast<U32>(PassExtensions.size());
        U64 NumPassExtensions_U64 = NumPassExtensions;
        U64 HeuristicFactor = Min<U64>(NumPassExtensions_U64, ABYTEK_U16_MAX);
        
        for (U32 PassExtensionIndex = 0; PassExtensionIndex < NumPassExtensions; ++PassExtensionIndex)
        {
            const auto& PassExtension = PassExtensions[PassExtensionIndex];
            auto ProcessData_PassExtension = PassExtension->GetProcessData_PassExtension();
            ProcessData_PassExtension->DependencyScore = 0;
        }
        
        for (U32 PassExtensionIndex = 0; PassExtensionIndex < NumPassExtensions; ++PassExtensionIndex)
        {
            const auto& PassExtension = PassExtensions[PassExtensionIndex];
            auto ProcessData_PassExtension = PassExtension->GetProcessData_PassExtension();
            auto& Dependencies = ProcessData_PassExtension->Dependencies;
            
            auto CommandQueue = ProcessData_PassExtension->CommandQueue;
            auto PassBatchType = PassExtension->GetPassBatchType();
            auto ExecutionRangeType = PassExtension->GetExecutionRangeType();
            
            U64 BaseDependencyScore = 0;
            switch (PassBatchType)
            {
            case E_DirectX12RHIPassBatchType::GPU:
                BaseDependencyScore += 0;
                break;
            case E_DirectX12RHIPassBatchType::CPU_SYNC:
                BaseDependencyScore += 1 * HeuristicFactor;
                break;
            default:
                ABYTEK_LOG_FATAL() << "Unknown pass batch type";
                break;
            }
            switch (CommandQueue->GetCommandListType())
            {
            case DirectX12SharedAPIWrapper::E_CommandListType::DIRECT:
                BaseDependencyScore += 0;
                break;
            case DirectX12SharedAPIWrapper::E_CommandListType::COMPUTE:
                BaseDependencyScore += 1 * HeuristicFactor * HeuristicFactor;
                break;
            case DirectX12SharedAPIWrapper::E_CommandListType::COPY:
                BaseDependencyScore += 2 * HeuristicFactor * HeuristicFactor;
                break;
            default:
                ABYTEK_LOG_FATAL() << "Unknown command list type";
                break;
            }
            switch (ExecutionRangeType)
            {
            case E_DirectX12RHIExecutionRangeType::USE_COMMAND_LIST:
                BaseDependencyScore += 0;
                break;
            case E_DirectX12RHIExecutionRangeType::CPU_ACCESS:
                BaseDependencyScore += 1 * HeuristicFactor * HeuristicFactor * HeuristicFactor;
                break;
            case E_DirectX12RHIExecutionRangeType::USE_COMMAND_QUEUE:
                BaseDependencyScore += 2 * HeuristicFactor * HeuristicFactor * HeuristicFactor;
                break;
            default:
                ABYTEK_LOG_FATAL() << "Unknown execution range type";
                break;
            }
             
            ProcessData_PassExtension->DependencyScore = BaseDependencyScore;
            
            for (auto& Dependency : Dependencies)
            {
                auto ProcessData_PassExtension_Dependency = Dependency->GetProcessData_PassExtension();
                
                auto CommandQueue_Dependency = ProcessData_PassExtension_Dependency->CommandQueue;
                auto PassBatchType_Dependency = Dependency->GetPassBatchType();
                auto ExecutionRangeType_Dependency = Dependency->GetExecutionRangeType();
                
                U64 DeltaScore = 1;
                if (PassBatchType != PassBatchType_Dependency)
                {
                    DeltaScore += HeuristicFactor * HeuristicFactor * HeuristicFactor;
                }
                if (CommandQueue != CommandQueue_Dependency)
                {
                    DeltaScore += HeuristicFactor * HeuristicFactor;
                }
                if (ExecutionRangeType != ExecutionRangeType_Dependency)
                {
                    DeltaScore += HeuristicFactor;
                }
                
                ProcessData_PassExtension->DependencyScore = Max(
                    ProcessData_PassExtension->DependencyScore,
                    ProcessData_PassExtension_Dependency->DependencyScore + DeltaScore
                );
            }
        }
    }
    void F_DirectX12RHIProcess::_SortPassesByDependencyScore()
    {
        ABYTEK_PROFILER_EVENT();
        const auto& PassExtensions = CompileSectionData.PassExtensions;
        auto& PassExtensions_SortedByDependencyScore = CompileSectionData.PassExtensions_SortedByDependencyScore;
        
        PassExtensions_SortedByDependencyScore = PassExtensions;
        boost::sort(
            PassExtensions_SortedByDependencyScore,
            [](const TW<A_DirectX12RHIPassExtension>& A, const TW<A_DirectX12RHIPassExtension>& B)
            {
                const auto& ProcessData_PassExtension_A = A->GetProcessData_PassExtension();
                const auto& ProcessData_PassExtension_B = B->GetProcessData_PassExtension();
                
                auto PassBatchType_A = A->GetPassBatchType();
                auto PassBatchType_B = B->GetPassBatchType();
                
                // Main compare
                if (ProcessData_PassExtension_A->DependencyScore != ProcessData_PassExtension_B->DependencyScore)
                {
                    return (ProcessData_PassExtension_A->DependencyScore < ProcessData_PassExtension_B->DependencyScore);
                }
                
                // Command queue splits optimization
                if (ProcessData_PassExtension_A->CommandQueue != ProcessData_PassExtension_B->CommandQueue)
                {
                    return ProcessData_PassExtension_A->CommandQueue < ProcessData_PassExtension_B->CommandQueue;
                }
                
                // Pass batch type splits optimization
                if (PassBatchType_A != PassBatchType_B)
                {
                    return PassBatchType_A < PassBatchType_B;
                }
                
                return A.GetObjectRawP() < B.GetObjectRawP();
            }
        );
        
        U32 NumPassExtensions = static_cast<U32>(PassExtensions.size());
        for (U32 PassExtensionIndex = 0; PassExtensionIndex < NumPassExtensions; ++PassExtensionIndex)
        {
            PassExtensions_SortedByDependencyScore[PassExtensionIndex]->GetProcessData_PassExtension()->SortedIndexInSection = PassExtensionIndex;
        }
    }

    void F_DirectX12RHIProcess::_CreatePassBatches()
    {
        ABYTEK_PROFILER_EVENT();
        auto& PassBatches = CompileData.PassBatches;
        auto& CurrentSection_BeginPassBatchIndex = CompileData.CurrentSection_BeginPassBatchIndex;
        auto& CurrentSection_EndPassBatchIndex = CompileData.CurrentSection_EndPassBatchIndex;
        auto& CurrentSection_PassExtensions = CompileSectionData.PassExtensions;
        auto& CurrentSection_PassExtensions_SortedByDependencyScore = CompileSectionData.PassExtensions_SortedByDependencyScore;
        
        U32 CurrentSection_NumPassExtensions = static_cast<U32>(CurrentSection_PassExtensions.size());
        
        // Creating pass batches
        {
            struct F_MinimalPassRange
            {
                U32 BeginIndex = 0;
                U32 EndIndex = 0;
                TW<F_DirectX12RHICommandQueue> CommandQueue;
                E_DirectX12RHIPassBatchType PassBatchType = E_DirectX12RHIPassBatchType::DEFAULT;
                
                U32 GetSize() const
                {
                    return EndIndex - BeginIndex;
                }
                
                U32 MinReverseDependencyIndex = ~U32(0);
                U32 MaxDependencyIndex = ~U32(0);
                
                U32 MergeNext = ~U32(0);
                B8 PassBatchCreated = false;
            };
            TF_SmallVector<F_MinimalPassRange, 64> MinimalPassRanges;
            TF_SmallVector<U32, 256> PassExtensionIndexToMinimalPassRangeIndex;
            PassExtensionIndexToMinimalPassRangeIndex.resize(CurrentSection_NumPassExtensions);
            memset(
                PassExtensionIndexToMinimalPassRangeIndex.data(),
                0xFF,
                sizeof(U32) * CurrentSection_NumPassExtensions
            );
            
            // Build minimal pass ranges
            {
                F_MinimalPassRange MinimalPassRange;
                auto FlushMinimalPassRange = [&]
                {
                    if (MinimalPassRange.GetSize() > 0)
                    {
                        MinimalPassRanges.push_back(MinimalPassRange);
                    }
                    MinimalPassRange.BeginIndex = MinimalPassRange.EndIndex;
                };
                for (U32 PassExtensionIndex = 0; PassExtensionIndex < CurrentSection_NumPassExtensions; ++PassExtensionIndex)
                {
                    const auto& PassExtension = CurrentSection_PassExtensions_SortedByDependencyScore[PassExtensionIndex];
                    auto& MinimalPassRangeIndex = PassExtensionIndexToMinimalPassRangeIndex[PassExtensionIndex];
                
                    auto ProcessData_PassExtension = PassExtension->GetProcessData_PassExtension();
                    if (MinimalPassRangeIndex != ~U32(0))
                    {
                        continue;
                    }
                    
                    auto PassBatchType_PassExtension = PassExtension->GetPassBatchType();
                    
                    if (
                        (MinimalPassRange.CommandQueue != ProcessData_PassExtension->CommandQueue)
                        || (MinimalPassRange.PassBatchType != PassBatchType_PassExtension)
                    )
                    {
                        FlushMinimalPassRange();
                    }
                    MinimalPassRange.CommandQueue = ProcessData_PassExtension->CommandQueue;
                    MinimalPassRange.PassBatchType = PassBatchType_PassExtension;
                    MinimalPassRange.EndIndex = PassExtensionIndex + 1;
            
                    MinimalPassRangeIndex = static_cast<U32>(MinimalPassRanges.size());
                    
                    if (MinimalPassRange.GetSize() >= DirectX12RHIMaxPassBatchSize)
                    {
                        FlushMinimalPassRange();
                    }
                }
                FlushMinimalPassRange();
            }
            U32 NumMinimalPassRanges = static_cast<U32>(MinimalPassRanges.size());
            
            // Setup MinReverseDependencyIndex and MaxDependencyIndex
            for (U32 PassExtensionIndex = 0; PassExtensionIndex < CurrentSection_NumPassExtensions; ++PassExtensionIndex)
            {
                const auto& PassExtension = CurrentSection_PassExtensions_SortedByDependencyScore[PassExtensionIndex];
                auto ProcessData_PassExtension = PassExtension->GetProcessData_PassExtension();
                auto MinimalPassRangeIndex = PassExtensionIndexToMinimalPassRangeIndex[PassExtensionIndex];
                
                if (MinimalPassRangeIndex == ~U32(0))
                {
                    continue;
                }
                
                auto& MinimalPassRange = MinimalPassRanges[MinimalPassRangeIndex];
                
                for (const auto& Dependency : ProcessData_PassExtension->Dependencies)
                {
                    auto ProcessData_PassExtension_Dependency = Dependency->GetProcessData_PassExtension();
                    if (ProcessData_PassExtension_Dependency->SectionIndex != ProcessData_PassExtension->SectionIndex)
                    {
                        continue;
                    }
                    
                    auto& MinimalPassRangeIndex_Dependency = PassExtensionIndexToMinimalPassRangeIndex[
                        ProcessData_PassExtension_Dependency->SortedIndexInSection
                    ];
                    
                    if (MinimalPassRangeIndex == MinimalPassRangeIndex_Dependency)
                    {
                        continue;
                    }
                    if (MinimalPassRangeIndex_Dependency == ~U32(0))
                    {
                        continue;
                    }
                    
                    auto& MinimalPassRange_Dependency = MinimalPassRanges[MinimalPassRangeIndex_Dependency];
                    
                    if (MinimalPassRange_Dependency.MinReverseDependencyIndex == ~U32(0))
                    {
                        MinimalPassRange_Dependency.MinReverseDependencyIndex = MinimalPassRangeIndex;
                    }
                    else
                    {
                        MinimalPassRange_Dependency.MinReverseDependencyIndex = Min<U32>(
                            MinimalPassRange_Dependency.MinReverseDependencyIndex,
                            MinimalPassRangeIndex
                        );
                    }
                    
                    if (MinimalPassRange.MaxDependencyIndex == ~U32(0))
                    {
                        MinimalPassRange.MaxDependencyIndex = MinimalPassRangeIndex_Dependency;
                    }
                    else
                    {
                        MinimalPassRange.MaxDependencyIndex = Max<U32>(
                            MinimalPassRange.MaxDependencyIndex,
                            MinimalPassRangeIndex_Dependency
                        );
                    }
                }
            }
            
            // Calculate MergeNext
            for (U32 MinimalPassRangeIndex = 0; MinimalPassRangeIndex < NumMinimalPassRanges; ++MinimalPassRangeIndex)
            {
                auto& MinimalPassRange = MinimalPassRanges[MinimalPassRangeIndex];
                auto MinReverseDependencyIndex = MinimalPassRange.MinReverseDependencyIndex;
                auto MaxDependencyIndex = MinimalPassRange.MaxDependencyIndex;
                
                U32 TestIndex = MinimalPassRangeIndex - 1;
                for (; (TestIndex != ~U32(0)) && (TestIndex >= MaxDependencyIndex); --TestIndex)
                {
                    auto& TestMinimalPassRange = MinimalPassRanges[TestIndex];
                    if (
                        (MinimalPassRange.CommandQueue != TestMinimalPassRange.CommandQueue)
                        || (MinimalPassRange.PassBatchType != TestMinimalPassRange.PassBatchType)
                    )
                    {
                        continue;
                    }
                    if ((MinimalPassRange.GetSize() + TestMinimalPassRange.GetSize()) > DirectX12RHIMaxPassBatchSize)
                    {
                        break;
                    }
                    if (TestMinimalPassRange.MinReverseDependencyIndex < MinimalPassRangeIndex)
                    {
                        break;
                    }
                
                    TestMinimalPassRange.MergeNext = MinimalPassRangeIndex;
                    break;
                }
            }
            
            // Create pass batches
            for (U32 MinimalPassRangeIndex = 0; MinimalPassRangeIndex < NumMinimalPassRanges; ++MinimalPassRangeIndex)
            {
                auto& MinimalPassRange = MinimalPassRanges[MinimalPassRangeIndex];
                if (MinimalPassRange.PassBatchCreated)
                {
                    continue; 
                }
                
                F_DirectX12RHIPassBatch PassBatch;
                PassBatch.CommandQueue = MinimalPassRange.CommandQueue;
                PassBatch.CommandQueueProxy = MinimalPassRange.CommandQueue->GetProxy().FastCast<F_DirectX12RHICommandQueueProxy>().Weak();
                PassBatch.Type = MinimalPassRange.PassBatchType;
                
                U32 MinimalPassRangeIndexToMerge = MinimalPassRangeIndex;
                while (MinimalPassRangeIndexToMerge != ~U32(0))
                {
                    auto& MinimalPassRangeToMerge = MinimalPassRanges[MinimalPassRangeIndexToMerge];
                    ABYTEK_ENGINE_RHI_ASSERT(MinimalPassRangeToMerge.CommandQueue == PassBatch.CommandQueue);
                    
                    MinimalPassRangeToMerge.PassBatchCreated = true;
                    
                    for (U32 PassExtensionIndex = MinimalPassRangeToMerge.BeginIndex; PassExtensionIndex < MinimalPassRangeToMerge.EndIndex; ++PassExtensionIndex)
                    {
                        const auto& PassExtension = CurrentSection_PassExtensions_SortedByDependencyScore[PassExtensionIndex];
                        
                        auto ProcessData_PassExtension = PassExtension->GetProcessData_PassExtension();
                        ProcessData_PassExtension->BatchIndex = static_cast<U32>(PassBatches.size());
                        PassBatch.PassExtensions.push_back(PassExtension);
                    }
                    
                    MinimalPassRangeIndexToMerge = MinimalPassRangeToMerge.MergeNext;
                }
                
                ABYTEK_ENGINE_RHI_ASSERT(!PassBatch.PassExtensions.empty());
                PassBatches.push_back(ABYTEK_MOVE(PassBatch));
                ++CurrentSection_EndPassBatchIndex;
            }
        } 
        
        // 
        _CreateResourceBarriersForPasses();
        
        // Gather pass-batch-level barriers before (CPU-sync)
        {
            for (U32 PassBatchIndex = CurrentSection_BeginPassBatchIndex; PassBatchIndex < CurrentSection_EndPassBatchIndex; ++PassBatchIndex)
            {
                auto& PassBatch = PassBatches[PassBatchIndex];
                auto& ExecutionRanges = PassBatch.ExecutionRanges;
                
                if (PassBatch.Type != E_DirectX12RHIPassBatchType::CPU_SYNC)
                {
                    continue;
                }
                
                F_DirectX12RHIResourceBarrierProxySet BarrierProxies;
                
                for (const auto& PassExtension : PassBatch.PassExtensions)
                {
                    auto ProcessData_PassExtension = PassExtension->GetProcessData_PassExtension();
                    const auto& D3D12ResourceBarrierProxies_Before_PassExtension = ProcessData_PassExtension->D3D12ResourceBarrierProxies_Before;
                    if (D3D12ResourceBarrierProxies_Before_PassExtension.size() > 0)
                    {
                        BarrierProxies.insert(
                            BarrierProxies.end(),
                            D3D12ResourceBarrierProxies_Before_PassExtension.begin(),
                            D3D12ResourceBarrierProxies_Before_PassExtension.end()
                        );
                    }
                }
                
                if (!BarrierProxies.empty())
                {
                    F_DirectX12RHIExecutionRange ExecutionRange;
                    ExecutionRange.Type = E_DirectX12RHIExecutionRangeType::USE_COMMAND_LIST;
                    ExecutionRange.D3D12ResourceBarrierProxies_Before = ABYTEK_MOVE(BarrierProxies);
                    ExecutionRanges.push_back(ExecutionRange);
                }
            }
        }
        
        // Create execution ranges
        {        
            for (U32 PassBatchIndex = CurrentSection_BeginPassBatchIndex; PassBatchIndex < CurrentSection_EndPassBatchIndex; ++PassBatchIndex)
            {
                auto& PassBatch = PassBatches[PassBatchIndex];
                const auto& PassExtensions = PassBatch.PassExtensions;
                auto& ExecutionRanges = PassBatch.ExecutionRanges;
                
                U32 NumLocalPassExtensions = static_cast<U32>(PassExtensions.size());
                
                F_DirectX12RHIExecutionRange ExecutionRange;
                
                auto FlushExecutionRange = [&]
                {
                    if (ExecutionRange.GetNumPassExtensions() > 0)
                    {
                        ExecutionRanges.push_back(ExecutionRange);
                    }
                    U32 NewBeginPassExtensionIndex = ExecutionRange.EndLocalPassExtensionIndex;
                    ExecutionRange = {};
                    ExecutionRange.BeginLocalPassExtensionIndex = NewBeginPassExtensionIndex;
                    ExecutionRange.EndLocalPassExtensionIndex = NewBeginPassExtensionIndex;
                };
                
                for (U32 LocalPassExtensionIndex = 0; LocalPassExtensionIndex < NumLocalPassExtensions; ++LocalPassExtensionIndex)
                {
                    const auto& PassExtension = PassExtensions[LocalPassExtensionIndex];
                    auto ProcessData_PassExtension = PassExtension->GetProcessData_PassExtension();
                    
                    auto ExecutionRangeType = PassExtension->GetExecutionRangeType();
                   
                    ABYTEK_ENGINE_RHI_ASSERT(ExecutionRangeType != E_DirectX12RHIExecutionRangeType::NONE) 
                        << "Invalid execution range type";
                   
                    ABYTEK_ENGINE_RHI_ASSERT(
                        (ExecutionRangeType != E_DirectX12RHIExecutionRangeType::CPU_ACCESS)
                        || (PassBatch.Type == E_DirectX12RHIPassBatchType::CPU_SYNC)
                    ) << "E_DirectX12RHIExecutionRangeType::CPU_ACCESS is not allowed on GPU pass batch";
                    
                    ABYTEK_ENGINE_RHI_ASSERT(
                        (ExecutionRangeType != E_DirectX12RHIExecutionRangeType::USE_COMMAND_LIST)
                        || (PassBatch.Type == E_DirectX12RHIPassBatchType::GPU)
                    ) << "E_DirectX12RHIExecutionRangeType::USE_COMMAND_LIST is not allowed on CPU-sync pass batch";
                    
                    ABYTEK_ENGINE_RHI_ASSERT(
                        (ExecutionRangeType != E_DirectX12RHIExecutionRangeType::USE_COMMAND_QUEUE)
                        || (PassBatch.Type == E_DirectX12RHIPassBatchType::GPU)
                    ) << "E_DirectX12RHIExecutionRangeType::USE_COMMAND_QUEUE is not allowed on CPU-sync pass batch";
                    
                    if (ExecutionRange.Type != ExecutionRangeType)
                    {
                        FlushExecutionRange();
                    }
                    
                    if (ExecutionRangeType == E_DirectX12RHIExecutionRangeType::USE_COMMAND_QUEUE)
                    {
                        FlushExecutionRange();
                        
                        const auto& BarrierProxies_Before = ProcessData_PassExtension->D3D12ResourceBarrierProxies_Before;
                        F_DirectX12RHIExecutionRange BarrierExecutionRange;
                        BarrierExecutionRange.Type = E_DirectX12RHIExecutionRangeType::USE_COMMAND_LIST;
                        BarrierExecutionRange.BeginLocalPassExtensionIndex = ExecutionRange.BeginLocalPassExtensionIndex;
                        BarrierExecutionRange.EndLocalPassExtensionIndex = ExecutionRange.EndLocalPassExtensionIndex;
                        BarrierExecutionRange.D3D12ResourceBarrierProxies_Before = BarrierProxies_Before;
                        ExecutionRanges.push_back(BarrierExecutionRange);
                    }
                    
                    ExecutionRange.Type = ExecutionRangeType; 
                    ExecutionRange.EndLocalPassExtensionIndex = LocalPassExtensionIndex + 1;
                    
                    if (ExecutionRangeType == E_DirectX12RHIExecutionRangeType::USE_COMMAND_QUEUE)
                    {
                        FlushExecutionRange();
                        
                        const auto& BarrierProxies_After = ProcessData_PassExtension->D3D12ResourceBarrierProxies_After;
                        F_DirectX12RHIExecutionRange BarrierExecutionRange;
                        BarrierExecutionRange.Type = E_DirectX12RHIExecutionRangeType::USE_COMMAND_LIST;
                        BarrierExecutionRange.BeginLocalPassExtensionIndex = ExecutionRange.BeginLocalPassExtensionIndex;
                        BarrierExecutionRange.EndLocalPassExtensionIndex = ExecutionRange.EndLocalPassExtensionIndex;
                        BarrierExecutionRange.D3D12ResourceBarrierProxies_After = BarrierProxies_After;
                        ExecutionRanges.push_back(BarrierExecutionRange);
                    }
                }
                FlushExecutionRange();
            }
        }
        
        // Gather pass-batch-level barriers after (CPU-sync)
        {
            for (U32 PassBatchIndex = CurrentSection_BeginPassBatchIndex; PassBatchIndex < CurrentSection_EndPassBatchIndex; ++PassBatchIndex)
            {
                auto& PassBatch = PassBatches[PassBatchIndex];
                auto& ExecutionRanges = PassBatch.ExecutionRanges;
                
                if (PassBatch.Type != E_DirectX12RHIPassBatchType::CPU_SYNC)
                {
                    continue;
                }
                
                F_DirectX12RHIResourceBarrierProxySet BarrierProxies;
                
                for (const auto& PassExtension : PassBatch.PassExtensions)
                {
                    auto ProcessData_PassExtension = PassExtension->GetProcessData_PassExtension();
                    const auto& D3D12ResourceBarrierProxies_After_PassExtension = ProcessData_PassExtension->D3D12ResourceBarrierProxies_After;
                    if (!D3D12ResourceBarrierProxies_After_PassExtension.empty())
                    {
                        BarrierProxies.insert(
                            BarrierProxies.end(),
                            D3D12ResourceBarrierProxies_After_PassExtension.begin(),
                            D3D12ResourceBarrierProxies_After_PassExtension.end()
                        );
                    }
                }
                
                if (!BarrierProxies.empty())
                {
                    F_DirectX12RHIExecutionRange ExecutionRange;
                    ExecutionRange.Type = E_DirectX12RHIExecutionRangeType::USE_COMMAND_LIST;
                    ExecutionRange.D3D12ResourceBarrierProxies_After = ABYTEK_MOVE(BarrierProxies);
                    ExecutionRanges.push_back(ExecutionRange);
                }
            }
        }
        
        // Setting up pass batch dependencies
        for (U32 PassBatchIndex = CurrentSection_BeginPassBatchIndex; PassBatchIndex < CurrentSection_EndPassBatchIndex; ++PassBatchIndex)
        {
            auto& PassBatch = PassBatches[PassBatchIndex];
            for (const auto& PassExtension : PassBatch.PassExtensions)
            {
                auto ProcessData_PassExtension = PassExtension->GetProcessData_PassExtension();
                
                for (const auto& Dependency : ProcessData_PassExtension->Dependencies)
                {
                    const auto& ProcessData_PassExtension_Dependency = Dependency->GetProcessData_PassExtension();
                    
                    if (ProcessData_PassExtension_Dependency->BatchIndex == PassBatchIndex)
                    {
                        continue;
                    }
                    
                    if (
                        std::find(
                            PassBatch.DependencyIndices.begin(),    
                            PassBatch.DependencyIndices.end(),
                            ProcessData_PassExtension_Dependency->BatchIndex
                        )    
                        == PassBatch.DependencyIndices.end()
                    )
                    {
                        PassBatch.DependencyIndices.push_back(ProcessData_PassExtension_Dependency->BatchIndex);
                    }
                }
            }
        }
        
        // Setting up pass batch reverse dependencies
        for (U32 PassBatchIndex = CurrentSection_BeginPassBatchIndex; PassBatchIndex < CurrentSection_EndPassBatchIndex; ++PassBatchIndex)
        {
            auto& PassBatch = PassBatches[PassBatchIndex];
            for (auto DependencyIndex : PassBatch.DependencyIndices)
            {
                auto& ReverseDependencyIndices = PassBatches[DependencyIndex].ReverseDependencyIndices;
                if (
                    std::find(
                        ReverseDependencyIndices.begin(),
                        ReverseDependencyIndices.end(),
                        PassBatchIndex
                    )    
                    == ReverseDependencyIndices.end()
                )
                {
                    ReverseDependencyIndices.push_back(PassBatchIndex);
                }
            }
        }
        
        // Setup HasCommands_Begin && HasCommands_End
        for (U32 PassBatchIndex = CurrentSection_BeginPassBatchIndex; PassBatchIndex < CurrentSection_EndPassBatchIndex; ++PassBatchIndex)
        {
            auto& PassBatch = PassBatches[PassBatchIndex];
            auto& ExecutionRanges = PassBatch.ExecutionRanges;
            
            PassBatch.HasCommands_Begin = (ExecutionRanges.front().Type != E_DirectX12RHIExecutionRangeType::CPU_ACCESS);
            PassBatch.HasCommands_End = (ExecutionRanges.back().Type != E_DirectX12RHIExecutionRangeType::CPU_ACCESS);
        }
        
        // Setup execution range sync-related params
        for (U32 PassBatchIndex = CurrentSection_BeginPassBatchIndex; PassBatchIndex < CurrentSection_EndPassBatchIndex; ++PassBatchIndex)
        {
            auto& PassBatch = PassBatches[PassBatchIndex];
            auto& ExecutionRanges = PassBatch.ExecutionRanges;
            U32 NumExecutionRanges = static_cast<U32>(ExecutionRanges.size());
            
            // Sync between execution ranges
            for (U32 ExecutionRangeIndex = 1; ExecutionRangeIndex < NumExecutionRanges; ++ExecutionRangeIndex)
            {
                auto& ExecutionRange_Before = ExecutionRanges[ExecutionRangeIndex - 1];
                auto& ExecutionRange_After = ExecutionRanges[ExecutionRangeIndex];
                
                auto IsCPUExecutionRange_Before = DirectX12RHIIsCPUExecutionRange(ExecutionRange_Before.Type); 
                auto IsCPUExecutionRange_After = DirectX12RHIIsCPUExecutionRange(ExecutionRange_After.Type); 
                
                if (IsCPUExecutionRange_Before == IsCPUExecutionRange_After)
                {
                    continue;
                }
                
                if (IsCPUExecutionRange_After)
                {
                    ExecutionRange_Before.ShouldSignalFence = true;
                    ExecutionRange_Before.ShouldSyncGPU = true;
                }
            }
            
            // Batch end
            {
                auto& BackExecutionRange = ExecutionRanges.back();
                if (BackExecutionRange.Type != E_DirectX12RHIExecutionRangeType::CPU_ACCESS)
                {
                    for (auto ReverseDependencyIndex : PassBatch.ReverseDependencyIndices)
                    {
                        const auto& PassBatch_ReverseDependency = PassBatches[ReverseDependencyIndex];
                        if (
                            PassBatch.HasCommands_End 
                            && (
                                (
                                    (PassBatch.CommandQueue != PassBatch_ReverseDependency.CommandQueue)
                                    && PassBatch_ReverseDependency.HasCommands_Begin
                                )
                                || (!PassBatch_ReverseDependency.HasCommands_Begin)
                            )
                        )
                        {
                            BackExecutionRange.ShouldSignalFence = true;
                            break;
                        }
                    }
                    if (PassBatch.ReverseDependencyIndices.size() == 0)
                    {
                        BackExecutionRange.ShouldSignalFence = true;
                        BackExecutionRange.ShouldSyncGPU = true;
                    }
                }
            }
        }
        
        //
#ifdef ABYTEK_ENGINE_RHI_ENABLE_CAPTURE
        _CreateCaptureDataForPassBatches();
#endif
        
        // Create pass proxies
        for (U32 PassBatchIndex = CurrentSection_BeginPassBatchIndex; PassBatchIndex < CurrentSection_EndPassBatchIndex; ++PassBatchIndex)
        {
            auto& PassBatch = PassBatches[PassBatchIndex];
            for (const auto& PassExtension : PassBatch.PassExtensions)
            {
                PassBatch.PassProxies.push_back(
                    PassExtension->GetPass()->CreateProxy()
                );
            }
        }
        
        // Clear compile data on pass batches
        for (U32 PassBatchIndex = CurrentSection_BeginPassBatchIndex; PassBatchIndex < CurrentSection_EndPassBatchIndex; ++PassBatchIndex)
        {
            auto& PassBatch = PassBatches[PassBatchIndex];
            PassBatch.CommandQueue = {};
            PassBatch.PassExtensions = {};
        }
    }

    void F_DirectX12RHIProcess::_UpdateLastSubresourceD3D12States()
    {
        ABYTEK_PROFILER_EVENT();
        
        for (const auto& SubresourceReference : CompileSectionData.SubresourceReferences)
        {
            auto CompileData_Subresource = SubresourceReference->GetCompileData();
            auto& D3D12States = CompileData_Subresource->D3D12States;
            
            SubresourceReference->LastD3D12State = D3D12States.back();
        }
    }

    void F_DirectX12RHIProcess::_EndCompileSection()
    {
        ABYTEK_PROFILER_EVENT();
        
        _EndCurrentSectionForResources();
        _EndCurrentSectionForSubresources();
        
        CompileSectionData = {};
            
        CompileData.CurrentSection_BeginSubmissionListExtensionIndex = CompileData.CurrentSection_EndSubmissionListExtensionIndex;
        CompileData.CurrentSection_BeginPassExtensionIndex = CompileData.CurrentSection_EndPassExtensionIndex;
        CompileData.CurrentSection_BeginViewportIndex = CompileData.CurrentSection_EndViewportIndex;
            
        CompileData.CurrentSection_BeginSubresourceReferenceIndex = CompileData.CurrentSection_EndSubresourceReferenceIndex;
        CompileData.CurrentSection_BeginResourceIndex = CompileData.CurrentSection_EndResourceIndex;
        
        CompileData.CurrentSection_BeginPassBatchIndex = CompileData.CurrentSection_EndPassBatchIndex;
        
        ++CompileData.CurrentSectionIndex;
    }

    void F_DirectX12RHIProcess::_EndCurrentSectionForSubresources()
    {
        ABYTEK_PROFILER_EVENT();
        for (const auto& SubresourceReference : CompileSectionData.SubresourceReferences)
        {
            auto CompileData_Subresource = SubresourceReference->GetCompileData();
            CompileData_Subresource->CurrentSection_BeginPassTrackingIndex = CompileData_Subresource->CurrentSection_EndPassTrackingIndex;
        }
    }
    void F_DirectX12RHIProcess::_EndCurrentSectionForResources()
    {
        ABYTEK_PROFILER_EVENT();
        for (const auto& Resource : CompileSectionData.Resources)
        {
            auto CompileData_Resource = Resource.FastCast<F_DirectX12RHIResource>()->GetCompileData();
            CompileData_Resource->CurrentSection_BeginPassTrackingIndex = CompileData_Resource->CurrentSection_EndPassTrackingIndex;
        }
    }

    void F_DirectX12RHIProcess::_CreateResourceBarriersForPasses()
    {
        ABYTEK_PROFILER_EVENT();
        _CreateResourceUAVBarriersForPasses();
        _CreateResourceTransitionBarriersForPasses();
        _CreateResourceAliasingBarriersForPasses();
    }
    void F_DirectX12RHIProcess::_CreateResourceUAVBarriersForPasses()
    {
        ABYTEK_PROFILER_EVENT();
        const auto& PassExtensions = CompileSectionData.PassExtensions;
        
        // Set ResourceUse.IsUAVSyncPoint
        for (const auto& PassExtension : PassExtensions)
        {
            auto ProcessData_PassExtension = PassExtension->GetProcessData_PassExtension();
            auto& SubresourceBindingSet = ProcessData_PassExtension->SubresourceBindingSet;
            for (auto& SubresourceBinding : SubresourceBindingSet)
            {
                for (const auto& WriteDependency : SubresourceBinding.WriteDependencies)
                {
                    auto& SubresourceBinding_WriteDependency = *WriteDependency;
                
                    const auto& ProcessData_PassExtension_WriteDependency = WriteDependency.PassExtension->GetProcessData_PassExtension();
                
                    if (ProcessData_PassExtension_WriteDependency->SectionIndex != ProcessData_PassExtension->SectionIndex)
                    {
                        continue;
                    }
                    
                    // Only enable UAVSyncPoint if both are UAV
                    if (
                        (SubresourceBinding_WriteDependency.AccessCategories != E_RHIResourceAccessCategory::UAV)
                        || (SubresourceBinding.AccessCategories != E_RHIResourceAccessCategory::UAV)
                    )
                    {
                        continue;
                    }
                
                    // If they are in 2 different batches, no need to use UAV barriers
                    if (ProcessData_PassExtension_WriteDependency->BatchIndex != ProcessData_PassExtension->BatchIndex)
                    {
                        continue;
                    }
                
                    // Set IsUAVSyncPoint to true
                    SubresourceBinding.ResourceUseReference->IsUAVSyncPoint = true;
                }
            }
        }
        
        // Set ResourceUse.IsUAVSyncPoint
        for (const auto& PassExtension : PassExtensions)
        {
            auto ProcessData_PassExtension = PassExtension->GetProcessData_PassExtension();
            auto& ResourceUseSet = ProcessData_PassExtension->ResourceUseSet;
            for (auto& ResourceUse : ResourceUseSet)
            {
                if (!ResourceUse.IsUAVSyncPoint)
                {
                    continue;
                }
                
                F_DirectX12RHIResourceBarrierProxy ResourceBarrierProxy;
                ResourceBarrierProxy.Type = E_DirectX12RHIResourceBarrierProxyType::UAV;
                ResourceBarrierProxy.UAV.ResourceProxy = ResourceUse.Resource->GetProxy().FastCast<A_RHIResourceProxy>();
                
                ProcessData_PassExtension->D3D12ResourceBarrierProxies_Before.push_back(ResourceBarrierProxy);
            }
        }
    }
    void F_DirectX12RHIProcess::_CreateResourceTransitionBarriersForPasses()
    {
        ABYTEK_PROFILER_EVENT();
        for (const auto& PassExtension : CompileSectionData.PassExtensions)
        {
            auto ProcessData_PassExtension = PassExtension->GetProcessData_PassExtension();
            auto& SubresourceBindingSet = ProcessData_PassExtension->SubresourceBindingSet;
            
            // State join points
            for (auto& SubresourceBinding : SubresourceBindingSet)
            {
                if (!SubresourceBinding.IsStateJoinPoint)
                {
                    continue;
                }
                
                auto& D3D12States = SubresourceBinding.SubresourceReference->GetCompileData()->D3D12States;
                U32 D3D12StateIndex = F_DirectX12RHISubresourceCompileData::PassTrackingIndexToD3D12StateIndex(
                    SubresourceBinding.PassTrackingReference.Index
                );
                if (D3D12StateIndex == (D3D12States.size() - 1))
                {
                    continue;
                }
                
                auto D3D12StateBefore = D3D12States[D3D12StateIndex];
                auto D3D12StateAfter = D3D12States[D3D12StateIndex + 1];
            
                if (D3D12StateBefore == D3D12StateAfter)
                {
                    continue;
                }
                
                F_DirectX12RHIResourceBarrierProxy ResourceBarrierProxy;
                ResourceBarrierProxy.Type = E_DirectX12RHIResourceBarrierProxyType::TRANSITION;
                ResourceBarrierProxy.Transition.ResourceProxy = SubresourceBinding.ResourceUseReference->Resource->GetProxy().FastCast<A_RHIResourceProxy>();
                ResourceBarrierProxy.Transition.SubresourceIndex = SubresourceBinding.SubresourceReference.Index;
                ResourceBarrierProxy.Transition.D3D12StateBefore = D3D12StateBefore;
                ResourceBarrierProxy.Transition.D3D12StateAfter = D3D12StateAfter;
                
                ProcessData_PassExtension->D3D12ResourceBarrierProxies_After.push_back(ResourceBarrierProxy);
            }
            
            // Prologue state
            for (auto& SubresourceBinding : SubresourceBindingSet)
            {
                if (!SubresourceBinding.NeedPrologueStateTransitionBarrier)
                {
                    continue;
                }
                
                auto& D3D12States = SubresourceBinding.SubresourceReference->GetCompileData()->D3D12States;
                
                auto D3D12StateIndex = F_DirectX12RHISubresourceCompileData::PassTrackingIndexToD3D12StateIndex(
                    SubresourceBinding.PassTrackingReference.Index    
                );
                
                auto D3D12StateBefore = D3D12States[D3D12StateIndex - 1];
                auto D3D12StateAfter = D3D12States[D3D12StateIndex];
                
                if (D3D12StateBefore == D3D12StateAfter)
                {
                    continue;
                }
                
                F_DirectX12RHIResourceBarrierProxy ResourceBarrierProxy;
                ResourceBarrierProxy.Type = E_DirectX12RHIResourceBarrierProxyType::TRANSITION;
                ResourceBarrierProxy.Transition.ResourceProxy = SubresourceBinding.ResourceUseReference->Resource->GetProxy().FastCast<A_RHIResourceProxy>();
                ResourceBarrierProxy.Transition.SubresourceIndex = SubresourceBinding.SubresourceReference.Index;
                ResourceBarrierProxy.Transition.D3D12StateBefore = D3D12StateBefore;
                ResourceBarrierProxy.Transition.D3D12StateAfter = D3D12StateAfter;
                
                ProcessData_PassExtension->D3D12ResourceBarrierProxies_Before.push_back(ResourceBarrierProxy);
            }
        }
    }
    void F_DirectX12RHIProcess::_CreateResourceAliasingBarriersForPasses()
    {
        ABYTEK_PROFILER_EVENT();
        for (const auto& PassExtension : CompileSectionData.PassExtensions)
        {
            auto ProcessData_PassExtension = PassExtension->GetProcessData_PassExtension();
            auto& ResourceUseSet = ProcessData_PassExtension->ResourceUseSet;
            
            for (auto& ResourceUse : ResourceUseSet)
            {
                auto& AliasingDependency = ResourceUse.AliasingDependency;
                if (!AliasingDependency)
                {
                    continue;
                }
                
                const auto& ProcessData_PassExtension_AliasingDependency = AliasingDependency.PassExtension->GetProcessData_PassExtension();
                
                // No needs to add barriers between 2 passes in different batches 
                if (ProcessData_PassExtension->BatchIndex != ProcessData_PassExtension_AliasingDependency->BatchIndex)
                {
                    continue;   
                }
                
                F_DirectX12RHIResourceBarrierProxy ResourceBarrierProxy;
                ResourceBarrierProxy.Type = E_DirectX12RHIResourceBarrierProxyType::ALIASING;
                ResourceBarrierProxy.Aliasing.ResourceProxyBefore = AliasingDependency->Resource->GetProxy().FastCast<A_RHIResourceProxy>();
                ResourceBarrierProxy.Aliasing.ResourceProxyAfter = ResourceUse.Resource->GetProxy().FastCast<A_RHIResourceProxy>();
                
                ProcessData_PassExtension->D3D12ResourceBarrierProxies_Before.push_back(ResourceBarrierProxy);
            }
        }
    }
    
#ifdef ABYTEK_ENGINE_RHI_ENABLE_CAPTURE
    void F_DirectX12RHIProcess::_CreateCaptureDataForPassBatches()
    {
        ABYTEK_PROFILER_EVENT();
        auto GetCaptureEventStatesForPassExtensions = [this](const TW_Valid<A_DirectX12RHIPassExtension>& PassExtension)
        {
            auto ProcessData_SubmissionItemExtension = PassExtension->GetProcessData_SubmissionItemExtension();
            TF_SmallVector<F_RHICaptureEventState, 4> PassCaptureEventStates;
            if (auto CaptureEventState = GetCaptureEventState())
            {
                PassCaptureEventStates.push_back(CaptureEventState);
            }
            for (const auto& ListExtension : ProcessData_SubmissionItemExtension->GraphData.ListExtensions)
            {
                auto SubmissionItem = ListExtension->GetSubmissionItem();
                for (const auto& StackCaptureEventState : SubmissionItem->GetStackCaptureEventStates())
                {
                    if (StackCaptureEventState)
                    {
                        PassCaptureEventStates.push_back(StackCaptureEventState);
                    }
                }
                if (auto CaptureEventState = SubmissionItem->GetCaptureEventState())
                {
                    PassCaptureEventStates.push_back(CaptureEventState);
                }
            }
            {
                auto SubmissionItem = PassExtension->GetSubmissionItem();
                for (const auto& StackCaptureEventState : SubmissionItem->GetStackCaptureEventStates())
                {
                    if (StackCaptureEventState)
                    {
                        PassCaptureEventStates.push_back(StackCaptureEventState);
                    }
                }
                if (auto CaptureEventState = SubmissionItem->GetCaptureEventState())
                {
                    PassCaptureEventStates.push_back(CaptureEventState);
                }
            }
            return ABYTEK_MOVE(PassCaptureEventStates);
        };
        
        for (
            U32 PassBatchIndex = CompileData.CurrentSection_BeginPassBatchIndex; 
            PassBatchIndex < CompileData.CurrentSection_EndPassBatchIndex; 
            ++PassBatchIndex
        )
        {
            auto& PassBatch = CompileData.PassBatches[PassBatchIndex];
            
            for (const auto& ExecuteRange : PassBatch.ExecutionRanges)
            {
                if (
                    (ExecuteRange.Type != E_DirectX12RHIExecutionRangeType::USE_COMMAND_LIST)
                    && (ExecuteRange.Type != E_DirectX12RHIExecutionRangeType::USE_COMMAND_QUEUE)
                )
                {
                    continue;
                }
                
                TF_SmallVector<F_RHICaptureEventState, 4> CaptureEventStates;
                for (
                    auto PassExtensionIndex = ExecuteRange.BeginLocalPassExtensionIndex; 
                    PassExtensionIndex != ExecuteRange.EndLocalPassExtensionIndex;
                )
                {
                    auto PassExtension = PassBatch.PassExtensions[PassExtensionIndex];
                    auto ProcessData_PassExtension = PassExtension->GetProcessData_PassExtension();

                    auto& CaptureEventStatesToBegin = ProcessData_PassExtension->CaptureEventStatesToBegin;
                    auto& CaptureEventStatesToEnd = ProcessData_PassExtension->CaptureEventStatesToEnd;
                
                    auto PassCaptureEventStates = GetCaptureEventStatesForPassExtensions(PassExtension);
                
                    // Begin
                    {
                        U32 BeginChangeIdx = 0;
                        for (; 
                            BeginChangeIdx < Min<U32>(
                                static_cast<U32>(CaptureEventStates.size()), 
                                static_cast<U32>(PassCaptureEventStates.size())
                            ); 
                            ++BeginChangeIdx
                        )
                        {
                            if (CaptureEventStates[BeginChangeIdx] != PassCaptureEventStates[BeginChangeIdx])
                            {
                                break;
                            }
                        }
                        for (U32 Idx = BeginChangeIdx; Idx < PassCaptureEventStates.size(); ++Idx)
                        {
                            CaptureEventStatesToBegin.push_back(PassCaptureEventStates[Idx]);
                        }
                    }
                    CaptureEventStates = PassCaptureEventStates;
                
                    auto NextPassExtensionIndex = PassExtensionIndex;
                    ++NextPassExtensionIndex;
                
                    // End
                    if (NextPassExtensionIndex == ExecuteRange.EndLocalPassExtensionIndex)
                    {
                        CaptureEventStatesToEnd.insert(
                            CaptureEventStatesToEnd.end(),
                            CaptureEventStates.begin(),
                            CaptureEventStates.end()
                        );
                    }
                    else
                    {
                        auto PassCaptureEventStates_Next = GetCaptureEventStatesForPassExtensions(PassBatch.PassExtensions[NextPassExtensionIndex]);
                        U32 BeginChangeIdx = 0;
                        for (; 
                            BeginChangeIdx < Min<U32>(
                                static_cast<U32>(CaptureEventStates.size()), 
                                static_cast<U32>(PassCaptureEventStates_Next.size())
                            ); 
                            ++BeginChangeIdx
                        )
                        {
                            if (CaptureEventStates[BeginChangeIdx] != PassCaptureEventStates_Next[BeginChangeIdx])
                            {
                                break;
                            }
                        }
                        for (U32 Idx = BeginChangeIdx; Idx < CaptureEventStates.size(); ++Idx)
                        {
                            CaptureEventStatesToEnd.push_back(CaptureEventStates[Idx]);
                        }
                    }

                    PassExtensionIndex = NextPassExtensionIndex;
                }
            }
        }
    }
#endif
    
    void F_DirectX12RHIProcess::_TransferCompileDataToExecutionData()
    {
        ABYTEK_PROFILER_EVENT();
        ExecutionData.PassBatches = CompileData.PassBatches;
        _TransferCompileDataToTransientUploadBuffers();
        _TransferCompileDataToTransientReadbackBuffers();
    }
    void F_DirectX12RHIProcess::_TransferCompileDataToTransientUploadBuffers()
    {
        ABYTEK_PROFILER_EVENT();
        for (const auto& Context : GetContexts())
        {
            auto TransientUploadBufferManager = Context->GetTransientUploadBufferManager_V2();
            for (const auto& Page : TransientUploadBufferManager->SectionData.Pages)
            {
                DirectX12RHIProcessData::Execution::F_TransientUploadBuffer Buffer;
                Buffer.BufferProxy = Page->GetBuffer()->GetProxy().StaticCast<A_RHIResourceProxy>();
                
                F_RHITransientUploadBufferCandidate_V2 Candidate;
                while (Page->Queue.TryPop(Candidate))
                {
                    Buffer.Candidates.push_back(ABYTEK_MOVE(Candidate));
                }
                
                ExecutionData.TransientUploadBuffers.push_back(ABYTEK_MOVE(Buffer));
            }
        }
    }
    void F_DirectX12RHIProcess::_TransferCompileDataToTransientReadbackBuffers()
    {
        ABYTEK_PROFILER_EVENT();
        for (const auto& Context : GetContexts())
        {
            auto TransientReadbackBufferManager = Context->GetTransientReadbackBufferManager_V2();
            for (const auto& Page : TransientReadbackBufferManager->SectionData.Pages)
            {
                DirectX12RHIProcessData::Execution::F_TransientReadbackBuffer Buffer;
                Buffer.BufferProxy = Page->GetBuffer()->GetProxy().StaticCast<A_RHIResourceProxy>();
                
                F_RHITransientReadbackBufferCandidate_V2 Candidate;
                while (Page->Queue.TryPop(Candidate))
                {
                    Buffer.Candidates.push_back(ABYTEK_MOVE(Candidate));
                }
                
                ExecutionData.TransientReadbackBuffers.push_back(ABYTEK_MOVE(Buffer));
            }
        }
    }
    void F_DirectX12RHIProcess::_TransferCompileDataToLateExecutionData()
    {
        ABYTEK_PROFILER_EVENT();
        for (const auto& Viewport : CompileSectionData.Viewports)
        {
            LateExecutionData.ViewportProxies.push_back(Viewport->GetProxy().FastCast<A_RHIViewportProxy>());
        }
    }
    void F_DirectX12RHIProcess::_UpdateViewports()
    {
        ABYTEK_PROFILER_EVENT();
        for (const auto& Viewport : CompileSectionData.Viewports)
        {
            Viewport.FastCast<F_DirectX12RHIViewport>()->Update();
        }
    }

    void F_DirectX12RHIProcess::_TransferDataToContextQueues()
    {
        ABYTEK_PROFILER_EVENT();
        {
            DirectX12RHIProcessQueries::Compile::F_DeallocateDescriptors Query;
            while (Queues.Compile.DeallocateDescriptors.TryPop(Query))
            {
                auto CastedContext = Query.DescriptorAllocation.Manager->GetContext().FastCast<F_DirectX12RHIContext>();
                CastedContext->Queues.DeallocateDescriptors.Push(Query);
            }
        }
    }

    void F_DirectX12RHIProcess::_InitCommandQueues()
    {
        ABYTEK_PROFILER_EVENT();
        DirectX12RHIProcessQueries::Execution::F_InitCommandQueue Query;
        while (Queues.Execution.InitCommandQueue.TryPop(Query))
        {
            Query.CommandQueueProxy->InitD3D12CommandQueue();
        }
    }

    void F_DirectX12RHIProcess::_InitRootSignatures()
    {
        ABYTEK_PROFILER_EVENT();
        DirectX12RHIProcessQueries::Execution::F_InitRootSignature Query;
        while (Queues.Execution.InitRootSignature.TryPop(Query))
        {
            Query.RootSignatureTemplateRuntimeProxy.FastCast<F_DirectX12RHIRootSignatureTemplateRuntimeProxy>()->InitD3D12RootSignature();
        }
    }
    void F_DirectX12RHIProcess::_InitPipelineStates()
    {
        ABYTEK_PROFILER_EVENT();
        DirectX12RHIProcessQueries::Execution::F_InitPipelineState Query;
        while (Queues.Execution.InitPipelineState.TryPop(Query))
        {
            Query.PipelineStateTemplateRuntimeProxy.FastCast<F_DirectX12RHIPipelineStateTemplateRuntimeProxy>()->InitD3D12PipelineState();
        }
    }

    void F_DirectX12RHIProcess::_InitViewports()
    {
        ABYTEK_PROFILER_EVENT();
        DirectX12RHIProcessQueries::Execution::F_InitViewport Query;
        while (Queues.Execution.InitViewport.TryPop(Query))
        {
            Query.ViewportProxy.FastCast<F_DirectX12RHIViewportProxy>()->InitDXGISwapchain();
        }
    }
    void F_DirectX12RHIProcess::_ResizeViewports()
    {
        ABYTEK_PROFILER_EVENT();
        DirectX12RHIProcessQueries::Execution::F_ResizeViewport Query;
        while (Queues.Execution.ResizeViewport.TryPop(Query))
        {
            Query.ViewportProxy.FastCast<F_DirectX12RHIViewportProxy>()->ResizeDXGISwapchain(Query.NewSize);
        }
    }
    void F_DirectX12RHIProcess::_AssignViewportFrames()
    {
        ABYTEK_PROFILER_EVENT();
        DirectX12RHIProcessQueries::Execution::F_AssignViewportFrame Query;
        while (Queues.Execution.AssignViewportFrame.TryPop(Query))
        {
            auto DXGISwapchain = Query.ViewportProxy.FastCast<F_DirectX12RHIViewportProxy>()->GetDXGISwapChain();
            
            Microsoft::WRL::ComPtr<ID3D12Resource> D3D12Frame = nullptr;
            HRESULT HR = DXGISwapchain->GetBuffer(Query.FrameIndex, IID_PPV_ARGS(&D3D12Frame));
            ABYTEK_ENGINE_RHI_ASSERT(SUCCEEDED(HR)) << "can't get d3d12 viewport frame";
            
            Query.ResourceProxy.FastCast<F_DirectX12RHIResourceProxy>()->AssignExternalD3D12Resource(D3D12Frame);
        }
    }

    void F_DirectX12RHIProcess::_AssignD3D12ExternalResources()
    {
        ABYTEK_PROFILER_EVENT();
        DirectX12RHIProcessQueries::Execution::F_AssignExternalD3D12Resource Query;
        while (Queues.Execution.AssignExternalD3D12Resource.TryPop(Query))
        {
            Query.ResourceProxy.FastCast<F_DirectX12RHIResourceProxy>()->AssignExternalD3D12Resource(Query.D3D12Resource);
        }
    }

    void F_DirectX12RHIProcess::_CreateResources()
    {
        ABYTEK_PROFILER_EVENT();
        _CreateCommittedResources();
        _CreatePlacedResources();
    }
    void F_DirectX12RHIProcess::_CreateCommittedResources()
    {
        ABYTEK_PROFILER_EVENT();
        DirectX12RHIProcessQueries::Execution::F_CreateCommittedResource Query;
        while (Queues.Execution.CreateCommittedResource.TryPop(Query))
        {
            auto CastedResourceProxy = Query.ResourceProxy.FastCast<F_DirectX12RHIResourceProxy>();
        
            Microsoft::WRL::ComPtr<ID3D12Resource> D3D12Resource = nullptr;
            
            D3D12_CLEAR_VALUE* D3D12ClearValuePtr = nullptr;
            if (Query.D3D12ClearValue)
            {
                D3D12ClearValuePtr = &*Query.D3D12ClearValue;
            }
        
            auto D3D12Device = CastedResourceProxy->GetContextProxy()->GetDevice().FastCast<F_DirectX12RHIDevice>()->GetD3D12Device();
            HRESULT HR = D3D12Device->CreateCommittedResource(
                &Query.D3D12HeapProperties,
                D3D12_HEAP_FLAG_NONE,
                &Query.D3D12ResourceDesc,
                Query.InitialD3D12ResourceState,
                D3D12ClearValuePtr,
                IID_PPV_ARGS(&D3D12Resource)
            );
            ABYTEK_ENGINE_RHI_ASSERT(D3D12Resource) << "Failed to create d3d12 committed resource";
        
#ifdef ABYTEK_DEBUG_INFO
            D3D12Resource->SetName(CastedResourceProxy->GetDebugName()->c_str());
#endif

            CastedResourceProxy->AssignCommittedD3D12Resource(D3D12Resource);
        }
    }
    void F_DirectX12RHIProcess::_CreatePlacedResources()
    {
        ABYTEK_PROFILER_EVENT();
        DirectX12RHIProcessQueries::Execution::F_CreatePlacedResource Query;
        while (Queues.Execution.CreatePlacedResource.TryPop(Query))
        {
            auto CastedResourceProxy = Query.ResourceProxy.FastCast<F_DirectX12RHIResourceProxy>();
        
            Microsoft::WRL::ComPtr<ID3D12Resource> D3D12Resource = nullptr;
            
            D3D12_CLEAR_VALUE* D3D12ClearValuePtr = nullptr;
            if (Query.D3D12ClearValue)
            {
                D3D12ClearValuePtr = &*Query.D3D12ClearValue;
            }
        
            auto D3D12Device = CastedResourceProxy->GetContextProxy()->GetDevice().FastCast<F_DirectX12RHIDevice>()->GetD3D12Device();
            HRESULT HR = D3D12Device->CreatePlacedResource(
                Query.ResourcePlacementProxy.PageProxy->GetD3D12Heap().Get(),
                Query.ResourcePlacementProxy.BeginOffsetInBytes,
                &Query.D3D12ResourceDesc,
                Query.InitialD3D12ResourceState,
                D3D12ClearValuePtr,
                IID_PPV_ARGS(&D3D12Resource)
            );
            ABYTEK_ENGINE_RHI_ASSERT(D3D12Resource) << "Failed to create d3d12 placed resource";
        
#ifdef ABYTEK_DEBUG_INFO
            D3D12Resource->SetName(CastedResourceProxy->GetDebugName()->c_str());
#endif

            CastedResourceProxy->AssignPlacedD3D12Resource(D3D12Resource);
        }
    }

    void F_DirectX12RHIProcess::_SendCommitToBindGroupProxies()
    {
        ABYTEK_PROFILER_EVENT();
        DirectX12RHIProcessQueries::Execution::F_SendCommitToBindGroupProxy Query;
        while (Queues.Execution.SendCommitToBindGroupProxy.TryPop(Query))
        {
            Query.BindGroupProxy.FastCast<F_DirectX12RHIBindGroupProxy>()->AssignCommit(
                ABYTEK_MOVE(Query.SlotDataProxy),
                ABYTEK_MOVE(Query.AllocationProxy)
            );
        }
    }
    void F_DirectX12RHIProcess::_CopyDescriptorsForBindGroupProxies()
    {
        ABYTEK_PROFILER_EVENT();
        DirectX12RHIProcessQueries::Execution::F_CopyDescriptorsForBindGroupProxy Query;
        while (Queues.Execution.CopyDescriptorsForBindGroupProxy.TryPop(Query))
        {
            for (const auto& DescriptorAllocationPair : Query.DescriptorAllocationPairs)
            {
                DirectX12RHIProcessQueries::Execution::F_CopyDescriptors ForwardQuery;
                ForwardQuery.Payload.DstDescriptorRange = DescriptorAllocationPair.first.ConvertToRange();
                ForwardQuery.Payload.SrcDescriptorRange = DescriptorAllocationPair.second.ConvertToRange();
                Queues.Execution.CopyDescriptors.Push(ForwardQuery);
            }
        }
    }

    void F_DirectX12RHIProcess::_InitResourceViewDescriptors()
    {
        ABYTEK_PROFILER_EVENT();
        
        struct F_Assignment
        {
            TS<F_DirectX12RHIResourceViewProxy> CastedResourceViewProxy;
            F_DirectX12RHIDescriptorAllocation DescriptorAllocation;
        };
        TF_SmallVector<F_Assignment, 128> Assignments;
        
        TF_Map<TW<F_DirectX12RHIDescriptorManagerProxy>, F_DirectX12RHIDescriptorIndex> DescriptorManagerProxyToSize;
        
        DirectX12RHIProcessQueries::Execution::F_InitResourceViewDescriptor Query;
        while (Queues.Execution.InitResourceViewDescriptor.TryPop(Query))
        {
            auto CastedResourceViewProxy = Query.ResourceViewProxy.FastCast<F_DirectX12RHIResourceViewProxy>();
            auto CastedResourceProxy = Query.ResourceViewProxy->GetResourceProxy().FastCast<F_DirectX12RHIResourceProxy>();
            auto D3D12Resource = CastedResourceProxy->GetD3D12Resource();
            
            const auto& Access = CastedResourceViewProxy->GetAccess();
        
            auto DescriptorManagerProxy = Query.DescriptorAllocation.Manager->GetProxy().FastCast<F_DirectX12RHIDescriptorManagerProxy>();
            
            DirectX12RHIDescriptorManagerQueries::F_Init ForwardQuery;
            ForwardQuery.DescriptorIndex = Query.DescriptorAllocation.BeginOffset;
            ForwardQuery.D3D12DescriptorDesc = CastedResourceViewProxy->GetDescriptorDesc();
            
            DescriptorManagerProxy->Queues.Init.Push(ForwardQuery);
            
            F_Assignment Assignment;
            Assignment.CastedResourceViewProxy = CastedResourceViewProxy;
            Assignment.DescriptorAllocation = Query.DescriptorAllocation;
            Assignments.push_back(Assignment);
            
            {
                F_DirectX12RHIDescriptorIndex TargetSize = DescriptorManagerProxy->GetSize();
                auto It = DescriptorManagerProxyToSize.find(DescriptorManagerProxy.Weak());
                if (It != DescriptorManagerProxyToSize.end())
                {
                    TargetSize = Max<F_DirectX12RHIDescriptorIndex>(TargetSize, It->second);
                }
                TargetSize = Max<F_DirectX12RHIDescriptorIndex>(TargetSize, Query.DescriptorAllocation.EndOffset);
                DescriptorManagerProxyToSize[DescriptorManagerProxy.Weak()] = TargetSize;
            }
        }
        
        for (const auto& Pair : DescriptorManagerProxyToSize)
        {
            if (Pair.first->GetSize() < Pair.second)
            {
                Pair.first->Resize(Pair.second);
            }
        }
        
        _FlushDescriptorManagers();
        
        for (const auto& Assignment : Assignments)
        {
            Assignment.CastedResourceViewProxy->AssignDescriptorRange(
                Assignment.DescriptorAllocation.ConvertToRange()
            );
        }
    }
    void F_DirectX12RHIProcess::_InitSamplerDescriptors()
    {
        ABYTEK_PROFILER_EVENT();
        
        struct F_Assignment
        {
            TS<F_DirectX12RHISamplerProxy> CastedSamplerProxy;
            F_DirectX12RHIDescriptorAllocation DescriptorAllocation;
        };
        TF_SmallVector<F_Assignment, 128> Assignments;
        
        TF_Map<TW<F_DirectX12RHIDescriptorManagerProxy>, F_DirectX12RHIDescriptorIndex> DescriptorManagerProxyToSize;
        
        DirectX12RHIProcessQueries::Execution::F_InitSamplerDescriptor Query;
        while (Queues.Execution.InitSamplerDescriptor.TryPop(Query))
        {
            auto CastedSamplerProxy = Query.SamplerProxy.FastCast<F_DirectX12RHISamplerProxy>();

            auto DescriptorManagerProxy = Query.DescriptorAllocation.Manager->GetProxy().FastCast<F_DirectX12RHIDescriptorManagerProxy>();
            
            DirectX12RHIDescriptorManagerQueries::F_Init ForwardQuery;
            ForwardQuery.DescriptorIndex = Query.DescriptorAllocation.BeginOffset;
            ForwardQuery.D3D12DescriptorDesc = CastedSamplerProxy->GetDescriptorDesc();
            
            DescriptorManagerProxy->Queues.Init.Push(ForwardQuery);
            
            F_Assignment Assignment;
            Assignment.CastedSamplerProxy = CastedSamplerProxy;
            Assignment.DescriptorAllocation = Query.DescriptorAllocation;
            Assignments.push_back(Assignment);
            
            {
                F_DirectX12RHIDescriptorIndex TargetSize = DescriptorManagerProxy->GetSize();
                auto It = DescriptorManagerProxyToSize.find(DescriptorManagerProxy.Weak());
                if (It != DescriptorManagerProxyToSize.end())
                {
                    TargetSize = Max<F_DirectX12RHIDescriptorIndex>(TargetSize, It->second);
                }
                TargetSize = Max<F_DirectX12RHIDescriptorIndex>(TargetSize, Query.DescriptorAllocation.EndOffset);
                DescriptorManagerProxyToSize[DescriptorManagerProxy.Weak()] = TargetSize;
            }
        }
        
        for (const auto& Pair : DescriptorManagerProxyToSize)
        {
            if (Pair.first->GetSize() < Pair.second)
            {
                Pair.first->Resize(Pair.second);
            }
        }
        
        _FlushDescriptorManagers();
        
        for (const auto& Assignment : Assignments)
        {
            Assignment.CastedSamplerProxy->AssignDescriptorRange(
                Assignment.DescriptorAllocation.ConvertToRange()
            );
        }
    }
    void F_DirectX12RHIProcess::_CopyDescriptors()
    {
        ABYTEK_PROFILER_EVENT();
        DirectX12RHIProcessQueries::Execution::F_CopyDescriptors Query;
        while (Queues.Execution.CopyDescriptors.TryPop(Query))
        {
            Query.Payload.DstDescriptorRange.ManagerProxy->Queues.Copy.Push(Query.Payload);
        }
        _FlushDescriptorManagers();
    }

    void F_DirectX12RHIProcess::_FlushDescriptorManagers()
    {
        ABYTEK_PROFILER_EVENT();
        for (const auto& ContextProxy : GetContextProxies())
        {
            ContextProxy.FastCast<F_DirectX12RHIContextProxy>()->ForEachDescriptorManagerProxy(
                [](const TW_Valid<F_DirectX12RHIDescriptorManagerProxy>& DescriptorManager)
                {
                    DescriptorManager->Flush();
                }
            );
        }
    }

    void F_DirectX12RHIProcess::_TransientUploadBuffers()
    {
        ABYTEK_PROFILER_EVENT();
        for (auto& Buffer : ExecutionData.TransientUploadBuffers)
        {
            const auto& BufferAspect = Buffer.BufferProxy->GetBufferAspect();
            auto D3D12Resource = Buffer.BufferProxy.StaticCast<F_DirectX12RHIResourceProxy>()->GetD3D12Resource();
        
            U8* DataPtr = nullptr;
            HRESULT HR = D3D12Resource->Map(0, nullptr, (void**)&DataPtr);
            ABYTEK_ENGINE_RHI_ASSERT(SUCCEEDED(HR)) << "Cannot map resource";
        
            for (auto& Candidate : Buffer.Candidates)
            {
                memcpy(
                    DataPtr + Candidate.BeginOffsetInBytes,
                    Candidate.BufferDataView.data(),
                    Min(Candidate.GetSizeInBytes(), Candidate.BufferDataView.size())
                );
                Candidate = {};
            }
        
            D3D12_RANGE WrittenRange = { 0, BufferAspect.SizeInBytes };
            D3D12Resource->Unmap(0, &WrittenRange);
        }
    }

    void F_DirectX12RHIProcess::_ExecutePassBatches()
    {
        ABYTEK_PROFILER_EVENT();
        auto& PassBatches = ExecutionData.PassBatches;
        auto& PassBatchesPromise = ExecutionData.PassBatchesPromise;
        
        U32 NumPassBatches = static_cast<U32>(PassBatches.size());
        
        PassBatchesPromise = TS_Unmanaged<F_TaskPromise>()(NumPassBatches);
#ifdef ABYTEK_DEBUG_INFO
        PassBatchesPromise->SetDebugName(ABYTEK_DEBUG_NAME("RHIProcess::PassBatchesPromise"));
#endif
        for (U32 PassBatchIndex = 0; PassBatchIndex < NumPassBatches; PassBatchIndex++)
        {
            H_TaskUtilities::Schedule(
                [this, PassBatchIndex]()
                {
                    _ExecutePassBatch(PassBatchIndex);
                },
                E_TaskPriority::EXTREME,
                ABYTEK_NAME("RHIExecutePassBatch"),
                PassBatchesPromise
            );
        }
    }
    void F_DirectX12RHIProcess::_JoinPassBatches()
    {
        ABYTEK_PROFILER_EVENT();
        const auto& PassBatchesPromise = ExecutionData.PassBatchesPromise;
        ABYTEK_AWAIT PassBatchesPromise;
    }

    void F_DirectX12RHIProcess::_TransientReadbackBuffers()
    {
        ABYTEK_PROFILER_EVENT();
        for (auto& Buffer : ExecutionData.TransientReadbackBuffers)
        {
            const auto& BufferAspect = Buffer.BufferProxy->GetBufferAspect();
            auto D3D12Resource = Buffer.BufferProxy.StaticCast<F_DirectX12RHIResourceProxy>()->GetD3D12Resource();
        
            U8* DataPtr = nullptr;
            D3D12_RANGE D3D12ReadRange = { 0, BufferAspect.SizeInBytes };
            HRESULT HR = D3D12Resource->Map(0, &D3D12ReadRange, (void**)&DataPtr);
            ABYTEK_ENGINE_RHI_ASSERT(SUCCEEDED(HR)) << "Cannot map resource";
        
            for (auto& Candidate : Buffer.Candidates)
            {
                F_RHIBufferDataView BufferDataView(
                    DataPtr + Candidate.BeginOffsetInBytes,
                    DataPtr + Candidate.EndOffsetInBytes
                );
                Candidate.Callback(BufferDataView);
                Candidate = {};
            }
        
            D3D12Resource->Unmap(0, nullptr);
        }
    }

    void F_DirectX12RHIProcess::_CleanCommandListManagers()
    {
        ABYTEK_PROFILER_EVENT();
        
        for (const auto& ContextProxy : GetContextProxies())
        {
            auto CastedContextProxy = ContextProxy.FastCast<F_DirectX12RHIContextProxy>();
            CastedContextProxy->ForEachCommandListManager(
                [](const TW_Valid<F_DirectX12RHICommandListManager>& CommandListManager)
                {
                    CommandListManager->ResetAllocators();
                    CommandListManager->ResetLists();
                }
            );
        }
    }

    void F_DirectX12RHIProcess::_ExecutePassBatch(U32 PassBatchIndex)
    {
        ABYTEK_PROFILER_EVENT();
        auto& PassBatches = ExecutionData.PassBatches;
        
        auto& PassBatch = PassBatches[PassBatchIndex];
            
        // 
        auto ContextProxy = PassBatch.CommandQueueProxy->GetContextProxy().FastCast<F_DirectX12RHIContextProxy>();
        
        // Acquire common objects if needed
        TW<F_DirectX12RHICommandQueueProxy> CommandQueueProxy = PassBatch.CommandQueueProxy;
        TW<F_DirectX12RHICommandListManager> CommandListManager;
        TW<F_DirectX12RHISynchronizer> Synchronizer;
        {
            auto CommandListType = CommandQueueProxy->GetCommandListType();
            CommandListManager = ContextProxy->QueryCommandListManager(CommandListType);
            Synchronizer = ContextProxy->QuerySynchronizer(CommandListType);
        }
            
        // Wait for dependencies to submit
        {
            ABYTEK_PROFILER_EVENT_NAMED("RHIExecutePassBatch::WaitForDependenciesToSubmit");
            for (auto DependencyIndex : PassBatch.DependencyIndices)
            {
                const auto& DependentPassBatch = PassBatches[DependencyIndex];
                const auto& DependentBackExecutionRange = DependentPassBatch.ExecutionRanges.back();
                while (!DependentBackExecutionRange.GetAtomic_IsSubmitted().load(boost::memory_order_acquire))
                {
                    H_TaskUtilities::Switch();
                }
            }
        }
            
        // Wait for dependencies to done
        {
            ABYTEK_PROFILER_EVENT_NAMED("RHIExecutePassBatch::SyncDependencies");
            for (auto DependencyIndex : PassBatch.DependencyIndices)
            {
                const auto& DependentPassBatch = PassBatches[DependencyIndex];
                const auto& DependentBackExecutionRange = DependentPassBatch.ExecutionRanges.back();
                if (DependentBackExecutionRange.ShouldSignalFence)
                {
                    auto DependentContextProxy = DependentPassBatch.CommandQueueProxy->GetContextProxy().FastCast<F_DirectX12RHIContextProxy>();
                    auto DependentSynchronizer = DependentContextProxy->QuerySynchronizer(
                        DependentPassBatch.CommandQueueProxy->GetCommandListType()
                    );
                    if (PassBatch.HasCommands_Begin)
                    {
                        if (DependentPassBatch.CommandQueueProxy != PassBatch.CommandQueueProxy)
                        {
                            DependentSynchronizer->WaitOnGPUSide(
                                DependentBackExecutionRange.GetAtomic_FenceValueOnCompletion().load(boost::memory_order_acquire),
                                CommandQueueProxy
                            );
                        }
                    }
                    else
                    {
                        DependentSynchronizer->WaitOnCPUSide(
                            DependentBackExecutionRange.GetAtomic_FenceValueOnCompletion().load(boost::memory_order_acquire)
                        );
                    }
                }
            }
        }
        
        // Execute execution ranges
        {
            ABYTEK_PROFILER_EVENT_NAMED("RHIExecutePassBatch::ExecuteRanges");
            for (const auto& ExecutionRange : PassBatch.ExecutionRanges)
            {
                // Pop list and allocator
                Microsoft::WRL::ComPtr<ID3D12CommandAllocator> D3D12CommandAllocator = nullptr;
                Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> D3D12CommandList = nullptr;
                if (ExecutionRange.Type == E_DirectX12RHIExecutionRangeType::USE_COMMAND_LIST)
                {
                    D3D12CommandAllocator = CommandListManager->PopAllocator();
                    D3D12CommandList = CommandListManager->PopList(D3D12CommandAllocator);
                
#ifdef ABYTEK_DEBUG_INFO
                    D3D12CommandList->SetName((ABYTEK_TEXT("PassBatch[") + ToText(PassBatchIndex) + ABYTEK_TEXT("]")).c_str());
#endif
                }
                
                // Execution barriers before
                if (D3D12CommandList)
                {
                    H_DirectX12RHIResourceBarrierProxy::AddResourceBarrier(
                        D3D12CommandList,
                        ExecutionRange.D3D12ResourceBarrierProxies_Before
                    );
                }
            
                // Execute passes
                {
                    ABYTEK_PROFILER_EVENT_NAMED("RHIExecutePassBatch::ExecuteRanges::ExecutePasses");
                    F_DirectX12RHIPassProxyExtensionExecuteParams ExecuteParams;
                    ExecuteParams.PassBatchIndex = PassBatchIndex;
                    ExecuteParams.CommandQueueProxy = CommandQueueProxy;
                    ExecuteParams.D3D12CommandList = D3D12CommandList;

#ifdef ABYTEK_ENGINE_RHI_ENABLE_CAPTURE
                    F_DirectX12RHIEventStack EventStack;
                    EventStack.D3D12CommandList = D3D12CommandList;
#endif
                    
                    for (U32 LocalPassProxyIndex = ExecutionRange.BeginLocalPassExtensionIndex; LocalPassProxyIndex < ExecutionRange.EndLocalPassExtensionIndex; ++LocalPassProxyIndex)
                    {
                        const auto& PassProxy = PassBatch.PassProxies[LocalPassProxyIndex];
                        auto PassProxyExtension = PassProxy.DynamicCast<A_DirectX12RHIPassProxyExtension>();
                
                        // Begin events
#ifdef ABYTEK_ENGINE_RHI_ENABLE_CAPTURE
                        if (EventStack)
                        {
                            EventStack.Push(PassProxyExtension->GetCaptureEventStatesToBegin());
                        }
#endif
                        
                        // Pass proxy barriers before
                        if (D3D12CommandList)
                        {
                            H_DirectX12RHIResourceBarrierProxy::AddResourceBarrier(
                                D3D12CommandList,
                                PassProxyExtension->GetResourceBarrierProxies_Before()
                            );
                        }
                        
                        // Execute pass proxy
                        PassProxyExtension->Execute(ExecuteParams);
                        
                        // Pass proxy barriers after
                        if (D3D12CommandList)
                        {
                            H_DirectX12RHIResourceBarrierProxy::AddResourceBarrier(
                                D3D12CommandList,
                                PassProxyExtension->GetResourceBarrierProxies_After()
                            );
                        }
                        
                        // End events
#ifdef ABYTEK_ENGINE_RHI_ENABLE_CAPTURE
                        if (EventStack)
                        {
                            EventStack.Pop(PassProxyExtension->GetCaptureEventStatesToEnd());
                        }
#endif
                    }
                }
            
                // Execution barriers after
                if (D3D12CommandList)
                {
                    H_DirectX12RHIResourceBarrierProxy::AddResourceBarrier(
                        D3D12CommandList,
                        ExecutionRange.D3D12ResourceBarrierProxies_After
                    );
                }
                
                // Execute command list 
                if (ExecutionRange.Type == E_DirectX12RHIExecutionRangeType::USE_COMMAND_LIST)
                {
                    D3D12CommandList->Close();
                    CommandQueueProxy->ExecuteCommandList(D3D12CommandList);
                }
            
                // Push list and allocator
                if (ExecutionRange.Type == E_DirectX12RHIExecutionRangeType::USE_COMMAND_LIST)
                {
                    CommandListManager->PushList(D3D12CommandList);
                    CommandListManager->PushAllocator(D3D12CommandAllocator);
                }
            
                // Signal fence
                if (ExecutionRange.ShouldSignalFence)
                {
                    U64 FenceValueOnCompletion = Synchronizer->AllocateNewValue();
                    Synchronizer->SignalFromGPUSide(FenceValueOnCompletion, CommandQueueProxy);
                
                    ExecutionRange.GetAtomic_FenceValueOnCompletion().store(FenceValueOnCompletion, boost::memory_order_release);
                }
                
                // Signal submitted
                ExecutionRange.GetAtomic_IsSubmitted().store(true, boost::memory_order_release);
            
                // Sync GPU
                if (ExecutionRange.ShouldSyncGPU)
                {
                    ABYTEK_PROFILER_EVENT_NAMED("RHIExecutePassBatch::ExecuteRanges::SyncGPU");
                    U64 FenceValueOnCompletion = ExecutionRange.GetAtomic_FenceValueOnCompletion().load(boost::memory_order_acquire);
                    Synchronizer->WaitOnCPUSide(FenceValueOnCompletion);
                } 
            }
        }
    }

    void F_DirectX12RHIProcess::_BeginLateExecuteForViewportProxies()
    {
        ABYTEK_PROFILER_EVENT();
        U32 NumViewportProxies = LateExecutionData.ViewportProxies.size();
        for (U32 ViewportProxyIndex = 0; ViewportProxyIndex < NumViewportProxies; ++ViewportProxyIndex)
        {
            const auto& ViewportProxy = LateExecutionData.ViewportProxies[ViewportProxyIndex];
            auto CastedViewportProxy = ViewportProxy.FastCast<F_DirectX12RHIViewportProxy>();
            CastedViewportProxy->LockDXGISwapChain();
        }
        for (U32 ViewportProxyIndex = 0; ViewportProxyIndex < NumViewportProxies; ++ViewportProxyIndex)
        {
            const auto& ViewportProxy = LateExecutionData.ViewportProxies[ViewportProxyIndex];
            auto CastedViewportProxy = ViewportProxy.FastCast<F_DirectX12RHIViewportProxy>();

            auto CastedContextProxy = ViewportProxy->GetContextProxy().FastCast<F_DirectX12RHIContextProxy>();
            auto CommandQueueProxy = CastedContextProxy->GetCommandQueueProxy_DIRECT();
            auto Synchronizer = CastedContextProxy->GetSynchronizer_DIRECT();
            
            U64 FenceValueOnCompletion = Synchronizer->AllocateNewValue();
            LateExecutionData.ViewportPresentFenceValuesOnCompletion.push_back(FenceValueOnCompletion);
            
            auto DXGISwapchain = CastedViewportProxy->GetDXGISwapChain();
            
            DXGISwapchain->Present(
                0, 
                DXGI_PRESENT_ALLOW_TEARING
            );
            Synchronizer->SignalFromGPUSide(FenceValueOnCompletion, CommandQueueProxy);
        }
    }
    void F_DirectX12RHIProcess::_EndLateExecuteForViewportProxies()
    {
        ABYTEK_PROFILER_EVENT();
        U32 NumViewportProxies = static_cast<U32>(LateExecutionData.ViewportProxies.size());
        for (U32 ViewportProxyIndex = 0; ViewportProxyIndex < NumViewportProxies; ++ViewportProxyIndex)
        {
            const auto& ViewportProxy = LateExecutionData.ViewportProxies[ViewportProxyIndex];
            auto CastedViewportProxy = ViewportProxy.FastCast<F_DirectX12RHIViewportProxy>();
            
            auto CastedContextProxy = ViewportProxy->GetContextProxy().FastCast<F_DirectX12RHIContextProxy>();
            auto CommandQueueProxy = CastedContextProxy->GetCommandQueueProxy_DIRECT();
            auto Synchronizer = CastedContextProxy->GetSynchronizer_DIRECT();
            
            U64 FenceValueOnCompletion = LateExecutionData.ViewportPresentFenceValuesOnCompletion[ViewportProxyIndex];
            Synchronizer->WaitOnCPUSide(FenceValueOnCompletion);
        }
        for (U32 ViewportProxyIndex = 0; ViewportProxyIndex < NumViewportProxies; ++ViewportProxyIndex)
        {
            const auto& ViewportProxy = LateExecutionData.ViewportProxies[ViewportProxyIndex];
            auto CastedViewportProxy = ViewportProxy.FastCast<F_DirectX12RHIViewportProxy>();
            CastedViewportProxy->UnlockDXGISwapChain();
        }
    }
}
#endif