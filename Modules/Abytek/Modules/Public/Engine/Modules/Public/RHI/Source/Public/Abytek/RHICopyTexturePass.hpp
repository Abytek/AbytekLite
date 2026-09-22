#pragma once

#include "Abytek/RHIPass.hpp"
#include "Abytek/RHIResource.hpp"


namespace Abytek
{
    struct F_RHICopyTexturePassBuildParams : public F_RHIPassBuildParams
    {
        TS<A_RHIResource> SrcTexture;
        TS<A_RHIResource> DestTexture;
        F_RHITextureElementSet SrcElementSet = {
            {
                F_RHITextureElement {}
            }
        };
        F_RHITextureElementSet DstElementSet = {
            {
                F_RHITextureElement {}
            }
        };
    };
    
    class ABYTEK_ENGINE_RHI_API A_RHICopyTexturePass : public A_RHIPass
    {
    private:
        TS<A_RHIResource> _SrcTexture;
        TS<A_RHIResource> _DestTexture;
        F_RHITextureElementSet _SrcElementSet;
        F_RHITextureElementSet _DstElementSet;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetSrcTexture() const noexcept
        {
            return _SrcTexture;
        }
        ABYTEK_FORCE_INLINE auto& InjectSrcTexture() noexcept
        {
            return _SrcTexture;
        }
        ABYTEK_FORCE_INLINE const auto& GetDestTexture() const noexcept
        {
            return _DestTexture;
        }
        ABYTEK_FORCE_INLINE auto& InjectDestTexture() noexcept
        {
            return _DestTexture;
        }
        ABYTEK_FORCE_INLINE const auto& GetSrcElementSet() const noexcept
        {
            return _SrcElementSet;
        }
        ABYTEK_FORCE_INLINE auto& InjectSrcElementSet() noexcept
        {
            return _SrcElementSet;
        }
        ABYTEK_FORCE_INLINE const auto& GetDstElementSet() const noexcept
        {
            return _DstElementSet;
        }
        ABYTEK_FORCE_INLINE auto& InjectDstElementSet() noexcept
        {
            return _DstElementSet;
        }

    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHICopyTexturePass);
        virtual void Build(const F_RHICopyTexturePassBuildParams& BuildParams);
        virtual void Release() override;
        
    public:
        virtual E_RHIPassClass GetPassClass() const override
        {
            return E_RHIPassClass::BLIT;
        }
        
    public:
        virtual TS_Valid<A_RHIPassProxy> CreateProxy() override;   
    };
}