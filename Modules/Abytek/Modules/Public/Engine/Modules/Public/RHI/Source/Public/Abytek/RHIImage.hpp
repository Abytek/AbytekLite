#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"
#include "Abytek/RHIFormat.hpp"


namespace Abytek
{
    struct F_RHISubImageSettingRaw;
    template<typename __F_Allocator = TF_DefaultAllocator<U8>>
    struct TF_RHIImageSettingRaw;
    template<B8 __EnableWrite>
    struct TF_RHIImageSettingRawView;
    
    struct F_RHISubImageSetting;
    template<typename __F_Allocator = TF_DefaultAllocator<U8>>
    struct TF_RHIImageSetting;
    template<B8 __EnableWrite>
    struct TF_RHIImageSettingView;
    
    struct F_RHISubImageTexelStructureRaw;
    template<typename __F_Allocator = TF_DefaultAllocator<U8>>
    struct TF_RHIImageTexelStructureRaw;
    template<B8 __EnableWrite>
    struct TF_RHIImageTexelStructureRawView;
    
    struct F_RHISubImageMemoryStructure;
    template<typename __F_Allocator = TF_DefaultAllocator<U8>>
    struct TF_RHIImageMemoryStructure;
    template<B8 __EnableWrite>
    struct TF_RHIImageMemoryStructureView;
    
    template<E_RHIFormat __Format, typename __F_Allocator = TF_DefaultAllocator<U8>>
    struct TF_RHIImagePayloadRaw;
    template<B8 __EnableWrite, E_RHIFormat __Format>
    struct TF_RHIImagePayloadRawView;
    
    template<typename __F_Allocator = TF_DefaultAllocator<U8>>
    struct TF_RHIImagePayload;
    template<B8 __EnableWrite>
    struct TF_RHIImagePayloadView;
    
    template<E_RHIFormat __Format, typename __F_Allocator = TF_DefaultAllocator<U8>>
    struct TF_RHIImageRaw;
    template<B8 __EnableWrite, E_RHIFormat __Format>
    struct TF_RHIImageRawView;
    
    template<B8 __EnableWrite, E_RHIFormat __Format>
    struct TF_RHIImageRawAccessor;
    
    template<typename __F_Allocator = TF_DefaultAllocator<U8>>
    struct TF_RHIImage;
    template<B8 __EnableWrite>
    struct TF_RHIImageView;
    
    static constexpr Sz RHI_IMAGE_OPTIMAL_ROW_PITCH_ALIGNMENT = 256;
    
    struct F_RHIImageElement
    {
        U32 MipLevel = 0;
        U32 ArraySlice = 0;
        U32 PlaneIndex = 0;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_RHIImageElement& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.MipLevel);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.ArraySlice);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.PlaneIndex);
            return F_FeedbackStatus::MakeSucceeded();   
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_RHIImageElement& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.MipLevel);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.ArraySlice);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.PlaneIndex);
            return F_FeedbackStatus::MakeSucceeded();   
        }
    };
    using F_RHIImageElementSet = TF_SmallVector<F_RHIImageElement, 6>;
    
    struct F_RHIImageRawElement
    {
        U32 MipLevel = 0;
        U32 ArraySlice = 0;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_RHIImageRawElement& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.MipLevel);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.ArraySlice);
            return F_FeedbackStatus::MakeSucceeded();   
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_RHIImageRawElement& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.MipLevel);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.ArraySlice);
            return F_FeedbackStatus::MakeSucceeded();   
        }
    };
    using F_RHIImageRawElementSet = TF_SmallVector<F_RHIImageRawElement, 6>;
    
    inline U32 RHICalculateNumSubImages(E_RHIFormat Format, U32 MipLevelCount = 1, U32 ArraySize = 1)
    {
        return MipLevelCount * ArraySize * RHIFormatPlaneCount(Format);
    }

    enum class E_RHIImageFlag
    {
        NONE = 0x0,
        
        ARRAY = 0x1,
        CUBE = 0x2,

        DEFAULT = NONE
    };
    ABYTEK_DEFINE_FLAG_OPERATORS(E_RHIImageFlag);
    
    enum class E_RHIImageLayout : U8
    {
        NONE,
        
        LINEAR,
        
        DEFAULT = NONE
    };
    
    struct F_RHISubImageSettingRaw : F_RHIImageRawElement
    {
        U32 Width = 1;
        U32 Height = 1;
        U32 Depth = 1;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_RHISubImageSettingRaw& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << static_cast<const F_RHIImageRawElement&>(Value));
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Width);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Height);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Depth);
            return F_FeedbackStatus::MakeSucceeded();   
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_RHISubImageSettingRaw& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> static_cast<F_RHIImageRawElement&>(Value));
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Width);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Height);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Depth);
            return F_FeedbackStatus::MakeSucceeded();   
        }
    };
    template<typename __F_Allocator>
    struct TF_RHIImageSettingRaw
    {
        using F_Allocator = __F_Allocator;
        
        E_RHIFormat Format = E_RHIFormat::NONE;
        
        U32 Width = 1;
        U32 Height = 1;
        U32 Depth = 1;
        
        U32 DimensionCount = 2;
        
        U32 MipLevelCount = 1;
        U32 ArraySize = 1;
        
        E_RHIImageFlag Flags = E_RHIImageFlag::DEFAULT;
        
        ContainerTemplates::TF_SmallVector<F_RHISubImageSettingRaw, 1, TF_RebindAllocator<F_RHISubImageSettingRaw, F_Allocator>> SubImages;
        
        TF_RHIImageSettingRaw() = default;
        TF_RHIImageSettingRaw(const TF_RHIImageSettingRaw& X) = default;
        TF_RHIImageSettingRaw& operator = (const TF_RHIImageSettingRaw& X) = default;
        TF_RHIImageSettingRaw(TF_RHIImageSettingRaw&& X) = default;
        TF_RHIImageSettingRaw& operator = (TF_RHIImageSettingRaw&& X) = default;
        
        template<B8 __EnableWrite2>
        TF_RHIImageSettingRaw(const TF_RHIImageSettingRawView<__EnableWrite2>& X);
        template<B8 __EnableWrite2>
        TF_RHIImageSettingRaw& operator = (const TF_RHIImageSettingRawView<__EnableWrite2>& X);
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const TF_RHIImageSettingRaw& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Format);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Width);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Height);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Depth);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.DimensionCount);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.MipLevelCount);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.ArraySize);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Flags);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.SubImages);
            return F_FeedbackStatus::MakeSucceeded();   
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, TF_RHIImageSettingRaw& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Format);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Width);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Height);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Depth);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.DimensionCount);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.MipLevelCount);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.ArraySize);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Flags);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.SubImages);
            return F_FeedbackStatus::MakeSucceeded();   
        }
    };
    template<>
    struct TF_RHIImageSettingRawView<false>
    {
        static constexpr B8 EnableWrite = false;
        
        E_RHIFormat Format = E_RHIFormat::NONE;
        
        U32 Width = 1;
        U32 Height = 1;
        U32 Depth = 1;
        
        U32 DimensionCount = 2;
        
        U32 MipLevelCount = 1;
        U32 ArraySize = 1;
        
        E_RHIImageFlag Flags = E_RHIImageFlag::DEFAULT;
        
        TF_Span<
            std::conditional_t<
                EnableWrite,
                F_RHISubImageSettingRaw,
                const F_RHISubImageSettingRaw
            >    
        > SubImages;
        
        TF_RHIImageSettingRawView() = default;
        TF_RHIImageSettingRawView(const TF_RHIImageSettingRawView& X) = default;
        TF_RHIImageSettingRawView& operator = (const TF_RHIImageSettingRawView& X) = default;
        TF_RHIImageSettingRawView(TF_RHIImageSettingRawView&& X) = default;
        TF_RHIImageSettingRawView& operator = (TF_RHIImageSettingRawView&& X) = default;
        
        template<typename __F_Allocator2>
        TF_RHIImageSettingRawView(const TF_RHIImageSettingRaw<__F_Allocator2>& X);
        template<typename __F_Allocator2>
        TF_RHIImageSettingRawView& operator = (const TF_RHIImageSettingRaw<__F_Allocator2>& X);
        
        TF_RHIImageSettingRawView<false> GetReadOnly() const noexcept;
        TF_RHIImageSettingRawView<true> GetReadWrite() const noexcept;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const TF_RHIImageSettingRawView& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Format);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Width);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Height);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Depth);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.DimensionCount);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.MipLevelCount);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.ArraySize);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Flags);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.SubImages);
            return F_FeedbackStatus::MakeSucceeded();   
        }
    };
    template<>
    struct TF_RHIImageSettingRawView<true>
    {
        static constexpr B8 EnableWrite = true;
        
        E_RHIFormat Format = E_RHIFormat::NONE;
        
        U32 Width = 1;
        U32 Height = 1;
        U32 Depth = 1;
        
        U32 DimensionCount = 2;
        
        U32 MipLevelCount = 1;
        U32 ArraySize = 1;
        
        E_RHIImageFlag Flags = E_RHIImageFlag::DEFAULT;
        
        TF_Span<
            std::conditional_t<
                EnableWrite,
                F_RHISubImageSettingRaw,
                const F_RHISubImageSettingRaw
            >    
        > SubImages;
        
        TF_RHIImageSettingRawView() = default;
        TF_RHIImageSettingRawView(const TF_RHIImageSettingRawView& X) = default;
        TF_RHIImageSettingRawView& operator = (const TF_RHIImageSettingRawView& X) = default;
        TF_RHIImageSettingRawView(TF_RHIImageSettingRawView&& X) = default;
        TF_RHIImageSettingRawView& operator = (TF_RHIImageSettingRawView&& X) = default;
        
        template<typename __F_Allocator2>
        TF_RHIImageSettingRawView(TF_RHIImageSettingRaw<__F_Allocator2>& X);
        template<typename __F_Allocator2>
        TF_RHIImageSettingRawView& operator = (TF_RHIImageSettingRaw<__F_Allocator2>& X);
        
        TF_RHIImageSettingRawView<false> GetReadOnly() const noexcept;
        TF_RHIImageSettingRawView<true> GetReadWrite() const noexcept;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const TF_RHIImageSettingRawView& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Format);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Width);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Height);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Depth);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.DimensionCount);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.MipLevelCount);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.ArraySize);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Flags);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.SubImages);
            return F_FeedbackStatus::MakeSucceeded();   
        }
    };

    template <typename __F_Allocator>
    template <B8 __EnableWrite2>
    TF_RHIImageSettingRaw<__F_Allocator>::TF_RHIImageSettingRaw(const TF_RHIImageSettingRawView<__EnableWrite2>& X) :
        Format(X.Format),
    
        Width(X.Width),
        Height(X.Height),
        Depth(X.Depth),
    
        DimensionCount(X.DimensionCount),
    
        MipLevelCount(X.MipLevelCount),
        ArraySize(X.ArraySize),
    
        Flags(X.Flags),
    
        SubImages(X.SubImages.begin(), X.SubImages.end())
    {
    }
    template <typename __F_Allocator>
    template <B8 __EnableWrite2>
    TF_RHIImageSettingRaw<__F_Allocator>& TF_RHIImageSettingRaw<__F_Allocator>::operator = (const TF_RHIImageSettingRawView<__EnableWrite2>& X)
    {
        Format = X.Format;
        
        Width = X.Width;
        Height = X.Height;
        Depth = X.Depth;
        
        DimensionCount = X.DimensionCount;
        
        MipLevelCount = X.MipLevelCount;
        ArraySize = X.ArraySize;
        
        Flags = X.Flags;
        
        using F_SubImages = decltype(SubImages);
        SubImages = F_SubImages(X.SubImages.begin(), X.SubImages.end());
        return *this;
    }
    
    template <typename __F_Allocator2>
    TF_RHIImageSettingRawView<false>::TF_RHIImageSettingRawView(const TF_RHIImageSettingRaw<__F_Allocator2>& X) :
        Format(X.Format),
    
        Width(X.Width),
        Height(X.Height),
        Depth(X.Depth),
    
        DimensionCount(X.DimensionCount),
    
        MipLevelCount(X.MipLevelCount),
        ArraySize(X.ArraySize),
    
        Flags(X.Flags),
    
        SubImages(X.SubImages)
    {
    }
    template <typename __F_Allocator2>
    TF_RHIImageSettingRawView<false>& TF_RHIImageSettingRawView<false>::operator = (const TF_RHIImageSettingRaw<__F_Allocator2>& X)
    {
        Format = X.Format;
        
        Width = X.Width;
        Height = X.Height;
        Depth = X.Depth;
        
        DimensionCount = X.DimensionCount;
        
        MipLevelCount = X.MipLevelCount;
        ArraySize = X.ArraySize;
        
        Flags = X.Flags;
        
        SubImages = X.SubImages;
        return *this;
    }
    template <typename __F_Allocator2>
    TF_RHIImageSettingRawView<true>::TF_RHIImageSettingRawView(TF_RHIImageSettingRaw<__F_Allocator2>& X) :
        Format(X.Format),
    
        Width(X.Width),
        Height(X.Height),
        Depth(X.Depth),
    
        DimensionCount(X.DimensionCount),
    
        MipLevelCount(X.MipLevelCount),
        ArraySize(X.ArraySize),
    
        Flags(X.Flags),
    
        SubImages(X.SubImages)
    {
    }
    template <typename __F_Allocator2>
    TF_RHIImageSettingRawView<true>& TF_RHIImageSettingRawView<true>::operator = (TF_RHIImageSettingRaw<__F_Allocator2>& X)
    {
        Format = X.Format;
        
        Width = X.Width;
        Height = X.Height;
        Depth = X.Depth;
        
        DimensionCount = X.DimensionCount;
        
        MipLevelCount = X.MipLevelCount;
        ArraySize = X.ArraySize;
        
        Flags = X.Flags;
        
        SubImages = X.SubImages;
        return *this;
    }
    
    inline TF_RHIImageSettingRawView<false> TF_RHIImageSettingRawView<false>::GetReadOnly() const noexcept
    {
        return *(const TF_RHIImageSettingRawView<false>*)this;
    }
    inline TF_RHIImageSettingRawView<true> TF_RHIImageSettingRawView<false>::GetReadWrite() const noexcept
    {
        return *(const TF_RHIImageSettingRawView<true>*)this;
    }
    inline TF_RHIImageSettingRawView<false> TF_RHIImageSettingRawView<true>::GetReadOnly() const noexcept
    {
        return *(const TF_RHIImageSettingRawView<false>*)this;
    }
    inline TF_RHIImageSettingRawView<true> TF_RHIImageSettingRawView<true>::GetReadWrite() const noexcept
    {
        return *(const TF_RHIImageSettingRawView<true>*)this;
    }
    
    struct F_RHIImageSettingMinimal
    {
        U32 Width = 1;
        U32 Height = 1;
        U32 Depth = 1;
        
        U32 DimensionCount = 2;
        
        U32 MipLevelCount = 1;
        U32 ArraySize = 1;
        
        E_RHIImageFlag Flags = E_RHIImageFlag::DEFAULT;
        E_RHIImageLayout Layout = E_RHIImageLayout::DEFAULT;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_RHIImageSettingMinimal& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Width);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Height);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Depth);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.DimensionCount);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.MipLevelCount);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.ArraySize);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Flags);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Layout);
            return F_FeedbackStatus::MakeSucceeded();   
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_RHIImageSettingMinimal& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Width);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Height);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Depth);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.DimensionCount);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.MipLevelCount);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.ArraySize);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Flags);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Layout);
            return F_FeedbackStatus::MakeSucceeded();   
        }
    };
    
    struct F_RHISubImageSetting : F_RHIImageElement
    {
        U32 Width = 1;
        U32 Height = 1;
        U32 Depth = 1;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_RHISubImageSetting& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << static_cast<const F_RHIImageElement&>(Value));
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Width);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Height);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Depth);
            return F_FeedbackStatus::MakeSucceeded();   
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_RHISubImageSetting& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> static_cast<F_RHIImageElement&>(Value));
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Width);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Height);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Depth);
            return F_FeedbackStatus::MakeSucceeded();   
        }
    };
    template<typename __F_Allocator>
    struct TF_RHIImageSetting : F_RHIImageSettingMinimal
    {
        using F_Allocator = __F_Allocator;
        
        E_RHIFormat Format = E_RHIFormat::NONE;
        
        ContainerTemplates::TF_SmallVector<F_RHISubImageSetting, 1, TF_RebindAllocator<F_RHISubImageSetting, F_Allocator>> SubImages;
        
        TF_RHIImageSetting() = default;
        TF_RHIImageSetting(const TF_RHIImageSetting& X) = default;
        TF_RHIImageSetting& operator = (const TF_RHIImageSetting& X) = default;
        TF_RHIImageSetting(TF_RHIImageSetting&& X) = default;
        TF_RHIImageSetting& operator = (TF_RHIImageSetting&& X) = default;
        
        template<B8 __EnableWrite2>
        TF_RHIImageSetting(const TF_RHIImageSettingView<__EnableWrite2>& X);
        template<B8 __EnableWrite2>
        TF_RHIImageSetting& operator = (const TF_RHIImageSettingView<__EnableWrite2>& X);
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const TF_RHIImageSetting& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << static_cast<const F_RHIImageSettingMinimal&>(Value));
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Format);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.SubImages);
            return F_FeedbackStatus::MakeSucceeded();   
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, TF_RHIImageSetting& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> static_cast<F_RHIImageSettingMinimal&>(Value));
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Format);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.SubImages);
            return F_FeedbackStatus::MakeSucceeded();   
        }
    };
    template<>
    struct TF_RHIImageSettingView<false> : F_RHIImageSettingMinimal
    {
        static constexpr B8 EnableWrite = false;
        
        E_RHIFormat Format = E_RHIFormat::NONE;
        
        TF_Span<
            std::conditional_t<
                EnableWrite,
                F_RHISubImageSetting,
                const F_RHISubImageSetting
            >    
        > SubImages;
        
        TF_RHIImageSettingView() = default;
        TF_RHIImageSettingView(const TF_RHIImageSettingView& X) = default;
        TF_RHIImageSettingView& operator = (const TF_RHIImageSettingView& X) = default;
        TF_RHIImageSettingView(TF_RHIImageSettingView&& X) = default;
        TF_RHIImageSettingView& operator = (TF_RHIImageSettingView&& X) = default;
        
        template<typename __F_Allocator2>
        TF_RHIImageSettingView(const TF_RHIImageSetting<__F_Allocator2>& X);
        template<typename __F_Allocator2>
        TF_RHIImageSettingView& operator = (const TF_RHIImageSetting<__F_Allocator2>& X);
        
        TF_RHIImageSettingView<false> GetReadOnly() const noexcept;
        TF_RHIImageSettingView<true> GetReadWrite() const noexcept;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const TF_RHIImageSettingView& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << static_cast<const F_RHIImageSettingMinimal&>(Value));
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Format);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.SubImages);
            return F_FeedbackStatus::MakeSucceeded();   
        }
    };
    template<>
    struct TF_RHIImageSettingView<true> : F_RHIImageSettingMinimal
    {
        static constexpr B8 EnableWrite = true;
        
        E_RHIFormat Format = E_RHIFormat::NONE;
        
        TF_Span<
            std::conditional_t<
                EnableWrite,
                F_RHISubImageSetting,
                const F_RHISubImageSetting
            >    
        > SubImages;
        
        TF_RHIImageSettingView() = default;
        TF_RHIImageSettingView(const TF_RHIImageSettingView& X) = default;
        TF_RHIImageSettingView& operator = (const TF_RHIImageSettingView& X) = default;
        TF_RHIImageSettingView(TF_RHIImageSettingView&& X) = default;
        TF_RHIImageSettingView& operator = (TF_RHIImageSettingView&& X) = default;
        
        template<typename __F_Allocator2>
        TF_RHIImageSettingView(TF_RHIImageSetting<__F_Allocator2>& X);
        template<typename __F_Allocator2>
        TF_RHIImageSettingView& operator = (TF_RHIImageSetting<__F_Allocator2>& X);
        
        TF_RHIImageSettingView<false> GetReadOnly() const noexcept;
        TF_RHIImageSettingView<true> GetReadWrite() const noexcept;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const TF_RHIImageSettingView& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << static_cast<const F_RHIImageSettingMinimal&>(Value));
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Format);
            
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.SubImages);
            return F_FeedbackStatus::MakeSucceeded();   
        }
    };

    template <typename __F_Allocator>
    template <B8 __EnableWrite2>
    TF_RHIImageSetting<__F_Allocator>::TF_RHIImageSetting(const TF_RHIImageSettingView<__EnableWrite2>& X)
    {
        Format = X.Format;
        
        Width = X.Width;
        Height = X.Height;
        Depth = X.Depth;
        
        DimensionCount = X.DimensionCount;
        
        MipLevelCount = X.MipLevelCount;
        ArraySize = X.ArraySize;
        
        Flags = X.Flags;
        Layout = X.Layout;
        
        using F_SubImages = decltype(SubImages);
        SubImages = F_SubImages(X.SubImages.begin(), X.SubImages.end());
    }
    template <typename __F_Allocator>
    template <B8 __EnableWrite2>
    TF_RHIImageSetting<__F_Allocator>& TF_RHIImageSetting<__F_Allocator>::operator = (const TF_RHIImageSettingView<__EnableWrite2>& X)
    {
        Format = X.Format;
        
        Width = X.Width;
        Height = X.Height;
        Depth = X.Depth;
        
        DimensionCount = X.DimensionCount;
        
        MipLevelCount = X.MipLevelCount;
        ArraySize = X.ArraySize;
        
        Flags = X.Flags;
        Layout = X.Layout;
        
        using F_SubImages = decltype(SubImages);
        SubImages = F_SubImages(X.SubImages.begin(), X.SubImages.end());
        return *this;
    }
    
    template <typename __F_Allocator2>
    TF_RHIImageSettingView<false>::TF_RHIImageSettingView(const TF_RHIImageSetting<__F_Allocator2>& X)
    {
        Format = X.Format;
        
        Width = X.Width;
        Height = X.Height;
        Depth = X.Depth;
        
        DimensionCount = X.DimensionCount;
        
        MipLevelCount = X.MipLevelCount;
        ArraySize = X.ArraySize;
        
        Flags = X.Flags;
        Layout = X.Layout;
        
        SubImages = X.SubImages;
    }
    template <typename __F_Allocator2>
    TF_RHIImageSettingView<false>& TF_RHIImageSettingView<false>::operator = (const TF_RHIImageSetting<__F_Allocator2>& X)
    {
        Format = X.Format;
        
        Width = X.Width;
        Height = X.Height;
        Depth = X.Depth;
        
        DimensionCount = X.DimensionCount;
        
        MipLevelCount = X.MipLevelCount;
        ArraySize = X.ArraySize;
        
        Flags = X.Flags;
        Layout = X.Layout;
        
        SubImages = X.SubImages;
        return *this;
    }
    template <typename __F_Allocator2>
    TF_RHIImageSettingView<true>::TF_RHIImageSettingView(TF_RHIImageSetting<__F_Allocator2>& X)
    {
        Format = X.Format;
        
        Width = X.Width;
        Height = X.Height;
        Depth = X.Depth;
        
        DimensionCount = X.DimensionCount;
        
        MipLevelCount = X.MipLevelCount;
        ArraySize = X.ArraySize;
        
        Flags = X.Flags;
        Layout = X.Layout;
        
        SubImages = X.SubImages;
    }
    template <typename __F_Allocator2>
    TF_RHIImageSettingView<true>& TF_RHIImageSettingView<true>::operator = (TF_RHIImageSetting<__F_Allocator2>& X)
    {
        Format = X.Format;
        
        Width = X.Width;
        Height = X.Height;
        Depth = X.Depth;
        
        DimensionCount = X.DimensionCount;
        
        MipLevelCount = X.MipLevelCount;
        ArraySize = X.ArraySize;
        
        Flags = X.Flags;
        Layout = X.Layout;
        
        SubImages = X.SubImages;
        return *this;
    }
    
    inline TF_RHIImageSettingView<false> TF_RHIImageSettingView<false>::GetReadOnly() const noexcept
    {
        return *(const TF_RHIImageSettingView<false>*)this;
    }
    inline TF_RHIImageSettingView<true> TF_RHIImageSettingView<false>::GetReadWrite() const noexcept
    {
        return *(const TF_RHIImageSettingView<true>*)this;
    }
    inline TF_RHIImageSettingView<false> TF_RHIImageSettingView<true>::GetReadOnly() const noexcept
    {
        return *(const TF_RHIImageSettingView<false>*)this;
    }
    inline TF_RHIImageSettingView<true> TF_RHIImageSettingView<true>::GetReadWrite() const noexcept
    {
        return *(const TF_RHIImageSettingView<true>*)this;
    }
    
    struct F_RHISubImageMemoryStructure
    {
        U64 OffsetInBytes = 0;
        U64 SizeInBytes = 0;
        U64 RowPitchInBytes = 0;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_RHISubImageMemoryStructure& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.OffsetInBytes);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.SizeInBytes);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.RowPitchInBytes);
            return F_FeedbackStatus::MakeSucceeded();   
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_RHISubImageMemoryStructure& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.OffsetInBytes);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.SizeInBytes);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.RowPitchInBytes);
            return F_FeedbackStatus::MakeSucceeded();   
        }
    };
    template<typename __F_Allocator>
    struct TF_RHIImageMemoryStructure
    {
        using F_Allocator = __F_Allocator;
        
        ContainerTemplates::TF_SmallVector<F_RHISubImageMemoryStructure, 1, TF_RebindAllocator<F_RHISubImageMemoryStructure, F_Allocator>> SubImages;
        
        TF_RHIImageMemoryStructure() = default;
        TF_RHIImageMemoryStructure(const TF_RHIImageMemoryStructure& X) = default;
        TF_RHIImageMemoryStructure& operator = (const TF_RHIImageMemoryStructure& X) = default;
        TF_RHIImageMemoryStructure(TF_RHIImageMemoryStructure&& X) = default;
        TF_RHIImageMemoryStructure& operator = (TF_RHIImageMemoryStructure&& X) = default;
        
        template<B8 __EnableWrite2>
        TF_RHIImageMemoryStructure(const TF_RHIImageMemoryStructureView<__EnableWrite2>& X);
        template<B8 __EnableWrite2>
        TF_RHIImageMemoryStructure& operator = (const TF_RHIImageMemoryStructureView<__EnableWrite2>& X);
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const TF_RHIImageMemoryStructure& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.SubImages);
            return F_FeedbackStatus::MakeSucceeded();   
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, TF_RHIImageMemoryStructure& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.SubImages);
            return F_FeedbackStatus::MakeSucceeded();   
        }
    };
    template<>
    struct TF_RHIImageMemoryStructureView<false>
    {
        static constexpr B8 EnableWrite = false;
        
        TF_Span<
            std::conditional_t<
                EnableWrite,
                F_RHISubImageMemoryStructure,
                const F_RHISubImageMemoryStructure
            >    
        > SubImages;
        
        TF_RHIImageMemoryStructureView() = default;
        TF_RHIImageMemoryStructureView(const TF_RHIImageMemoryStructureView& X) = default;
        TF_RHIImageMemoryStructureView& operator = (const TF_RHIImageMemoryStructureView& X) = default;
        TF_RHIImageMemoryStructureView(TF_RHIImageMemoryStructureView&& X) = default;
        TF_RHIImageMemoryStructureView& operator = (TF_RHIImageMemoryStructureView&& X) = default;
        
        template<typename __F_Allocator2>
        TF_RHIImageMemoryStructureView(const TF_RHIImageMemoryStructure<__F_Allocator2>& X);
        template<typename __F_Allocator2>
        TF_RHIImageMemoryStructureView& operator = (const TF_RHIImageMemoryStructure<__F_Allocator2>& X);
        
        TF_RHIImageMemoryStructureView<false> GetReadOnly() const noexcept;
        TF_RHIImageMemoryStructureView<true> GetReadWrite() const noexcept;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const TF_RHIImageMemoryStructureView& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.SubImages);
            return F_FeedbackStatus::MakeSucceeded();   
        }
    };
    template<>
    struct TF_RHIImageMemoryStructureView<true>
    {
        static constexpr B8 EnableWrite = true;
        
        TF_Span<
            std::conditional_t<
                EnableWrite,
                F_RHISubImageMemoryStructure,
                const F_RHISubImageMemoryStructure
            >    
        > SubImages;
        
        TF_RHIImageMemoryStructureView() = default;
        TF_RHIImageMemoryStructureView(const TF_RHIImageMemoryStructureView& X) = default;
        TF_RHIImageMemoryStructureView& operator = (const TF_RHIImageMemoryStructureView& X) = default;
        TF_RHIImageMemoryStructureView(TF_RHIImageMemoryStructureView&& X) = default;
        TF_RHIImageMemoryStructureView& operator = (TF_RHIImageMemoryStructureView&& X) = default;
        
        template<typename __F_Allocator2>
        TF_RHIImageMemoryStructureView(TF_RHIImageMemoryStructure<__F_Allocator2>& X);
        template<typename __F_Allocator2>
        TF_RHIImageMemoryStructureView& operator = (TF_RHIImageMemoryStructure<__F_Allocator2>& X);
        
        TF_RHIImageMemoryStructureView<false> GetReadOnly() const noexcept;
        TF_RHIImageMemoryStructureView<true> GetReadWrite() const noexcept;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const TF_RHIImageMemoryStructureView& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.SubImages);
            return F_FeedbackStatus::MakeSucceeded();   
        }
    };

    template <typename __F_Allocator>
    template <B8 __EnableWrite2>
    TF_RHIImageMemoryStructure<__F_Allocator>::TF_RHIImageMemoryStructure(const TF_RHIImageMemoryStructureView<__EnableWrite2>& X) :
        SubImages(X.SubImages.begin(), X.SubImages.end())
    {
    }
    template <typename __F_Allocator>
    template <B8 __EnableWrite2>
    TF_RHIImageMemoryStructure<__F_Allocator>& TF_RHIImageMemoryStructure<__F_Allocator>::operator = (const TF_RHIImageMemoryStructureView<__EnableWrite2>& X)
    {
        using F_SubImages = decltype(SubImages);
        SubImages = F_SubImages(X.SubImages.begin(), X.SubImages.end());
        return *this;
    }
    
    template <typename __F_Allocator2>
    TF_RHIImageMemoryStructureView<false>::TF_RHIImageMemoryStructureView(const TF_RHIImageMemoryStructure<__F_Allocator2>& X) :
        SubImages(X.SubImages)
    {
    }
    template <typename __F_Allocator2>
    TF_RHIImageMemoryStructureView<false>& TF_RHIImageMemoryStructureView<false>::operator = (const TF_RHIImageMemoryStructure<__F_Allocator2>& X)
    {
        SubImages = X.SubImages;
        return *this;
    }
    template <typename __F_Allocator2>
    TF_RHIImageMemoryStructureView<true>::TF_RHIImageMemoryStructureView(TF_RHIImageMemoryStructure<__F_Allocator2>& X) :
        SubImages(X.SubImages)
    {
    }
    template <typename __F_Allocator2>
    TF_RHIImageMemoryStructureView<true>& TF_RHIImageMemoryStructureView<true>::operator = (TF_RHIImageMemoryStructure<__F_Allocator2>& X)
    {
        SubImages = X.SubImages;
        return *this;
    }
    
    inline TF_RHIImageMemoryStructureView<false> TF_RHIImageMemoryStructureView<false>::GetReadOnly() const noexcept
    {
        return *(const TF_RHIImageMemoryStructureView<false>*)this;
    }
    inline TF_RHIImageMemoryStructureView<true> TF_RHIImageMemoryStructureView<false>::GetReadWrite() const noexcept
    {
        return *(const TF_RHIImageMemoryStructureView<true>*)this;
    }
    inline TF_RHIImageMemoryStructureView<false> TF_RHIImageMemoryStructureView<true>::GetReadOnly() const noexcept
    {
        return *(const TF_RHIImageMemoryStructureView<false>*)this;
    }
    inline TF_RHIImageMemoryStructureView<true> TF_RHIImageMemoryStructureView<true>::GetReadWrite() const noexcept
    {
        return *(const TF_RHIImageMemoryStructureView<true>*)this;
    }
    
    struct F_RHISubImageTexelStructureRaw
    {
        U64 OffsetInTexels = 0;
        U64 SizeInTexels = 0;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_RHISubImageTexelStructureRaw& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.OffsetInTexels);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.SizeInTexels);
            return F_FeedbackStatus::MakeSucceeded();   
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_RHISubImageTexelStructureRaw& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.OffsetInTexels);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.SizeInTexels);
            return F_FeedbackStatus::MakeSucceeded();   
        }
    };
    template<typename __F_Allocator>
    struct TF_RHIImageTexelStructureRaw
    {
        using F_Allocator = __F_Allocator;
        
        TF_SmallVector<F_RHISubImageTexelStructureRaw, 1> SubImages;
        
        TF_RHIImageTexelStructureRaw() = default;
        TF_RHIImageTexelStructureRaw(const TF_RHIImageTexelStructureRaw& X) = default;
        TF_RHIImageTexelStructureRaw& operator = (const TF_RHIImageTexelStructureRaw& X) = default;
        TF_RHIImageTexelStructureRaw(TF_RHIImageTexelStructureRaw&& X) = default;
        TF_RHIImageTexelStructureRaw& operator = (TF_RHIImageTexelStructureRaw&& X) = default;
        
        template<B8 __EnableWrite2>
        TF_RHIImageTexelStructureRaw(const TF_RHIImageTexelStructureRawView<__EnableWrite2>& X);
        template<B8 __EnableWrite2>
        TF_RHIImageTexelStructureRaw& operator = (const TF_RHIImageTexelStructureRawView<__EnableWrite2>& X);
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const TF_RHIImageTexelStructureRaw& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.SubImages);
            return F_FeedbackStatus::MakeSucceeded();   
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, TF_RHIImageTexelStructureRaw& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.SubImages);
            return F_FeedbackStatus::MakeSucceeded();   
        }
    };
    template<>
    struct TF_RHIImageTexelStructureRawView<false>
    {
        static constexpr B8 EnableWrite = false;
        
        TF_Span<
            std::conditional_t<
                EnableWrite,
                F_RHISubImageTexelStructureRaw,
                const F_RHISubImageTexelStructureRaw
            >    
        > SubImages;
        
        TF_RHIImageTexelStructureRawView() = default;
        TF_RHIImageTexelStructureRawView(const TF_RHIImageTexelStructureRawView& X) = default;
        TF_RHIImageTexelStructureRawView& operator = (const TF_RHIImageTexelStructureRawView& X) = default;
        TF_RHIImageTexelStructureRawView(TF_RHIImageTexelStructureRawView&& X) = default;
        TF_RHIImageTexelStructureRawView& operator = (TF_RHIImageTexelStructureRawView&& X) = default;
        
        template<typename __F_Allocator2>
        TF_RHIImageTexelStructureRawView(const TF_RHIImageTexelStructureRaw<__F_Allocator2>& X);
        template<typename __F_Allocator2>
        TF_RHIImageTexelStructureRawView& operator = (const TF_RHIImageTexelStructureRaw<__F_Allocator2>& X);
        
        TF_RHIImageTexelStructureRawView<false> GetReadOnly() const noexcept;
        TF_RHIImageTexelStructureRawView<true> GetReadWrite() const noexcept;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const TF_RHIImageTexelStructureRawView& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.SubImages);
            return F_FeedbackStatus::MakeSucceeded();   
        }
    };
    template<>
    struct TF_RHIImageTexelStructureRawView<true>
    {
        static constexpr B8 EnableWrite = true;
        
        TF_Span<
            std::conditional_t<
                EnableWrite,
                F_RHISubImageTexelStructureRaw,
                const F_RHISubImageTexelStructureRaw
            >    
        > SubImages;
        
        TF_RHIImageTexelStructureRawView() = default;
        TF_RHIImageTexelStructureRawView(const TF_RHIImageTexelStructureRawView& X) = default;
        TF_RHIImageTexelStructureRawView& operator = (const TF_RHIImageTexelStructureRawView& X) = default;
        TF_RHIImageTexelStructureRawView(TF_RHIImageTexelStructureRawView&& X) = default;
        TF_RHIImageTexelStructureRawView& operator = (TF_RHIImageTexelStructureRawView&& X) = default;
        
        template<typename __F_Allocator2>
        TF_RHIImageTexelStructureRawView(TF_RHIImageTexelStructureRaw<__F_Allocator2>& X);
        template<typename __F_Allocator2>
        TF_RHIImageTexelStructureRawView& operator = (TF_RHIImageTexelStructureRaw<__F_Allocator2>& X);
        
        TF_RHIImageTexelStructureRawView<false> GetReadOnly() const noexcept;
        TF_RHIImageTexelStructureRawView<true> GetReadWrite() const noexcept;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const TF_RHIImageTexelStructureRawView& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.SubImages);
            return F_FeedbackStatus::MakeSucceeded();   
        }
    };

    template <typename __F_Allocator>
    template <B8 __EnableWrite2>
    TF_RHIImageTexelStructureRaw<__F_Allocator>::TF_RHIImageTexelStructureRaw(const TF_RHIImageTexelStructureRawView<__EnableWrite2>& X) :
        SubImages(X.SubImages.begin(), X.SubImages.end())
    {
    }
    template <typename __F_Allocator>
    template <B8 __EnableWrite2>
    TF_RHIImageTexelStructureRaw<__F_Allocator>& TF_RHIImageTexelStructureRaw<__F_Allocator>::operator = (const TF_RHIImageTexelStructureRawView<__EnableWrite2>& X)
    {
        using F_SubImages = decltype(SubImages);
        SubImages = F_SubImages(X.SubImages.begin(), X.SubImages.end());
        return *this;
    }
    
    template <typename __F_Allocator2>
    TF_RHIImageTexelStructureRawView<false>::TF_RHIImageTexelStructureRawView(const TF_RHIImageTexelStructureRaw<__F_Allocator2>& X) :
        SubImages(X.SubImages)
    {
    }
    template <typename __F_Allocator2>
    TF_RHIImageTexelStructureRawView<false>& TF_RHIImageTexelStructureRawView<false>::operator = (const TF_RHIImageTexelStructureRaw<__F_Allocator2>& X)
    {
        SubImages = X.SubImages;
        return *this;
    }
    template <typename __F_Allocator2>
    TF_RHIImageTexelStructureRawView<true>::TF_RHIImageTexelStructureRawView(TF_RHIImageTexelStructureRaw<__F_Allocator2>& X) :
        SubImages(X.SubImages)
    {
    }
    template <typename __F_Allocator2>
    TF_RHIImageTexelStructureRawView<true>& TF_RHIImageTexelStructureRawView<true>::operator = (TF_RHIImageTexelStructureRaw<__F_Allocator2>& X)
    {
        SubImages = X.SubImages;
        return *this;
    }
    
    inline TF_RHIImageTexelStructureRawView<false> TF_RHIImageTexelStructureRawView<false>::GetReadOnly() const noexcept
    {
        return *(const TF_RHIImageTexelStructureRawView<false>*)this;
    }
    inline TF_RHIImageTexelStructureRawView<true> TF_RHIImageTexelStructureRawView<false>::GetReadWrite() const noexcept
    {
        return *(const TF_RHIImageTexelStructureRawView<true>*)this;
    }
    inline TF_RHIImageTexelStructureRawView<false> TF_RHIImageTexelStructureRawView<true>::GetReadOnly() const noexcept
    {
        return *(const TF_RHIImageTexelStructureRawView<false>*)this;
    }
    inline TF_RHIImageTexelStructureRawView<true> TF_RHIImageTexelStructureRawView<true>::GetReadWrite() const noexcept
    {
        return *(const TF_RHIImageTexelStructureRawView<true>*)this;
    }
    
    template<E_RHIFormat __Format, typename __F_Allocator>
    struct TF_RHIImagePayloadRaw
    {
        static constexpr E_RHIFormat Format = __Format;
        using F_Allocator = __F_Allocator;
        
        using F_TexelData = TF_RHIFormatData<Format>;
        
        B8 Enable = true;
        Sz SizeInTexels = 0;
        ContainerTemplates::TF_Vector<F_TexelData, TF_RebindAllocator<F_TexelData, F_Allocator>> Texels;
        
        TF_RHIImagePayloadRaw() = default;
        TF_RHIImagePayloadRaw(const TF_RHIImagePayloadRaw& X) = default;
        TF_RHIImagePayloadRaw& operator = (const TF_RHIImagePayloadRaw& X) = default;
        TF_RHIImagePayloadRaw(TF_RHIImagePayloadRaw&& X) = default;
        TF_RHIImagePayloadRaw& operator = (TF_RHIImagePayloadRaw&& X) = default;
        
        template<B8 __EnableWrite2>
        TF_RHIImagePayloadRaw(const TF_RHIImagePayloadRawView<__EnableWrite2, Format>& X);
        template<B8 __EnableWrite2>
        TF_RHIImagePayloadRaw& operator = (const TF_RHIImagePayloadRawView<__EnableWrite2, Format>& X);
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const TF_RHIImagePayloadRaw& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Texels);
            return F_FeedbackStatus::MakeSucceeded();   
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, TF_RHIImagePayloadRaw& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Texels);
            return F_FeedbackStatus::MakeSucceeded();   
        }
    };
    
    template<E_RHIFormat __Format>
    struct TF_RHIImagePayloadRawView<false, __Format>
    {
        static constexpr B8 EnableWrite = false;
        static constexpr E_RHIFormat Format = __Format;
        
        using F_TexelData = TF_RHIFormatData<Format>;
        
        B8 Enable = true;
        Sz SizeInTexels = 0;
        TF_Span<
            std::conditional_t<
                EnableWrite,
                F_TexelData,
                const F_TexelData
            >    
        > Texels;
        
        TF_RHIImagePayloadRawView() = default;
        TF_RHIImagePayloadRawView(const TF_RHIImagePayloadRawView& X) = default;
        TF_RHIImagePayloadRawView& operator = (const TF_RHIImagePayloadRawView& X) = default;
        TF_RHIImagePayloadRawView(TF_RHIImagePayloadRawView&& X) = default;
        TF_RHIImagePayloadRawView& operator = (TF_RHIImagePayloadRawView&& X) = default;
        
        template<typename __F_Allocator2>
        TF_RHIImagePayloadRawView(const TF_RHIImagePayloadRaw<Format, __F_Allocator2>& X);
        template<typename __F_Allocator2>
        TF_RHIImagePayloadRawView& operator = (const TF_RHIImagePayloadRaw<Format, __F_Allocator2>& X);
        
        TF_RHIImagePayloadRawView<false, __Format> GetReadOnly() const noexcept;
        TF_RHIImagePayloadRawView<true, __Format> GetReadWrite() const noexcept;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const TF_RHIImagePayloadRawView& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Texels);
            return F_FeedbackStatus::MakeSucceeded();   
        }
    };
    template<E_RHIFormat __Format>
    struct TF_RHIImagePayloadRawView<true, __Format>
    {
        static constexpr B8 EnableWrite = true;
        static constexpr E_RHIFormat Format = __Format;
        
        using F_TexelData = TF_RHIFormatData<Format>;
        
        B8 Enable = true;
        Sz SizeInTexels = 0;
        TF_Span<
            std::conditional_t<
                EnableWrite,
                F_TexelData,
                const F_TexelData
            >    
        > Texels;
        
        TF_RHIImagePayloadRawView() = default;
        TF_RHIImagePayloadRawView(const TF_RHIImagePayloadRawView& X) = default;
        TF_RHIImagePayloadRawView& operator = (const TF_RHIImagePayloadRawView& X) = default;
        TF_RHIImagePayloadRawView(TF_RHIImagePayloadRawView&& X) = default;
        TF_RHIImagePayloadRawView& operator = (TF_RHIImagePayloadRawView&& X) = default;
        
        template<typename __F_Allocator2>
        TF_RHIImagePayloadRawView(TF_RHIImagePayloadRaw<Format, __F_Allocator2>& X);
        template<typename __F_Allocator2>
        TF_RHIImagePayloadRawView& operator = (TF_RHIImagePayloadRaw<Format, __F_Allocator2>& X);
        
        TF_RHIImagePayloadRawView<false, __Format> GetReadOnly() const noexcept;
        TF_RHIImagePayloadRawView<true, __Format> GetReadWrite() const noexcept;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const TF_RHIImagePayloadRawView& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Texels);
            return F_FeedbackStatus::MakeSucceeded();   
        }
    };

    template <E_RHIFormat __Format, typename __F_Allocator>
    template <B8 __EnableWrite2>
    TF_RHIImagePayloadRaw<__Format, __F_Allocator>::TF_RHIImagePayloadRaw(const TF_RHIImagePayloadRawView<__EnableWrite2, Format>& X) :
        Enable(X.Enable),
        SizeInTexels(X.SizeInTexels),
        Texels(X.Texels.begin(), X.Texels.end())
    {
    }
    template <E_RHIFormat __Format, typename __F_Allocator>
    template <B8 __EnableWrite2>
    TF_RHIImagePayloadRaw<__Format, __F_Allocator>& TF_RHIImagePayloadRaw<__Format, __F_Allocator>::operator = (const TF_RHIImagePayloadRawView<__EnableWrite2, Format>& X)
    {
        Enable = X.Enable;
        SizeInTexels = X.SizeInTexels;
        using F_Texels = decltype(Texels);
        Texels = F_Texels(X.Texels.begin(), X.Texels.end());
        return *this;
    }

    template <E_RHIFormat __Format>
    template <typename __F_Allocator2>
    TF_RHIImagePayloadRawView<false, __Format>::TF_RHIImagePayloadRawView(const TF_RHIImagePayloadRaw<Format, __F_Allocator2>& X) :
        Enable(X.Enable),
        SizeInTexels(X.SizeInTexels),
        Texels(X.Texels)
    {
    }
    template <E_RHIFormat __Format>
    template <typename __F_Allocator2>
    TF_RHIImagePayloadRawView<false, __Format>& TF_RHIImagePayloadRawView<false, __Format>::operator = (const TF_RHIImagePayloadRaw<Format, __F_Allocator2>& X)
    {
        Enable = X.Enable;
        SizeInTexels = X.SizeInTexels;
        Texels = X.Texels;
        return *this;
    }
    template <E_RHIFormat __Format>
    template <typename __F_Allocator2>
    TF_RHIImagePayloadRawView<true, __Format>::TF_RHIImagePayloadRawView(TF_RHIImagePayloadRaw<Format, __F_Allocator2>& X) :
        Enable(X.Enable),
        SizeInTexels(X.SizeInTexels),
        Texels(X.Texels)
    {
    }
    template <E_RHIFormat __Format>
    template <typename __F_Allocator2>
    TF_RHIImagePayloadRawView<true, __Format>& TF_RHIImagePayloadRawView<true, __Format>::operator = (TF_RHIImagePayloadRaw<Format, __F_Allocator2>& X)
    {
        Enable = X.Enable;
        SizeInTexels = X.SizeInTexels;
        Texels = X.Texels;
        return *this;
    }
    
    template <E_RHIFormat __Format>
    inline TF_RHIImagePayloadRawView<false, __Format> TF_RHIImagePayloadRawView<false, __Format>::GetReadOnly() const noexcept
    {
        return *(const TF_RHIImagePayloadRawView<false, __Format>*)this;
    }
    template <E_RHIFormat __Format>
    inline TF_RHIImagePayloadRawView<true, __Format> TF_RHIImagePayloadRawView<false, __Format>::GetReadWrite() const noexcept
    {
        return *(const TF_RHIImagePayloadRawView<true, __Format>*)this;
    }
    template <E_RHIFormat __Format>
    inline TF_RHIImagePayloadRawView<false, __Format> TF_RHIImagePayloadRawView<true, __Format>::GetReadOnly() const noexcept
    {
        return *(const TF_RHIImagePayloadRawView<false, __Format>*)this;
    }
    template <E_RHIFormat __Format>
    inline TF_RHIImagePayloadRawView<true, __Format> TF_RHIImagePayloadRawView<true, __Format>::GetReadWrite() const noexcept
    {
        return *(const TF_RHIImagePayloadRawView<true, __Format>*)this;
    }
    
    template<typename __F_Allocator>
    struct TF_RHIImagePayload
    {
        using F_Allocator = __F_Allocator;
        
        B8 Enable = true;
        Sz SizeInBytes = 0;
        ContainerTemplates::TF_Vector<U8, TF_RebindAllocator<U8, F_Allocator>> Bytes;
        
        TF_RHIImagePayload() = default;
        TF_RHIImagePayload(const TF_RHIImagePayload& X) = default;
        TF_RHIImagePayload& operator = (const TF_RHIImagePayload& X) = default;
        TF_RHIImagePayload(TF_RHIImagePayload&& X) = default;
        TF_RHIImagePayload& operator = (TF_RHIImagePayload&& X) = default;
        
        template<B8 __EnableWrite2>
        TF_RHIImagePayload(const TF_RHIImagePayloadView<__EnableWrite2>& X);
        template<B8 __EnableWrite2>
        TF_RHIImagePayload& operator = (const TF_RHIImagePayloadView<__EnableWrite2>& X);
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const TF_RHIImagePayload& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Bytes);
            return F_FeedbackStatus::MakeSucceeded();   
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, TF_RHIImagePayload& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Bytes);
            return F_FeedbackStatus::MakeSucceeded();   
        }
    };
    
    template<>
    struct TF_RHIImagePayloadView<false>
    {
        static constexpr B8 EnableWrite = false;
        
        B8 Enable = true;
        Sz SizeInBytes = 0;
        TF_Span<
            std::conditional_t<
                EnableWrite,
                U8,
                const U8
            >    
        > Bytes;
        
        TF_RHIImagePayloadView() = default;
        TF_RHIImagePayloadView(const TF_RHIImagePayloadView& X) = default;
        TF_RHIImagePayloadView& operator = (const TF_RHIImagePayloadView& X) = default;
        TF_RHIImagePayloadView(TF_RHIImagePayloadView&& X) = default;
        TF_RHIImagePayloadView& operator = (TF_RHIImagePayloadView&& X) = default;
        
        template<typename __F_Allocator2>
        TF_RHIImagePayloadView(const TF_RHIImagePayload<__F_Allocator2>& X);
        template<typename __F_Allocator2>
        TF_RHIImagePayloadView& operator = (const TF_RHIImagePayload<__F_Allocator2>& X);
        
        TF_RHIImagePayloadView<false> GetReadOnly() const noexcept;
        TF_RHIImagePayloadView<true> GetReadWrite() const noexcept;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const TF_RHIImagePayloadView& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Bytes);
            return F_FeedbackStatus::MakeSucceeded();   
        }
    };
    template<>
    struct TF_RHIImagePayloadView<true>
    {
        static constexpr B8 EnableWrite = true;
        
        B8 Enable = true;
        Sz SizeInBytes = 0;
        TF_Span<
            std::conditional_t<
                EnableWrite,
                U8,
                const U8
            >    
        > Bytes;
        
        TF_RHIImagePayloadView() = default;
        TF_RHIImagePayloadView(const TF_RHIImagePayloadView& X) = default;
        TF_RHIImagePayloadView& operator = (const TF_RHIImagePayloadView& X) = default;
        TF_RHIImagePayloadView(TF_RHIImagePayloadView&& X) = default;
        TF_RHIImagePayloadView& operator = (TF_RHIImagePayloadView&& X) = default;
        
        template<typename __F_Allocator2>
        TF_RHIImagePayloadView(TF_RHIImagePayload<__F_Allocator2>& X);
        template<typename __F_Allocator2>
        TF_RHIImagePayloadView& operator = (TF_RHIImagePayload<__F_Allocator2>& X);
        
        TF_RHIImagePayloadView<false> GetReadOnly() const noexcept;
        TF_RHIImagePayloadView<true> GetReadWrite() const noexcept;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const TF_RHIImagePayloadView& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Bytes);
            return F_FeedbackStatus::MakeSucceeded();   
        }
    };

    template <typename __F_Allocator>
    template <B8 __EnableWrite2>
    TF_RHIImagePayload<__F_Allocator>::TF_RHIImagePayload(const TF_RHIImagePayloadView<__EnableWrite2>& X) :
        Enable(X.Enable),
        SizeInBytes(X.SizeInBytes),
        Bytes(X.Bytes.begin(), X.Bytes.end())
    {
    }
    template <typename __F_Allocator>
    template <B8 __EnableWrite2>
    TF_RHIImagePayload<__F_Allocator>& TF_RHIImagePayload<__F_Allocator>::operator = (const TF_RHIImagePayloadView<__EnableWrite2>& X)
    {
        Enable = X.Enable;
        SizeInBytes = X.SizeInBytes;
        using F_Bytes = decltype(Bytes);
        Bytes = F_Bytes(X.Bytes.begin(), X.Bytes.end());
        return *this;
    }
    
    template <typename __F_Allocator2>
    TF_RHIImagePayloadView<false>::TF_RHIImagePayloadView(const TF_RHIImagePayload<__F_Allocator2>& X) :
        Enable(X.Enable),
        SizeInBytes(X.SizeInBytes),
        Bytes(X.Bytes)
    {
    }
    template <typename __F_Allocator2>
    TF_RHIImagePayloadView<false>& TF_RHIImagePayloadView<false>::operator = (const TF_RHIImagePayload<__F_Allocator2>& X)
    {
        Enable = X.Enable;
        SizeInBytes = X.SizeInBytes;
        Bytes = X.Bytes;
        return *this;
    }
    template <typename __F_Allocator2>
    TF_RHIImagePayloadView<true>::TF_RHIImagePayloadView(TF_RHIImagePayload<__F_Allocator2>& X) :
        Enable(X.Enable),
        SizeInBytes(X.SizeInBytes),
        Bytes(X.Bytes)
    {
    }
    template <typename __F_Allocator2>
    TF_RHIImagePayloadView<true>& TF_RHIImagePayloadView<true>::operator = (TF_RHIImagePayload<__F_Allocator2>& X)
    {
        Enable = X.Enable;
        SizeInBytes = X.SizeInBytes;
        Bytes = X.Bytes;
        return *this;
    }
    
    inline TF_RHIImagePayloadView<false> TF_RHIImagePayloadView<false>::GetReadOnly() const noexcept
    {
        return *(const TF_RHIImagePayloadView<false>*)this;
    }
    inline TF_RHIImagePayloadView<true> TF_RHIImagePayloadView<false>::GetReadWrite() const noexcept
    {
        return *(const TF_RHIImagePayloadView<true>*)this;
    }
    inline TF_RHIImagePayloadView<false> TF_RHIImagePayloadView<true>::GetReadOnly() const noexcept
    {
        return *(const TF_RHIImagePayloadView<false>*)this;
    }
    inline TF_RHIImagePayloadView<true> TF_RHIImagePayloadView<true>::GetReadWrite() const noexcept
    {
        return *(const TF_RHIImagePayloadView<true>*)this;
    }
    
    template<E_RHIFormat __Format, typename __F_Allocator>
    struct TF_RHIImageRaw
    {
        static constexpr E_RHIFormat Format = __Format;
        using F_Allocator = __F_Allocator;
        
        TF_RHIImageSettingRaw<F_Allocator> Setting;
        TF_RHIImageTexelStructureRaw<F_Allocator> TexelStructure;
        TF_RHIImagePayloadRaw<Format, F_Allocator> Payload;
        
        TF_RHIImageRaw() = default;
        TF_RHIImageRaw(const TF_RHIImageRaw& X) = default;
        TF_RHIImageRaw& operator = (const TF_RHIImageRaw& X) = default;
        TF_RHIImageRaw(TF_RHIImageRaw&& X) = default;
        TF_RHIImageRaw& operator = (TF_RHIImageRaw&& X) = default;
        TF_RHIImageRaw(
            B8 EnablePayload,
            U32 Width = 1,
            U32 Height = 1,
            U32 Depth = 1,
            U32 DimensionCount = 2,
            U32 MipLevelCount = 1,
            U32 ArraySize = 1,
            E_RHIImageFlag Flags = E_RHIImageFlag::DEFAULT
        )
        {
            U32 NumTexels = 0;
            
            Setting.Format = Format;
            Setting.Width = Width;
            Setting.Height = Height;
            Setting.Depth = Depth;
            Setting.DimensionCount = DimensionCount;
            Setting.MipLevelCount = MipLevelCount;
            Setting.ArraySize = ArraySize;
            Setting.Flags = Flags;
            
            Payload.Enable = EnablePayload;
            
            for (U32 ArraySlice = 0; ArraySlice < ArraySize; ++ArraySlice)
            {
                U32 SubWidth = Setting.Width;
                U32 SubHeight = Setting.Height;
                U32 SubDepth = Setting.Depth;
                for (U32 MipLevel = 0; MipLevel < MipLevelCount; ++MipLevel)
                {
                    F_RHISubImageSettingRaw SubImageSetting;
                    SubImageSetting.Width = SubWidth;
                    SubImageSetting.Height = SubHeight;
                    SubImageSetting.Depth = SubDepth;
                    SubImageSetting.MipLevel = MipLevel;
                    SubImageSetting.ArraySlice = ArraySlice;
                    
                    F_RHISubImageTexelStructureRaw SubImageTexelStructure;
                    SubImageTexelStructure.OffsetInTexels = NumTexels;
                    SubImageTexelStructure.SizeInTexels = SubWidth * SubHeight * SubDepth;
                    
                    Setting.SubImages.push_back(SubImageSetting);
                    TexelStructure.SubImages.push_back(SubImageTexelStructure);
                    
                    NumTexels += SubImageTexelStructure.SizeInTexels;
                    
                    SubWidth = Max<U32>(1, SubWidth >> 1);
                    SubHeight = Max<U32>(1, SubHeight >> 1);
                    SubDepth = Max<U32>(1, SubDepth >> 1);
                }
            }
            
            if (Payload.Enable)
            {
                Payload.Texels.resize(NumTexels);
            }
        }
        
        TF_RHIImageRaw(const TF_RHIImageView<false>& ImageView);
        
        TF_RHIImageRaw(const TF_RHIImage<F_Allocator>& Image) :
            TF_RHIImageRaw(
                TF_RHIImageView<false>(Image)    
            )
        {
        }
        
        template<B8 __EnableWrite2>
        TF_RHIImageRaw(const TF_RHIImageRawView<__EnableWrite2, Format>& X);
        template<B8 __EnableWrite2>
        TF_RHIImageRaw& operator = (const TF_RHIImageRawView<__EnableWrite2, Format>& X);
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const TF_RHIImageRaw& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Setting);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.TexelStructure);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Payload);
            return F_FeedbackStatus::MakeSucceeded();   
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, TF_RHIImageRaw& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Setting);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.TexelStructure);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Payload);
            return F_FeedbackStatus::MakeSucceeded();   
        }
        
    private:
        void _InternalAddSubImage(
            U32 Index,
            U32 PlaneIndex,
            const F_RHISubImageSetting& SubImageSetting, 
            const F_RHISubImageMemoryStructure& SubImageMemoryStructure, 
            const TF_Span<const U8>& SubImagePayload
        );
    };
    
    template<E_RHIFormat __Format>
    struct TF_RHIImageRawView<false, __Format>
    {
        static constexpr B8 EnableWrite = false;
        static constexpr E_RHIFormat Format = __Format;
        
        TF_RHIImageSettingRawView<EnableWrite> Setting;
        TF_RHIImageTexelStructureRawView<EnableWrite> TexelStructure;
        TF_RHIImagePayloadRawView<EnableWrite, Format> Payload;
        
        TF_RHIImageRawView() = default;
        TF_RHIImageRawView(const TF_RHIImageRawView& X) = default;
        TF_RHIImageRawView& operator = (const TF_RHIImageRawView& X) = default;
        TF_RHIImageRawView(TF_RHIImageRawView&& X) = default;
        TF_RHIImageRawView& operator = (TF_RHIImageRawView&& X) = default;
        
        template<typename __F_Allocator2>
        TF_RHIImageRawView(const TF_RHIImageRaw<Format, __F_Allocator2>& X);
        template<typename __F_Allocator2>
        TF_RHIImageRawView& operator = (const TF_RHIImageRaw<Format, __F_Allocator2>& X);
        
        TF_RHIImageRawView<false, __Format> GetReadOnly() const noexcept;
        TF_RHIImageRawView<true, __Format> GetReadWrite() const noexcept;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const TF_RHIImageRawView& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Setting);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.TexelStructure);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Payload);
            return F_FeedbackStatus::MakeSucceeded();   
        }
    };
    template<E_RHIFormat __Format>
    struct TF_RHIImageRawView<true, __Format>
    {
        static constexpr B8 EnableWrite = true;
        static constexpr E_RHIFormat Format = __Format;
        
        TF_RHIImageSettingRawView<EnableWrite> Setting;
        TF_RHIImageTexelStructureRawView<EnableWrite> TexelStructure;
        TF_RHIImagePayloadRawView<EnableWrite, Format> Payload;
        
        TF_RHIImageRawView() = default;
        TF_RHIImageRawView(const TF_RHIImageRawView& X) = default;
        TF_RHIImageRawView& operator = (const TF_RHIImageRawView& X) = default;
        TF_RHIImageRawView(TF_RHIImageRawView&& X) = default;
        TF_RHIImageRawView& operator = (TF_RHIImageRawView&& X) = default;
        
        template<typename __F_Allocator2>
        TF_RHIImageRawView(TF_RHIImageRaw<Format, __F_Allocator2>& X);
        template<typename __F_Allocator2>
        TF_RHIImageRawView& operator = (TF_RHIImageRaw<Format, __F_Allocator2>& X);
        
        TF_RHIImageRawView<false, __Format> GetReadOnly() const noexcept;
        TF_RHIImageRawView<true, __Format> GetReadWrite() const noexcept;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const TF_RHIImageRawView& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Setting);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.TexelStructure);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Payload);
            return F_FeedbackStatus::MakeSucceeded();   
        }
    };

    template <E_RHIFormat __Format, typename __F_Allocator>
    template <B8 __EnableWrite2>
    TF_RHIImageRaw<__Format, __F_Allocator>::TF_RHIImageRaw(const TF_RHIImageRawView<__EnableWrite2, Format>& X) :
        Setting(X.Setting),
        TexelStructure(X.TexelStructure),
        Payload(X.Payload)
    {
    }
    template <E_RHIFormat __Format, typename __F_Allocator>
    template <B8 __EnableWrite2>
    TF_RHIImageRaw<__Format, __F_Allocator>& TF_RHIImageRaw<__Format, __F_Allocator>::operator = (const TF_RHIImageRawView<__EnableWrite2, Format>& X)
    {
        Setting = X.Setting;
        TexelStructure = X.TexelStructure;
        Payload = X.Payload;
        return *this;
    }

    template <E_RHIFormat __Format>
    template <typename __F_Allocator2>
    TF_RHIImageRawView<false, __Format>::TF_RHIImageRawView(const TF_RHIImageRaw<Format, __F_Allocator2>& X) :
        Setting(X.Setting),
        TexelStructure(X.TexelStructure),
        Payload(X.Payload)
    {
    }
    template <E_RHIFormat __Format>
    template <typename __F_Allocator2>
    TF_RHIImageRawView<false, __Format>& TF_RHIImageRawView<false, __Format>::operator = (const TF_RHIImageRaw<Format, __F_Allocator2>& X)
    {
        Setting = X.Setting;
        TexelStructure = X.TexelStructure;
        Payload = X.Payload;
        return *this;
    }
    template <E_RHIFormat __Format>
    template <typename __F_Allocator2>
    TF_RHIImageRawView<true, __Format>::TF_RHIImageRawView(TF_RHIImageRaw<Format, __F_Allocator2>& X) :
        Setting(X.Setting),
        TexelStructure(X.TexelStructure),
        Payload(X.Payload)
    {
    }
    template <E_RHIFormat __Format>
    template <typename __F_Allocator2>
    TF_RHIImageRawView<true, __Format>& TF_RHIImageRawView<true, __Format>::operator = (TF_RHIImageRaw<Format, __F_Allocator2>& X)
    {
        Setting = X.Setting;
        TexelStructure = X.TexelStructure;
        Payload = X.Payload;
        return *this;
    }
    
    template <E_RHIFormat __Format>
    inline TF_RHIImageRawView<false, __Format> TF_RHIImageRawView<false, __Format>::GetReadOnly() const noexcept
    {
        return *(const TF_RHIImageRawView<false, __Format>*)this;
    }
    template <E_RHIFormat __Format>
    inline TF_RHIImageRawView<true, __Format> TF_RHIImageRawView<false, __Format>::GetReadWrite() const noexcept
    {
        return *(const TF_RHIImageRawView<true, __Format>*)this;
    }
    template <E_RHIFormat __Format>
    inline TF_RHIImageRawView<false, __Format> TF_RHIImageRawView<true, __Format>::GetReadOnly() const noexcept
    {
        return *(const TF_RHIImageRawView<false, __Format>*)this;
    }
    template <E_RHIFormat __Format>
    inline TF_RHIImageRawView<true, __Format> TF_RHIImageRawView<true, __Format>::GetReadWrite() const noexcept
    {
        return *(const TF_RHIImageRawView<true, __Format>*)this;
    }
    
    template<B8 __EnableWrite, E_RHIFormat __Format>
    struct TF_RHIImageRawAccessor
    {
        static constexpr E_RHIFormat Format = __Format;
        static constexpr B8 EnableWrite = __EnableWrite;
        
        using F_TexelData = TF_RHIFormatData<Format>;
        
        using F_AccessibleTexel = std::conditional_t<
            EnableWrite,
            F_TexelData,
            const F_TexelData
        >;
        
        U32 SubImageIndex = ~U32(0);
        TF_RHIImageRawView<EnableWrite, Format> View;
        F_RHISubImageSettingRaw Setting;
        TF_Span<F_AccessibleTexel> Texels;
        
        TF_RHIImageRawAccessor() = default;
        TF_RHIImageRawAccessor(const TF_RHIImageRawAccessor& X) = default;
        TF_RHIImageRawAccessor& operator = (const TF_RHIImageRawAccessor& X) = default;
        TF_RHIImageRawAccessor(TF_RHIImageRawAccessor&& X) = default;
        TF_RHIImageRawAccessor& operator = (TF_RHIImageRawAccessor&& X) = default;
        
        TF_RHIImageRawAccessor(const TF_RHIImageRawView<EnableWrite, Format>& InView, U32 MipLevel = 0, U32 ArraySlice = 0)
        {
            ABYTEK_ENGINE_RHI_ASSERT(InView.Payload.Enable);
            View = InView;
            {
                B8 FoundSubImage = false;
                Sz NumSubImages = View.Setting.SubImages.size();
                for (Sz Idx = 0; Idx < NumSubImages; ++Idx)
                {
                    const F_RHISubImageSettingRaw& SubImageSetting = View.Setting.SubImages[Idx];
                    const F_RHISubImageTexelStructureRaw& SubImageTexelStructure = View.TexelStructure.SubImages[Idx];
                    if (
                        (SubImageSetting.MipLevel == MipLevel)
                        && (SubImageSetting.ArraySlice == ArraySlice)
                    )
                    {
                        SubImageIndex = Idx;
                        Setting = SubImageSetting;
                        Texels = {
                            View.Payload.Texels.begin() + SubImageTexelStructure.OffsetInTexels,
                            View.Payload.Texels.end() + SubImageTexelStructure.OffsetInTexels + SubImageTexelStructure.SizeInTexels,
                        };
                        FoundSubImage = true;
                        break;
                    }
                }
                ABYTEK_ENGINE_RHI_ASSERT(FoundSubImage) << "Not found subimage (mip level: " << MipLevel << ", array slice: " << ArraySlice << ")";
            }
        }
        
        B8 IsValid() const noexcept
        {
            return (SubImageIndex != ~U32(0));
        }
        explicit operator B8 () const noexcept
        {
            return IsValid();
        }
        
        U32 CalculateFlatIndex(const F_Vector3_U32& Coord) const noexcept
        {
            ABYTEK_ENGINE_RHI_ASSERT(IsValid());
            return (
                Coord.X 
                + Coord.Y * Setting.Width
                + Coord.Z * Setting.Width * Setting.Height
            );
        }
        U32 CalculateFlatIndex(const F_Vector2_U32& Coord) const noexcept
        {
            return CalculateFlatIndex(
                F_Vector3_U32(Coord, 0)  
            );
        }
        U32 CalculateFlatIndex(U32 Coord) const noexcept
        {
            return CalculateFlatIndex(
                F_Vector2_U32(Coord, 0)  
            );
        }
        F_TexelData& At(const F_Vector3_U32& Coord) const noexcept
        {
            ABYTEK_ENGINE_RHI_ASSERT(IsValid());
            ABYTEK_ENGINE_RHI_ASSERT(
                (Coord.X < Setting.Width)    
                && (Coord.Y < Setting.Height)    
                && (Coord.Z < Setting.Depth)    
            );
            return *(F_TexelData*)&Texels[CalculateFlatIndex(Coord)];
        }
        F_TexelData& At(const F_Vector2_U32& Coord) const noexcept
        {
            return At(
                F_Vector3_U32(Coord, 0)  
            );
        }
        F_TexelData& At(U32 Coord) const noexcept
        {
            return At(
                F_Vector2_U32(Coord, 0)  
            );
        }
        F_TexelData& operator [] (const F_Vector3_U32& Coord) const noexcept
        {
            return At(Coord);
        }
        F_TexelData& operator [] (const F_Vector2_U32& Coord) const noexcept
        {
            return At(Coord);
        }
        F_TexelData& operator [] (U32 Coord) const noexcept
        {
            return At(Coord);
        }
    };
    
    template<typename __F_Allocator>
    struct TF_RHIImage
    {
        using F_Allocator = __F_Allocator;
        
        TF_RHIImageSetting<F_Allocator> Setting;
        TF_RHIImageMemoryStructure<F_Allocator> MemoryStructure;
        TF_RHIImagePayload<F_Allocator> Payload;
        
        TF_RHIImage() = default;
        TF_RHIImage(const TF_RHIImage& X) = default;
        TF_RHIImage& operator = (const TF_RHIImage& X) = default;
        TF_RHIImage(TF_RHIImage&& X) = default;
        TF_RHIImage& operator = (TF_RHIImage&& X) = default;
        
        template<B8 __EnableWrite2>
        TF_RHIImage(const TF_RHIImageView<__EnableWrite2>& X);
        template<B8 __EnableWrite2>
        TF_RHIImage& operator = (const TF_RHIImageView<__EnableWrite2>& X);
        
        template<E_RHIFormat __Format>
        TF_RHIImage(const TF_RHIImageRawView<false, __Format>& RawView);
        
        template<E_RHIFormat __Format>
        TF_RHIImage(const TF_RHIImageRaw<__Format, F_Allocator>& Raw) :
            TF_RHIImage(
                TF_RHIImageRawView<false, __Format>(Raw)    
            )
        {
        }
        
        TF_RHIImage(
            B8 EnablePayload,
            E_RHIFormat Format,
            const F_RHIImageSettingMinimal& SettingMinimal, 
            const TF_Span<const U64>& SubImages_RowPitchInBytes = {}
        )
        {
            U64 SizeInBytes = 0;

            Setting.Format = Format;
            static_cast<F_RHIImageSettingMinimal&>(Setting) = SettingMinimal; 
            
            Payload.Enable = EnablePayload;
            
            U32 PlaneCount = RHIFormatPlaneCount(Format);
            for (U32 PlaneIndex = 0; PlaneIndex < PlaneCount; ++PlaneIndex)
            {
                for (U32 ArraySlice = 0; ArraySlice < Setting.ArraySize; ++ArraySlice)
                {
                    U32 SubWidth = Setting.Width;
                    U32 SubHeight = Setting.Height;
                    U32 SubDepth = Setting.Depth;
                    for (U32 MipLevel = 0; MipLevel < Setting.MipLevelCount; ++MipLevel)
                    {
                        U32 SubImageIndex = Setting.SubImages.size();
                        
                        U32 PlaneDataSizeInBytes = RHIFormatPlaneDataSize(Format, PlaneIndex);
                            
                        F_RHISubImageSetting SubImageSetting;
                        SubImageSetting.Width = SubWidth;
                        SubImageSetting.Height = SubHeight;
                        SubImageSetting.Depth = SubDepth;
                        SubImageSetting.MipLevel = MipLevel;
                        SubImageSetting.ArraySlice = ArraySlice;
                        SubImageSetting.PlaneIndex = PlaneIndex;
                        
                        F_RHISubImageMemoryStructure SubImageMemoryStructure;
                        SubImageMemoryStructure.OffsetInBytes = SizeInBytes;
                        if (SubImages_RowPitchInBytes.size() > SubImageIndex)
                        {
                            SubImageMemoryStructure.RowPitchInBytes = SubImages_RowPitchInBytes[SubImageIndex];
                        }
                        else
                        {
                            SubImageMemoryStructure.RowPitchInBytes = AlignSize(SubWidth * PlaneDataSizeInBytes, RHI_IMAGE_OPTIMAL_ROW_PITCH_ALIGNMENT);
                        }
                        SubImageMemoryStructure.SizeInBytes = SubImageMemoryStructure.RowPitchInBytes * SubHeight * SubDepth;
                        
                        Setting.SubImages.push_back(SubImageSetting);
                        MemoryStructure.SubImages.push_back(SubImageMemoryStructure);
                        
                        SizeInBytes += SubImageMemoryStructure.SizeInBytes;
                        
                        SubWidth = Max<U32>(1, SubWidth >> 1);
                        SubHeight = Max<U32>(1, SubHeight >> 1);
                        SubDepth = Max<U32>(1, SubDepth >> 1);
                    }
                }
            }
            
            if (Payload.Enable)
            {
                Payload.Bytes.resize(SizeInBytes);
            }
        }
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const TF_RHIImage& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Setting);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.MemoryStructure);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Payload);
            return F_FeedbackStatus::MakeSucceeded();   
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, TF_RHIImage& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Setting);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.MemoryStructure);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Payload);
            return F_FeedbackStatus::MakeSucceeded();   
        }
        
    private:
        template<E_RHIFormat __Format>
        void _InternalAddSubImage(
            U32 Index,
            U32 PlaneIndex,
            const F_RHISubImageSettingRaw& SubImageSettingRaw, 
            const F_RHISubImageTexelStructureRaw& SubImageTexelStructureRaw, 
            const TF_Span<const TF_RHIFormatData<__Format>>& SubImagePayloadRaw
        );
    };
    
    template<>
    struct TF_RHIImageView<false>
    {
        static constexpr B8 EnableWrite = false;
        
        TF_RHIImageSettingView<EnableWrite> Setting;
        TF_RHIImageMemoryStructureView<EnableWrite> MemoryStructure;
        TF_RHIImagePayloadView<EnableWrite> Payload;
        
        TF_RHIImageView() = default;
        TF_RHIImageView(const TF_RHIImageView& X) = default;
        TF_RHIImageView& operator = (const TF_RHIImageView& X) = default;
        TF_RHIImageView(TF_RHIImageView&& X) = default;
        TF_RHIImageView& operator = (TF_RHIImageView&& X) = default;
        
        template<typename __F_Allocator2>
        TF_RHIImageView(const TF_RHIImage<__F_Allocator2>& X);
        template<typename __F_Allocator2>
        TF_RHIImageView& operator = (const TF_RHIImage<__F_Allocator2>& X);
        
        TF_RHIImageView<false> GetReadOnly() const noexcept;
        TF_RHIImageView<true> GetReadWrite() const noexcept;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const TF_RHIImageView& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Setting);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.MemoryStructure);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Payload);
            return F_FeedbackStatus::MakeSucceeded();   
        }
    };
    template<>
    struct TF_RHIImageView<true>
    {
        static constexpr B8 EnableWrite = true;
        
        TF_RHIImageSettingView<EnableWrite> Setting;
        TF_RHIImageMemoryStructureView<EnableWrite> MemoryStructure;
        TF_RHIImagePayloadView<EnableWrite> Payload;
        
        TF_RHIImageView() = default;
        TF_RHIImageView(const TF_RHIImageView& X) = default;
        TF_RHIImageView& operator = (const TF_RHIImageView& X) = default;
        TF_RHIImageView(TF_RHIImageView&& X) = default;
        TF_RHIImageView& operator = (TF_RHIImageView&& X) = default;
        
        template<typename __F_Allocator2>
        TF_RHIImageView(TF_RHIImage<__F_Allocator2>& X);
        template<typename __F_Allocator2>
        TF_RHIImageView& operator = (TF_RHIImage<__F_Allocator2>& X);
        
        TF_RHIImageView<false> GetReadOnly() const noexcept;
        TF_RHIImageView<true> GetReadWrite() const noexcept;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const TF_RHIImageView& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Setting);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.MemoryStructure);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Payload);
            return F_FeedbackStatus::MakeSucceeded();   
        }
    };

    template <typename __F_Allocator>
    template <B8 __EnableWrite2>
    TF_RHIImage<__F_Allocator>::TF_RHIImage(const TF_RHIImageView<__EnableWrite2>& X) :
        Setting(X.Setting),
        MemoryStructure(X.MemoryStructure),
        Payload(X.Payload)
    {
    }
    template <typename __F_Allocator>
    template <B8 __EnableWrite2>
    TF_RHIImage<__F_Allocator>& TF_RHIImage<__F_Allocator>::operator = (const TF_RHIImageView<__EnableWrite2>& X)
    {
        Setting = X.Setting;
        MemoryStructure = X.MemoryStructure;
        Payload = X.Payload;
        return *this;
    }

    template <typename __F_Allocator>
    template<E_RHIFormat __Format>
    TF_RHIImage<__F_Allocator>::TF_RHIImage(const TF_RHIImageRawView<false, __Format>& RawView)
    {
        Setting.Format = __Format;
        
        Setting.Width = RawView.Setting.Width;
        Setting.Height = RawView.Setting.Height;
        Setting.Depth = RawView.Setting.Depth;
        
        Setting.DimensionCount = RawView.Setting.DimensionCount;
        
        Setting.MipLevelCount = RawView.Setting.MipLevelCount;
        Setting.ArraySize = RawView.Setting.ArraySize;
        
        Setting.Flags = RawView.Setting.Flags;
        Setting.Layout = E_RHIImageLayout::LINEAR;
        
        Payload.Enable = RawView.Payload.Enable;
        
        U32 NumSubImages = RawView.Setting.SubImages.size();
        for (U32 PlaneIndex = 0; PlaneIndex < RHIFormatPlaneCount(__Format); ++PlaneIndex)
        {
            for (U32 SubImageIndex = 0; SubImageIndex < NumSubImages; ++SubImageIndex)
            {
                const auto& SubImageSettingRaw = RawView.Setting.SubImages[SubImageIndex];
                const auto& SubImageTexelStructureRaw = RawView.TexelStructure.SubImages[SubImageIndex];
                TF_Span<const TF_RHIFormatData<__Format>> SubImagePayloadRaw(
                    RawView.Payload.Texels.begin() + SubImageTexelStructureRaw.OffsetInTexels,
                    RawView.Payload.Texels.begin() + SubImageTexelStructureRaw.OffsetInTexels + SubImageTexelStructureRaw.SizeInTexels
                );
            
                _InternalAddSubImage<__Format>(
                    SubImageIndex + NumSubImages * PlaneIndex,
                    PlaneIndex,
                    SubImageSettingRaw, 
                    SubImageTexelStructureRaw, 
                    SubImagePayloadRaw
                );
            }
        }
    }

    template <typename __F_Allocator>
    template<E_RHIFormat __Format>
    void TF_RHIImage<__F_Allocator>::_InternalAddSubImage(
        U32 Index,
        U32 PlaneIndex,
        const F_RHISubImageSettingRaw& SubImageSettingRaw, 
        const F_RHISubImageTexelStructureRaw& SubImageTexelStructureRaw, 
        const TF_Span<const TF_RHIFormatData<__Format>>& SubImagePayloadRaw
    )
    {
        Sz PlaneDataOffsetInBytes = RHIFormatPlaneDataOffset(Setting.Format, PlaneIndex);
        Sz PlaneDataSizeInBytes = RHIFormatPlaneDataSize(Setting.Format, PlaneIndex);
        Sz TexelSizeInBytes = RHIFormatSize(Setting.Format);
        
        Sz RowPitchInBytes = AlignSize(PlaneDataSizeInBytes * SubImageSettingRaw.Width, RHI_IMAGE_OPTIMAL_ROW_PITCH_ALIGNMENT);
        Sz SlicePitchInBytes = RowPitchInBytes * SubImageSettingRaw.Height;
        
        F_RHISubImageSetting SubImageSetting;
        SubImageSetting.Width = SubImageSettingRaw.Width;
        SubImageSetting.Height = SubImageSettingRaw.Height;
        SubImageSetting.Depth = SubImageSettingRaw.Depth;
        SubImageSetting.MipLevel = SubImageSettingRaw.MipLevel;
        SubImageSetting.ArraySlice = SubImageSettingRaw.ArraySlice;
        SubImageSetting.PlaneIndex = PlaneIndex;
        
        F_RHISubImageMemoryStructure SubImageMemoryStructure;
        SubImageMemoryStructure.OffsetInBytes = Payload.SizeInBytes;
        SubImageMemoryStructure.RowPitchInBytes = RowPitchInBytes;
        SubImageMemoryStructure.SizeInBytes = SlicePitchInBytes * SubImageSetting.Depth;
        Payload.SizeInBytes = SubImageMemoryStructure.OffsetInBytes + SubImageMemoryStructure.SizeInBytes;
        if (Payload.Enable)
        {
            Payload.Bytes.resize(Payload.SizeInBytes);
        }
        
        Setting.SubImages.push_back(SubImageSetting);
        MemoryStructure.SubImages.push_back(SubImageMemoryStructure);
        
        if (Payload.Enable)
        {
            void* BeginTexelPayloadAddress = Payload.Bytes.data() + SubImageMemoryStructure.OffsetInBytes;
            for (U32 Z = 0; Z < SubImageSetting.Depth; ++Z)
            {
                for (U32 Y = 0; Y < SubImageSetting.Height; ++Y)
                {
                    void* TexelPayloadAddress = (
                        (U8*)(BeginTexelPayloadAddress)
                        + Y * RowPitchInBytes
                        + Z * SlicePitchInBytes
                    );
                    const TF_RHIFormatData<__Format>* RowPtr = (
                        SubImagePayloadRaw.data() 
                        + Y * SubImageSettingRaw.Width 
                        + Z * SubImageSettingRaw.Width * SubImageSettingRaw.Height
                    );
                    if (TexelSizeInBytes == PlaneDataSizeInBytes)
                    {
                        memcpy(
                            TexelPayloadAddress,
                            RowPtr,
                            PlaneDataSizeInBytes * SubImageSettingRaw.Width
                        );
                    }
                    else
                    {
                        for (U32 X = 0; X < SubImageSetting.Width; ++X)
                        {
                            memcpy(
                                (U8*)(TexelPayloadAddress)
                                + X * PlaneDataSizeInBytes,
                                (const U8*)(RowPtr)
                                + X * TexelSizeInBytes
                                + PlaneDataOffsetInBytes,
                                PlaneDataSizeInBytes
                            );
                        }
                    }
                }
            }
        }
    }

    template <typename __F_Allocator2>
    TF_RHIImageView<false>::TF_RHIImageView(const TF_RHIImage<__F_Allocator2>& X) :
        Setting(X.Setting),
        MemoryStructure(X.MemoryStructure),
        Payload(X.Payload)
    {
    }
    template <typename __F_Allocator2>
    TF_RHIImageView<false>& TF_RHIImageView<false>::operator = (const TF_RHIImage<__F_Allocator2>& X)
    {
        Setting = X.Setting;
        MemoryStructure = X.MemoryStructure;
        Payload = X.Payload;
        return *this;
    }
    template <typename __F_Allocator2>
    TF_RHIImageView<true>::TF_RHIImageView(TF_RHIImage<__F_Allocator2>& X) :
        Setting(X.Setting),
        MemoryStructure(X.MemoryStructure),
        Payload(X.Payload)
    {
    }
    template <typename __F_Allocator2>
    TF_RHIImageView<true>& TF_RHIImageView<true>::operator = (TF_RHIImage<__F_Allocator2>& X)
    {
        Setting = X.Setting;
        MemoryStructure = X.MemoryStructure;
        Payload = X.Payload;
        return *this;
    }
    
    inline TF_RHIImageView<false> TF_RHIImageView<false>::GetReadOnly() const noexcept
    {
        return *(const TF_RHIImageView<false>*)this;
    }
    inline TF_RHIImageView<true> TF_RHIImageView<false>::GetReadWrite() const noexcept
    {
        return *(const TF_RHIImageView<true>*)this;
    }
    inline TF_RHIImageView<false> TF_RHIImageView<true>::GetReadOnly() const noexcept
    {
        return *(const TF_RHIImageView<false>*)this;
    }
    inline TF_RHIImageView<true> TF_RHIImageView<true>::GetReadWrite() const noexcept
    {
        return *(const TF_RHIImageView<true>*)this;
    }

    template <E_RHIFormat __Format, typename __F_Allocator>
    TF_RHIImageRaw<__Format, __F_Allocator>::TF_RHIImageRaw(const TF_RHIImageView<false>& Image)
    {
        Setting.Format = __Format;
        
        Setting.Width = Image.Setting.Width;
        Setting.Height = Image.Setting.Height;
        Setting.Depth = Image.Setting.Depth;
        
        Setting.DimensionCount = Image.Setting.DimensionCount;
        
        Setting.MipLevelCount = Image.Setting.MipLevelCount;
        Setting.ArraySize = Image.Setting.ArraySize;
        
        Setting.Flags = Image.Setting.Flags;
        
        Payload.Enable = Image.Payload.Enable;
        
        U32 PlaneCount = RHIFormatPlaneCount(Image.Setting.Format);
        
        U32 NumSubImages = Image.Setting.SubImages.size();
        U32 NumSubImagesPerPlane = NumSubImages / PlaneCount;
        for (U32 SubImageIndex = 0; SubImageIndex < NumSubImages; ++SubImageIndex)
        {
            const auto& SubImageSetting = Image.Setting.SubImages[SubImageIndex];
            const auto& SubImageMemoryStructure = Image.MemoryStructure.SubImages[SubImageIndex];
            TF_Span<const U8> SubImagePayload(
                Image.Payload.Bytes.begin() + SubImageMemoryStructure.OffsetInBytes,
                Image.Payload.Bytes.begin() + SubImageMemoryStructure.OffsetInBytes + SubImageMemoryStructure.SizeInBytes
            );
            _InternalAddSubImage(
                SubImageIndex % NumSubImagesPerPlane,
                SubImageIndex / NumSubImagesPerPlane,
                SubImageSetting, 
                SubImageMemoryStructure, 
                SubImagePayload
            );
        }
    }

    template <E_RHIFormat __Format, typename __F_Allocator>
    void TF_RHIImageRaw<__Format, __F_Allocator>::_InternalAddSubImage(
        U32 Index,
        U32 PlaneIndex,
        const F_RHISubImageSetting& SubImageSetting, 
        const F_RHISubImageMemoryStructure& SubImageMemoryStructure, 
        const TF_Span<const U8>& SubImagePayload
    )
    {
        Sz PlaneDataOffsetInBytes = RHIFormatPlaneDataOffset(Setting.Format, SubImageSetting.PlaneIndex);
        Sz PlaneDataSizeInBytes = RHIFormatPlaneDataSize(Setting.Format, SubImageSetting.PlaneIndex);
        Sz TexelSizeInBytes = RHIFormatSize(Setting.Format);
        
        Sz RowPitchInBytes = SubImageMemoryStructure.RowPitchInBytes;
        Sz SlicePitchInBytes = RowPitchInBytes * SubImageSetting.Height;
        
        F_RHISubImageSettingRaw SubImageSettingRaw;
        F_RHISubImageTexelStructureRaw SubImageTexelStructureRaw;
        if (PlaneIndex == 0)
        {
            SubImageSettingRaw.Width = SubImageSetting.Width;
            SubImageSettingRaw.Height = SubImageSetting.Height;
            SubImageSettingRaw.Depth = SubImageSetting.Depth;
            SubImageSettingRaw.MipLevel = SubImageSetting.MipLevel;
            SubImageSettingRaw.ArraySlice = SubImageSetting.ArraySlice;
            
            SubImageTexelStructureRaw.OffsetInTexels = Payload.SizeInTexels;
            SubImageTexelStructureRaw.SizeInTexels = SubImageSetting.Width * SubImageSetting.Height * SubImageSetting.Depth;
            Payload.SizeInTexels = SubImageTexelStructureRaw.OffsetInTexels + SubImageTexelStructureRaw.SizeInTexels;
            if (Payload.Enable)
            {
                Payload.Texels.resize(Payload.SizeInTexels);
            }
        
            Setting.SubImages.push_back(SubImageSettingRaw);
            TexelStructure.SubImages.push_back(SubImageTexelStructureRaw);
        }
        else
        {
            SubImageSettingRaw = Setting.SubImages[Index];
            SubImageTexelStructureRaw = TexelStructure.SubImages[Index];
        }
        
        if (Payload.Enable)
        {
            TF_RHIFormatData<__Format>* BeginRowPtrAddress = Payload.Texels.data() + SubImageTexelStructureRaw.OffsetInTexels;
            for (U32 Z = 0; Z < SubImageSetting.Depth; ++Z)
            {
                for (U32 Y = 0; Y < SubImageSetting.Height; ++Y)
                {
                    TF_RHIFormatData<__Format>* RowPtr = (
                        (TF_RHIFormatData<__Format>*)BeginRowPtrAddress
                        + Y * SubImageSettingRaw.Width 
                        + Z * SubImageSettingRaw.Width * SubImageSettingRaw.Height
                    );
                    const void* TexelPayloadRawAddress = (
                        SubImagePayload.data()
                        + Y * RowPitchInBytes
                        + Z * SlicePitchInBytes
                    );
                    if (TexelSizeInBytes == PlaneDataSizeInBytes)
                    {
                        memcpy(
                            RowPtr,
                            TexelPayloadRawAddress,
                            PlaneDataSizeInBytes * SubImageSettingRaw.Width
                        );
                    }
                    else
                    {
                        for (U32 X = 0; X < SubImageSetting.Width; ++X)
                        {
                            memcpy(
                                (U8*)(RowPtr)
                                + X * TexelSizeInBytes
                                + PlaneDataOffsetInBytes,
                                (const U8*)(TexelPayloadRawAddress)
                                + X * PlaneDataSizeInBytes,
                                PlaneDataSizeInBytes
                            );
                        }
                    }
                }
            }
        }
    }
    
    using F_RHIImageSettingRaw = TF_RHIImageSettingRaw<>;
    using F_RHIImageSettingRawROView = TF_RHIImageSettingRawView<false>;
    using F_RHIImageSettingRawRWView = TF_RHIImageSettingRawView<true>;
    
    using F_RHIImageSetting = TF_RHIImageSetting<>;
    using F_RHIImageSettingROView = TF_RHIImageSettingView<false>;
    using F_RHIImageSettingRWView = TF_RHIImageSettingView<true>;
    
    using F_RHIImageTexelStructureRaw = TF_RHIImageTexelStructureRaw<>;
    using F_RHIImageTexelStructureRawROView = TF_RHIImageTexelStructureRawView<false>;
    using F_RHIImageTexelStructureRawRWView = TF_RHIImageTexelStructureRawView<true>;
    
    using F_RHIImageMemoryStructure = TF_RHIImageMemoryStructure<>;
    using F_RHIImageMemoryStructureROView = TF_RHIImageMemoryStructureView<false>;
    using F_RHIImageMemoryStructureRWView = TF_RHIImageMemoryStructureView<true>;
    
    using F_RHIImagePayload = TF_RHIImagePayload<>;
    using F_RHIImagePayloadROView = TF_RHIImagePayloadView<false>;
    using F_RHIImagePayloadRWView = TF_RHIImagePayloadView<true>;
    
    using F_RHIImage = TF_RHIImage<>;
    using F_RHIImageROView = TF_RHIImageView<false>;
    using F_RHIImageRWView = TF_RHIImageView<true>;
}