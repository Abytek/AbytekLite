#pragma once

#include "Abytek/RHIClearDSVPass.hpp"


namespace Abytek
{
    class A_RHIResourceViewProxy;
    class A_RHIClearDSVPass;
    
    class ABYTEK_ENGINE_RHI_API A_RHIClearDSVPassProxy : public A_RHIPassProxy
    {
    private:
        TS<A_RHIResourceViewProxy> _DSVProxy;
        E_RHIClearDSVFlag _Flags = E_RHIClearDSVFlag::NONE;
        F32 _Depth = 0.0f;
        U8 _Stencil = 0;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetDSVProxy() const noexcept
        {
            return _DSVProxy;
        }
        ABYTEK_FORCE_INLINE const auto& GetFlags() const noexcept
        {
            return _Flags;
        }
        ABYTEK_FORCE_INLINE auto& InjectFlags() noexcept
        {
            return _Flags;
        }
        ABYTEK_FORCE_INLINE const auto& GetDepth() const noexcept
        {
            return _Depth;
        }
        ABYTEK_FORCE_INLINE auto& InjectDepth() noexcept
        {
            return _Depth;
        }
        ABYTEK_FORCE_INLINE const auto& GetStencil() const noexcept
        {
            return _Stencil;
        }
        ABYTEK_FORCE_INLINE auto& InjectStencil() noexcept
        {
            return _Stencil;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIClearDSVPassProxy);
        virtual void Build(const TW_Valid<A_RHIClearDSVPass>& Pass);
        virtual void Release() override;
    };
}