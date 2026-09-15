#include "Abytek/DirectX12/RHIDescriptorManagerProxy.hpp"
#include "Abytek/DirectX12/RHIContextProxy.hpp"
#include "Abytek/DirectX12/RHIDevice.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void H_DirectX12RHIDescriptorManagerProxy::PopulateCommandList(
        F_DirectX12RHISubmissionContext& SubmissionContext,
        const TF_Span<TW<F_DirectX12RHIDescriptorManagerProxy>>& DescriptorManagerProxies
    )
    {
        TF_SmallVector<Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>, 2> D3D12DescriptorHeaps;
        for (const auto& DescriptorManagerProxy : DescriptorManagerProxies)
        {
            if (auto D3D12DescriptorHeap = DescriptorManagerProxy->GetD3D12DescriptorHeap_ShaderVisible())
            {
                D3D12DescriptorHeaps.push_back(D3D12DescriptorHeap);
            }
        }
        SubmissionContext.Update_D3D12DescriptorHeaps(D3D12DescriptorHeaps);
    }
    void H_DirectX12RHIDescriptorManagerProxy::PopulateCommandList(
        F_DirectX12RHISubmissionContext& SubmissionContext,
        const TW_Valid<A_RHIContextProxy>& ContextProxy
    )
    {
        auto CastedContextProxy = ContextProxy.FastCast<F_DirectX12RHIContextProxy>();
        TF_SmallVector<Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>, 2> D3D12DescriptorHeaps = {
            CastedContextProxy->GetDescriptorManagerProxy_GPU_CBV_SRV_UAV()->GetD3D12DescriptorHeap_ShaderVisible(),
            CastedContextProxy->GetDescriptorManagerProxy_GPU_SAMPLER()->GetD3D12DescriptorHeap_ShaderVisible()
        };
        SubmissionContext.Update_D3D12DescriptorHeaps(D3D12DescriptorHeaps);
    }

    ABYTEK_RA_OBJECT_DEFAULT(F_DirectX12RHIDescriptorManagerProxy);
    void F_DirectX12RHIDescriptorManagerProxy::LateBuildDescriptorManager_DirectX12()
    {
        auto DescriptorManager = GetDescriptorManager();
        
        _DescriptorHeapType = DescriptorManager->GetDescriptorHeapType();
        _IsShaderVisible = DescriptorManager->IsShaderVisible();
        _Size = 0;
        _D3D12DescriptorHandleStride = GetContextProxy()->GetDevice().FastCast<F_DirectX12RHIDevice>()->GetD3D12Device()->GetDescriptorHandleIncrementSize(
            DirectX12SharedAPIWrapper::Conversions::DescriptorHeapTypeToD3D12DescriptorHeapType(_DescriptorHeapType)    
        );
        
        Resize(256);
    }
    void F_DirectX12RHIDescriptorManagerProxy::Release()
    {
        ABYTEK_ENGINE_RHI_ASSERT(Queues.Copy.GetSize() == 0);
        ABYTEK_ENGINE_RHI_ASSERT(Queues.Init.GetSize() == 0);
        
        _D3D12DescriptorHeap_ShaderVisible = nullptr;
        _D3D12DescriptorHeap_NonShaderVisible = nullptr;
        
        _D3D12DescriptorHandleStride = 0;
        _Size = 0;
        _IsShaderVisible = false;
        _DescriptorHeapType = DirectX12SharedAPIWrapper::E_DescriptorHeapType::NONE;
        A_RHIContextChildProxy::Release();
    }

    void F_DirectX12RHIDescriptorManagerProxy::Resize(F_DirectX12RHIDescriptorIndex NewSize)
    {
        ABYTEK_ENGINE_RHI_ASSERT(NewSize > 0) << "Size cannot be zero";
        
        auto D3D12Device = GetContextProxy()->GetDevice().FastCast<F_DirectX12RHIDevice>()->GetD3D12Device();
        
        F_DirectX12RHIDescriptorIndex MinSize = Min<F_DirectX12RHIDescriptorIndex>(_Size, NewSize);
        
        auto D3D12DescriptorHeapType = DirectX12SharedAPIWrapper::Conversions::DescriptorHeapTypeToD3D12DescriptorHeapType(_DescriptorHeapType);

        // Create new heap
        ID3D12DescriptorHeap* NewD3D12DescriptorHeap_NonShaderVisible = nullptr;
        ID3D12DescriptorHeap* NewD3D12DescriptorHeap_ShaderVisible = nullptr;
        {
            D3D12_DESCRIPTOR_HEAP_DESC D3D12DescriptorHeapDesc = {};
            D3D12DescriptorHeapDesc.NumDescriptors = NewSize;
            D3D12DescriptorHeapDesc.Type = D3D12DescriptorHeapType;
            D3D12DescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
            HRESULT HR = D3D12Device->CreateDescriptorHeap(&D3D12DescriptorHeapDesc, IID_PPV_ARGS(&NewD3D12DescriptorHeap_NonShaderVisible));
            ABYTEK_ENGINE_RHI_ASSERT(SUCCEEDED(HR)) << "Cannot extends descriptor heap (CPU)";
        }
        if (_IsShaderVisible)
        {
            D3D12_DESCRIPTOR_HEAP_DESC D3D12DescriptorHeapDesc = {};
            D3D12DescriptorHeapDesc.NumDescriptors = NewSize;
            D3D12DescriptorHeapDesc.Type = D3D12DescriptorHeapType;
            D3D12DescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
            HRESULT HR = D3D12Device->CreateDescriptorHeap(&D3D12DescriptorHeapDesc, IID_PPV_ARGS(&NewD3D12DescriptorHeap_ShaderVisible));
            ABYTEK_ENGINE_RHI_ASSERT(SUCCEEDED(HR)) << "Cannot extends descriptor heap (GPU)";
        }

        if (_D3D12DescriptorHeap_NonShaderVisible)
        {
            D3D12Device->CopyDescriptorsSimple(
                MinSize,
                NewD3D12DescriptorHeap_NonShaderVisible->GetCPUDescriptorHandleForHeapStart(),
                _D3D12DescriptorHeap_NonShaderVisible->GetCPUDescriptorHandleForHeapStart(),
                D3D12DescriptorHeapType
            );
        }
        if (_D3D12DescriptorHeap_ShaderVisible && _IsShaderVisible)
        {
            D3D12Device->CopyDescriptorsSimple(
                MinSize,
                NewD3D12DescriptorHeap_ShaderVisible->GetCPUDescriptorHandleForHeapStart(),
                _D3D12DescriptorHeap_NonShaderVisible->GetCPUDescriptorHandleForHeapStart(),
                D3D12DescriptorHeapType
            );
        }

        _D3D12DescriptorHeap_NonShaderVisible = nullptr;
        _D3D12DescriptorHeap_ShaderVisible = nullptr;
        
        _D3D12DescriptorHeap_NonShaderVisible = NewD3D12DescriptorHeap_NonShaderVisible;
        _D3D12DescriptorHeap_ShaderVisible = NewD3D12DescriptorHeap_ShaderVisible;
        
        _Size = NewSize;
    }

    void F_DirectX12RHIDescriptorManagerProxy::Flush()
    {
        ABYTEK_PROFILER_EVENT();
        {
            DirectX12RHIDescriptorManagerQueries::F_Init Query;
            while (Queues.Init.TryPop(Query))
            {
                FlushInit(Query);
            }
        }
        FlushCopies();
    }
    void F_DirectX12RHIDescriptorManagerProxy::FlushInit(const DirectX12RHIDescriptorManagerQueries::F_Init& Query)
    {
        auto D3D12Device = GetContextProxy()->GetDevice().FastCast<F_DirectX12RHIDevice>()->GetD3D12Device();
        
        ABYTEK_ENGINE_RHI_ASSERT(_D3D12DescriptorHeap_NonShaderVisible);
        ABYTEK_ENGINE_RHI_ASSERT(_D3D12DescriptorHeap_ShaderVisible || !_IsShaderVisible);
        
        auto D3D12CPUDescriptorStride = D3D12Device->GetDescriptorHandleIncrementSize(
            DirectX12SharedAPIWrapper::Conversions::DescriptorHeapTypeToD3D12DescriptorHeapType(_DescriptorHeapType)    
        );
        
        D3D12_CPU_DESCRIPTOR_HANDLE D3D12CPUDescriptorHandle = _D3D12DescriptorHeap_NonShaderVisible->GetCPUDescriptorHandleForHeapStart();
        D3D12CPUDescriptorHandle.ptr += static_cast<Sz>(D3D12CPUDescriptorStride) * static_cast<Sz>(Query.DescriptorIndex);
        
        D3D12_CPU_DESCRIPTOR_HANDLE D3D12CPUDescriptorHandle_ShaderVisible = {};
        if (_IsShaderVisible)
        {
            D3D12CPUDescriptorHandle_ShaderVisible = _D3D12DescriptorHeap_NonShaderVisible->GetCPUDescriptorHandleForHeapStart();
            D3D12CPUDescriptorHandle_ShaderVisible.ptr += static_cast<Sz>(D3D12CPUDescriptorStride) * static_cast<Sz>(Query.DescriptorIndex);
        }
        
        switch (Query.D3D12DescriptorDesc.Type)
        {
        case DirectX12SharedAPIWrapper::E_DescriptorType::CBV:
            D3D12Device->CreateConstantBufferView(
                &Query.D3D12DescriptorDesc.D3D12CBVDesc, 
                D3D12CPUDescriptorHandle
            );
            if (_IsShaderVisible)
            {
                D3D12Device->CreateConstantBufferView(
                    &Query.D3D12DescriptorDesc.D3D12CBVDesc, 
                    D3D12CPUDescriptorHandle_ShaderVisible
                );
            }
            break;
        case DirectX12SharedAPIWrapper::E_DescriptorType::SRV:
            D3D12Device->CreateShaderResourceView(
                Query.D3D12DescriptorDesc.D3D12Resource.Get(), 
                &Query.D3D12DescriptorDesc.D3D12SRVDesc, 
                D3D12CPUDescriptorHandle
            );
            if (_IsShaderVisible)
            {
                D3D12Device->CreateShaderResourceView(
                    Query.D3D12DescriptorDesc.D3D12Resource.Get(), 
                    &Query.D3D12DescriptorDesc.D3D12SRVDesc, 
                    D3D12CPUDescriptorHandle_ShaderVisible
                );
            }
            break;
        case DirectX12SharedAPIWrapper::E_DescriptorType::UAV:
            D3D12Device->CreateUnorderedAccessView(
                Query.D3D12DescriptorDesc.D3D12Resource.Get(), 
                nullptr, 
                &Query.D3D12DescriptorDesc.D3D12UAVDesc, 
                D3D12CPUDescriptorHandle
            );
            if (_IsShaderVisible)
            {
                D3D12Device->CreateUnorderedAccessView(
                    Query.D3D12DescriptorDesc.D3D12Resource.Get(), 
                    nullptr, 
                    &Query.D3D12DescriptorDesc.D3D12UAVDesc, 
                    D3D12CPUDescriptorHandle_ShaderVisible
                );
            }
            break;
        case DirectX12SharedAPIWrapper::E_DescriptorType::RTV:
            D3D12Device->CreateRenderTargetView(
                Query.D3D12DescriptorDesc.D3D12Resource.Get(), 
                &Query.D3D12DescriptorDesc.D3D12RTVDesc,
                D3D12CPUDescriptorHandle
            );
            break;
        case DirectX12SharedAPIWrapper::E_DescriptorType::DSV:
            D3D12Device->CreateDepthStencilView(
                Query.D3D12DescriptorDesc.D3D12Resource.Get(), 
                &Query.D3D12DescriptorDesc.D3D12DSVDesc, 
                D3D12CPUDescriptorHandle
            );
            break;
        case DirectX12SharedAPIWrapper::E_DescriptorType::SAMPLER:
            D3D12Device->CreateSampler(
                &Query.D3D12DescriptorDesc.D3D12SamplerDesc, 
                D3D12CPUDescriptorHandle
            );
            break;
        default:
            ABYTEK_ENGINE_RHI_ASSERT(false) << "Unknown init type";
            break;
        }
    }
    void F_DirectX12RHIDescriptorManagerProxy::FlushCopies()
    {
        auto D3D12Device = GetContextProxy()->GetDevice().FastCast<F_DirectX12RHIDevice>()->GetD3D12Device();
        
        auto D3D12DescriptorHeapType = DirectX12SharedAPIWrapper::Conversions::DescriptorHeapTypeToD3D12DescriptorHeapType(_DescriptorHeapType);

        U32 NumCopies = 0;
        TF_SmallVector<D3D12_CPU_DESCRIPTOR_HANDLE, 32> DstD3D12CPUDescriptorHandles;
        TF_SmallVector<D3D12_CPU_DESCRIPTOR_HANDLE, 32> SrcD3D12CPUDescriptorHandles;
        TF_SmallVector<UINT, 32> CopySizes;
        
        DirectX12RHIDescriptorManagerQueries::F_Copy Query;
        while (Queues.Copy.TryPop(Query))
        {
            ABYTEK_ENGINE_RHI_ASSERT(Query.DestDescriptorRange.Manager == ABYTEK_WTHIS());
            ABYTEK_ENGINE_RHI_ASSERT(Query.DestDescriptorRange.GetSize() == Query.SrcDescriptorRange.GetSize());
            
            ++NumCopies;
            DstD3D12CPUDescriptorHandles.push_back(
                Query.DestDescriptorRange.GetCPUHandle(false)
            );
            SrcD3D12CPUDescriptorHandles.push_back(
                Query.SrcDescriptorRange.GetCPUHandle(false)
            );
            CopySizes.push_back(Query.DestDescriptorRange.GetSize());

            if (_IsShaderVisible)
            {
                ++NumCopies;
                DstD3D12CPUDescriptorHandles.push_back(
                    Query.DestDescriptorRange.GetCPUHandle(true)
                );
                SrcD3D12CPUDescriptorHandles.push_back(
                    Query.SrcDescriptorRange.GetCPUHandle(false)
                );
                CopySizes.push_back(Query.DestDescriptorRange.GetSize());
            }
        }
        
        if (NumCopies > 0)
        {
            D3D12Device->CopyDescriptors(
                NumCopies,
                DstD3D12CPUDescriptorHandles.data(),
                CopySizes.data(),
                NumCopies,
                SrcD3D12CPUDescriptorHandles.data(),
                CopySizes.data(),
                D3D12DescriptorHeapType
            );
        }
    }
}
#endif