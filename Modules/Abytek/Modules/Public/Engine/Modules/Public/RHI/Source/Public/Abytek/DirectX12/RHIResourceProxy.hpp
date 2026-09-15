#pragma once

#include "Abytek/RHIResourceProxy.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIResourceProxy : public A_RHIResourceProxy
    {
    private:
        Microsoft::WRL::ComPtr<ID3D12Resource> _D3D12Resource = nullptr;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetD3D12Resource() const noexcept
        {
            return _D3D12Resource;
        }
        ABYTEK_FORCE_INLINE auto& InjectD3D12Resource() noexcept
        {
            return _D3D12Resource;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHIResourceProxy);
        void LateBuildResource_DirectX12();
        void LateBuildBuffer_DirectX12();
        void LateBuildTexture_DirectX12();
        void LateBuildRTAS_DirectX12();
        virtual void Release() override;
        
    public:
        void AssignExternalD3D12Resource(const Microsoft::WRL::ComPtr<ID3D12Resource>& D3D12Resource);
        void AssignCommittedD3D12Resource(const Microsoft::WRL::ComPtr<ID3D12Resource>& D3D12Resource);
        void AssignPlacedD3D12Resource(const Microsoft::WRL::ComPtr<ID3D12Resource>& D3D12Resource);
        
    public:
        void ReleaseD3D12Resource();
        
    public:
        D3D12_HEAP_TYPE GetD3D12HeapType();
        D3D12_HEAP_PROPERTIES GetD3D12HeapProperties();
        D3D12_RESOURCE_STATES GetInitialD3D12State();
        D3D12_RESOURCE_DESC GetD3D12ResourceDesc();
        TF_Optional<D3D12_CLEAR_VALUE> GetD3D12ClearValue();
        D3D12_RESOURCE_ALLOCATION_INFO GetD3D12ResourceAllocationInfo();
    };
}
#endif