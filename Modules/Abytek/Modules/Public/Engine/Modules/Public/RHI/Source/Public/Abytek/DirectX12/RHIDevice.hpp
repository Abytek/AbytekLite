#pragma once

#include "Abytek/RHIDevice.hpp"
#include "Abytek/RHIResourceAccess.hpp"
#include "Abytek/DirectX12Shared/APIWrapper.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class F_DirectX12RHIDescriptorManager;
    
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIDevice : public A_RHIDevice
    {
    public:
        friend class F_DirectX12RHIDeviceManager;
      
    private:
        Microsoft::WRL::ComPtr<IDXGIAdapter> _DXGIAdapter = nullptr;
        Microsoft::WRL::ComPtr<ID3D12Device> _D3D12Device = nullptr;

    public:
        ABYTEK_FORCE_INLINE const auto& GetDXGIAdapter() const noexcept
        {
            return _DXGIAdapter;
        }
        ABYTEK_FORCE_INLINE auto& InjectDXGIAdapter() noexcept
        {
            return _DXGIAdapter;
        }
        ABYTEK_FORCE_INLINE const auto& GetD3D12Device() const noexcept
        {
            return _D3D12Device;
        }
        ABYTEK_FORCE_INLINE auto& InjectD3D12Device() noexcept
        {
            return _D3D12Device;
        }
#ifdef ABYTEK_DEBUG_INFO
        virtual void SetDebugName(const F_DebugName& DebugName) noexcept override;
#endif

    public:
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHIDevice);
        virtual void Build() override;
        virtual void Release() override;

    public:
        virtual void Activate() override;
        virtual void Deactivate() override;

    private:
        void _PostActivationSetup();

    protected:
        virtual F_RHIFeatureSet GenerateFeatureSet() override;
    };
}
#endif