#pragma once

#include "Abytek/Engine.Resource.prerequisites.hpp"

namespace Abytek
{
    struct F_TaggedPath
    {
        // The first tag is resolver tag
        TF_SmallVector<F_Name, 4> Tags;

        F_TaggedPath() noexcept = default;
        F_TaggedPath(const F_TaggedPath& x) noexcept = default;
        F_TaggedPath& operator = (const F_TaggedPath& x) noexcept = default;
        F_TaggedPath(F_TaggedPath&& x) noexcept = default;
        F_TaggedPath& operator = (F_TaggedPath&& x) noexcept = default;

        F_TaggedPath(const TF_Span<F_Name>& InTags) :
            Tags(InTags.begin(), InTags.end())
        {
        }
        template<Sz __N>
        F_TaggedPath(const F_Name (&InTags)[__N]) :
            Tags(InTags, InTags + GetStaticArraySize(InTags))
        {
        }
        F_TaggedPath(const TF_SmallVector<F_Name, 4>& InTags) :
            Tags(InTags.begin(), InTags.end())
        {
        }
        F_TaggedPath(const TF_Vector<F_Name>& InTags) :
            Tags(InTags.begin(), InTags.end())
        {
        }
        F_TaggedPath(const F_Name& CoreTag) :
            Tags({ CoreTag })
        {
        }
    
        ABYTEK_FORCE_INLINE const auto& GetCoreTag() const noexcept
        {
            ABYTEK_ENGINE_ASSETS_ASSERT(IsValid()) << "Invalid asset path";
            return Tags[0];
        }
        ABYTEK_FORCE_INLINE TF_Span<F_Name> GetSubTags() const noexcept
        {
            ABYTEK_ENGINE_ASSETS_ASSERT(IsValid()) << "Invalid asset path";
            return TF_Span<F_Name> {
                (F_Name*)(Tags.data() + 1),
                Tags.size() - 1
            };
        }
        ABYTEK_FORCE_INLINE F_TaggedPath GetSubPath() const noexcept
        {
            ABYTEK_ENGINE_ASSETS_ASSERT(IsValid()) << "Invalid asset path";
            F_TaggedPath Result;
            Result.Tags.insert(Result.Tags.end(), Tags.begin() + 1, Tags.end());
            return ABYTEK_MOVE(Result);
        }
        ABYTEK_FORCE_INLINE F_TaggedPath GetTail() const noexcept
        {
            return Tags.back();
        }

        ABYTEK_FORCE_INLINE bool IsCoreOnly() const noexcept
        {
            return Tags.size() == 1;
        }

        static F_TaggedPath Make(const F_Name& CoreTag, const TF_Span<F_Name>& SubTags) noexcept
        {
            F_TaggedPath Result;
            Result.Tags.push_back(CoreTag);
            Result.Tags.insert(Result.Tags.end(), SubTags.begin(), SubTags.end());
            return ABYTEK_MOVE(Result);
        }
        static F_TaggedPath Make(const TF_Span<F_Name>& InTags) noexcept
        {
            F_TaggedPath Result;
            Result.Tags.insert(Result.Tags.end(), InTags.begin(), InTags.end());
            return ABYTEK_MOVE(Result);
        }
        static F_TaggedPath Parse(const F_Text& TextPath) noexcept
        {
            F_TaggedPath Result;
            TF_SmallVector<F_Text, 4> TagTexts;
            boost::split(TagTexts, TextPath, boost::is_any_of(ABYTEK_TEXT("/")));
            for (const auto& TagText : TagTexts)
            {
                Result.Tags.push_back(TagText);
            }
            return ABYTEK_MOVE(Result);
        }

        ABYTEK_FORCE_INLINE B8 IsValid() const noexcept
        {
            return Tags.size() > 0;
        }
        ABYTEK_FORCE_INLINE explicit operator bool() const noexcept
        {
            return IsValid();
        }

        ABYTEK_FORCE_INLINE auto GetBegin() noexcept
        {
            return Tags.begin();
        }
        ABYTEK_FORCE_INLINE auto GetBegin() const noexcept
        {
            return Tags.begin();
        }
        ABYTEK_FORCE_INLINE auto GetEnd() noexcept
        {
            return Tags.end();
        }
        ABYTEK_FORCE_INLINE auto GetEnd() const noexcept
        {
            return Tags.end();
        }
        ABYTEK_FORCE_INLINE auto GetReverseBegin() noexcept
        {
            return Tags.rbegin();
        }
        ABYTEK_FORCE_INLINE auto GetReverseBegin() const noexcept
        {
            return Tags.rbegin();
        }
        ABYTEK_FORCE_INLINE auto GetReverseEnd() noexcept
        {
            return Tags.rend();
        }
        ABYTEK_FORCE_INLINE auto GetReverseEnd() const noexcept
        {
            return Tags.rend();
        }
        
        ABYTEK_FORCE_INLINE auto begin() noexcept
        {
            return Tags.begin();
        }
        ABYTEK_FORCE_INLINE auto begin() const noexcept
        {
            return Tags.begin();
        }
        ABYTEK_FORCE_INLINE auto end() noexcept
        {
            return Tags.end();
        }
        ABYTEK_FORCE_INLINE auto end() const noexcept
        {
            return Tags.end();
        }
        ABYTEK_FORCE_INLINE auto rbegin() noexcept
        {
            return Tags.rbegin();
        }
        ABYTEK_FORCE_INLINE auto rbegin() const noexcept
        {
            return Tags.rbegin();
        }
        ABYTEK_FORCE_INLINE auto rend() noexcept
        {
            return Tags.rend();
        }
        ABYTEK_FORCE_INLINE auto rend() const noexcept
        {
            return Tags.rend();
        }

        ABYTEK_FORCE_INLINE auto GetSize() const noexcept
        {
            return Tags.size();
        }

        friend ABYTEK_FORCE_INLINE F_TaggedPath operator + (const F_TaggedPath& A, const F_TaggedPath& B) noexcept
        {
            F_TaggedPath Result = A;
            Result.Tags.insert(Result.Tags.end(), B.Tags.begin(), B.Tags.end());
            return ABYTEK_MOVE(Result);
        }
        ABYTEK_FORCE_INLINE F_TaggedPath& operator += (const F_TaggedPath& X) noexcept
        {
            Tags.insert(Tags.end(), X.Tags.begin(), X.Tags.end());
            return *this;
        }
        ABYTEK_FORCE_INLINE F_TaggedPath& operator -- () noexcept
        {
            Tags.pop_back();
            return *this;
        }
        ABYTEK_FORCE_INLINE F_TaggedPath operator - (Sz Num) const noexcept
        {
            ABYTEK_ENGINE_RESOURCE_ASSERT(GetSize() >= Num);
            F_TaggedPath Result;
            Result.Tags.insert(Result.Tags.end(), Tags.begin(), Tags.begin() + (GetSize() - Num));
            return ABYTEK_MOVE(Result);
        }
        ABYTEK_FORCE_INLINE F_TaggedPath& operator -= (Sz Num) noexcept
        {
            F_TaggedPath Result = (*this - Num);
            *this = Result;
            return *this;
        }
        ABYTEK_FORCE_INLINE B8 operator == (const F_TaggedPath& X) const noexcept
        {
            return Tags == X.Tags;
        }
        ABYTEK_FORCE_INLINE B8 operator != (const F_TaggedPath& X) const noexcept
        {
            return Tags != X.Tags;
        }

        F_Text ToText() const noexcept
        {
            F_Text Result;
            U32 NumTags = Tags.size();
            for (U32 Idx = 0; Idx < NumTags; ++Idx)
            {
                Result += *Tags[Idx];
                if (Idx < (NumTags - 1))
                {
                    Result += ABYTEK_TEXT("/");
                }
            }
            return ABYTEK_MOVE(Result);
        }
        F_String ToString() const noexcept
        {
            F_String Result;
            U32 NumTags = Tags.size();
            for (U32 Idx = 0; Idx < NumTags; ++Idx)
            {
                Result += Abytek::ToString(*Tags[Idx]);
                if (Idx < (NumTags - 1))
                {
                    Result += "/";
                }
            }
            return ABYTEK_MOVE(Result);
        }
        
        ABYTEK_FORCE_INLINE F_Text operator * () const noexcept
        {
            return ToText();
        }
    };
}

namespace boost
{
    template<>
    struct hash<Abytek::F_TaggedPath>
    {
        size_t operator()(const Abytek::F_TaggedPath& Value) const noexcept
        {
            size_t Result = 0;
            hash_combine(Result, Value.Tags);
            return Result;
        }
    };
}

#define ABYTEK_TAGGED_CORE_PATH(...) Abytek::F_TaggedPath(ABYTEK_NAME(__VA_ARGS__))
#define ABYTEK_TAGGED_PATH(...) Abytek::F_TaggedPath::Parse(ABYTEK_TEXT(__VA_ARGS__))