#pragma once

#include "Abytek/RHIReadbackTexturePass.hpp"


namespace Abytek
{
    class A_RHIResourceProxy;
    
    class ABYTEK_ENGINE_RHI_API A_RHIReadbackTexturePassProxy : public A_RHIPassProxy
    {
    private:
        TS<A_RHIResourceProxy> _TextureProxy;
        F_RHIReadbackTextureCallback _Callback;

    public:
        ABYTEK_FORCE_INLINE const auto& GetTextureProxy() const noexcept
        {
            return _TextureProxy;
        }
        ABYTEK_FORCE_INLINE auto& InjectTextureProxy() noexcept
        {
            return _TextureProxy;
        }
        ABYTEK_FORCE_INLINE const auto& GetCallback() const noexcept
        {
            return _Callback;
        }
        ABYTEK_FORCE_INLINE auto& InjectCallback() noexcept
        {
            return _Callback;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIReadbackTexturePassProxy);
        virtual void Build(const TW_Valid<A_RHIReadbackTexturePass>& Pass);
        virtual void Release() override;
    };
}