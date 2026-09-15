#pragma once

#include "FeedbackStatus.hpp"
#include "Abytek/Base.Core.prerequisites.pch.hpp"
#include "Abytek/CrtUseAllocatorTypedefs.hpp"


namespace Abytek
{
    struct F_Archive;
    struct F_ArchiveViewBase;
    struct F_ArchiveReadOnlyView;
    struct F_ArchiveReadWriteView;
    
    struct ABYTEK_ALIGN(64) F_ArchiveDataAlignedElement
    {
    };
    struct F_ArchiveData
    {
        Sz Size = 0;
        TF_SmallVector<F_ArchiveDataAlignedElement, 1> Raw;
        ABYTEK_FORCE_INLINE auto Resize(Sz NewSize) noexcept
        {
            Sz Result = Size;
            Size = NewSize;
            ResizeRaw(NewSize); 
            return Result;
        }
        ABYTEK_FORCE_INLINE void ResizeRaw(Sz NewSize) noexcept
        {
            return Raw.resize(RoundUpDivide(NewSize, sizeof(F_ArchiveDataAlignedElement)));
        }
        ABYTEK_FORCE_INLINE auto GetSpan() noexcept
        {
            return TF_Span<U8>((U8*)Raw.data(), Size);
        }
        ABYTEK_FORCE_INLINE auto GetSpan() const noexcept
        {
            return TF_Span<const U8>((U8*)Raw.data(), Size);
        }
    };
    
    struct F_Archive
    {
        F_ArchiveData Data;
        
        static F_Archive From(const TF_Span<const U8>& Bytes) noexcept
        {
            F_Archive Result;
            auto Size = Bytes.size();
            Result.Resize(Size);
            memcpy(
                Result.Data.GetSpan().data(),
                Bytes.data(),
                Size
            );
            return ABYTEK_MOVE(Result);
        }
        
        ABYTEK_FORCE_INLINE auto GetSize() const noexcept
        {
            return Data.Size;
        }
        template<typename __F = U8>
        ABYTEK_FORCE_INLINE auto GetDataPtr(Sz Offset = 0) noexcept
        {
            return (std::remove_const_t<__F>*)(Data.GetSpan().data() + Offset);
        }
        template<typename __F = U8>
        ABYTEK_FORCE_INLINE auto GetDataPtr(Sz Offset = 0) const noexcept
        {
            return (const std::remove_const_t<__F>*)(Data.GetSpan().data() + Offset);
        }
        
        ABYTEK_FORCE_INLINE auto GetBegin() noexcept
        {
            return Data.GetSpan().begin();
        }
        ABYTEK_FORCE_INLINE auto GetEnd() noexcept
        {
            return Data.GetSpan().end();
        }
        ABYTEK_FORCE_INLINE auto GetBegin() const noexcept
        {
            return Data.GetSpan().cbegin();
        }
        ABYTEK_FORCE_INLINE auto GetEnd() const noexcept
        {
            return Data.GetSpan().cend();
        }
        ABYTEK_FORCE_INLINE auto GetReverseBegin() noexcept
        {
            return Data.GetSpan().rbegin();
        }
        ABYTEK_FORCE_INLINE auto GetReverseEnd() noexcept
        {
            return Data.GetSpan().rend();
        }
        ABYTEK_FORCE_INLINE auto GetReverseBegin() const noexcept
        {
            return Data.GetSpan().crbegin();
        }
        ABYTEK_FORCE_INLINE auto GetReverseEnd() const noexcept
        {
            return Data.GetSpan().crend();
        }
        
        ABYTEK_FORCE_INLINE auto begin() noexcept
        {
            return Data.GetSpan().begin();
        }
        ABYTEK_FORCE_INLINE auto end() noexcept
        {
            return Data.GetSpan().end();
        }
        ABYTEK_FORCE_INLINE auto begin() const noexcept
        {
            return Data.GetSpan().cbegin();
        }
        ABYTEK_FORCE_INLINE auto end() const noexcept
        {
            return Data.GetSpan().cend();
        }
        ABYTEK_FORCE_INLINE auto rbegin() noexcept
        {
            return Data.GetSpan().rbegin();
        }
        ABYTEK_FORCE_INLINE auto rend() noexcept
        {
            return Data.GetSpan().rend();
        }
        ABYTEK_FORCE_INLINE auto rbegin() const noexcept
        {
            return Data.GetSpan().crbegin();
        }
        ABYTEK_FORCE_INLINE auto rend() const noexcept
        {
            return Data.GetSpan().crend();
        }
        
        ABYTEK_FORCE_INLINE Sz Resize(Sz Bytes) noexcept
        {
            Sz Result = Data.Size;
            Data.Resize(Bytes);
            return Result;
        }
        ABYTEK_FORCE_INLINE Sz Extends(Sz Bytes, Sz Alignment = 1) noexcept
        {
            ABYTEK_BASE_CORE_ASSERT(Alignment <= ABYTEK_ALIGNOF(F_ArchiveDataAlignedElement)) 
                << "Alignment must be smaller than or equal to ABYTEK_ALIGNOF(F_ArchiveDataAlignedElement) = " 
                << ABYTEK_ALIGNOF(F_ArchiveDataAlignedElement);
            Sz Result = Data.Size;
            Result = AlignAddress_PO2(Result, Alignment);
            Data.Resize(Result + Bytes);
            return Result;
        }
    };
    
    struct F_ArchiveViewBase
    {
        F_ArchiveData* DataPtr = nullptr;
        Sz Offset = 0;
        TF_Map<F_Name, F_Any> Metadata;
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        B8 HasDevelopmentBuild = true;
#else
        B8 HasDevelopmentBuild = false;
#endif
        B8 IsTopLevel = false;
        
        TF_Vector<F_Name> RequiredPropertyTags;
        
        ABYTEK_FORCE_INLINE B8 IsValid() const noexcept
        {
            return DataPtr;
        }
        ABYTEK_FORCE_INLINE explicit operator B8 () const noexcept
        {
            return IsValid();
        }
        
        ABYTEK_FORCE_INLINE void Reset() noexcept
        {
            DataPtr = nullptr;
            GoToHead();
        }
        ABYTEK_FORCE_INLINE void GoToHead() noexcept
        {
            Offset = 0;
        }
        ABYTEK_FORCE_INLINE void GoToEnd() noexcept
        {
            ABYTEK_BASE_CORE_ASSERT(IsValid());
            Offset = DataPtr->Size;
        }
        
        ABYTEK_FORCE_INLINE auto GetSize() const noexcept
        {
            return DataPtr->Size;
        }
        
        ABYTEK_FORCE_INLINE auto GetBegin() noexcept
        {
            return DataPtr->GetSpan().begin();
        }
        ABYTEK_FORCE_INLINE auto GetEnd() noexcept
        {
            return DataPtr->GetSpan().end();
        }
        ABYTEK_FORCE_INLINE auto GetBegin() const noexcept
        {
            return DataPtr->GetSpan().cbegin();
        }
        ABYTEK_FORCE_INLINE auto GetEnd() const noexcept
        {
            return DataPtr->GetSpan().cend();
        }
        ABYTEK_FORCE_INLINE auto GetReverseBegin() noexcept
        {
            return DataPtr->GetSpan().rbegin();
        }
        ABYTEK_FORCE_INLINE auto GetReverseEnd() noexcept
        {
            return DataPtr->GetSpan().rend();
        }
        ABYTEK_FORCE_INLINE auto GetReverseBegin() const noexcept
        {
            return DataPtr->GetSpan().crbegin();
        }
        ABYTEK_FORCE_INLINE auto GetReverseEnd() const noexcept
        {
            return DataPtr->GetSpan().crend();
        }
        
        ABYTEK_FORCE_INLINE auto begin() noexcept
        {
            return DataPtr->GetSpan().begin();
        }
        ABYTEK_FORCE_INLINE auto end() noexcept
        {
            return DataPtr->GetSpan().end();
        }
        ABYTEK_FORCE_INLINE auto begin() const noexcept
        {
            return DataPtr->GetSpan().cbegin();
        }
        ABYTEK_FORCE_INLINE auto end() const noexcept
        {
            return DataPtr->GetSpan().cend();
        }
        ABYTEK_FORCE_INLINE auto rbegin() noexcept
        {
            return DataPtr->GetSpan().rbegin();
        }
        ABYTEK_FORCE_INLINE auto rend() noexcept
        {
            return DataPtr->GetSpan().rend();
        }
        ABYTEK_FORCE_INLINE auto rbegin() const noexcept
        {
            return DataPtr->GetSpan().crbegin();
        }
        ABYTEK_FORCE_INLINE auto rend() const noexcept
        {
            return DataPtr->GetSpan().crend();
        }
        
        template<typename __F>
        ABYTEK_FORCE_INLINE auto Shift(Sz Num = 1, Sz Alignment = ABYTEK_ALIGNOF(__F)) noexcept
        {
            ABYTEK_BASE_CORE_ASSERT(Alignment <= ABYTEK_ALIGNOF(F_ArchiveDataAlignedElement)) 
                << "Alignment must be smaller than or equal to ABYTEK_ALIGNOF(F_ArchiveDataAlignedElement) = " 
                << ABYTEK_ALIGNOF(F_ArchiveDataAlignedElement);
            Sz Result = Offset;
            Result = AlignAddress_PO2(Result, Alignment);
            Offset = Result + sizeof(__F) * Num;
            if (Offset > GetSize())
            {
                DataPtr->Resize(Offset);
            }
            return Result;
        }
        template<typename __F = U8>
        ABYTEK_FORCE_INLINE auto ShiftNoResize(Sz Num = 1, Sz Alignment = ABYTEK_ALIGNOF(__F)) noexcept
        {
            ABYTEK_BASE_CORE_ASSERT(Alignment <= ABYTEK_ALIGNOF(F_ArchiveDataAlignedElement)) 
                << "Alignment must be smaller than or equal to ABYTEK_ALIGNOF(F_ArchiveDataAlignedElement) = " 
                << ABYTEK_ALIGNOF(F_ArchiveDataAlignedElement);
            Sz Result = Offset;
            Result = AlignAddress_PO2(Result, Alignment);
            Offset = Result + sizeof(__F) * Num;
            ABYTEK_BASE_CORE_ASSERT(Offset <= DataPtr->Size) << "Offset out of bound";
            return Result;
        }
        template<typename __F = U8>
        ABYTEK_FORCE_INLINE auto CheckSize(Sz Num = 1, Sz Alignment = ABYTEK_ALIGNOF(__F)) const noexcept
        {
            ABYTEK_BASE_CORE_ASSERT(Alignment <= ABYTEK_ALIGNOF(F_ArchiveDataAlignedElement)) 
                << "Alignment must be smaller than or equal to ABYTEK_ALIGNOF(F_ArchiveDataAlignedElement) = " 
                << ABYTEK_ALIGNOF(F_ArchiveDataAlignedElement);
            Sz Result = Offset;
            Result = AlignAddress_PO2(Result, Alignment);
            return ((Result + sizeof(__F) * Num) <= DataPtr->Size);
        }
    };
    
    struct F_ArchiveReadOnlyView : F_ArchiveViewBase
    {
        static F_ArchiveReadOnlyView From(const F_Archive& Archive, Sz InOffset = 0)
        {
            F_ArchiveReadOnlyView Result;
            Result.DataPtr = (F_ArchiveData*)&Archive.Data;
            Result.Offset = InOffset;
            return Result;
        }
        
        ABYTEK_FORCE_INLINE F_ArchiveViewBase GetBase() const noexcept
        {
            F_ArchiveViewBase Result;
            Result.DataPtr = DataPtr;
            return Result;
        }
        F_ArchiveReadWriteView GetReadWrite() const noexcept;
        
        template<typename __F = U8>
        ABYTEK_FORCE_INLINE auto GetSpan() const noexcept
        {
            auto U8Span = DataPtr->GetSpan();
            return TF_Span<const std::remove_const_t<__F>>(
                (const std::remove_const_t<__F>*)U8Span.data(),
                U8Span.size() / sizeof(__F)
            );
        }
        template<typename __F = U8>
        ABYTEK_FORCE_INLINE auto AccessSpan(Sz Num, Sz Alignment = ABYTEK_ALIGNOF(__F)) noexcept
        {
            ABYTEK_BASE_CORE_ASSERT(Alignment <= ABYTEK_ALIGNOF(F_ArchiveDataAlignedElement)) 
                << "Alignment must be smaller than or equal to ABYTEK_ALIGNOF(F_ArchiveDataAlignedElement) = " 
                << ABYTEK_ALIGNOF(F_ArchiveDataAlignedElement);
            auto ResultOffset = ShiftNoResize<__F>(Num, Alignment);
            return TF_Span<const std::remove_const_t<__F>>(
                (const std::remove_const_t<__F>*)(DataPtr->GetSpan().data() + ResultOffset),
                Num
            );
        }
        template<typename __F = U8>
        ABYTEK_FORCE_INLINE auto& Access(Sz Alignment = ABYTEK_ALIGNOF(__F)) noexcept
        {
            ABYTEK_BASE_CORE_ASSERT(Alignment <= ABYTEK_ALIGNOF(F_ArchiveDataAlignedElement)) 
                << "Alignment must be smaller than or equal to ABYTEK_ALIGNOF(F_ArchiveDataAlignedElement) = " 
                << ABYTEK_ALIGNOF(F_ArchiveDataAlignedElement);
            auto ResultOffset = ShiftNoResize<__F>(1, Alignment);
            return *(const std::remove_const_t<__F>*)(DataPtr->GetSpan().data() + ResultOffset);
        }
    };
    struct F_ArchiveReadWriteView : F_ArchiveViewBase
    {
        static F_ArchiveReadWriteView From(F_Archive& Archive, Sz InOffset = 0)
        {
            F_ArchiveReadWriteView Result;
            Result.DataPtr = (F_ArchiveData*)&Archive.Data;
            Result.Offset = InOffset;
            return Result;
        }
        
        ABYTEK_FORCE_INLINE F_ArchiveViewBase GetBase() const noexcept
        {
            F_ArchiveViewBase Result;
            Result.DataPtr = DataPtr;
            return Result;
        }
        F_ArchiveReadOnlyView GetReadOnly() const noexcept;
        
        template<typename __F = U8>
        ABYTEK_FORCE_INLINE auto GetSpan() const noexcept
        {
            auto U8Span = DataPtr->GetSpan();
            return TF_Span<std::remove_const_t<__F>>(
                (std::remove_const_t<__F>*)U8Span.data(),
                U8Span.size() / sizeof(__F)
            );
        }
        template<typename __F = U8>
        ABYTEK_FORCE_INLINE auto AccessSpan(Sz Num, Sz Alignment = ABYTEK_ALIGNOF(__F)) noexcept
        {
            ABYTEK_BASE_CORE_ASSERT(Alignment <= ABYTEK_ALIGNOF(F_ArchiveDataAlignedElement)) 
                << "Alignment must be smaller than or equal to ABYTEK_ALIGNOF(F_ArchiveDataAlignedElement) = " 
                << ABYTEK_ALIGNOF(F_ArchiveDataAlignedElement);
            auto ResultOffset = Shift<__F>(Num, Alignment);
            return TF_Span<std::remove_const_t<__F>>(
                (std::remove_const_t<__F>*)(DataPtr->GetSpan().data() + ResultOffset),
                Num
            );
        }
        template<typename __F = U8>
        ABYTEK_FORCE_INLINE auto& Access(Sz Alignment = ABYTEK_ALIGNOF(__F)) noexcept
        {
            ABYTEK_BASE_CORE_ASSERT(Alignment <= ABYTEK_ALIGNOF(F_ArchiveDataAlignedElement)) 
                << "Alignment must be smaller than or equal to ABYTEK_ALIGNOF(F_ArchiveDataAlignedElement) = " 
                << ABYTEK_ALIGNOF(F_ArchiveDataAlignedElement);
            auto ResultOffset = Shift<__F>(1, Alignment);
            return *(std::remove_const_t<__F>*)(DataPtr->GetSpan().data() + ResultOffset);
        }
        template<typename __F = U8>
        ABYTEK_FORCE_INLINE auto AccessSpanNoResize(Sz Num, Sz Alignment = ABYTEK_ALIGNOF(__F)) noexcept
        {
            ABYTEK_BASE_CORE_ASSERT(Alignment <= ABYTEK_ALIGNOF(F_ArchiveDataAlignedElement)) 
                << "Alignment must be smaller than or equal to ABYTEK_ALIGNOF(F_ArchiveDataAlignedElement) = " 
                << ABYTEK_ALIGNOF(F_ArchiveDataAlignedElement);
            auto ResultOffset = ShiftNoResize<__F>(Num, Alignment);
            return TF_Span<std::remove_const_t<__F>>(
                (std::remove_const_t<__F>*)(DataPtr->GetSpan().data() + ResultOffset),
                Num
            );
        }
        template<typename __F = U8>
        ABYTEK_FORCE_INLINE auto& AccessNoResize(Sz Alignment = ABYTEK_ALIGNOF(__F)) noexcept
        {
            ABYTEK_BASE_CORE_ASSERT(Alignment <= ABYTEK_ALIGNOF(F_ArchiveDataAlignedElement)) 
                << "Alignment must be smaller than or equal to ABYTEK_ALIGNOF(F_ArchiveDataAlignedElement) = " 
                << ABYTEK_ALIGNOF(F_ArchiveDataAlignedElement);
            auto ResultOffset = ShiftNoResize<__F>(1, Alignment);
            return *(std::remove_const_t<__F>*)(DataPtr->GetSpan().data() + ResultOffset);
        }
    };
    
    ABYTEK_FORCE_INLINE F_ArchiveReadWriteView F_ArchiveReadOnlyView::GetReadWrite() const noexcept
    {
        return { GetBase() };
    }
    ABYTEK_FORCE_INLINE F_ArchiveReadOnlyView F_ArchiveReadWriteView::GetReadOnly() const noexcept
    {
        return { GetBase() };
    }
    
    struct I_ArchiveReader
    {
        virtual F_FeedbackStatus ReadArchive(F_ArchiveReadOnlyView& View) = 0;
        
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, I_ArchiveReader& ArchiveReader)
        {
            return ArchiveReader.ReadArchive(View);
        }
    };
    struct I_ArchiveWriter
    {
        virtual F_FeedbackStatus WriteArchive(F_ArchiveReadWriteView& View) = 0;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, I_ArchiveWriter& ArchiveReader)
        {
            return ArchiveReader.WriteArchive(View);
        }
    };
    template<typename __F>
    ABYTEK_FORCE_INLINE F_FeedbackStatus operator >> (F_ArchiveReadWriteView& View, __F& Value)
    {
        return (View.GetReadOnly() >> Value);
    }
    
    template<typename __F>
    constexpr B8 IsShallowReadWriteType()
    {
        if constexpr (std::is_same_v<__F, B8>)
        {
            return true;
        }
        if constexpr (std::is_same_v<__F, U8>)
        {
            return true;
        }
        if constexpr (std::is_same_v<__F, U16>)
        {
            return true;
        }
        if constexpr (std::is_same_v<__F, U32>)
        {
            return true;
        }
        if constexpr (std::is_same_v<__F, U64>)
        {
            return true;
        }
        if constexpr (std::is_same_v<__F, I8>)
        {
            return true;
        }
        if constexpr (std::is_same_v<__F, I16>)
        {
            return true;
        }
        if constexpr (std::is_same_v<__F, I32>)
        {
            return true;
        }
        if constexpr (std::is_same_v<__F, I64>)
        {
            return true;
        }
        return false;
    }
    
    namespace Internal::Archive
    {
        template<typename TValue, typename = void>
        struct TH_HasSerialize : std::false_type
        {
        };
        template<typename TValue>
        struct TH_HasSerialize<
            TValue,
            std::void_t<
                decltype(std::declval<F_ArchiveReadWriteView&>() << std::declval<const TValue&>())
            >
        > : std::true_type
        {
        };


        template<typename TValue, typename = void>
        struct TH_HasDeserialize : std::false_type
        {
        };
        template<typename TValue>
        struct TH_HasDeserialize<
            TValue,
            std::void_t<
                decltype(std::declval<F_ArchiveReadOnlyView&>() >> std::declval<TValue&>())
            >
        > : std::true_type
        {
        };
    }
    template<typename TValue>
    constexpr B8 CanWriteArchive()
    {
        return Internal::Archive::TH_HasSerialize<TValue>::value;
    }
    template<typename TValue>
    constexpr B8 CanReadArchive()
    {
        return Internal::Archive::TH_HasDeserialize<TValue>::value;
    }
}
