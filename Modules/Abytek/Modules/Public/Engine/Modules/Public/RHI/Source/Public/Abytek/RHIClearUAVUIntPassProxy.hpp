#pragma once

#include "Abytek/RHIClearUAVUIntPass.hpp"


namespace Abytek
{
    class A_RHIResourceViewProxy;
    
    class ABYTEK_ENGINE_RHI_API A_RHIClearUAVUIntPassProxy : public A_RHIPassProxy
    {
    private:
        TS<A_RHIResourceViewProxy> _UAVProxy;
        F_Vector4_U64 _Value = F_Vector4_U64::Zero();
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetUAVProxy() const noexcept
        {
            return _UAVProxy;
        }
        ABYTEK_FORCE_INLINE const auto& GetValue() const noexcept
        {
            return _Value;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIClearUAVUIntPassProxy);
        virtual void Build(const TW_Valid<A_RHIClearUAVUIntPass>& Pass);
        virtual void Release() override;
    };
}