#pragma once

#include "Abytek/RHIPass.hpp"


namespace Abytek
{
    class A_RHIResourceView;
    
    struct F_RHIClearUAVUIntPassBuildParams : public F_RHIPassBuildParams
    {
        TS<A_RHIResourceView> UAV;
        F_Vector4_U64 Value = F_Vector4_U64::Zero();
    };
    
    class ABYTEK_ENGINE_RHI_API A_RHIClearUAVUIntPass : public A_RHIPass
    {
    private:
        TS<A_RHIResourceView> _UAV;
        F_Vector4_U64 _Value = F_Vector4_U64::Zero();
        
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
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIClearUAVUIntPass);
        virtual void Build(const F_RHIClearUAVUIntPassBuildParams& BuildParams);
        virtual void Release() override;
        
    public:
        virtual E_RHIPassClass GetPassClass() override
        {
            return E_RHIPassClass::COMPUTE;
        }
        
    public:
        virtual TS_Valid<A_RHIPassProxy> CreateProxy() override;
    };
}\