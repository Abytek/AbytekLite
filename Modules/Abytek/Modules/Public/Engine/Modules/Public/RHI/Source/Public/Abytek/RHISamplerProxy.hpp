#pragma once

#include "Abytek/RHIContextChild.hpp"
#include "Abytek/RHISampler.hpp"


namespace Abytek
{
    class A_RHISampler;

    class ABYTEK_ENGINE_RHI_API A_RHISamplerProxy :
        public A_RHIContextChildProxy
    {
    private:
        F_RHISamplerAccess _Access;
        
        E_RHITextureFilter _MinFilter = E_RHITextureFilter::DEFAULT;
        E_RHITextureFilter _MagFilter = E_RHITextureFilter::DEFAULT;
        E_RHITextureFilter _MipFilter = E_RHITextureFilter::DEFAULT;

        E_RHITextureFilterReductionType _FilterReductionType = E_RHITextureFilterReductionType::DEFAULT;

        E_RHITextureAddressMode _AddressModeU = E_RHITextureAddressMode::DEFAULT;
        E_RHITextureAddressMode _AddressModeV = E_RHITextureAddressMode::DEFAULT;
        E_RHITextureAddressMode _AddressModeW = E_RHITextureAddressMode::DEFAULT;

        F32 _LODBias = 0.0f;
        F32 _MinLOD = 0.0f;
        F32 _MaxLOD = 9999.0f;

        U32 _MaxAnisotropy = 1;

        E_RHICompareOperation _CompareOperation = E_RHICompareOperation::NONE;
        
        F_Vector4_F32 _BorderColor = F_Vector4_F32::Zero();
        
        B8 _AllowBindless = false;
        
    public:
        ABYTEK_FORCE_INLINE TW_Valid<A_RHISampler> GetSampler() const noexcept
        {
            return GetContextChild().FastCast<A_RHISampler>();
        }
        
        ABYTEK_FORCE_INLINE auto GetAccess() const noexcept
        {
            return _Access;
        }
        ABYTEK_FORCE_INLINE auto& InjectAccess() noexcept
        {
            return _Access;
        }

        ABYTEK_FORCE_INLINE auto GetMinFilter() const noexcept
        {
            return _MinFilter;
        }
        ABYTEK_FORCE_INLINE auto& InjectMinFilter() noexcept
        {
            return _MinFilter;
        }
        ABYTEK_FORCE_INLINE auto GetMagFilter() const noexcept
        {
            return _MagFilter;
        }
        ABYTEK_FORCE_INLINE auto& InjectMagFilter() noexcept
        {
            return _MagFilter;
        }
        ABYTEK_FORCE_INLINE auto GetMipFilter() const noexcept
        {
            return _MipFilter;
        }
        ABYTEK_FORCE_INLINE auto& InjectMipFilter() noexcept
        {
            return _MipFilter;
        }
        
        ABYTEK_FORCE_INLINE auto GetFilterReductionType() const noexcept
        {
            return _FilterReductionType;
        }
        ABYTEK_FORCE_INLINE auto& InjectFilterReductionType() noexcept
        {
            return _FilterReductionType;
        }
        
        ABYTEK_FORCE_INLINE auto GetAddressModeU() const noexcept
        {
            return _AddressModeU;
        }
        ABYTEK_FORCE_INLINE auto& InjectAddressModeU() noexcept
        {
            return _AddressModeU;
        }
        ABYTEK_FORCE_INLINE auto GetAddressModeV() const noexcept
        {
            return _AddressModeV;
        }
        ABYTEK_FORCE_INLINE auto& InjectAddressModeV() noexcept
        {
            return _AddressModeV;
        }
        ABYTEK_FORCE_INLINE auto GetAddressModeW() const noexcept
        {
            return _AddressModeW;
        }
        ABYTEK_FORCE_INLINE auto& InjectAddressModeW() noexcept
        {
            return _AddressModeW;
        }
        
        ABYTEK_FORCE_INLINE auto GetLODBias() const noexcept
        {
            return _LODBias;
        }
        ABYTEK_FORCE_INLINE auto& InjectLODBias() noexcept
        {
            return _LODBias;
        }
        ABYTEK_FORCE_INLINE auto GetMinLOD() const noexcept
        {
            return _MinLOD;
        }
        ABYTEK_FORCE_INLINE auto& InjectMinLOD() noexcept
        {
            return _MinLOD;
        }
        ABYTEK_FORCE_INLINE auto GetMaxLOD() const noexcept
        {
            return _MaxLOD;
        }
        ABYTEK_FORCE_INLINE auto& InjectMaxLOD() noexcept
        {
            return _MaxLOD;
        }
        
        ABYTEK_FORCE_INLINE auto GetMaxAnisotropy() const noexcept
        {
            return _MaxAnisotropy;
        }
        ABYTEK_FORCE_INLINE auto& InjectMaxAnisotropy() noexcept
        {
            return _MaxAnisotropy;
        }
        
        ABYTEK_FORCE_INLINE auto GetCompareOperation() const noexcept
        {
            return _CompareOperation;
        }
        ABYTEK_FORCE_INLINE auto& InjectCompareOperation() noexcept
        {
            return _CompareOperation;
        }
        
        ABYTEK_FORCE_INLINE auto GetBorderColor() const noexcept
        {
            return _BorderColor;
        }
        ABYTEK_FORCE_INLINE auto& InjectBorderColor() noexcept
        {
            return _BorderColor;
        }
        
        ABYTEK_FORCE_INLINE auto GetAllowBindless() const noexcept
        {
            return _AllowBindless;
        }
        ABYTEK_FORCE_INLINE auto& InjectAllowBindless() noexcept
        {
            return _AllowBindless;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHISamplerProxy);
        void LateBuildSampler();
        virtual void Release() override;
    };
}
