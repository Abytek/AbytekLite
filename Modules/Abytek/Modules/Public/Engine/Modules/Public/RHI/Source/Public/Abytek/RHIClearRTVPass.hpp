#pragma once

#include "Abytek/RHIPass.hpp"


namespace Abytek
{
    class A_RHIResourceView;
    
    struct F_RHIClearRTVPassBuildParams : public F_RHIPassBuildParams
    {
        TS<A_RHIResourceView> RTV;
        F_Vector4_F32 Color = F_Vector4_F32::Zero();
    };
    
    class ABYTEK_ENGINE_RHI_API A_RHIClearRTVPass : public A_RHIPass
    {
    private:
        TS<A_RHIResourceView> _RTV;
        F_Vector4_F32 _Color = F_Vector4_F32::Zero();
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetRTV() const noexcept
        {
            return _RTV;
        }
        ABYTEK_FORCE_INLINE auto& InjectRTV() noexcept
        {
            return _RTV;
        }
        ABYTEK_FORCE_INLINE const auto& GetColor() const noexcept
        {
            return _Color;
        }
        ABYTEK_FORCE_INLINE auto& InjectColor() noexcept
        {
            return _Color;
        }

    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIClearRTVPass);
        virtual void Build(const F_RHIClearRTVPassBuildParams& BuildParams);
        virtual void Release() override;
        
    public:
        virtual E_RHIPassClass GetPassClass() const override
        {
            return E_RHIPassClass::GRAPHICS;
        }
        
    public:
        virtual TS_Valid<A_RHIPassProxy> CreateProxy() override;
    };
}