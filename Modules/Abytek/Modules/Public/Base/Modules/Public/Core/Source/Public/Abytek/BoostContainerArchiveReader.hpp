#pragma once

#include "Abytek/Archive.hpp"
#include "Abytek/PrimitiveArchiveReader.hpp"


namespace Abytek
{
    template<typename __F_Char, typename __F_CharTraits, typename __F_Allocator>
    F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, boost::container::basic_string<__F_Char, __F_CharTraits, __F_Allocator>& Value) noexcept
    {
        Sz UTF8ValueSize;
        ABYTEK_FEEDBACK_STATUS_CHECK(View >> UTF8ValueSize);
        
        if (!View.CheckSize<F_Char>(UTF8ValueSize))
        {
            return F_FeedbackStatus::MakeFailed();
        }
        
        auto UTF8Bytes = View.AccessSpan<F_Char>(UTF8ValueSize);
        
        F_String UTF8Value;
        UTF8Value.resize(UTF8ValueSize);
        memcpy(
            UTF8Value.data(),
            UTF8Bytes.data(),
            UTF8ValueSize * sizeof(F_Char)
        );
        
        Value = ToString<__F_Char, F_String>(UTF8Value);
        return F_FeedbackStatus::MakeSucceeded();
    }
    template<typename __F_Item, Sz __Size>
    F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, boost::array<__F_Item, __Size>& Value) noexcept
    {
        if constexpr (IsShallowReadWriteType<__F_Item>())
        {
            if (!View.CheckSize<__F_Item>(__Size))
            {
                return F_FeedbackStatus::MakeFailed();
            }
            auto Chars = View.AccessSpan<__F_Item>(__Size);
            memcpy(
                Value.data(),
                Chars.data(),
                __Size * sizeof(__F_Item)
            );
            return F_FeedbackStatus::MakeSucceeded();
        }
        for (auto& Item : Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Item);
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    template<typename __F>
    F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, boost::optional<__F>& Value) noexcept
    {
        B8 HasValue;
        ABYTEK_FEEDBACK_STATUS_CHECK(View >> HasValue);
        if (HasValue)
        {
            __F ValueTemp;
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> ValueTemp);
            Value = ValueTemp;
        }
        else
        {
            Value = {};
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    template<typename __F_Item, typename __F_Allocator>
    F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, boost::container::vector<__F_Item, __F_Allocator>& Value) noexcept
    {
        Sz Size;
        ABYTEK_FEEDBACK_STATUS_CHECK(View >> Size);
        
        Value.resize(Size);
        
        if constexpr (IsShallowReadWriteType<__F_Item>())
        {
            if (!View.CheckSize<__F_Item>(Size))
            {
                return F_FeedbackStatus::MakeFailed();
            }
            auto Chars = View.AccessSpan<__F_Item>(Size);
            memcpy(
                Value.data(),
                Chars.data(),
                Size * sizeof(__F_Item)
            );
            return F_FeedbackStatus::MakeSucceeded();
        }

        for (auto& Item : Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Item);
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    template<typename __F_Item, typename __F_Allocator>
    F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, boost::container::devector<__F_Item, __F_Allocator>& Value) noexcept
    {
        Sz Size;
        ABYTEK_FEEDBACK_STATUS_CHECK(View >> Size);
        
        Value.resize(Size);

        for (auto& Item : Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Item);
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    template<typename __F_Item, Sz __StaticSize, typename __F_Allocator>
    F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, boost::container::small_vector<__F_Item, __StaticSize, __F_Allocator>& Value) noexcept
    {
        Sz Size;
        ABYTEK_FEEDBACK_STATUS_CHECK(View >> Size);
        
        Value.resize(Size);
        
        if constexpr (IsShallowReadWriteType<__F_Item>())
        {
            if (!View.CheckSize<__F_Item>(Size))
            {
                return F_FeedbackStatus::MakeFailed();
            }
            auto Chars = View.AccessSpan<__F_Item>(Size);
            memcpy(
                Value.data(),
                Chars.data(),
                Size * sizeof(__F_Item)
            );
            return F_FeedbackStatus::MakeSucceeded();
        }

        for (auto& Item : Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Item);
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    template<typename __F_Item, typename __F_Allocator>
    F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, boost::container::stable_vector<__F_Item, __F_Allocator>& Value) noexcept
    {
        Sz Size;
        ABYTEK_FEEDBACK_STATUS_CHECK(View >> Size);
        
        Value.resize(Size);
        
        if constexpr (IsShallowReadWriteType<__F_Item>())
        {
            if (!View.CheckSize<__F_Item>(Size))
            {
                return F_FeedbackStatus::MakeFailed();
            }
            auto Chars = View.AccessSpan<__F_Item>(Size);
            memcpy(
                Value.data(),
                Chars.data(),
                Size * sizeof(__F_Item)
            );
            return F_FeedbackStatus::MakeSucceeded();
        }

        for (auto& Item : Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Item);
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    template<typename __F_Item, Sz __StaticSize, typename __F_Options>
    F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, boost::container::static_vector<__F_Item, __StaticSize, __F_Options>& Value) noexcept
    {
        Sz Size;
        ABYTEK_FEEDBACK_STATUS_CHECK(View >> Size);
        
        Value.resize(Size);
        
        if constexpr (IsShallowReadWriteType<__F_Item>())
        {
            if (!View.CheckSize<__F_Item>(Size))
            {
                return F_FeedbackStatus::MakeFailed();
            }
            auto Chars = View.AccessSpan<__F_Item>(Size);
            memcpy(
                Value.data(),
                Chars.data(),
                Size * sizeof(__F_Item)
            );
            return F_FeedbackStatus::MakeSucceeded();
        }

        for (auto& Item : Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Item);
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    template<typename __F_Item, typename __F_Allocator>
    F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, boost::container::deque<__F_Item, __F_Allocator>& Value) noexcept
    {
        Sz Size;
        ABYTEK_FEEDBACK_STATUS_CHECK(View >> Size);
        
        Value.resize(Size);

        for (auto& Item : Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Item);
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    template<typename __F_Item, typename __F_Allocator>
    F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, boost::container::list<__F_Item, __F_Allocator>& Value) noexcept
    {
        Sz Size;
        ABYTEK_FEEDBACK_STATUS_CHECK(View >> Size);

        for (auto Idx = 0; Idx < Size; ++Idx)
        {
            __F_Item Item;
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Item);
            Value.push_back(ABYTEK_MOVE(Item));
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    template<typename __F_Item, typename __F_Allocator>
    F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, boost::container::slist<__F_Item, __F_Allocator>& Value) noexcept
    {
        Sz Size;
        ABYTEK_FEEDBACK_STATUS_CHECK(View >> Size);

        for (auto Idx = 0; Idx < Size; ++Idx)
        {
            __F_Item Item;
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Item);
            Value.push_back(ABYTEK_MOVE(Item));
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    template<typename __F_Item, typename __F_Compare, typename __F_Allocator, typename __F_Options>
    F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, boost::container::set<__F_Item, __F_Compare, __F_Allocator, __F_Options>& Value) noexcept
    {
        Sz Size;
        ABYTEK_FEEDBACK_STATUS_CHECK(View >> Size);
        
        for (auto Idx = 0; Idx < Size; ++Idx)
        {
            __F_Item Item;
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Item);
            Value.insert(ABYTEK_MOVE(Item));
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    template<typename __F_Key, typename __F_Value, typename __F_Compare, typename __F_Allocator, typename __F_Options>
    F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, boost::container::map<__F_Key, __F_Value, __F_Compare, __F_Allocator, __F_Options>& Value) noexcept
    {
        Sz Size;
        ABYTEK_FEEDBACK_STATUS_CHECK(View >> Size);
        
        for (auto Idx = 0; Idx < Size; ++Idx)
        {
            __F_Key Key;
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Key);
            
            __F_Value IValue;
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> IValue);
            
            Value.insert({ ABYTEK_MOVE(Key), ABYTEK_MOVE(IValue) });
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    template<typename __F_Key, typename __F_Value, typename __F_Compare, typename __F_Allocator, typename __F_Options>
    F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, boost::container::multimap<__F_Key, __F_Value, __F_Compare, __F_Allocator, __F_Options>& Value) noexcept
    {
        Sz Size;
        ABYTEK_FEEDBACK_STATUS_CHECK(View >> Size);
        
        for (auto Idx = 0; Idx < Size; ++Idx)
        {
            __F_Key Key;
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Key);
            
            __F_Value IValue;
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> IValue);
            
            Value.insert({ ABYTEK_MOVE(Key), ABYTEK_MOVE(IValue) });
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    template<typename __F_Item, typename __F_Hash, typename __F_Predicate, typename __F_Allocator>
    F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, boost::unordered_set<__F_Item, __F_Hash, __F_Predicate, __F_Allocator>& Value) noexcept
    {
        Sz Size;
        ABYTEK_FEEDBACK_STATUS_CHECK(View >> Size);
        
        for (auto Idx = 0; Idx < Size; ++Idx)
        {
            __F_Item Item;
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Item);
            Value.insert(ABYTEK_MOVE(Item));
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    template<typename __F_Key, typename __F_Value, typename __F_Hash, typename __F_Predicate, typename __F_Allocator>
    F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, boost::unordered_map<__F_Key, __F_Value, __F_Hash, __F_Predicate, __F_Allocator>& Value) noexcept
    {
        Sz Size;
        ABYTEK_FEEDBACK_STATUS_CHECK(View >> Size);
        
        for (auto Idx = 0; Idx < Size; ++Idx)
        {
            __F_Key Key;
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Key);
            
            __F_Value IValue;
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> IValue);
            
            Value.insert({ ABYTEK_MOVE(Key), ABYTEK_MOVE(IValue) });
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    template<typename __F_Key, typename __F_Value, typename __F_Hash, typename __F_Predicate, typename __F_Allocator>
    F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, boost::unordered_multimap<__F_Key, __F_Value, __F_Hash, __F_Predicate, __F_Allocator>& Value) noexcept
    {
        Sz Size;
        ABYTEK_FEEDBACK_STATUS_CHECK(View >> Size);
        
        for (auto Idx = 0; Idx < Size; ++Idx)
        {
            __F_Key Key;
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Key);
            
            __F_Value IValue;
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> IValue);
            
            Value.insert({ ABYTEK_MOVE(Key), ABYTEK_MOVE(IValue) });
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
}