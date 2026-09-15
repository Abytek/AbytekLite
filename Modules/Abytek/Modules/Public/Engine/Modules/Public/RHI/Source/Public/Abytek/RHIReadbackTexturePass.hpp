#pragma once

#include "Abytek/RHIPass.hpp"
#include "Abytek/RHIResource.hpp"


namespace Abytek
{
    class A_RHISubmissionList;
    
    using F_RHIReadbackTextureCallback = TF_Function<void(const F_RHITextureDataView& TextureDataView)>;
    
    struct F_RHIReadbackTexturePassBuildParams : F_RHIPassBuildParams
    {
        TS<A_RHIResource> Texture;
        F_RHIReadbackTextureCallback Callback;
    };
    
    class ABYTEK_ENGINE_RHI_API A_RHIReadbackTexturePass : public A_RHIPass
    {
    private:
        TS<A_RHIResource> _Texture;
        F_RHIReadbackTextureCallback _Callback;

    public:
        ABYTEK_FORCE_INLINE const auto& GetTexture() const noexcept
        {
            return _Texture;
        }
        ABYTEK_FORCE_INLINE auto& InjectTexture() noexcept
        {
            return _Texture;
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
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIReadbackTexturePass);
        virtual void Build(const F_RHIReadbackTexturePassBuildParams& BuildParams);
        virtual void Release() override;
        
    public:
        virtual E_RHIPassClass GetPassClass() override
        {
            return E_RHIPassClass::BLIT;
        }
        
    public:
        virtual TS_Valid<A_RHIPassProxy> CreateProxy() override;
        
    public:
        virtual B8 CanDetachCopyPass();
        virtual void DetachCopyPass(I_RHISubmissionItemContainer& SubmissionItemContainer);
    };
}