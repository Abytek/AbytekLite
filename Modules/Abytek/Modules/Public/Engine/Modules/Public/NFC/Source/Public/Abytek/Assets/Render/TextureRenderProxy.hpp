#pragma once

#include "Abytek/Renderer/WorldContextRenderProxy.hpp"
#include "Abytek/Assets/Texture.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_NFC_API F_TextureRenderProxy : public A_WorldContextRenderProxy
    {
    public:
        friend class F_Texture;
        
    private:
        TS_Unmanaged<F_RHIImage> _TempImage;
        F_TextureSetting _Setting;
        E_RHIFormat _Format = E_RHIFormat::NONE;
        F_RHIImageSettingMinimal _ImageSettingMinimal;
        TS<A_RHIResource> _RHITexture;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetSetting() const noexcept
        {
            return _Setting;
        }
        ABYTEK_FORCE_INLINE const auto& GetRHITexture() const noexcept
        {
            return _RHITexture;
        }
        
        ABYTEK_FORCE_INLINE E_RHIFormat GetFormat() const noexcept
        {
            return _Format;
        }
        ABYTEK_FORCE_INLINE F_Vector3_U32 GetSize() const noexcept
        {
            return { 
                _ImageSettingMinimal.Width,
                _ImageSettingMinimal.Height,
                _ImageSettingMinimal.Depth
            };
        }
        ABYTEK_FORCE_INLINE U32 GetDimensionCount() const noexcept
        {
            return _ImageSettingMinimal.DimensionCount;
        }
        ABYTEK_FORCE_INLINE U32 GetMipLevelCount() const noexcept
        {
            return _ImageSettingMinimal.MipLevelCount;
        }
        ABYTEK_FORCE_INLINE U32 GetArraySize() const noexcept
        {
            return _ImageSettingMinimal.ArraySize;
        }
        ABYTEK_FORCE_INLINE E_RHIImageFlag GetFlags() const noexcept
        {
            return _ImageSettingMinimal.Flags;
        }
        
    public:
        F_TextureRenderProxy(const TW_Valid<F_Texture>& Owner);
        ~F_TextureRenderProxy() override;
        
    protected:
        void OnCreateRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        void OnDestroyRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
    };
}
