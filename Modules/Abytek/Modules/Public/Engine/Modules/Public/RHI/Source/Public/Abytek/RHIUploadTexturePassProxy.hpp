#pragma once

#include "Abytek/RHIUploadTexturePass.hpp"


namespace Abytek
{
    class A_RHIResourceProxy;
    
    class ABYTEK_ENGINE_RHI_API A_RHIUploadTexturePassProxy : public A_RHIPassProxy
    {
    private:
        F_RHITextureDataView _TextureDataView;
        TS<A_RHIResourceProxy> _TextureProxy;

    public:
        ABYTEK_FORCE_INLINE const auto& GetTextureDataView() const noexcept
        {
            return _TextureDataView;
        }
        ABYTEK_FORCE_INLINE auto& InjectTextureDataView() noexcept
        {
            return _TextureDataView;
        }
        ABYTEK_FORCE_INLINE const auto& GetTextureProxy() const noexcept
        {
            return _TextureProxy;
        }
        ABYTEK_FORCE_INLINE auto& InjectTextureProxy() noexcept
        {
            return _TextureProxy;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIUploadTexturePassProxy);
        virtual void Build(const TW_Valid<A_RHIUploadTexturePass>& Pass);
        virtual void Release() override;
    };
}