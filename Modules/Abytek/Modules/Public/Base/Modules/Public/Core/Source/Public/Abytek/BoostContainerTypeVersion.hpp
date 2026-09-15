#pragma once

#include "Abytek/TypeVersion.hpp"


namespace Abytek
{
    template<typename __F_Char, typename __F_CharTraits, typename __F_Allocator>
    struct TF_TypeVersionHashCode<boost::container::basic_string<__F_Char, __F_CharTraits, __F_Allocator>>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            Sz Result = 0;
            Result = HashCombineU64(Result, HashStringView("boost::container::basic_string"));
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F_Char>());
            return Result;
        }
    };
    template<typename __F_Char, typename __F_CharTraits>
    struct TF_TypeVersionHashCode<std::basic_string_view<__F_Char, __F_CharTraits>>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            Sz Result = 0;
            Result = HashCombineU64(Result, HashStringView("std::basic_string_view"));
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F_Char>());
            return Result;
        }
    };
    template<typename __F_Item, Sz __Size>
    struct TF_TypeVersionHashCode<boost::array<__F_Item, __Size>>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            Sz Result = 0;
            Result = HashCombineU64(Result, HashStringView("boost::array"));
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F_Item>());
            Result = HashCombineU64(Result, __Size);
            Result = HashCombineU64(Result, __Size);
            return Result;
        }
    };
    template<typename __F_Item>
    struct TF_TypeVersionHashCode<boost::span<__F_Item>>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            Sz Result = 0;
            Result = HashCombineU64(Result, HashStringView("boost::span"));
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F_Item>());
            return Result;
        }
    };
    template<typename __F>
    struct TF_TypeVersionHashCode<boost::optional<__F>>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            Sz Result = 0;
            Result = HashCombineU64(Result, HashStringView("boost::optional"));
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F>());
            return Result;
        }
    };
    template<typename __F>
    struct TF_TypeVersionHashCode<boost::function<__F>>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            Sz Result = 0;
            Result = HashCombineU64(Result, HashStringView("boost::function"));
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F>());
            return Result;
        }
    };
    template<typename __F_Item, typename __F_Allocator>
    struct TF_TypeVersionHashCode<boost::container::vector<__F_Item, __F_Allocator>>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            Sz Result = 0;
            Result = HashCombineU64(Result, HashStringView("boost::container::vector"));
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F_Item>());
            return Result;
        }
    };
    template<typename __F_Item, typename __F_Allocator>
    struct TF_TypeVersionHashCode<boost::container::devector<__F_Item, __F_Allocator>>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            Sz Result = 0;
            Result = HashCombineU64(Result, HashStringView("boost::container::devector"));
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F_Item>());
            return Result;
        }
    };
    template<typename __F_Item, Sz __StaticSize, typename __F_Allocator>
    struct TF_TypeVersionHashCode<boost::container::small_vector<__F_Item, __StaticSize, __F_Allocator>>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            Sz Result = 0;
            Result = HashCombineU64(Result, HashStringView("boost::container::small_vector"));
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F_Item>());
            Result = HashCombineU64(Result, __StaticSize);
            return Result;
        }
    };
    template<typename __F_Item, typename __F_Allocator>
    struct TF_TypeVersionHashCode<boost::container::stable_vector<__F_Item, __F_Allocator>>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            Sz Result = 0;
            Result = HashCombineU64(Result, HashStringView("boost::container::stable_vector"));
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F_Item>());
            return Result;
        }
    };
    template<typename __F_Item, Sz __StaticSize, typename __F_Options>
    struct TF_TypeVersionHashCode<boost::container::static_vector<__F_Item, __StaticSize, __F_Options>>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            Sz Result = 0;
            Result = HashCombineU64(Result, HashStringView("boost::container::static_vector"));
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F_Item>());
            return Result;
        }
    };
    template<typename __F_Item, typename __F_Allocator, typename __F_Options>
    struct TF_TypeVersionHashCode<boost::container::deque<__F_Item, __F_Allocator, __F_Options>>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            Sz Result = 0;
            Result = HashCombineU64(Result, HashStringView("boost::container::deque"));
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F_Item>());
            return Result;
        }
    };
    template<typename __F_Item, typename __F_Allocator>
    struct TF_TypeVersionHashCode<boost::container::list<__F_Item, __F_Allocator>>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            Sz Result = 0;
            Result = HashCombineU64(Result, HashStringView("boost::container::list"));
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F_Item>());
            return Result;
        }
    };
    template<typename __F_Item, typename __F_Allocator>
    struct TF_TypeVersionHashCode<boost::container::slist<__F_Item, __F_Allocator>>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            Sz Result = 0;
            Result = HashCombineU64(Result, HashStringView("boost::container::slist"));
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F_Item>());
            return Result;
        }
    };
    template<typename __F_Item, typename __F_Compare, typename __F_Allocator, typename __F_Options>
    struct TF_TypeVersionHashCode<boost::container::set<__F_Item, __F_Compare, __F_Allocator, __F_Options>>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            Sz Result = 0;
            Result = HashCombineU64(Result, HashStringView("boost::container::set"));
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F_Item>());
            return Result;
        }
    };
    template<typename __F_Key, typename __F_Value, typename __F_Compare, typename __F_Allocator, typename __F_Options>
    struct TF_TypeVersionHashCode<boost::container::map<__F_Key, __F_Value, __F_Compare, __F_Allocator, __F_Options>>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            Sz Result = 0;
            Result = HashCombineU64(Result, HashStringView("boost::container::map"));
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F_Key>());
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F_Value>());
            return Result;
        }
    };
    template<typename __F_Key, typename __F_Value, typename __F_Compare, typename __F_Allocator, typename __F_Options>
    struct TF_TypeVersionHashCode<boost::container::multimap<__F_Key, __F_Value, __F_Compare, __F_Allocator, __F_Options>>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            Sz Result = 0;
            Result = HashCombineU64(Result, HashStringView("boost::container::multimap"));
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F_Key>());
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F_Value>());
            return Result;
        }
    };
    template<typename __F_Item, typename __F_Hash, typename __F_Predicate, typename __F_Allocator>
    struct TF_TypeVersionHashCode<boost::unordered_set<__F_Item, __F_Hash, __F_Predicate, __F_Allocator>>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            Sz Result = 0;
            Result = HashCombineU64(Result, HashStringView("boost::unordered_set"));
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F_Item>());
            return Result;
        }
    };
    template<typename __F_Key, typename __F_Value, typename __F_Hash, typename __F_Predicate, typename __F_Allocator>
    struct TF_TypeVersionHashCode<boost::unordered_map<__F_Key, __F_Value, __F_Hash, __F_Predicate, __F_Allocator>>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            Sz Result = 0;
            Result = HashCombineU64(Result, HashStringView("boost::container::map"));
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F_Key>());
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F_Value>());
            return Result;
        }
    };
    template<typename __F_Key, typename __F_Value, typename __F_Hash, typename __F_Predicate, typename __F_Allocator>
    struct TF_TypeVersionHashCode<boost::unordered_multimap<__F_Key, __F_Value, __F_Hash, __F_Predicate, __F_Allocator>>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            Sz Result = 0;
            Result = HashCombineU64(Result, HashStringView("boost::unordered_multimap"));
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F_Key>());
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F_Value>());
            return Result;
        }
    };
    template<typename __F_Item, typename __F_Compare, typename __F_Allocator>
    struct TF_TypeVersionHashCode<boost::container::flat_set<__F_Item, __F_Compare, __F_Allocator>>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            Sz Result = 0;
            Result = HashCombineU64(Result, HashStringView("boost::container::flat_set"));
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F_Item>());
            return Result;
        }
    };
    template<typename __F_Key, typename __F_Value, typename __F_Compare, typename __F_Allocator>
    struct TF_TypeVersionHashCode<boost::container::flat_map<__F_Key, __F_Value, __F_Compare, __F_Allocator>>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            Sz Result = 0;
            Result = HashCombineU64(Result, HashStringView("boost::container::flat_map"));
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F_Key>());
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F_Value>());
            return Result;
        }
    };
    template<typename __F_Key, typename __F_Value, typename __F_Compare, typename __F_Allocator>
    struct TF_TypeVersionHashCode<boost::container::flat_multimap<__F_Key, __F_Value, __F_Compare, __F_Allocator>>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            Sz Result = 0;
            Result = HashCombineU64(Result, HashStringView("boost::container::flat_multimap"));
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F_Key>());
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F_Value>());
            return Result;
        }
    };
}