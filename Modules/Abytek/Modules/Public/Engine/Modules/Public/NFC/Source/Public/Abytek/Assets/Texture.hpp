#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/World/WorldContext.hpp"
#include "Abytek/Cookable.hpp"
#include "Abytek/Renderable.hpp"
#include "Abytek/RHIImage.hpp"

   
namespace Abytek
{
    class F_TextureRenderProxy;
    
    struct F_TextureImageBulkHeader
    {
        U64 PayloadOffsetInBytes = 0;
        U64 PayloadSizeInBytes = 0;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_TextureImageBulkHeader& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.PayloadOffsetInBytes);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.PayloadSizeInBytes);
            return F_FeedbackStatus::MakeSucceeded();
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_TextureImageBulkHeader& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.PayloadOffsetInBytes);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.PayloadSizeInBytes);
            return F_FeedbackStatus::MakeSucceeded();
        }
    };
    
    struct F_TextureSetting
    {
        ABYTEK_BEGIN_REFLECTOR(I_Serializable)
        ABYTEK_END_REFLECTOR(F_TextureSetting);
        
        B8 EnableSRV = true;
        B8 EnableUAV = false;
        B8 EnableRTV = false;
        B8 EnableDSV = false;
        
        B8 CanUseRHIResourceStaticAccess() const noexcept
        {
            return (
                EnableSRV 
                && !EnableUAV
                && !EnableRTV
                && !EnableDSV
            );
        }
        E_RHIResourceAdditionalFlag GetRHIResourceAdditionalFlags() const noexcept
        {
            E_RHIResourceAdditionalFlag Result = E_RHIResourceAdditionalFlag::NONE;
            if (CanUseRHIResourceStaticAccess())
            {
                Result |= E_RHIResourceAdditionalFlag::STATIC_AFTER_SUBMIT;
            }
            return Result;
        }
        TF_Optional<F_RHIResourceAccess> GetRHIResourceStaticAccess() const noexcept
        {
            if (CanUseRHIResourceStaticAccess())
            {
                return F_RHIResourceAccess::MakeSRV();
            }
            return {};
        }
        F_RHIResourceAccess GetRHIResourceAccessCapabilities() const noexcept
        {
            F_RHIResourceAccess Result = F_RHIResourceAccess::MakeBaseCapabilities();
            if (EnableSRV)
            {
                Result.GPU |= E_RHIResourceGPUAccess::SRV;
            }
            if (EnableUAV)
            {
                Result.GPU |= E_RHIResourceGPUAccess::UAV;
            }
            if (EnableRTV)
            {
                Result.GPU |= E_RHIResourceGPUAccess::RTV;
            }
            if (EnableDSV)
            {
                Result.GPU |= E_RHIResourceGPUAccess::DSV;
            }
            return Result;
        }
    };
    
    enum class E_TextureFileImportFlag : U8
    {
        NONE = 0x0,
        SUPPORT_HDR32 = 0x1,
        DEFAULT = NONE
    };
    ABYTEK_DEFINE_FLAG_OPERATORS(E_TextureFileImportFlag);
    
    struct F_TextureFileImportConfig
    {
        E_TextureFileImportFlag Flags = E_TextureFileImportFlag::DEFAULT;
    };
    
    class ABYTEK_ENGINE_NFC_API F_Texture : public A_WorldContext, public A_Renderable, public I_Cookable
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_WorldContext)
        ABYTEK_END_REFLECTOR(F_Texture);
        
    protected:
        TF_Optional<F_TextureImageBulkHeader> _LastImageBulkHeader;
        TF_Optional<E_RHIFormat> _LastFormat;
        TF_Optional<F_RHIImageSettingMinimal> _LastImageSettingMinimal;
        TF_Optional<F_RHIImage> _NewImage;
        
        TF_Optional<F_TextureImageBulkHeader> _TempSerializationData_LastImageBulkHeader;
        TF_Optional<E_RHIFormat> _TempSerializationData_LastFormat;
        TF_Optional<F_RHIImageSettingMinimal> _TempSerializationData_LastImageSettingMinimal;
        
        F_TextureSetting _Setting;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetLastImageBulkHeader() const noexcept
        {
            return _LastImageBulkHeader;
        }
        ABYTEK_FORCE_INLINE const auto& GetLastFormat() const noexcept
        {
            return _LastFormat;
        }
        ABYTEK_FORCE_INLINE const auto& GetLastImageSettingMinimal() const noexcept
        {
            return _LastImageSettingMinimal;
        }
        ABYTEK_FORCE_INLINE const auto& GetNewImage() const noexcept
        {
            return _NewImage;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetSetting() const noexcept
        {
            return _Setting;
        }
        
        ABYTEK_FORCE_INLINE E_RHIFormat GetFormat() const noexcept
        {
            if (_NewImage)
            {
                return _NewImage->Setting.Format;
            }
            if (_LastFormat)
            {
                return *_LastFormat;
            }
            return E_RHIFormat::NONE;
        }
        ABYTEK_FORCE_INLINE F_Vector3_U32 GetSize() const noexcept
        {
            if (_NewImage)
            {
                return { 
                    _NewImage->Setting.Width,
                    _NewImage->Setting.Height,
                    _NewImage->Setting.Depth
                };
            }
            if (_LastImageSettingMinimal)
            {
                return { 
                    _LastImageSettingMinimal->Width,
                    _LastImageSettingMinimal->Height,
                    _LastImageSettingMinimal->Depth
                };
            }
            return F_Vector3_U32::Zero();
        }
        ABYTEK_FORCE_INLINE U32 GetDimensionCount() const noexcept
        {
            if (_NewImage)
            {
                return _NewImage->Setting.DimensionCount;
            }
            if (_LastImageSettingMinimal)
            {
                return _LastImageSettingMinimal->DimensionCount;
            }
            return 0;
        }
        ABYTEK_FORCE_INLINE U32 GetMipLevelCount() const noexcept
        {
            if (_NewImage)
            {
                return _NewImage->Setting.MipLevelCount;
            }
            if (_LastImageSettingMinimal)
            {
                return _LastImageSettingMinimal->MipLevelCount;
            }
            return 0;
        }
        ABYTEK_FORCE_INLINE U32 GetArraySize() const noexcept
        {
            if (_NewImage)
            {
                return _NewImage->Setting.ArraySize;
            }
            if (_LastImageSettingMinimal)
            {
                return _LastImageSettingMinimal->ArraySize;
            }
            return 0;
        }
        ABYTEK_FORCE_INLINE E_RHIImageFlag GetFlags() const noexcept
        {
            if (_NewImage)
            {
                return _NewImage->Setting.Flags;
            }
            if (_LastImageSettingMinimal)
            {
                return _LastImageSettingMinimal->Flags;
            }
            return E_RHIImageFlag::NONE;
        }
        
    public:
        F_Texture(const F_SerializableObjectInitParams& InitParam);
        ~F_Texture() override;
        
    protected:
        void OnLoad() override;
        void OnUnload() override;
        
    protected:
        F_FeedbackStatus BinarySerialize(F_SerializableObjectBinarySerializeParams& Params) override;
        F_FeedbackStatus BinaryDeserialize(F_SerializableObjectBinaryDeserializeParams& Params) override;
        
    protected:
        void OnCleanUpAfterSaving(const TW_Valid<F_SerializableEnvironment>& Environment) override;
        
    public:
        B8 IsRenderable() const override;
        
    protected:
        TS<A_RenderProxy> CreateRenderProxy() override;
        void OnCreateRenderState() override;
        void OnDestroyRenderState() override;
        
    public:
        void Import(const TF_Span<const U8>& Bytes, const F_TextureFileImportConfig& Config = {}, const TF_Optional<F_TextureSetting>& Setting = {});
        void Import(const F_Text& FilePath, const F_TextureFileImportConfig& Config = {}, const TF_Optional<F_TextureSetting>& Setting = {});
        void Import(const F_RHIImageROView& ImageView, const TF_Optional<F_TextureSetting>& Setting = {});
        template<B8 __EnableWrite, E_RHIFormat __Format>
        void Import(const TF_RHIImageRawView<__EnableWrite, __Format>& ImageRawView, const TF_Optional<F_TextureSetting>& Setting = {})
        {
            F_RHIImage Image(ImageRawView);
            return Import(Image, Setting);
        }
        template<E_RHIFormat __Format, typename __F_Allocator>
        void Import(const TF_RHIImageRaw<__Format, __F_Allocator>& ImageRaw, const TF_Optional<F_TextureSetting>& Setting = {})
        {
            F_RHIImage Image(ImageRaw);
            return Import(Image, Setting);
        }
        void Import(B8 EnablePayload, E_RHIFormat Format, const F_RHIImageSettingMinimal& SettingMinimal, const TF_Optional<F_TextureSetting>& Setting = {})
        {
            F_RHIImage Image(EnablePayload, Format, SettingMinimal);
            return Import(Image, Setting);
        }
        
    public:
        void UpdateSetting(const F_TextureSetting& Setting);
        
    public:
        B8 LoadImage_(F_RHIImage& OutImage);
    };
}
