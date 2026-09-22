#pragma once

#include "Abytek/RHIPass.hpp"


namespace Abytek
{
    class A_RHIResourceView;
    
    struct F_RHIClearUAVFloatPassBuildParams : public F_RHIPassBuildParams
    {
        TS<A_RHIResourceView> UAV;
        F_Vector4_F32 Value = F_Vector4_F32::Zero();
    };
    
    class ABYTEK_ENGINE_RHI_API A_RHIClearUAVFloatPass : public A_RHIPass
    {
    private:
        TS<A_RHIResourceView> _UAV;
        F_Vector4_F32 _Value = F_Vector4_F32::Zero();
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetUAV() const noexcept
        {
            return _UAV;
        }
        ABYTEK_FORCE_INLINE const auto& GetValue() const noexcept
        {
            return _Value;
        }

    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIClearUAVFloatPass);
        virtual void Build(const F_RHIClearUAVFloatPassBuildParams& BuildParams);
        virtual void Release() override;
        
    public:
        virtual E_RHIPassClass GetPassClass() const override
        {
            return E_RHIPassClass::COMPUTE;
        }
        
    public:
        virtual TS_Valid<A_RHIPassProxy> CreateProxy() override;
    };
}