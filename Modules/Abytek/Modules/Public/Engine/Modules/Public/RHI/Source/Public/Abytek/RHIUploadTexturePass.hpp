#pragma once

#include "Abytek/RHIPass.hpp"
#include "Abytek/RHIResource.hpp"


namespace Abytek
{
    class A_RHIResource;
    class A_RHISubmissionList;
    
    struct F_RHIUploadTexturePassBuildParams : F_RHIPassBuildParams
    {
        F_RHITextureDataView TextureDataView;
        TS<A_RHIResource> Texture;
    };
    
    class ABYTEK_ENGINE_RHI_API A_RHIUploadTexturePass : public A_RHIPass
    {
    private:
        F_RHITextureDataView _TextureDataView;
        TS<A_RHIResource> _Texture;

    public:
        ABYTEK_FORCE_INLINE const auto& GetTextureDataView() const noexcept
        {
            return _TextureDataView;
        }
        ABYTEK_FORCE_INLINE auto& InjectTextureDataView() noexcept
        {
            return _TextureDataView;
        }
        ABYTEK_FORCE_INLINE const auto& GetTexture() const noexcept
        {
            return _Texture;
        }
        ABYTEK_FORCE_INLINE auto& InjectTexture() noexcept
        {
            return _Texture;
        }

    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIUploadTexturePass);
        virtual void Build(const F_RHIUploadTexturePassBuildParams& BuildParams);
        virtual void Release() override;
        
    public:
        virtual E_RHIPassClass GetPassClass() const override
        {
            return E_RHIPassClass::BLIT;
        }
        
    public:
        virtual TS_Valid<A_RHIPassProxy> CreateProxy() override;
        
    public:
        virtual B8 CanDetachCopyPass();
        virtual void DetachCopyPass(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
    };
}