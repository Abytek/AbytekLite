#include "Abytek/DirectX12/RHIBindGroupProxy.hpp"

#include "Abytek/RHIBindGroupTemplateRuntimeProxy.hpp"
#include "Abytek/DirectX12/RHIFormat.hpp"
#include "Abytek/DirectX12/RHIResourceProxy.hpp"
#include "Abytek/DirectX12/RHIResourceViewProxy.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void H_DirectX12RHIBindGroupProxy::PopulateCommandList(
        F_DirectX12RHISubmissionContext& SubmissionContext,
        const TF_Span<TW<A_RHIBindGroupProxy>>& BindGroupProxies,
        const TF_Span<F_DirectX12SharedRHIPipelineStateTemplateCompiledData::F_BindGroupMap>& BindGroupMaps,
        E_RHIPassClass PassClass
    )
    {
        auto& BindGroupProxySet = SubmissionContext.GetBindGroupProxySet(PassClass);
        
        U32 NumBindGroupProxies = BindGroupProxies.size();
        for (U32 BindGroupProxyIndex = 0; BindGroupProxyIndex < NumBindGroupProxies; ++BindGroupProxyIndex)
        {
            const auto& BindGroupProxy = BindGroupProxies[BindGroupProxyIndex];
            
            B8 IsDirty = false;
            if (BindGroupProxyIndex >= BindGroupProxySet.size())
            {
                BindGroupProxySet.resize(NumBindGroupProxies);
                IsDirty = true;
            }
            else
            {
                IsDirty = (BindGroupProxySet[BindGroupProxyIndex] != BindGroupProxy);
            }
            if (!IsDirty)
            {
                continue;
            }
            
            const auto& BindGroupMap = BindGroupMaps[BindGroupProxyIndex];
            
            auto CastedBindGroupProxy = BindGroupProxy.FastCast<F_DirectX12RHIBindGroupProxy>();
            auto CastedBindGroupTemplate = CastedBindGroupProxy->GetTemplateRuntimeProxy()->GetTemplate().FastCast<F_DirectX12SharedRHIBindGroupTemplate>();
            const auto& BindGroupTemplateCompiledData = CastedBindGroupTemplate->GetCompiledData();
            const auto& BindGroupAllocationProxy = CastedBindGroupProxy->GetAllocationProxy();
            
            const auto& SlotDataProxy = CastedBindGroupProxy->GetSlotDataProxy();
            
            const auto& CompiledDescriptorBindings = BindGroupTemplateCompiledData.DescriptorBindings;
            const auto& BoundDescriptorBindings = BindGroupAllocationProxy.DescriptorBindings;
            U32 NumDescriptorBindings = CompiledDescriptorBindings.size();
            ABYTEK_ENGINE_RHI_ASSERT(CompiledDescriptorBindings.size() == BoundDescriptorBindings.size());
            
            const auto& VertexBufferBindings = BindGroupTemplateCompiledData.VertexBufferBindings;
            const auto& IndexBufferBinding = BindGroupTemplateCompiledData.IndexBufferBinding;
            
            const auto& RTVs = BindGroupTemplateCompiledData.RTVBindings;
            const auto& DSV = BindGroupTemplateCompiledData.DSVBinding;

            auto& BoundUniformDataBindings = BindGroupAllocationProxy.UniformDataBindings;
            const auto& CompiledUniformDataBindings = BindGroupTemplateCompiledData.UniformDataBindings;
            U32 NumUniformDataBindings = CompiledUniformDataBindings.size(); 
            ABYTEK_ENGINE_RHI_ASSERT(BoundUniformDataBindings.size() == NumUniformDataBindings) << "Mismatched num uniform data bindings";
            
            // Descriptor tables
            for (U32 Idx = 0; Idx < NumDescriptorBindings; ++Idx)
            {
                const auto& CompiledDescriptorBinding = CompiledDescriptorBindings[Idx];
                const auto& BoundDescriptorBinding = BoundDescriptorBindings[Idx];
                
                U32 TargetRootParameterIndex = BindGroupMap.RootParameterIndex + CompiledDescriptorBinding.RootParameterIndex;
                switch (PassClass)
                {
                case E_RHIPassClass::GRAPHICS:
                    SubmissionContext.D3D12CommandList->SetGraphicsRootDescriptorTable(
                        TargetRootParameterIndex,
                        BoundDescriptorBinding.DescriptorRange.GetGPUHandle()
                    );
                    break;
                case E_RHIPassClass::COMPUTE:
                    SubmissionContext.D3D12CommandList->SetComputeRootDescriptorTable(
                        TargetRootParameterIndex,
                        BoundDescriptorBinding.DescriptorRange.GetGPUHandle()
                    );
                    break;
                default:
                    ABYTEK_ENGINE_RHI_ASSERT(false) << "Invalid pass class";
                    break;
                }
            }
            
            // Vertex buffer bindings
            {
                TF_SmallVector<D3D12_VERTEX_BUFFER_VIEW, 8> D3D12VertexBufferViews;
                for (const auto& VertexBufferBinding : VertexBufferBindings)
                {
                    const auto& VertexBufferProxy = SlotDataProxy.GetVertexBuffer(VertexBufferBinding.SlotIndex).VertexBufferBindingProxy;
                    
                    D3D12_VERTEX_BUFFER_VIEW D3D12VertexBufferView;
                    D3D12VertexBufferView.BufferLocation = (
                        VertexBufferProxy.ResourceProxy.FastCast<F_DirectX12RHIResourceProxy>()
                        ->GetD3D12Resource()->GetGPUVirtualAddress() 
                        + VertexBufferProxy.OffsetInBytes
                    );
                    D3D12VertexBufferView.SizeInBytes = static_cast<U32>(VertexBufferProxy.SizeInBytes);
                    D3D12VertexBufferView.StrideInBytes = static_cast<U32>(VertexBufferProxy.StrideInBytes);
                    D3D12VertexBufferViews.push_back(D3D12VertexBufferView);
                }
                if (D3D12VertexBufferViews.size() > 0)
                {
                    ABYTEK_ENGINE_RHI_ASSERT(PassClass == E_RHIPassClass::GRAPHICS) << "Invalid pass class (requires E_RHIPassClass::GRAPHICS), cannot bind vertex buffers";
                    
                    SubmissionContext.D3D12CommandList->IASetVertexBuffers(
                        0,
                        D3D12VertexBufferViews.size(),
                        D3D12VertexBufferViews.data()
                    );
                }
            }
            
            // Index buffer binding
            if (IndexBufferBinding)
            {
                ABYTEK_ENGINE_RHI_ASSERT(PassClass == E_RHIPassClass::GRAPHICS) << "Invalid pass class (requires E_RHIPassClass::GRAPHICS), cannot bind index buffer";
                
                const auto& IndexBufferBindingProxy = SlotDataProxy.GetIndexBuffer(IndexBufferBinding->SlotIndex).IndexBufferBindingProxy;
                    
                D3D12_INDEX_BUFFER_VIEW D3D12IndexBufferView = {};
                D3D12IndexBufferView.Format = RHIFormatToD3DFormat(IndexBufferBindingProxy.Format);
                D3D12IndexBufferView.BufferLocation = (
                    IndexBufferBindingProxy.ResourceProxy.FastCast<F_DirectX12RHIResourceProxy>()
                    ->GetD3D12Resource()->GetGPUVirtualAddress() 
                    + IndexBufferBindingProxy.OffsetInBytes
                );
                SubmissionContext.D3D12CommandList->IASetIndexBuffer(&D3D12IndexBufferView);
            }
            
            // RTVs & DSV
            U32 NumRTVs = RTVs.size();
            if (DSV || (NumRTVs > 0))
            {
                TF_SmallVector<D3D12_CPU_DESCRIPTOR_HANDLE, 8> RTVCPUDescriptorHandles;
                RTVCPUDescriptorHandles.reserve(NumRTVs);
                
                for (const auto& RTV : RTVs)
                {
                    const auto& RTVData = SlotDataProxy.GetRTV(RTV.SlotIndex);
                    RTVCPUDescriptorHandles.push_back({
                        RTVData.RTVProxy.FastCast<F_DirectX12RHIResourceViewProxy>()
                        ->GetDescriptorRange()
                        .GetCPUHandle(false)
                    });
                } 
                
                D3D12_CPU_DESCRIPTOR_HANDLE DSVCPUDescriptorHandle;
                D3D12_CPU_DESCRIPTOR_HANDLE* DSVCPUDescriptorHandlePtr = nullptr;
                if (DSV)
                {
                    const auto& DSVData = SlotDataProxy.GetDSV(DSV->SlotIndex);
                    DSVCPUDescriptorHandle = {
                        DSVData.DSVProxy.FastCast<F_DirectX12RHIResourceViewProxy>()
                        ->GetDescriptorRange()
                        .GetCPUHandle(false)
                    };
                    DSVCPUDescriptorHandlePtr = &DSVCPUDescriptorHandle;
                }
                
                SubmissionContext.D3D12CommandList->OMSetRenderTargets(
                    NumRTVs,
                    RTVCPUDescriptorHandles.data(),
                    false,
                    DSVCPUDescriptorHandlePtr
                );
            }
            
            // Uniform data bindings
            for (U32 UniformDataBindingIndex = 0; UniformDataBindingIndex < NumUniformDataBindings; UniformDataBindingIndex++)
            {
                const auto& BoundUniformDataBinding = BoundUniformDataBindings[UniformDataBindingIndex];
                const auto& CompiledUniformDataBinding = CompiledUniformDataBindings[UniformDataBindingIndex];
                
                auto UniformDataPtr = SlotDataProxy.GetUniformData(CompiledUniformDataBinding.SlotIndex).GetPayloadPtr();
                
                U32 TargetRootParameterIndex = BindGroupMap.RootParameterIndex + CompiledUniformDataBinding.RootParameterIndex;
                
                auto InputDataSizeInBytes = CompiledUniformDataBinding.SizeInBytes;
                
                if (CompiledUniformDataBinding.UseRootConstants)
                {
                    U32 NumConstants = (InputDataSizeInBytes + sizeof(U32) - 1) / sizeof(U32);
                    switch (PassClass)
                    {
                    case E_RHIPassClass::GRAPHICS:
                        SubmissionContext.D3D12CommandList->SetGraphicsRoot32BitConstants(
                            TargetRootParameterIndex,
                            NumConstants,
                            UniformDataPtr,
                            0
                        );
                        break;
                    case E_RHIPassClass::COMPUTE:
                        SubmissionContext.D3D12CommandList->SetComputeRoot32BitConstants(
                            TargetRootParameterIndex,
                            NumConstants,
                            UniformDataPtr,
                            0
                        );
                        break;
                    default:
                        ABYTEK_ENGINE_RHI_ASSERT(false) << "Invalid pass class";
                        break;
                    }
                }
                else
                {
                    D3D12_GPU_VIRTUAL_ADDRESS CBAddress = static_cast<D3D12_GPU_VIRTUAL_ADDRESS>(
                            BoundUniformDataBinding.ConstantDataRangeProxy.BufferProxy
                                .FastCast<F_DirectX12RHIResourceProxy>()
                                ->GetD3D12Resource()
                                ->GetGPUVirtualAddress()
                            + BoundUniformDataBinding.ConstantDataRangeProxy.BeginOffsetInBytes
                        );

                    switch (PassClass)
                    {
                    case E_RHIPassClass::GRAPHICS:
                        SubmissionContext.D3D12CommandList->SetGraphicsRootConstantBufferView(
                            TargetRootParameterIndex,
                            CBAddress
                        );
                        break;
                    case E_RHIPassClass::COMPUTE:
                        SubmissionContext.D3D12CommandList->SetComputeRootConstantBufferView(
                            TargetRootParameterIndex,
                            CBAddress
                        );
                        break;
                    default:
                        ABYTEK_ENGINE_RHI_ASSERT(false) << "Invalid pass class";
                        break;
                    }
                }
            }
            
            BindGroupProxySet[BindGroupProxyIndex] = BindGroupProxy;
        }
    }
    
    void F_DirectX12RHIBindGroupProxy::LateBuildBindGroup_DirectX12()
    {
    }
    void F_DirectX12RHIBindGroupProxy::Release()
    {
        _AllocationProxy = {};
        _SlotDataProxy.Destruct();
        A_RHIBindGroupProxy::Release();
    }

    void F_DirectX12RHIBindGroupProxy::AssignCommit(
        F_RHIBindGroupSlotDataProxy&& SlotDataProxy,
        F_DirectX12RHIBindGroupAllocationProxy&& AllocationProxy
    )
    {
        _SlotDataProxy = ABYTEK_MOVE(SlotDataProxy);
        _AllocationProxy = ABYTEK_MOVE(AllocationProxy);
    }
}
#endif