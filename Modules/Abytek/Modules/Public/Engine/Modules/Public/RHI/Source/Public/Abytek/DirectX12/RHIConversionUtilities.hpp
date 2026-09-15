#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"
#include "Abytek/RHISampler.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIConversionUtilities : public A_RAObject
    {
    public:
        ABYTEK_DECLARE_OBJECT_SINGLETON_CRT(F_DirectX12RHIConversionUtilities);

    public:
        struct F_D3D12InputForFilterConversion
        {
            E_RHITextureFilter MinFilter = E_RHITextureFilter::DEFAULT;
            E_RHITextureFilter MagFilter = E_RHITextureFilter::DEFAULT;
            E_RHITextureFilter MipFilter = E_RHITextureFilter::DEFAULT;

            E_RHITextureFilterReductionType FilterReductionType = E_RHITextureFilterReductionType::NONE;

            friend B8 operator < (const F_D3D12InputForFilterConversion& A, const F_D3D12InputForFilterConversion& B)
            {
                if (A.MinFilter != B.MinFilter)
                {
                    return ToUInt(A.MinFilter) < ToUInt(B.MinFilter);
                }
                if (A.MagFilter != B.MagFilter)
                {
                    return ToUInt(A.MagFilter) < ToUInt(B.MagFilter);
                }
                if (A.MipFilter != B.MipFilter)
                {
                    return ToUInt(A.MipFilter) < ToUInt(B.MipFilter);
                }
                if (A.FilterReductionType != B.FilterReductionType)
                {
                    return ToUInt(A.FilterReductionType) < ToUInt(B.FilterReductionType);
                }
                return false;
            }
        };
        
    private:
        TF_Map<F_D3D12InputForFilterConversion, D3D12_FILTER> _D3D12SupportedFilterMap;

    public:
        ABYTEK_FORCE_INLINE const auto& GetD3D12SupportedFilterMap() const noexcept
        {
            return _D3D12SupportedFilterMap;
        }
        ABYTEK_FORCE_INLINE auto& InjectD3D12SupportedFilterMap() noexcept
        {
            return _D3D12SupportedFilterMap;
        }

        D3D12_TEXTURE_ADDRESS_MODE TextureAddressModeToD3D12TextureAddressMode(E_RHITextureAddressMode TextureAddressMode)
        {
            switch (TextureAddressMode)
            {
            case E_RHITextureAddressMode::REPEAT:
                return D3D12_TEXTURE_ADDRESS_MODE_WRAP;
            case E_RHITextureAddressMode::MIRRORED_REPEAT:
                return D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
            case E_RHITextureAddressMode::CLAMP_TO_EDGE:
                return D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
            case E_RHITextureAddressMode::CLAMP_TO_BORDER:
                return D3D12_TEXTURE_ADDRESS_MODE_BORDER;
            }
            return {};
        }

    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(F_DirectX12RHIConversionUtilities);
        virtual void Build();
        virtual void Release() override;

    private:
        void _GenerateD3D12SupportedFilterMap();
    };
}
#endif