#include "Abytek/DirectX12/RHISamplerProxy.hpp"
#include "Abytek/DirectX12/RHIConversionUtilities.hpp"
#include "Abytek/DirectX12/RHISubsystemProxy.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void F_DirectX12RHISamplerProxy::LateBuildSampler_DirectX12()
    {
    }
    void F_DirectX12RHISamplerProxy::Release()
    {
        _DescriptorRange = {};
        A_RHISamplerProxy::Release();
    }

    void F_DirectX12RHISamplerProxy::AssignDescriptorRange(const F_DirectX12RHIDescriptorRange& DescriptorRange)
    {
        _DescriptorRange = DescriptorRange;
    }

    DirectX12SharedAPIWrapper::F_D3D12DescriptorDesc F_DirectX12RHISamplerProxy::GetDescriptorDesc()
    {
        DirectX12SharedAPIWrapper::F_D3D12DescriptorDesc Result;
        
        auto ConversionUtilities = F_DirectX12RHIConversionUtilities::GetInstance();
        const auto& D3D12SupportedFilterMap = ConversionUtilities->GetD3D12SupportedFilterMap();
        
        Result.Type = DirectX12SharedAPIWrapper::E_DescriptorType::SAMPLER;
        
        D3D12_SAMPLER_DESC D3D12SamplerDesc = {};
        
        F_DirectX12RHIConversionUtilities::F_D3D12InputForFilterConversion D3D12InputForFilterConversion;
        D3D12InputForFilterConversion.MinFilter = GetMinFilter();
        D3D12InputForFilterConversion.MagFilter = GetMagFilter();
        D3D12InputForFilterConversion.MipFilter = GetMipFilter();
        D3D12InputForFilterConversion.FilterReductionType = GetFilterReductionType();

        auto D3D12FilterIt = D3D12SupportedFilterMap.find(D3D12InputForFilterConversion);
        ABYTEK_ENGINE_RHI_ASSERT(D3D12FilterIt != D3D12SupportedFilterMap.end()) << "Invalid RHI sampler build params, cannot resolve d3d12 filter";
        D3D12SamplerDesc.Filter = D3D12FilterIt->second;

        D3D12SamplerDesc.AddressU = ConversionUtilities->TextureAddressModeToD3D12TextureAddressMode(GetAddressModeU());
        D3D12SamplerDesc.AddressV = ConversionUtilities->TextureAddressModeToD3D12TextureAddressMode(GetAddressModeV());
        D3D12SamplerDesc.AddressW = ConversionUtilities->TextureAddressModeToD3D12TextureAddressMode(GetAddressModeW());
        
        D3D12SamplerDesc.MipLODBias = GetLODBias();
        D3D12SamplerDesc.MinLOD = GetMinLOD();
        D3D12SamplerDesc.MaxLOD = GetMaxLOD();
        
        D3D12SamplerDesc.MaxAnisotropy = GetMaxAnisotropy();

        D3D12SamplerDesc.ComparisonFunc = DirectX12SharedAPIWrapper::Conversions::RHICompareOperationToD3D12ComparisonFunc(GetCompareOperation());

        D3D12SamplerDesc.BorderColor[0] = GetBorderColor().X;
        D3D12SamplerDesc.BorderColor[1] = GetBorderColor().Y;
        D3D12SamplerDesc.BorderColor[2] = GetBorderColor().Z;
        D3D12SamplerDesc.BorderColor[3] = GetBorderColor().W;
        
        Result.D3D12SamplerDesc = D3D12SamplerDesc;
        
        return Result;
    }
}
#endif