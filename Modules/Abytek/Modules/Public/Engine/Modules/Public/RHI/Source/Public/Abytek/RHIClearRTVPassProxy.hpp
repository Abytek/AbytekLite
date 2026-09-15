#pragma once

#include "Abytek/RHIClearRTVPass.hpp"


namespace Abytek
{
    class A_RHIResourceViewProxy;
    
    class ABYTEK_ENGINE_RHI_API A_RHIClearRTVPassProxy : public A_RHIPassProxy
    {
    private:
        TS<A_RHIResourceViewProxy> _RTVProxy;
        F_Vector4_F32 _Color = F_Vector4_F32::Zero();
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetRTVProxy() const noexcept
        {
            return _RTVProxy;
        }
        ABYTEK_FORCE_INLINE const auto& GetColor() const noexcept
        {
            return _Color;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIClearRTVPassProxy);
        virtual void Build(const TW_Valid<A_RHIClearRTVPass>& Pass);
        virtual void Release() override;
    };
}