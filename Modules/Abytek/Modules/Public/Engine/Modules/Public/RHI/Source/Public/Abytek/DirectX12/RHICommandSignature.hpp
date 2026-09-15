#pragma once

#include "Abytek/RHIDeviceChild.hpp"
#include "Abytek/DirectX12Shared/APIWrapper.hpp"
#include "Abytek/RHIIndirectConfig.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    struct F_DirectX12RHICommandSignatureBuildParams
    {
        TW<A_RHIDevice> Device;
        E_RHIIndirectArgumentType IndirectArgumentType = E_RHIIndirectArgumentType::NONE;
    };
    
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHICommandSignature : public A_RHIDeviceChild
    {
    private:
        E_RHIIndirectArgumentType _IndirectArgumentType = E_RHIIndirectArgumentType::NONE;
        Microsoft::WRL::ComPtr<ID3D12CommandSignature> _D3D12CommandSignature;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetD3D12CommandSignature() const noexcept
        {
            return _D3D12CommandSignature;
        }
        ABYTEK_FORCE_INLINE auto& InjectD3D12CommandSignature() noexcept
        {
            return _D3D12CommandSignature;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(F_DirectX12RHICommandSignature);
        virtual void Build(const F_DirectX12RHICommandSignatureBuildParams& BuildParams);
        virtual void Release() override;
    };
}
#endif