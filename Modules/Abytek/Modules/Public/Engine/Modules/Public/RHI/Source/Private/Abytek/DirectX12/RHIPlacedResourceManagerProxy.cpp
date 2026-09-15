#include "Abytek/DirectX12/RHIPlacedResourceManagerProxy.hpp"
#include "Abytek/RHIContextProxy.hpp"
#include "Abytek/DirectX12/RHIDevice.hpp"
#include "Abytek/RHISubsystem.hpp"
#include "Abytek/DirectX12/RHISubsystemProxy.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    ABYTEK_RA_OBJECT_DEFAULT(F_DirectX12RHIPlacedResourcePageProxy);
    void F_DirectX12RHIPlacedResourcePageProxy::LateBuildPlacedResourcePage()
    {
        auto PlacedResourcePage = GetPlacedResourcePage();
        _Access = PlacedResourcePage->GetAccess();
        _ResourceArchetype = PlacedResourcePage->GetResourceArchetype();
        _SizeInBytes = PlacedResourcePage->GetSizeInBytes();
        _AlignmentInBytes = PlacedResourcePage->GetAlignmentInBytes();
        
        auto D3D12Device = GetContextProxy()->GetDevice().FastCast<F_DirectX12RHIDevice>()->GetD3D12Device();
        
        D3D12_HEAP_DESC D3D12HeapDesc = {};
        D3D12HeapDesc.SizeInBytes = _SizeInBytes;
        D3D12HeapDesc.Properties.Type = Internal::DirectX12RHIPlacedResourceManager::RHIResourceAccessToD3D12HeapType(_Access);
        D3D12HeapDesc.Alignment =_AlignmentInBytes;
        
        if (
            FlagHas(_ResourceArchetype, E_RHIResourceArchetype::BUFFER)
            || FlagHas(_ResourceArchetype, E_RHIResourceArchetype::RTAS)
        )
        {
            D3D12HeapDesc.Flags = D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS;
        }
        if (FlagHas(_ResourceArchetype, E_RHIResourceArchetype::TEXTURE))
        {
            if (
                FlagHas(_Access.GPU, E_RHIResourceGPUAccess::RTV)
                || FlagHas(_Access.GPU, E_RHIResourceGPUAccess::DSV)
            )
            {
                D3D12HeapDesc.Flags = D3D12_HEAP_FLAG_ALLOW_ONLY_RT_DS_TEXTURES;
            }
            else
            {
                D3D12HeapDesc.Flags = D3D12_HEAP_FLAG_ALLOW_ONLY_NON_RT_DS_TEXTURES;
            }
        }
        
        HRESULT HR = D3D12Device->CreateHeap(
            &D3D12HeapDesc,
            IID_PPV_ARGS(&_D3D12Heap)
        );
        ABYTEK_ENGINE_RHI_ASSERT(SUCCEEDED(HR)) << "Cannot create d3d12 heap";
    }
    void F_DirectX12RHIPlacedResourcePageProxy::Release()
    {
        _D3D12Heap = nullptr;
        
        _AlignmentInBytes = 0;
        _SizeInBytes = 0;
        _ResourceArchetype = E_RHIResourceArchetype::NONE;
        _Access = {};
        A_RHIContextChildProxy::Release();
    }
    
    ABYTEK_RA_OBJECT_DEFAULT(F_DirectX12RHIPlacedResourceManagerProxy);
    void F_DirectX12RHIPlacedResourceManagerProxy::LateBuildPlacedResourceManager()
    {
        auto PlacedResourceManager = GetPlacedResourceManager();
        _Access = PlacedResourceManager->GetAccess();
        _ResourceArchetype = PlacedResourceManager->GetResourceArchetype();
        _MinPageSizeInBytes = PlacedResourceManager->GetMinPageSizeInBytes();
        _MaxPageSizeInBytes = PlacedResourceManager->GetMaxPageSizeInBytes();
        _AlignmentInBytes = PlacedResourceManager->GetAlignmentInBytes();
    }
    void F_DirectX12RHIPlacedResourceManagerProxy::Release()
    {
        _AlignmentInBytes = 0;
        _MaxPageSizeInBytes = 0;
        _MinPageSizeInBytes = 0;
        _ResourceArchetype = E_RHIResourceArchetype::NONE;
        _Access = {};
        A_RHIContextChildProxy::Release();
    }
}
#endif