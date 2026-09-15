#pragma once

#include "Abytek/RHIClearUAVFloatPass.hpp"


namespace Abytek
{
    class A_RHIResourceViewProxy;
    
    class ABYTEK_ENGINE_RHI_API A_RHIClearUAVFloatPassProxy : public A_RHIPassProxy
    {
    private:
        TS<A_RHIResourceViewProxy> _UAVProxy;
        F_Vector4_F32 _Value = F_Vector4_F32::Zero();
        
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
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIClearUAVFloatPassProxy);
        virtual void Build(const TW_Valid<A_RHIClearUAVFloatPass>& Pass);
        virtual void Release() override;
    };
}