#include "Abytek/Assets/Texture.hpp"
#include "Abytek/Assets/Render/TextureRenderProxy.hpp"
#include "Abytek/Frame/FrameHelper.hpp"
#include "stb_image.h"
#include "fp16.h"


namespace Abytek
{
    ABYTEK_REFLECT(F_TextureSetting)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_TextureSetting"));
        
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(EnableSRV);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(EnableUAV);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(EnableRTV);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(EnableDSV);
    }
    
    ABYTEK_REFLECT(F_Texture)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_Texture"));
        
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(_Setting);
    }

    F_Texture::F_Texture(const F_SerializableObjectInitParams& InitParam) :
        A_WorldContext(InitParam)
    {
    }
    F_Texture::~F_Texture()
    {
    }

    void F_Texture::OnLoad()
    {
        SetupRenderable();
    }
    void F_Texture::OnUnload()
    {
        CleanUpRenderable();
    }

    F_FeedbackStatus F_Texture::BinarySerialize(F_SerializableObjectBinarySerializeParams& Params)
    {
        ABYTEK_FEEDBACK_STATUS_CHECK(
            A_SerializableObject::BinarySerialize(Params)
        );
        
        F_RHIImage Image;
        if (LoadImage_(Image))
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(Params.MainView << true);
            
            F_TextureImageBulkHeader ImageBulkHeader;
            Params.BulkView.Shift<F_ArchiveData>(0);
            ImageBulkHeader.PayloadOffsetInBytes = Params.BulkView.Offset;
            ABYTEK_FEEDBACK_STATUS_CHECK(
                Params.BulkView << Image  
            );
            ImageBulkHeader.PayloadSizeInBytes = Params.BulkView.Offset - ImageBulkHeader.PayloadOffsetInBytes;
            
            ABYTEK_FEEDBACK_STATUS_CHECK(Params.MainView << ImageBulkHeader);
            _TempSerializationData_LastImageBulkHeader = ImageBulkHeader;
            
            ABYTEK_FEEDBACK_STATUS_CHECK(Params.MainView << Image.Setting.Format);
            _TempSerializationData_LastFormat = Image.Setting.Format;
            
            ABYTEK_FEEDBACK_STATUS_CHECK(Params.MainView << static_cast<const F_RHIImageSettingMinimal&>(Image.Setting));
            _TempSerializationData_LastImageSettingMinimal = Image.Setting;
        }
        else
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(Params.MainView << false);
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus F_Texture::BinaryDeserialize(F_SerializableObjectBinaryDeserializeParams& Params)
    {
        ABYTEK_FEEDBACK_STATUS_CHECK(
            A_SerializableObject::BinaryDeserialize(Params)
        );
        
        B8 HasLastImageBulk = false;
        ABYTEK_FEEDBACK_STATUS_CHECK(Params.MainView >> HasLastImageBulk);
        if (HasLastImageBulk)
        {
            F_TextureImageBulkHeader ImageBulkHeader;
            ABYTEK_FEEDBACK_STATUS_CHECK(
                Params.MainView >> ImageBulkHeader
            );
            _LastImageBulkHeader = ImageBulkHeader;
            
            E_RHIFormat Format;
            ABYTEK_FEEDBACK_STATUS_CHECK(
                Params.MainView >> Format
            );
            _LastFormat = Format;
            
            F_RHIImageSettingMinimal ImageSettingMinimal;
            ABYTEK_FEEDBACK_STATUS_CHECK(
                Params.MainView >> ImageSettingMinimal
            );
            _LastImageSettingMinimal = ImageSettingMinimal;
        }
        else
        {
            _LastImageBulkHeader = {};
            _LastFormat = {};
            _LastImageSettingMinimal = {};
        }
        return F_FeedbackStatus::MakeSucceeded();
    }

    void F_Texture::OnCleanUpAfterSaving(const TW_Valid<F_SerializableEnvironment>& Environment)
    {
        if (Environment == GetEnvironment())
        {
            _LastImageBulkHeader = _TempSerializationData_LastImageBulkHeader;
            _LastFormat = _TempSerializationData_LastFormat;
            _LastImageSettingMinimal = _TempSerializationData_LastImageSettingMinimal;
            _NewImage = {};
        }
        _TempSerializationData_LastImageBulkHeader = {};
        _TempSerializationData_LastFormat = {};
        _TempSerializationData_LastImageSettingMinimal = {};
    }

    B8 F_Texture::IsRenderable() const
    {
        if (!A_Renderable::IsRenderable())
        {
            return false;
        }
        return (static_cast<B8>(_LastImageBulkHeader) && GetPackageName()) || static_cast<B8>(_NewImage);
    }

    TS<A_RenderProxy> F_Texture::CreateRenderProxy()
    {
        return TS<F_TextureRenderProxy>()(ABYTEK_WTHIS());
    }
    void F_Texture::OnCreateRenderState()
    {
        H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
            [
                RenderProxy = GetRenderProxy().FastCast<F_TextureRenderProxy>()
#ifdef ABYTEK_DEBUG_INFO
                , CachedName = GetName()
#endif
            ]() mutable
            {
#ifdef ABYTEK_DEBUG_INFO
                RenderProxy->_DebugName = CachedName;
#endif
            }
        );
        
        F_RHIImage Image;
        B8 Status = LoadImage_(Image);
        ABYTEK_ENGINE_NFC_ASSERT(Status) << "Failed to load the image while being renderable";
        H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
            [
                RenderProxy = GetRenderProxy().FastCast<F_TextureRenderProxy>(), 
                CachedImage = ABYTEK_MOVE(Image),
                CachedSetting = ABYTEK_MOVE(_Setting),
                Format = GetFormat(),
                Size = GetSize(),
                DimensionCount = GetDimensionCount(),
                MipLevelCount = GetMipLevelCount(),
                ArraySize = GetArraySize(),
                Flags = GetFlags()
            ]() mutable
            {
                RenderProxy->_TempImage = TS_Unmanaged<F_RHIImage>()(
                    ABYTEK_MOVE(CachedImage)    
                );
                RenderProxy->_Setting = ABYTEK_MOVE(CachedSetting);
                RenderProxy->_Format = Format;
                RenderProxy->_ImageSettingMinimal.Width = Size.X;
                RenderProxy->_ImageSettingMinimal.Height = Size.Y;
                RenderProxy->_ImageSettingMinimal.Depth = Size.Z;
                RenderProxy->_ImageSettingMinimal.DimensionCount = DimensionCount;
                RenderProxy->_ImageSettingMinimal.MipLevelCount = MipLevelCount;
                RenderProxy->_ImageSettingMinimal.ArraySize = ArraySize;
                RenderProxy->_ImageSettingMinimal.Flags = Flags;
            }
        );
    }
    void F_Texture::OnDestroyRenderState()
    {
    }

    namespace Internal::Texture
    {
        template<Sz __ChannelSize, Sz __NumChannels>
        constexpr E_RHIFormat ResolveFormat_HDR()
        {
            if (
                (__ChannelSize == 2)
                && (__NumChannels == 1)
            )
            {
                return E_RHIFormat::R16_FLOAT;
            }
            if (
                (__ChannelSize == 2)
                && (__NumChannels == 2)
            )
            {
                return E_RHIFormat::R16G16_FLOAT;
            }
            if (
                (__ChannelSize == 2)
                && (__NumChannels == 3)
            )
            {
                return E_RHIFormat::R16G16B16A16_FLOAT;
            }
            if (
                (__ChannelSize == 2)
                && (__NumChannels == 4)
            )
            {
                return E_RHIFormat::R16G16B16A16_FLOAT;
            }
            if (
                (__ChannelSize == 4)
                && (__NumChannels == 1)
            )
            {
                return E_RHIFormat::R32_FLOAT;
            }
            if (
                (__ChannelSize == 4)
                && (__NumChannels == 2)
            )
            {
                return E_RHIFormat::R32G32_FLOAT;
            }
            if (
                (__ChannelSize == 4)
                && (__NumChannels == 3)
            )
            {
                return E_RHIFormat::R32G32B32A32_FLOAT;
            }
            if (
                (__ChannelSize == 4)
                && (__NumChannels == 4)
            )
            {
                return E_RHIFormat::R32G32B32A32_FLOAT;
            }
            return E_RHIFormat::NONE;
        }
        
        template<Sz __NumChannels, typename __F_Pixel, typename __F_FormatData>
        struct TH_StorePixels_HDR;
        template<typename __F_Pixel, typename __F_FormatData>
        struct TH_StorePixels_HDR<1, __F_Pixel, __F_FormatData>
        {
            static void Invoke(const __F_Pixel* Pixels, __F_FormatData& FormatData)
            {
                FormatData = Pixels[0];
            }
        };
        template<typename __F_Pixel, typename __F_FormatData>
        struct TH_StorePixels_HDR<2, __F_Pixel, __F_FormatData>
        {
            static void Invoke(const __F_Pixel* Pixels, __F_FormatData& FormatData)
            {
                FormatData.X = Pixels[0];
                FormatData.Y = Pixels[1];
            }
        };
        template<typename __F_Pixel, typename __F_FormatData>
        struct TH_StorePixels_HDR<3, __F_Pixel, __F_FormatData>
        {
            static void Invoke(const __F_Pixel* Pixels, __F_FormatData& FormatData)
            {
                FormatData.X = Pixels[0];
                FormatData.Y = Pixels[1];
                FormatData.Z = Pixels[2];
            }
        };
        template<typename __F_Pixel, typename __F_FormatData>
        struct TH_StorePixels_HDR<4, __F_Pixel, __F_FormatData>
        {
            static void Invoke(const __F_Pixel* Pixels, __F_FormatData& FormatData)
            {
                FormatData.X = Pixels[0];
                FormatData.Y = Pixels[1];
                FormatData.Z = Pixels[2];
                FormatData.W = Pixels[3];
            }
        };
        
        using F_STBBuildImageFunction_HDR = void(const U8* PixelBytes, U32 Width, U32 Height, const F_TextureFileImportConfig& Config, F_RHIImage& OutImage);
        template<Sz __ChannelSize, Sz __NumChannels>
        void STBBuildImage_HDR(const U8* PixelBytes, U32 Width, U32 Height, const F_TextureFileImportConfig& Config, F_RHIImage& OutImage)
        {
            static constexpr E_RHIFormat Format = ResolveFormat_HDR<__ChannelSize, __NumChannels>();
            static_assert(Format != E_RHIFormat::NONE);
            using F_FormatData = TF_RHIFormatData<Format>;
            
            using F_Pixel = Abytek::TF_UInt<__ChannelSize>;
            const F_Pixel* Pixels = (const F_Pixel*)PixelBytes;
            
            TF_RHIImageRaw<Format> ImageRaw(true, Width, Height);
            TF_RHIImageRawAccessor<true, Format> ImageRawAccessor(ImageRaw);
            for (U32 Y = 0; Y < Height; ++Y)
            {
                for (U32 X = 0; X < Width; ++X)
                {
                    TH_StorePixels_HDR<__NumChannels, F_Pixel, F_FormatData>::Invoke(
                        Pixels
                        + X * __NumChannels
                        + Y * Width * __NumChannels,
                        ImageRawAccessor[F_Vector2_U32(X, Y)]
                    );
                }
            }
            
            OutImage = F_RHIImage(ImageRaw);
        }
        
        template<Sz __ChannelSize, Sz __NumChannels>
        constexpr E_RHIFormat ResolveFormat_NonHDR()
        {
            if (
                (__ChannelSize == 1)
                && (__NumChannels == 1)
            )
            {
                return E_RHIFormat::R8_UNORM;
            }
            if (
                (__ChannelSize == 1)
                && (__NumChannels == 2)
            )
            {
                return E_RHIFormat::R8G8_UNORM;
            }
            if (
                (__ChannelSize == 1)
                && (__NumChannels == 3)
            )
            {
                return E_RHIFormat::R8G8B8A8_UNORM;
            }
            if (
                (__ChannelSize == 1)
                && (__NumChannels == 4)
            )
            {
                return E_RHIFormat::R8G8B8A8_UNORM;
            }
            if (
                (__ChannelSize == 2)
                && (__NumChannels == 1)
            )
            {
                return E_RHIFormat::R16_UNORM;
            }
            if (
                (__ChannelSize == 2)
                && (__NumChannels == 2)
            )
            {
                return E_RHIFormat::R16G16_UNORM;
            }
            if (
                (__ChannelSize == 2)
                && (__NumChannels == 3)
            )
            {
                return E_RHIFormat::R16G16B16A16_UNORM;
            }
            if (
                (__ChannelSize == 2)
                && (__NumChannels == 4)
            )
            {
                return E_RHIFormat::R16G16B16A16_UNORM;
            }
            return E_RHIFormat::NONE;
        }
        
        template<Sz __NumChannels, typename __F_Pixel, typename __F_FormatData>
        struct TH_StorePixels_NonHDR;
        template<typename __F_Pixel, typename __F_FormatData>
        struct TH_StorePixels_NonHDR<1, __F_Pixel, __F_FormatData>
        {
            static void Invoke(const __F_Pixel* Pixels, __F_FormatData& FormatData)
            {
                FormatData = Pixels[0];
            }
        };
        template<typename __F_Pixel, typename __F_FormatData>
        struct TH_StorePixels_NonHDR<2, __F_Pixel, __F_FormatData>
        {
            static void Invoke(const __F_Pixel* Pixels, __F_FormatData& FormatData)
            {
                FormatData.X = Pixels[0];
                FormatData.Y = Pixels[1];
            }
        };
        template<typename __F_Pixel, typename __F_FormatData>
        struct TH_StorePixels_NonHDR<3, __F_Pixel, __F_FormatData>
        {
            static void Invoke(const __F_Pixel* Pixels, __F_FormatData& FormatData)
            {
                FormatData.X = Pixels[0];
                FormatData.Y = Pixels[1];
                FormatData.Z = Pixels[2];
            }
        };
        template<typename __F_Pixel, typename __F_FormatData>
        struct TH_StorePixels_NonHDR<4, __F_Pixel, __F_FormatData>
        {
            static void Invoke(const __F_Pixel* Pixels, __F_FormatData& FormatData)
            {
                FormatData.X = Pixels[0];
                FormatData.Y = Pixels[1];
                FormatData.Z = Pixels[2];
                FormatData.W = Pixels[3];
            }
        };
        
        using F_STBBuildImageFunction_NonHDR = void(const U8* PixelBytes, U32 Width, U32 Height, const F_TextureFileImportConfig& Config, F_RHIImage& OutImage);
        template<Sz __ChannelSize, Sz __NumChannels>
        void STBBuildImage_NonHDR(const U8* PixelBytes, U32 Width, U32 Height, const F_TextureFileImportConfig& Config, F_RHIImage& OutImage)
        {
            static constexpr E_RHIFormat Format = ResolveFormat_NonHDR<__ChannelSize, __NumChannels>();
            static_assert(Format != E_RHIFormat::NONE);
            using F_FormatData = TF_RHIFormatData<Format>;
            
            using F_Pixel = Abytek::TF_UInt<__ChannelSize>;
            const F_Pixel* Pixels = (const F_Pixel*)PixelBytes;
            
            TF_RHIImageRaw<Format> ImageRaw(true, Width, Height);
            TF_RHIImageRawAccessor<true, Format> ImageRawAccessor(ImageRaw);
            for (U32 Y = 0; Y < Height; ++Y)
            {
                for (U32 X = 0; X < Width; ++X)
                {
                    TH_StorePixels_NonHDR<__NumChannels, F_Pixel, F_FormatData>::Invoke(
                        Pixels
                        + X * __NumChannels
                        + Y * Width * __NumChannels,
                        ImageRawAccessor[F_Vector2_U32(X, Y)]
                    );
                }
            }
            
            OutImage = F_RHIImage(ImageRaw);
        }
        
        F_FeedbackStatus Decode(const TF_Span<const U8>& Bytes, const F_TextureFileImportConfig& Config, F_RHIImage& OutImage)
        {
            int Width = 0;
            int Height = 0;
            int NumChannels = 0;
            
            if (stbi_is_hdr_from_memory(Bytes.data(), static_cast<int>(Bytes.size())))
            {
                if (FlagHas(Config.Flags, E_TextureFileImportFlag::SUPPORT_HDR32))
                {
                    float* Pixels = stbi_loadf_from_memory(
                        Bytes.data(), 
                        static_cast<int>(Bytes.size()), 
                        &Width, 
                        &Height, 
                        &NumChannels, 
                        0
                    );
                
                    F_STBBuildImageFunction_HDR* BuildFunctions[] = {
                        &STBBuildImage_HDR<4, 1>,
                        &STBBuildImage_HDR<4, 2>,
                        &STBBuildImage_HDR<4, 3>,
                        &STBBuildImage_HDR<4, 4>
                    };
                    BuildFunctions[NumChannels - 1](
                        (const U8*)Pixels,
                        Width,
                        Height,
                        Config,
                        OutImage
                    );
                
                    stbi_image_free(Pixels);
                }
                else
                {
                    float* Pixels = stbi_loadf_from_memory(
                        Bytes.data(), 
                        static_cast<int>(Bytes.size()), 
                        &Width, 
                        &Height, 
                        &NumChannels, 
                        0
                    );
                    
                    TF_Vector<U16> Pixels16;
                    U32 NumPixels = Width * Height * NumChannels;
                    Pixels16.resize(NumPixels);
                    for (U32 Idx = 0; Idx < NumPixels; ++Idx)
                    {
                        Pixels16[Idx] = fp16_ieee_from_fp32_value(Pixels[Idx]);
                    }
                
                    F_STBBuildImageFunction_HDR* BuildFunctions[] = {
                        &STBBuildImage_HDR<2, 1>,
                        &STBBuildImage_HDR<2, 2>,
                        &STBBuildImage_HDR<2, 3>,
                        &STBBuildImage_HDR<2, 4>
                    };
                    BuildFunctions[NumChannels - 1](
                        (const U8*)Pixels16.data(),
                        Width,
                        Height,
                        Config,
                        OutImage
                    );
                
                    stbi_image_free(Pixels);
                }
                return F_FeedbackStatus::MakeSucceeded();
            }
            else if (stbi_is_16_bit_from_memory(Bytes.data(), static_cast<int>(Bytes.size())))
            {
                stbi_us* Pixels = stbi_load_16_from_memory(
                    Bytes.data(), 
                    static_cast<int>(Bytes.size()), 
                    &Width, 
                    &Height, 
                    &NumChannels, 
                    0
                );
                
                F_STBBuildImageFunction_NonHDR* BuildFunctions[] = {
                    &STBBuildImage_NonHDR<2, 1>,
                    &STBBuildImage_NonHDR<2, 2>,
                    &STBBuildImage_NonHDR<2, 3>,
                    &STBBuildImage_NonHDR<2, 4>
                };
                BuildFunctions[NumChannels - 1](
                    (const U8*)Pixels,
                    Width,
                    Height,
                    Config,
                    OutImage
                );
                
                stbi_image_free(Pixels);
                return F_FeedbackStatus::MakeSucceeded();
            }
            else
            {
                stbi_uc* Pixels = stbi_load_from_memory(
                    Bytes.data(), 
                    static_cast<int>(Bytes.size()), 
                    &Width, 
                    &Height, 
                    &NumChannels, 
                    0
                );
                
                F_STBBuildImageFunction_NonHDR* BuildFunctions[] = {
                    &STBBuildImage_NonHDR<1, 1>,
                    &STBBuildImage_NonHDR<1, 2>,
                    &STBBuildImage_NonHDR<1, 3>,
                    &STBBuildImage_NonHDR<1, 4>
                };
                BuildFunctions[NumChannels - 1](
                    (const U8*)Pixels,
                    Width,
                    Height,
                    Config,
                    OutImage
                );
                
                stbi_image_free(Pixels);
                return F_FeedbackStatus::MakeSucceeded();
            }
            return F_FeedbackStatus::MakeSucceeded();
        }
    }
    void F_Texture::Import(const TF_Span<const U8>& Bytes, const F_TextureFileImportConfig& Config, const TF_Optional<F_TextureSetting>& Setting)
    {
        F_RHIImage Image;
        ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
            Internal::Texture::Decode(Bytes, Config, Image)  
        );
        _NewImage = ABYTEK_MOVE(Image);
        if (Setting)
        {
            _Setting = *Setting;
        }
        MarkPackageDirty();
        RecreateRenderState();
    }
    void F_Texture::Import(const F_Text& FilePath, const F_TextureFileImportConfig& Config, const TF_Optional<F_TextureSetting>& Setting)
    {
        F_Text AbsoluteFilePath;
        ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
            GetEnvironment()->ResolveAbsolutePath(FilePath, AbsoluteFilePath)
        );
        
        TF_Vector<U8> Bytes;
        ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
            H_FSUtilities::ReadFileBinary(AbsoluteFilePath, Bytes)
        );
        Import(
            Bytes,
            Config,
            Setting
        );
    }
    void F_Texture::Import(const F_RHIImageROView& ImageView, const TF_Optional<F_TextureSetting>& Setting)
    {
        _NewImage = ImageView;
        if (Setting)
        {
            _Setting = *Setting;
        }
        MarkPackageDirty();
        RecreateRenderState();
    }

    void F_Texture::UpdateSetting(const F_TextureSetting& Setting)
    {
        _Setting = Setting;
        MarkPackageDirty();
        RecreateRenderState();
    }

    B8 F_Texture::LoadImage_(F_RHIImage& OutImage)
    {
        if (_NewImage)
        {
            OutImage = *_NewImage;
            return true;
        }
        if (!_LastImageBulkHeader)
        {
            return false;
        }
        auto Package = GetPackage();
        if (!Package)
        {
            return false;
        }
        
        TF_Vector<U8> Bytes;
        Package->LoadBulkPayload(
            _LastImageBulkHeader->PayloadOffsetInBytes,    
            _LastImageBulkHeader->PayloadSizeInBytes,
            Bytes
        );
        
        F_Archive Archive = F_Archive::From(Bytes);
        F_ArchiveReadOnlyView ArchiveView = F_ArchiveReadOnlyView::From(Archive);
        ArchiveView.HasDevelopmentBuild = GetEnvironment()->HasDevelopmentBuild();
        
        ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
            ArchiveView >> OutImage  
        );
        return true;
    }
}
