#pragma once

#include "Abytek/RHIPass.hpp"


namespace Abytek
{
    class A_RHIResourceView;
    
    enum class E_RHIClearDSVFlag : U8
    {
        NONE = 0x0,
        DEPTH = 0x1,
        STENCIL = 0x2,
    };
    
    struct F_RHIClearDSVPassBuildParams : public F_RHIPassBuildParams
    {
        TS<A_RHIResourceView> DSV;
        E_RHIClearDSVFlag Flags = E_RHIClearDSVFlag::NONE;
        F32 Depth = 0.0f;
        U8 Stencil = 0;
    };
    
    class ABYTEK_ENGINE_RHI_API A_RHIClearDSVPass : public A_RHIPass
    {
    private:
        TS<A_RHIResourceView> _DSV;
        E_RHIClearDSVFlag _Flags = E_RHIClearDSVFlag::NONE;
        F32 _Depth = 0.0f;
        U8 _Stencil = 0;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetFlags() const noexcept
        {
            return _Flags;
        }
        ABYTEK_FORCE_INLINE auto& InjectFlags() noexcept
        {
            return _Flags;
        }
        ABYTEK_FORCE_INLINE const auto& GetDSV() const noexcept
        {
            return _DSV;
        }
        ABYTEK_FORCE_INLINE auto& InjectDSV() noexcept
        {
            return _DSV;
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
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIClearDSVPass);
        virtual void Build(const F_RHIClearDSVPassBuildParams& BuildParams);
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