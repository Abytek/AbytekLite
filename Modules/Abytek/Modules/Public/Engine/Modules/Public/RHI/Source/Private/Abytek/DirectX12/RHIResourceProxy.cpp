#include "Abytek/DirectX12/RHIResourceProxy.hpp"
#include "Abytek/DirectX12/RHIResource.hpp"
#include "Abytek/DirectX12/RHIContextProxy.hpp"
#include "Abytek/RHISubmissionList.hpp"
#include "Abytek/DirectX12/RHIDevice.hpp"
#include "Abytek/RHISubsystem.hpp"
#include "Abytek/DirectX12/RHIFormat.hpp"
#include "Abytek/DirectX12/RHISubsystemProxy.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void F_DirectX12RHIResourceProxy::LateBuildResource_DirectX12()
    {
    }
    void F_DirectX12RHIResourceProxy::LateBuildBuffer_DirectX12()
    {
    }
    void F_DirectX12RHIResourceProxy::LateBuildTexture_DirectX12()
    {
    }
    void F_DirectX12RHIResourceProxy::LateBuildRTAS_DirectX12()
    {
    }
    
    void F_DirectX12RHIResourceProxy::Release()
    {
        ReleaseD3D12Resource();
        A_RHIResourceProxy::Release();
    }

    void F_DirectX12RHIResourceProxy::AssignExternalD3D12Resource(const Microsoft::WRL::ComPtr<ID3D12Resource>& D3D12Resource)
    {
        _D3D12Resource = D3D12Resource;
    }
    void F_DirectX12RHIResourceProxy::AssignCommittedD3D12Resource(const Microsoft::WRL::ComPtr<ID3D12Resource>& D3D12Resource)
    {
        _D3D12Resource = D3D12Resource;
    }
    void F_DirectX12RHIResourceProxy::AssignPlacedD3D12Resource(const Microsoft::WRL::ComPtr<ID3D12Resource>& D3D12Resource)
    {
        _D3D12Resource = D3D12Resource;
    }

    void F_DirectX12RHIResourceProxy::ReleaseD3D12Resource()
    {
        _D3D12Resource = nullptr;
    }

    D3D12_HEAP_TYPE F_DirectX12RHIResourceProxy::GetD3D12HeapType()
    {
        return H_DirectX12RHIResource::GetD3D12HeapType(GetAccessCapabilities());
    }
    D3D12_HEAP_PROPERTIES F_DirectX12RHIResourceProxy::GetD3D12HeapProperties()
    {
        return H_DirectX12RHIResource::GetD3D12HeapProperties(GetAccessCapabilities());
    }
    D3D12_RESOURCE_STATES F_DirectX12RHIResourceProxy::GetInitialD3D12State()
    {
        return H_DirectX12RHIResource::GetInitialD3D12State(
            GetAccessCapabilities(),
            GetInitialAccess()
        );;
    }
    D3D12_RESOURCE_DESC F_DirectX12RHIResourceProxy::GetD3D12ResourceDesc()
    {
        D3D12_RESOURCE_DESC Result = {};
        
        auto Archetype = GetArchetype();
        auto AccessCapabilities = GetAccessCapabilities();
        
        if (FlagHas(Archetype, E_RHIResourceArchetype::RTAS))
        {
            ABYTEK_ENGINE_RHI_ASSERT(false) << "RTAS was not supported";
        }
        else if (FlagHas(Archetype, E_RHIResourceArchetype::TEXTURE))
        {
            const auto& TextureAspect = GetTextureAspect();
            auto Format = GetFormat();
            return H_DirectX12RHIResource::GetD3D12ResourceDesc(
                AccessCapabilities,
                Format,
                TextureAspect
            );
        }
        else if (FlagHas(Archetype, E_RHIResourceArchetype::BUFFER))
        {
            const auto& BufferAspect = GetBufferAspect();
            auto Format = GetFormat();
            return H_DirectX12RHIResource::GetD3D12ResourceDesc(
                AccessCapabilities,
                Format,
                BufferAspect
            );
        }
        else
        {
            ABYTEK_ENGINE_RHI_ASSERT(false) << "Invalid archetype";
        }
        return Result;
    }
    TF_Optional<D3D12_CLEAR_VALUE> F_DirectX12RHIResourceProxy::GetD3D12ClearValue()
    {
        if (!FlagHas(GetArchetype(), E_RHIResourceArchetype::TEXTURE))
        {
            return {};
        }
        return H_DirectX12RHIResource::GetD3D12ClearValue(
            GetAccessCapabilities(),
            GetFormat(),
            GetTextureAspect()
        );
    }
    D3D12_RESOURCE_ALLOCATION_INFO F_DirectX12RHIResourceProxy::GetD3D12ResourceAllocationInfo()
    {
        auto D3D12ResourceDesc = GetD3D12ResourceDesc();
        
        auto D3D12Device = GetContextProxy()->GetDevice().FastCast<F_DirectX12RHIDevice>()->GetD3D12Device();
        return D3D12Device->GetResourceAllocationInfo(
            1, 
            1, 
            &D3D12ResourceDesc
        );
    }
}
#endif