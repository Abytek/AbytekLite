#include "Abytek/DirectX12/RHIBindGroup.hpp"

#include "Abytek/RHIContext.hpp"
#include "Abytek/DirectX12/RHIBindGroupProxy.hpp"
#include "Abytek/DirectX12/RHIDevice.hpp"
#include "Abytek/RHISubsystem.hpp"
#include "Abytek/DirectX12/RHISubsystemProxy.hpp"
#include "Abytek/DirectX12/RHIDescriptorManager.hpp"
#include "Abytek/DirectX12/RHIBindGroupTemplateRuntime.hpp"
#include "Abytek/DirectX12/RHIContext.hpp"
#include "Abytek/DirectX12/RHIProcess.hpp"
#include "Abytek/DirectX12/RHIResourceView.hpp"
#include "Abytek/DirectX12/RHISampler.hpp"
#include "Abytek/DirectX12Shared/RHIBindGroupTemplate.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void H_DirectX12RHIVertexBufferBinding::AppendSubresourceBindingSet(
        const F_RHIVertexBufferBinding& VertexBufferBinding, 
        F_DirectX12RHISubresourceBindingSet& Result
    )
    {
        Result.push_back(
            F_DirectX12RHISubresourceBinding::MakeCore(    
                F_DirectX12RHISubresourceReference::Make(VertexBufferBinding.Resource.Weak(), 0),
                F_RHIResourceAccess::MakeVertexBuffer()
            )
        );
    }
    void H_DirectX12RHIIndexBufferBinding::AppendSubresourceBindingSet(
        const F_RHIIndexBufferBinding& IndexBufferBinding, 
        F_DirectX12RHISubresourceBindingSet& Result
    )
    {
        Result.push_back(
            F_DirectX12RHISubresourceBinding::MakeCore(    
                F_DirectX12RHISubresourceReference::Make(IndexBufferBinding.Resource.Weak(), 0),
                F_RHIResourceAccess::MakeIndexBuffer()
            )
        );
    }
    
    void F_DirectX12RHIBindGroup::Build(const F_RHIBindGroupBuildParams& BuildParams)
    {
        A_RHIBindGroup::Build(BuildParams);
        GetProxy().FastCast<F_DirectX12RHIBindGroupProxy>()->LateBuildBindGroup_DirectX12();
    }
    void F_DirectX12RHIBindGroup::Release()
    {
        auto CastedProcess = GetContext()->GetCurrentProcess().FastCast<F_DirectX12RHIProcess>();
        auto CastedBindGroupTemplate = GetTemplateRuntime()->GetTemplate().FastCast<F_DirectX12SharedRHIBindGroupTemplate>();

        const auto& CompiledData = CastedBindGroupTemplate->GetCompiledData();

        auto& BoundDescriptorBindings = _Allocation.DescriptorBindings;
        U32 NumBoundDescriptorBindings = BoundDescriptorBindings.size();

        auto& BoundUniformDataBindings = _Allocation.UniformDataBindings;
        const auto& CompiledUniformDataBindings = CompiledData.UniformDataBindings;
        U32 NumBoundUniformDataBindings = BoundUniformDataBindings.size();
        
        if (NumBoundDescriptorBindings > 0)
        {
            for (U32 DescriptorBindingIndex = 0; DescriptorBindingIndex < NumBoundDescriptorBindings; ++DescriptorBindingIndex)
            {
                auto& BoundDescriptorBinding = BoundDescriptorBindings[DescriptorBindingIndex];
            
                if (BoundDescriptorBinding.DescriptorAllocation)
                {
                    DirectX12RHIProcessQueries::Compile::F_DeallocateDescriptors Query;
                    Query.DescriptorAllocation = BoundDescriptorBinding.DescriptorAllocation;
                    CastedProcess->Queues.Compile.DeallocateDescriptors.Push(Query);
                }
            }
        }
        
        if (NumBoundUniformDataBindings > 0)
        {
            for (U32 UniformDataBindingIndex = 0; UniformDataBindingIndex < NumBoundUniformDataBindings; ++UniformDataBindingIndex)
            {
                auto& BoundUniformDataBinding = BoundUniformDataBindings[UniformDataBindingIndex];
                const auto& CompiledUniformDataBinding = CompiledUniformDataBindings[UniformDataBindingIndex];
            
                if (CompiledUniformDataBinding.UseRootConstants)
                {
                    continue;
                }
            
                if (BoundUniformDataBinding.ConstantDataRange)
                {
                    BoundUniformDataBinding.ConstantDataRange.Page->Deallocate(BoundUniformDataBinding.ConstantDataRange);
                    BoundUniformDataBinding.ConstantDataRange = {};
                }
            }
        }

#ifdef ABYTEK_ENGINE_RHI_ENABLE_ASSERTIONS
        _IsCommitted.clear(boost::memory_order_relaxed);
#endif

        _Allocation = {};
        A_RHIBindGroup::Release();
    }

    void F_DirectX12RHIBindGroup::Commit()
    { 
        A_RHIBindGroup::Commit();
        
        _ReallocateIfNeeded();
        _CopyDescriptors();
        _UploadConstantData();
        _SendCommitToProxy();
    }
    void F_DirectX12RHIBindGroup::EncodeData(void* OutData)
    {
        A_RHIBindGroup::EncodeData(OutData);
        
        auto CastedTemplate = GetTemplateRuntime()->GetTemplate().FastCast<F_DirectX12SharedRHIBindGroupTemplate>();
        const auto& CompiledData = CastedTemplate->GetCompiledData();
        
        ABYTEK_ENGINE_RHI_ASSERT(CastedTemplate->GetAllowBindless()) << "Cannot encode data on non bindless template";
#ifdef ABYTEK_ENGINE_RHI_ENABLE_ASSERTIONS
        ABYTEK_ENGINE_RHI_ASSERT(_IsCommitted.test(boost::memory_order_acquire)) << "This bind group was not committed";
#endif
        
        const auto& SlotData = GetSlotData();
        const auto& SlotTemplates = CastedTemplate->GetSlots();
        
        for (const auto& UniformDataBinding : CompiledData.UniformDataBindings)
        {
            const auto& SlotTemplate = SlotTemplates[UniformDataBinding.SlotIndex];
            memcpy(
                ((U8*)OutData) + UniformDataBinding.EncodedDataOffsetInBytes,
                SlotData.GetUniformData(UniformDataBinding.SlotIndex).GetPayloadPtr(), 
                SlotTemplate.UniformDataSizeInBytes
            );
        }  
        
        U32 NumDescriptorBindings = CompiledData.DescriptorBindings.size();
        for (U32 DescriptorBindingIndex = 0; DescriptorBindingIndex < NumDescriptorBindings; ++DescriptorBindingIndex)
        {
            const auto& CompiledDescriptorBinding = CompiledData.DescriptorBindings[DescriptorBindingIndex];
            const auto& BoundDescriptorBinding = _Allocation.DescriptorBindings[DescriptorBindingIndex];
            ABYTEK_ENGINE_RHI_ASSERT(BoundDescriptorBinding.DescriptorAllocation) << "Invalid descriptor";
            (*(U32*)(((U8*)OutData) + CompiledDescriptorBinding.EncodedDataOffsetInBytes)) = BoundDescriptorBinding.DescriptorAllocation.BeginOffset;
            (*(U32*)(((U8*)OutData) + CompiledDescriptorBinding.EncodedDataOffsetInBytes + sizeof(U32))) = BoundDescriptorBinding.DescriptorAllocation.EndOffset;
        }
    }

    F_RHIResourceViewBindlessRange F_DirectX12RHIBindGroup::GetResourceViewBindlessRange(U32 Index) const
    {
        F_RHIResourceViewBindlessRange Result = A_RHIBindGroup::GetResourceViewBindlessRange(Index);
        
        auto CastedBindGroupTemplate = GetTemplateRuntime()->GetTemplate().FastCast<F_DirectX12SharedRHIBindGroupTemplate>();
        const auto& CompiledData = CastedBindGroupTemplate->GetCompiledData();
        const auto& SlotMap = CompiledData.SlotMaps[Index];
        
        const auto& DescriptorAllocation = _Allocation.DescriptorBindings[SlotMap.DescriptorBindingIndex].DescriptorAllocation;
        Result.BeginIndex = DescriptorAllocation.BeginOffset;
        Result.EndIndex = DescriptorAllocation.EndOffset;
        return Result;
    }
    F_RHISamplerBindlessRange F_DirectX12RHIBindGroup::GetSamplerBindlessRange(U32 Index) const
    {
        F_RHISamplerBindlessRange Result = A_RHIBindGroup::GetSamplerBindlessRange(Index);
        
        auto CastedBindGroupTemplate = GetTemplateRuntime()->GetTemplate().FastCast<F_DirectX12SharedRHIBindGroupTemplate>();
        const auto& CompiledData = CastedBindGroupTemplate->GetCompiledData();
        const auto& SlotMap = CompiledData.SlotMaps[Index];
        
        const auto& DescriptorAllocation = _Allocation.DescriptorBindings[SlotMap.DescriptorBindingIndex].DescriptorAllocation;
        Result.BeginIndex = DescriptorAllocation.BeginOffset;
        Result.EndIndex = DescriptorAllocation.EndOffset;
        return Result;
    }

    void F_DirectX12RHIBindGroup::_ReallocateIfNeeded()
    {
        auto CastedProcess = GetContext()->GetCurrentProcess().FastCast<F_DirectX12RHIProcess>();
        auto CastedBindGroupTemplate = GetTemplateRuntime()->GetTemplate().FastCast<F_DirectX12SharedRHIBindGroupTemplate>();
        auto CastedContext = GetContext().FastCast<F_DirectX12RHIContext>();
        auto ConstantDataManager = CastedContext->GetConstantDataManager();

        const auto& SlotData = GetSlotData();

        auto& Allocation = _Allocation;
        const auto& CompiledData = CastedBindGroupTemplate->GetCompiledData();

        auto& BoundDescriptorBindings = Allocation.DescriptorBindings;
        const auto& CompiledDescriptorBindings = CompiledData.DescriptorBindings;
        
        U32 NumDescriptorBindings = CompiledDescriptorBindings.size();
        ABYTEK_ENGINE_RHI_ASSERT(
            (BoundDescriptorBindings.size() == NumDescriptorBindings)
            || (BoundDescriptorBindings.size() == 0)
        ) << "Mismatched num descriptor bindings";
        
        BoundDescriptorBindings.resize(NumDescriptorBindings);

        auto& BoundUniformDataBindings = Allocation.UniformDataBindings;
        const auto& CompiledUniformDataBindings = CompiledData.UniformDataBindings;
        
        U32 NumUniformDataBindings = CompiledUniformDataBindings.size();
        ABYTEK_ENGINE_RHI_ASSERT(
            (BoundUniformDataBindings.size() == NumUniformDataBindings)
            || (BoundUniformDataBindings.size() == 0)
        ) << "Mismatched num uniform data bindings";

        BoundUniformDataBindings.resize(NumUniformDataBindings);
        
        // Ensure descriptor allocations
        for (U32 DescriptorBindingIndex = 0; DescriptorBindingIndex < NumDescriptorBindings; ++DescriptorBindingIndex)
        {
            const auto& CompiledDescriptorBinding = CompiledDescriptorBindings[DescriptorBindingIndex];
            auto& BoundDescriptorBinding = BoundDescriptorBindings[DescriptorBindingIndex];

            // Calculate target num descriptors
            U32 TargetNumDescriptors = CompiledDescriptorBinding.NumFixedDescriptors;
            if (CompiledDescriptorBinding.DynamicSizeSlotIndex != ~U32(0))
            {
                switch (SlotData.GetType(CompiledDescriptorBinding.DynamicSizeSlotIndex))
                {
                case E_RHIBindGroupSlotType::RESOURCE_VIEW_SET:
                    TargetNumDescriptors += SlotData.GetResourceViewSet(CompiledDescriptorBinding.DynamicSizeSlotIndex).ResourceViewSet.size();
                    break;
                case E_RHIBindGroupSlotType::SAMPLER_SET:
                    TargetNumDescriptors += SlotData.GetSamplerSet(CompiledDescriptorBinding.DynamicSizeSlotIndex).SamplerSet.size();
                    break;
                default:
                    ABYTEK_ENGINE_RHI_ASSERT(false) << "Invalid slot type, expecting resource view set or sampler set";
                }
            }

            // Reallocate if needed
            if (BoundDescriptorBinding.DescriptorAllocation.GetSize() != TargetNumDescriptors)
            {
                // Deallocate
                if (BoundDescriptorBinding.DescriptorAllocation)
                {
                    DirectX12RHIProcessQueries::Compile::F_DeallocateDescriptors Query;
                    Query.DescriptorAllocation = BoundDescriptorBinding.DescriptorAllocation;
                    CastedProcess->Queues.Compile.DeallocateDescriptors.Push(Query);
                }

                // Allocate
                TW<F_DirectX12RHIDescriptorManager> DescriptorManager = CastedContext->QueryDescriptorManager(
                    CompiledDescriptorBinding.HeapType,
                    true // CastedBindGroupTemplate->GetAllowBindless()
                );
                BoundDescriptorBinding.DescriptorAllocation = DescriptorManager->Allocate(TargetNumDescriptors);
            }
        }
        
        // Setup uniform data bindings
        for (U32 UniformDataBindingIndex = 0; UniformDataBindingIndex < NumUniformDataBindings; ++UniformDataBindingIndex)
        {
            auto& BoundUniformDataBinding = BoundUniformDataBindings[UniformDataBindingIndex];
            const auto& CompiledUniformDataBinding = CompiledUniformDataBindings[UniformDataBindingIndex];
            
            if (CompiledUniformDataBinding.UseRootConstants)
            {
                continue;
            }
            
            if (BoundUniformDataBinding.ConstantDataRange)
            {
                continue;
            } 
            
            ABYTEK_ENGINE_RHI_ASSERT(
                CompiledUniformDataBinding.SizeInBytes % DirectX12SharedAPIWrapper::MinConstantBufferRegionAlignment == 0
            );
            BoundUniformDataBinding.ConstantDataRange = ConstantDataManager->Allocate(CompiledUniformDataBinding.SizeInBytes);
        }
    }
    void F_DirectX12RHIBindGroup::_SendCommitToProxy()
    {
        auto CastedProcess = GetContext()->GetCurrentProcess().FastCast<F_DirectX12RHIProcess>();
        
        DirectX12RHIProcessQueries::Execution::F_SendCommitToBindGroupProxy Query;
        Query.BindGroupProxy = GetProxy().FastCast<A_RHIBindGroupProxy>();
        Query.SlotDataProxy = GetSlotData();
        Query.AllocationProxy = _Allocation.ConvertToProxy();
        CastedProcess->Queues.Execution.SendCommitToBindGroupProxy.Push(ABYTEK_MOVE(Query));
    }
    void F_DirectX12RHIBindGroup::_CopyDescriptors()
    {
        auto CastedProcess = GetContext()->GetCurrentProcess().FastCast<F_DirectX12RHIProcess>();
        auto CastedBindGroupTemplate = GetTemplateRuntime()->GetTemplate().FastCast<F_DirectX12SharedRHIBindGroupTemplate>();
        auto CastedContext = GetContext().FastCast<F_DirectX12RHIContext>();
        
        auto DescriptorManager_GPU_CBV_SRV_UAV = CastedContext->GetDescriptorManager_GPU_CBV_SRV_UAV().Weak();
        auto DescriptorManager_GPU_SAMPLER = CastedContext->GetDescriptorManager_GPU_SAMPLER().Weak();
        
        auto& CopyQueue = CastedProcess->Queues.Execution.CopyDescriptorsForBindGroupProxy;

        const auto& CompiledData = CastedBindGroupTemplate->GetCompiledData();

        const auto& SlotData = GetSlotData();
        U32 NumSlots = SlotData.GetSize();

        auto& BoundDescriptorBindings = _Allocation.DescriptorBindings;
        
        DirectX12RHIProcessQueries::Execution::F_CopyDescriptorsForBindGroupProxy Query;

        // Record descriptor copies
        {
            for (U32 SlotIndex = 0; SlotIndex < NumSlots; ++SlotIndex)
            {
                const auto& SlotMap = CompiledData.SlotMaps[SlotIndex];
                switch (SlotData.GetType(SlotIndex))
                {
                case E_RHIBindGroupSlotType::RESOURCE_VIEW:
                    {
                        const auto& ResourceView = SlotData.GetResourceView(SlotIndex).ResourceView;
                        ABYTEK_ENGINE_RHI_ASSERT(ResourceView) << "Invalid resource view at slot index: " << SlotIndex;
                    
                        const auto& BoundDescriptorBinding = BoundDescriptorBindings[SlotMap.DescriptorBindingIndex];

                        F_DirectX12RHIDescriptorAllocation DstDescriptor = BoundDescriptorBinding.DescriptorAllocation;
                        DstDescriptor.BeginOffset += SlotMap.DescriptorIndexFromRootParameterStart;
                        DstDescriptor.EndOffset = DstDescriptor.BeginOffset + 1;
                        F_DirectX12RHIDescriptorAllocation SrcDescriptor = ResourceView.FastCast<F_DirectX12RHIResourceView>()->GetDescriptorAllocation();

                        Query.DescriptorAllocationPairs.push_back({
                            DstDescriptor,
                            SrcDescriptor
                        });
                    }
                    break;
                case E_RHIBindGroupSlotType::RESOURCE_VIEW_SET:
                    {
                        const auto& ResourceViewSet = SlotData.GetResourceViewSet(SlotIndex).ResourceViewSet;
                        U32 NumResourceViews = ResourceViewSet.size();
                        for (U32 ResourceViewIndex = 0; ResourceViewIndex < NumResourceViews; ++ResourceViewIndex)
                        {
                            const auto& ResourceView = ResourceViewSet[ResourceViewIndex];
                            ABYTEK_ENGINE_RHI_ASSERT(ResourceView) << "Invalid resource view at slot index: " << SlotIndex << ", element index: " << ResourceViewIndex;
                            
                            const auto& BoundDescriptorBinding = BoundDescriptorBindings[SlotMap.DescriptorBindingIndex];

                            F_DirectX12RHIDescriptorAllocation DstDescriptor = BoundDescriptorBinding.DescriptorAllocation;
                            DstDescriptor.BeginOffset += SlotMap.DescriptorIndexFromRootParameterStart;
                            DstDescriptor.BeginOffset += ResourceViewIndex;
                            DstDescriptor.EndOffset = DstDescriptor.BeginOffset + 1;
                            F_DirectX12RHIDescriptorAllocation SrcDescriptor = ResourceView.FastCast<F_DirectX12RHIResourceView>()->GetDescriptorAllocation();
                        
                            Query.DescriptorAllocationPairs.push_back({
                                DstDescriptor,
                                SrcDescriptor
                            });
                        }
                    }
                    break;
                case E_RHIBindGroupSlotType::SAMPLER:
                    {
                        const auto& Sampler = SlotData.GetSampler(SlotIndex).Sampler;
                        ABYTEK_ENGINE_RHI_ASSERT(Sampler) << "Invalid sampler at slot index: " << SlotIndex;
                    
                        const auto& BoundDescriptorBinding = BoundDescriptorBindings[SlotMap.DescriptorBindingIndex];

                        F_DirectX12RHIDescriptorAllocation DstDescriptor = BoundDescriptorBinding.DescriptorAllocation;
                        DstDescriptor.BeginOffset += SlotMap.DescriptorIndexFromRootParameterStart;
                        DstDescriptor.EndOffset = DstDescriptor.BeginOffset + 1;
                        F_DirectX12RHIDescriptorAllocation SrcDescriptor = Sampler.FastCast<F_DirectX12RHISampler>()->GetDescriptorAllocation();
                        
                        Query.DescriptorAllocationPairs.push_back({
                            DstDescriptor,
                            SrcDescriptor
                        });
                    }
                    break;
                case E_RHIBindGroupSlotType::SAMPLER_SET:
                    {
                        const auto& SamplerSet = SlotData.GetSamplerSet(SlotIndex).SamplerSet;
                        U32 NumSamplers = SamplerSet.size();
                        for (U32 SamplerIndex = 0; SamplerIndex < NumSamplers; ++SamplerIndex)
                        {
                            const auto& Sampler = SamplerSet[SamplerIndex];
                            ABYTEK_ENGINE_RHI_ASSERT(Sampler) << "Invalid sampler at slot index: " << SlotIndex << ", element index: " << SamplerIndex;
                        
                            const auto& BoundDescriptorBinding = BoundDescriptorBindings[SlotMap.DescriptorBindingIndex];

                            F_DirectX12RHIDescriptorAllocation DstDescriptor = BoundDescriptorBinding.DescriptorAllocation;
                            DstDescriptor.BeginOffset += SlotMap.DescriptorIndexFromRootParameterStart;
                            DstDescriptor.BeginOffset += SamplerIndex;
                            DstDescriptor.EndOffset = DstDescriptor.BeginOffset + 1;
                            F_DirectX12RHIDescriptorAllocation SrcDescriptor = Sampler.FastCast<F_DirectX12RHISampler>()->GetDescriptorAllocation();
                        
                            Query.DescriptorAllocationPairs.push_back({
                                DstDescriptor,
                                SrcDescriptor
                            });
                        }
                    }
                    break;
                case E_RHIBindGroupSlotType::VERTEX_BUFFER:
                    break;
                case E_RHIBindGroupSlotType::INDEX_BUFFER:
                    break;
                case E_RHIBindGroupSlotType::RTV:
                    break;
                case E_RHIBindGroupSlotType::DSV:
                    break;
                case E_RHIBindGroupSlotType::UNIFORM_DATA:
                    break;
                default:
                    ABYTEK_ENGINE_RHI_ASSERT(false) << "Unknown bind group slot type";
                    break;
                }
            }
        }
        
        if (Query.DescriptorAllocationPairs.size() > 0)
        {
            CopyQueue.Push(ABYTEK_MOVE(Query));
        }
    }
    void F_DirectX12RHIBindGroup::_UploadConstantData()
    {
        const auto& Context = GetContext();
        auto CastedProcess = Context->GetCurrentProcess().FastCast<F_DirectX12RHIProcess>();
        const auto& Arena = CastedProcess->GetArena();
        auto CastedBindGroupTemplate = GetTemplateRuntime()->GetTemplate().FastCast<F_DirectX12SharedRHIBindGroupTemplate>();
        
        const auto& CompiledData = CastedBindGroupTemplate->GetCompiledData();

        const auto& SlotData = GetSlotData();

        auto& CompiledUniformDataBindings = CompiledData.UniformDataBindings;
        auto& BoundUniformDataBindings = _Allocation.UniformDataBindings;
        U32 NumUniformDataBindings = CompiledUniformDataBindings.size();
        ABYTEK_ENGINE_RHI_ASSERT(CompiledUniformDataBindings.size() == BoundUniformDataBindings.size());
        for (U32 UniformDataBindingIndex = 0; UniformDataBindingIndex < NumUniformDataBindings; ++UniformDataBindingIndex)
        {
            const auto& CompiledUniformDataBinding = CompiledUniformDataBindings[UniformDataBindingIndex];
            if (CompiledUniformDataBinding.UseRootConstants)
            {
                continue;
            }
            
            const auto& BoundUniformDataBinding = BoundUniformDataBindings[UniformDataBindingIndex];
            
            const auto& UniformData = SlotData.GetUniformData(CompiledUniformDataBinding.SlotIndex);
            auto BufferDataView = Arena->CacheData({
                (U8*)UniformData.GetPayloadPtr(),
                (Sz)UniformData.SizeInBytes
            });
            
            DirectX12RHIProcessQueries::Compile::F_UploadConstantData Query;
            Query.ConstantDataRange = BoundUniformDataBinding.ConstantDataRange;
            Query.BufferDataView = BufferDataView;
            CastedProcess->Queues.Compile.UploadConstantData.Push(Query);
        }
        
    }

    void F_DirectX12RHIBindGroup::AppendSubresourceBindingSet(F_DirectX12RHISubresourceBindingSet& Result)
    {
        ABYTEK_ENGINE_RHI_ASSERT(EverCommitted()) 
            << "RHI bind group was not committed"
#ifdef ABYTEK_DEBUG_INFO
            << ", name: " << *GetDebugName()
#endif
        ;
        
        auto CastedBindGroupTemplate = GetTemplateRuntime()->GetTemplate().FastCast<F_DirectX12SharedRHIBindGroupTemplate>();
        const auto& Allocation = GetAllocation();
        const auto& CompiledData = CastedBindGroupTemplate->GetCompiledData();
        
        const auto& SlotData = GetSlotData();
        const auto& SlotTemplates = CastedBindGroupTemplate->GetSlots();
        U32 NumSlots = SlotData.GetSize();
        
        for (U32 SlotIndex = 0; SlotIndex < NumSlots; ++SlotIndex)
        {
            const auto& SlotTemplate = SlotTemplates[SlotIndex];
            switch (SlotTemplate.Type)
            {
            case E_RHIBindGroupSlotType::RESOURCE_VIEW:
                SlotData.GetResourceView(SlotIndex).ResourceView.FastCast<F_DirectX12RHIResourceView>()->AppendSubresourceBindingSet(Result);
                break;
            case E_RHIBindGroupSlotType::RESOURCE_VIEW_SET:
                for (const auto& ResourceView : SlotData.GetResourceViewSet(SlotIndex).ResourceViewSet)
                {
                    ResourceView.FastCast<F_DirectX12RHIResourceView>()->AppendSubresourceBindingSet(Result);
                }
                break;
            case E_RHIBindGroupSlotType::SAMPLER:
                break;
            case E_RHIBindGroupSlotType::SAMPLER_SET:
                break;
            case E_RHIBindGroupSlotType::RTV:
                SlotData.GetRTV(SlotIndex).RTV.FastCast<F_DirectX12RHIResourceView>()->AppendSubresourceBindingSet(Result);
                break;
            case E_RHIBindGroupSlotType::DSV:
                SlotData.GetDSV(SlotIndex).DSV.FastCast<F_DirectX12RHIResourceView>()->AppendSubresourceBindingSet(Result);
                break;
            case E_RHIBindGroupSlotType::VERTEX_BUFFER:
                H_DirectX12RHIVertexBufferBinding::AppendSubresourceBindingSet(
                    SlotData.GetVertexBuffer(SlotIndex).VertexBufferBinding,
                    Result
                );
                break;
            case E_RHIBindGroupSlotType::INDEX_BUFFER:
                H_DirectX12RHIIndexBufferBinding::AppendSubresourceBindingSet(
                    SlotData.GetIndexBuffer(SlotIndex).IndexBufferBinding,
                    Result
                );
                break;
            case E_RHIBindGroupSlotType::UNIFORM_DATA:
                break;
            default:
                ABYTEK_ENGINE_RHI_ASSERT(false) << "Invalid bind group slot type";
                break;
            }
        }
        
        U32 NumUniformDataBindings = CompiledData.UniformDataBindings.size();
        ABYTEK_ENGINE_RHI_ASSERT(Allocation.UniformDataBindings.size() == NumUniformDataBindings) << "Uniform data bindings was not allocated";
        for (U32 UniformDataBindingIndex = 0; UniformDataBindingIndex < NumUniformDataBindings; ++UniformDataBindingIndex)
        {
            const auto& BoundUniformDataBinding = Allocation.UniformDataBindings[UniformDataBindingIndex];
            const auto& CompiledUniformDataBinding = CompiledData.UniformDataBindings[UniformDataBindingIndex];
            
            if (CompiledUniformDataBinding.UseRootConstants)
            {
                continue;
            }
            
            ABYTEK_ENGINE_RHI_ASSERT(BoundUniformDataBinding.ConstantDataRange) << "Invalid constant data range";
            Result.push_back(
                F_DirectX12RHISubresourceBinding::MakeCore(
                    F_DirectX12RHISubresourceReference::Make(
                        BoundUniformDataBinding.ConstantDataRange.GetBuffer().Weak(),
                        0
                    ),
                    F_RHIResourceAccess::MakeCBV()
                )
            );
        }
    }
}
#endif