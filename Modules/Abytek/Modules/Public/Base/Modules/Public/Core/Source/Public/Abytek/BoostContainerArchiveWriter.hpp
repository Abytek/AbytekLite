#pragma once

#include "Abytek/Archive.hpp"
#include "Abytek/PrimitiveArchiveWriter.hpp"


namespace Abytek
{
    template<typename __F_Char, typename __F_CharTraits, typename __F_Allocator>
    F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const boost::container::basic_string<__F_Char, __F_CharTraits, __F_Allocator>& Value) noexcept
    {
        auto UTF8Value = ToString(Value);
        
        auto UTF8ValueSize = UTF8Value.size();
        View << UTF8ValueSize;

        auto UTF8Bytes = View.AccessSpan<F_Char>(UTF8ValueSize);
        
        memcpy(
            UTF8Bytes.data(),
            UTF8Value.data(),
            UTF8ValueSize * sizeof(F_Char)
        );
        return F_FeedbackStatus::MakeSucceeded();
    }
    template<typename __F_Item, Sz __Size>
    F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const boost::array<__F_Item, __Size>& Value) noexcept
    {
        if constexpr (IsShallowReadWriteType<__F_Item>())
        {
            auto Chars = View.AccessSpan<__F_Item>(__Size);
            memcpy(
                Chars.data(),
                Value.data(),
                __Size * sizeof(__F_Item)
            );
            return F_FeedbackStatus::MakeSucceeded();
        }
        for (const auto& Item : Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Item);
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    template<typename __F>
    F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const boost::optional<__F>& Value) noexcept
    {
        ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.has_value());
        if (Value)
        {
            return (View << *Value);
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    template<typename __F_Item>
    F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const boost::span<__F_Item>& Value) noexcept
    {
        auto Size = Value.size();
        ABYTEK_FEEDBACK_STATUS_CHECK(View << Size);
        
        if constexpr (IsShallowReadWriteType<__F_Item>())
        {
            auto Chars = View.AccessSpan<__F_Item>(Size);
            memcpy(
                Chars.data(),
                Value.data(),
                Size * sizeof(__F_Item)
            );
            return F_FeedbackStatus::MakeSucceeded();
        }

        for (const auto& Item : Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Item);
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    template<typename __F_Item, typename __F_Allocator>
    F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const boost::container::vector<__F_Item, __F_Allocator>& Value) noexcept
    {
        auto Size = Value.size();
        ABYTEK_FEEDBACK_STATUS_CHECK(View << Size);
        
        if constexpr (IsShallowReadWriteType<__F_Item>())
        {
            auto Chars = View.AccessSpan<__F_Item>(Size);
            memcpy(
                Chars.data(),
                Value.data(),
                Size * sizeof(__F_Item)
            );
            return F_FeedbackStatus::MakeSucceeded();
        }

        for (const auto& Item : Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Item);
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    template<typename __F_Item, typename __F_Allocator>
    F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const boost::container::devector<__F_Item, __F_Allocator>& Value) noexcept
    {
        auto Size = Value.size();
        ABYTEK_FEEDBACK_STATUS_CHECK(View << Size);
        
        for (const auto& Item : Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Item);
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    template<typename __F_Item, Sz __StaticSize, typename __F_Allocator>
    F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const boost::container::small_vector<__F_Item, __StaticSize, __F_Allocator>& Value) noexcept
    {
        auto Size = Value.size();
        ABYTEK_FEEDBACK_STATUS_CHECK(View << Size);
        
        if constexpr (IsShallowReadWriteType<__F_Item>())
        {
            auto Chars = View.AccessSpan<__F_Item>(Size);
            memcpy(
                Chars.data(),
                Value.data(),
                Size * sizeof(__F_Item)
            );
            return F_FeedbackStatus::MakeSucceeded();
        }

        for (const auto& Item : Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Item);
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    template<typename __F_Item, typename __F_Allocator>
    F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const boost::container::stable_vector<__F_Item, __F_Allocator>& Value) noexcept
    {
        auto Size = Value.size();
        ABYTEK_FEEDBACK_STATUS_CHECK(View << Size);
        
        if constexpr (IsShallowReadWriteType<__F_Item>())
        {
            auto Chars = View.AccessSpan<__F_Item>(Size);
            memcpy(
                Chars.data(),
                Value.data(),
                Size * sizeof(__F_Item)
            );
            return F_FeedbackStatus::MakeSucceeded();
        }

        for (const auto& Item : Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Item);
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    template<typename __F_Item, Sz __StaticSize, typename __F_Options>
    F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const boost::container::static_vector<__F_Item, __StaticSize, __F_Options>& Value) noexcept
    {
        auto Size = Value.size();
        ABYTEK_FEEDBACK_STATUS_CHECK(View << Size);
        
        if constexpr (IsShallowReadWriteType<__F_Item>())
        {
            auto Chars = View.AccessSpan<__F_Item>(Size);
            memcpy(
                Chars.data(),
                Value.data(),
                Size * sizeof(__F_Item)
            );
            return F_FeedbackStatus::MakeSucceeded();
        }

        for (const auto& Item : Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Item);
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    template<typename __F_Item, typename __F_Allocator>
    F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const boost::container::deque<__F_Item, __F_Allocator>& Value) noexcept
    {
        auto Size = Value.size();
        ABYTEK_FEEDBACK_STATUS_CHECK(View << Size);
        
        for (const auto& Item : Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Item);
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    template<typename __F_Item, typename __F_Allocator>
    F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const boost::container::list<__F_Item, __F_Allocator>& Value) noexcept
    {
        auto Size = Value.size();
        ABYTEK_FEEDBACK_STATUS_CHECK(View << Size);

        for (const auto& Item : Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Item);
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    template<typename __F_Item, typename __F_Allocator>
    F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const boost::container::slist<__F_Item, __F_Allocator>& Value) noexcept
    {
        auto Size = Value.size();
        ABYTEK_FEEDBACK_STATUS_CHECK(View << Size);

        for (const auto& Item : Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Item);
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    template<typename __F_Item, typename __F_Compare, typename __F_Allocator, typename __F_Options>
    F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const boost::container::set<__F_Item, __F_Compare, __F_Allocator, __F_Options>& Value) noexcept
    {
        auto Size = Value.size();
        ABYTEK_FEEDBACK_STATUS_CHECK(View << Size);
        
        for (const auto& Item : Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Item);
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    template<typename __F_Key, typename __F_Value, typename __F_Compare, typename __F_Allocator, typename __F_Options>
    F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const boost::container::map<__F_Key, __F_Value, __F_Compare, __F_Allocator, __F_Options>& Value) noexcept
    {
        auto Size = Value.size();
        ABYTEK_FEEDBACK_STATUS_CHECK(View << Size);
        
        for (const auto& Pair : Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Pair.first);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Pair.second);
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    template<typename __F_Key, typename __F_Value, typename __F_Compare, typename __F_Allocator, typename __F_Options>
    F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const boost::container::multimap<__F_Key, __F_Value, __F_Compare, __F_Allocator, __F_Options>& Value) noexcept
    {
        auto Size = Value.size();
        ABYTEK_FEEDBACK_STATUS_CHECK(View << Size);
        
        for (const auto& Pair : Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Pair.first);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Pair.second);
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    template<typename __F_Item, typename __F_Hash, typename __F_Predicate, typename __F_Allocator>
    F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const boost::unordered_set<__F_Item, __F_Hash, __F_Predicate, __F_Allocator>& Value) noexcept
    {
        auto Size = Value.size();
        ABYTEK_FEEDBACK_STATUS_CHECK(View << Size);
        
        for (const auto& Item : Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Item);
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    template<typename __F_Key, typename __F_Value, typename __F_Hash, typename __F_Predicate, typename __F_Allocator>
    F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const boost::unordered_map<__F_Key, __F_Value, __F_Hash, __F_Predicate, __F_Allocator>& Value) noexcept
    {
        auto Size = Value.size();
        ABYTEK_FEEDBACK_STATUS_CHECK(View << Size);
        
        for (const auto& Pair : Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Pair.first);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Pair.second);
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    template<typename __F_Key, typename __F_Value, typename __F_Hash, typename __F_Predicate, typename __F_Allocator>
    F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const boost::unordered_multimap<__F_Key, __F_Value, __F_Hash, __F_Predicate, __F_Allocator>& Value) noexcept
    {
        auto Size = Value.size();
        ABYTEK_FEEDBACK_STATUS_CHECK(View << Size);
        
        for (const auto& Pair : Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Pair.first);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Pair.second);
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
}