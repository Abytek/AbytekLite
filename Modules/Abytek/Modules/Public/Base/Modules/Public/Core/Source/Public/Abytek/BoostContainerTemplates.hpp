/**
 * @file BoostContainerTemplates.hpp
 * @brief Template definitions for Boost-based container types used throughout the engine
 * 
 * This file defines template aliases for various container types using Boost implementations.
 * It includes string views, arrays, spans, optionals, functions, and various container templates.
 */
#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"
#include "Abytek/LogStream.hpp"


namespace Abytek
{
    using F_Any = boost::any;
    template<typename __F>
    const std::remove_reference_t<std::remove_const_t<__F>>& AnyCast(const F_Any& Value)
    {
        return boost::any_cast<const std::remove_reference_t<std::remove_const_t<__F>>&>(Value);
    }
    template<typename __F>
    std::remove_reference_t<std::remove_const_t<__F>>& AnyCast(F_Any& Value)
    {
        return boost::any_cast<std::remove_reference_t<std::remove_const_t<__F>>&>(Value);
    }
    
    /**
     * @brief Template for string view types
     * @tparam __F_Char Character type for the string view
     */
    template<typename __F_Char>
    using TF_StringView = std::basic_string_view<
        __F_Char,
        std::char_traits<__F_Char>
    >;
    /// @brief Default string view type using F_Char
    using F_StringView = TF_StringView<F_Char>;
    /// @brief Text view type using F_TextChar
    using F_TextView = TF_StringView<F_TextChar>;

    /**
     * @brief Hashes a string view
     * @tparam __F_Char Character type for the string view
     * @param View String view to hash
     * @return Hash value as U64
     */
    template<typename __F_Char = F_Char>
    constexpr U64 HashStringView(TF_StringView<__F_Char> View)
    {
        U64 Result = 0xcbf29ce484222325LL;

        for(const auto C : View) 
        {
            Result = (Result ^ U64(C)) * 0x100000001b3LL;
        }

        return Result;
    }
    /**
     * @brief Hashes a null-terminated string
     * @tparam __F_Char Character type for the string
     * @param View Null-terminated string to hash
     * @return Hash value as U64
     */
    template<typename __F_Char = F_Char>
    constexpr U64 HashStringView(const __F_Char* View)
    {
        U64 Result = 0xcbf29ce484222325LL;

        if (View)
        {
            const __F_Char* CP = View;
            while(*CP != 0)
            {
                __F_Char C = *CP;
                Result = (Result ^ U64(C)) * 0x100000001b3LL;
                ++CP;
            }
        }

        return Result;
    }
    
    /**
     * @brief Fixed-size array template using boost::array
     * @tparam __F_Item Type of items in the array
     * @tparam __Size Size of the array
     */
    template<typename __F_Item, Sz __Size>
    using TF_Array = boost::array<__F_Item, __Size>;
    /**
     * @brief Span template using boost::span
     * @tparam __F_Item Type of items in the span
     */
    template<typename __F_Item>
    using TF_Span = boost::span<__F_Item>;

    /**
     * @brief Creates a span from an initializer list
     * @tparam __F_Item Type of items in the initializer list
     * @param Items Initializer list to create span from
     * @return Span covering the items
     */
    template<typename __F_Item>
    ABYTEK_FORCE_INLINE TF_Span<__F_Item> MakeSpan(const std::initializer_list<__F_Item>& Items)
    {
        return {
            (__F_Item*)Items.begin(),
            Items.size()
        };
    }
    /**
     * @brief Creates a single-element span
     * @tparam __F_Item Type of the item
     * @param Item Item to create span from
     * @return Span covering the single item
     */
    template<typename __F_Item>
    ABYTEK_FORCE_INLINE TF_Span<__F_Item> MakeSpan(const __F_Item& Item)
    {
        return {
            (__F_Item*)&Item,
            1
        };
    }
    
    /**
     * @brief Optional value template using boost::optional
     * @tparam __F Type of the optional value
     */
    template<typename __F>
    using TF_Optional = boost::optional<__F>;
    
    /**
     * @brief Function template using boost::function
     * @tparam __F_Signature Function signature
     */
    template<typename __F_Signature>
    using TF_Function = boost::function<__F_Signature>;
    
    template<typename __F_Iterator>
    using TF_IteratorRange = boost::iterator_range<__F_Iterator>;
    
    /**
     * @brief Stream operator for logging boost::container::basic_string
     * @tparam __F_Char Character type
     * @tparam __F_CharTraits Character traits
     * @tparam __F_Allocator Allocator type
     * @param LogStream Log stream to output to
     * @param Value String to log
     * @return Reference to the log stream
     */
    template<typename __F_Char, typename __F_CharTraits, typename __F_Allocator>
    inline A_LogStream& operator << (
        A_LogStream& LogStream,
        const boost::container::basic_string<
            __F_Char,
            __F_CharTraits,
            __F_Allocator
        >& Value
    )
    {
        return (LogStream << Value.c_str());
    }    
}

/**
 * @brief Container template definitions using Boost containers
 */
namespace Abytek::ContainerTemplates
{
    /**
     * @brief Basic string template using boost::container::basic_string
     * @tparam __F_Char Character type
     * @tparam __F_Allocator Allocator type
     */
    template<typename __F_Char, typename __F_Allocator = boost::default_allocator<__F_Char>>
    using TF_String = boost::container::basic_string<
        __F_Char,
        std::char_traits<__F_Char>,
        __F_Allocator
    >;
    /// @brief Default string type using F_Char
    using F_String = TF_String<F_Char>;
    /// @brief Text type using F_TextChar
    using F_Text = TF_String<F_TextChar>;
    
    /**
     * @brief Vector template using boost::container::vector
     * @tparam __F_Item Item type
     * @tparam __F_Allocator Allocator type
     */
    template<typename __F_Item, typename __F_Allocator = boost::default_allocator<__F_Item>>
    using TF_Vector = boost::container::vector<__F_Item, __F_Allocator>;
    /**
     * @brief Devector template using boost::container::devector
     * @tparam __F_Item Item type
     * @tparam __F_Allocator Allocator type
     */
    template<typename __F_Item, typename __F_Allocator = boost::default_allocator<__F_Item>>
    using TF_DeVector = boost::container::devector<__F_Item, __F_Allocator>;
    /**
     * @brief Small vector template using boost::container::small_vector
     * @tparam __F_Item Item type
     * @tparam __StaticSize Static buffer size
     * @tparam __F_Allocator Allocator type
     */
    template<typename __F_Item, Sz __StaticSize, typename __F_Allocator = boost::default_allocator<__F_Item>>
    using TF_SmallVector = boost::container::small_vector<__F_Item, __StaticSize, __F_Allocator>;
    /**
     * @brief Stable vector template using boost::container::stable_vector
     * @tparam __F_Item Item type
     * @tparam __F_Allocator Allocator type
     */
    template<typename __F_Item, typename __F_Allocator = boost::default_allocator<__F_Item>>
    using TF_StableVector = boost::container::stable_vector<__F_Item, __F_Allocator>;
    /**
     * @brief Static vector template using boost::container::static_vector
     * @tparam __F_Item Item type
     * @tparam __StaticSize Static buffer size
     * @tparam __F_Options Options type
     */
    template<typename __F_Item, Sz __StaticSize, typename __F_Options = void>
    using TF_StaticVector = boost::container::static_vector<__F_Item, __StaticSize, __F_Options>;
    /**
     * @brief Deque template using boost::container::deque
     * @tparam __F_Item Item type
     * @tparam __F_Allocator Allocator type
     */
    template<typename __F_Item, typename __F_Allocator = boost::default_allocator<__F_Item>>
    using TF_Deque = boost::container::deque<__F_Item, __F_Allocator, void>;
    /**
     * @brief List template using boost::container::list
     * @tparam __F_Item Item type
     * @tparam __F_Allocator Allocator type
     */
    template<typename __F_Item, typename __F_Allocator = boost::default_allocator<__F_Item>>
    using TF_List = boost::container::list<__F_Item, __F_Allocator>;
    /**
     * @brief Singly linked list template using boost::container::slist
     * @tparam __F_Item Item type
     * @tparam __F_Allocator Allocator type
     */
    template<typename __F_Item, typename __F_Allocator = boost::default_allocator<__F_Item>>
    using TF_SinglyList = boost::container::slist<__F_Item, __F_Allocator>;
    /**
     * @brief Set template using boost::container::set
     * @tparam __F_Item Item type
     * @tparam __F_Compare Comparison function type
     * @tparam __F_Allocator Allocator type
     */
    template<
        typename __F_Item,
        typename __F_Compare = std::less<__F_Item>,
        typename __F_Allocator = boost::default_allocator<__F_Item>
    >
    using TF_Set = boost::container::set<
        __F_Item,
        __F_Compare,
        __F_Allocator
    >;
    /**
     * @brief Map template using boost::container::map
     * @tparam __F_Key Key type
     * @tparam __F_Value Value type
     * @tparam __F_Compare Key comparison function type
     * @tparam __F_Allocator Allocator type
     */
    template<
        typename __F_Key,
        typename __F_Value,
        typename __F_Compare = std::less<__F_Key>,
        typename __F_Allocator = boost::default_allocator<std::pair<const __F_Key, __F_Value>>
    >
    using TF_Map = boost::container::map<
        __F_Key,
        __F_Value,
        __F_Compare,
        __F_Allocator
    >;
    /**
     * @brief MultiMap template using boost::container::multimap
     * @tparam __F_Key Key type
     * @tparam __F_Value Value type
     * @tparam __F_Compare Key comparison function type
     * @tparam __F_Allocator Allocator type
     */
    template<
        typename __F_Key,
        typename __F_Value,
        typename __F_Compare = std::less<__F_Key>,
        typename __F_Allocator = boost::default_allocator<std::pair<const __F_Key, __F_Value>>
    >
    using TF_MultiMap = boost::container::multimap<
        __F_Key,
        __F_Value,
        __F_Compare,
        __F_Allocator
    >;
    /**
     * @brief Unordered set template using boost::unordered_set
     * @tparam __F_Item Item type
     * @tparam __F_Hash Hash function type
     * @tparam __F_Predicate Equality predicate type
     * @tparam __F_Allocator Allocator type
     */
    template<
        typename __F_Item,
        typename __F_Hash = boost::hash<__F_Item>,
        typename __F_Predicate = std::equal_to<__F_Item>,
        typename __F_Allocator  = boost::default_allocator<__F_Item>
    >
    using TF_UnorderedSet = boost::unordered_set<
        __F_Item,
        __F_Hash,
        __F_Predicate,
        __F_Allocator
    >;
    /**
     * @brief Unordered map template using boost::unordered_map
     * @tparam __F_Key Key type
     * @tparam __F_Value Value type
     * @tparam __F_Hash Hash function type
     * @tparam __F_Predicate Equality predicate type
     * @tparam __F_Allocator Allocator type
     */
    template<
        typename __F_Key,
        typename __F_Value,
        typename __F_Hash = boost::hash<__F_Key>,
        typename __F_Predicate = std::equal_to<__F_Key>,
        typename __F_Allocator  = boost::default_allocator<std::pair<const __F_Key, __F_Value>>
    >
    using TF_UnorderedMap = boost::unordered_map<
        __F_Key,
        __F_Value,
        __F_Hash,
        __F_Predicate,
        __F_Allocator
    >;
    /**
     * @brief Unordered multimap template using boost::unordered_multimap
     * @tparam __F_Key Key type
     * @tparam __F_Value Value type
     * @tparam __F_Hash Hash function type
     * @tparam __F_Predicate Equality predicate type
     * @tparam __F_Allocator Allocator type
     */
    template<
        typename __F_Key,
        typename __F_Value,
        typename __F_Hash = boost::hash<__F_Key>,
        typename __F_Predicate = std::equal_to<__F_Key>,
        typename __F_Allocator  = boost::default_allocator<std::pair<const __F_Key, __F_Value>>
    >
    using TF_UnorderedMultiMap = boost::unordered_multimap<
        __F_Key,
        __F_Value,
        __F_Hash,
        __F_Predicate,
        __F_Allocator
    >;
    /**
     * @brief Flat set template using boost::container::flat_set
     * @tparam __F_Item Item type
     * @tparam __F_Compare Comparison function type
     * @tparam __F_Allocator Allocator type
     */
    template<
        typename __F_Item,
        typename __F_Compare = std::less<__F_Item>,
        typename __F_Allocator = boost::default_allocator<__F_Item>
    >
    using TF_FlatSet = boost::container::flat_set<
        __F_Item,
        __F_Compare,
        __F_Allocator
    >;
    /**
     * @brief Flat map template using boost::container::flat_map
     * @tparam __F_Key Key type
     * @tparam __F_Value Value type
     * @tparam __F_Compare Key comparison function type
     * @tparam __F_Allocator Allocator type
     */
    template<
        typename __F_Key,
        typename __F_Value,
        typename __F_Compare = std::less<__F_Key>,
        typename __F_Allocator = boost::default_allocator<std::pair<const __F_Key, __F_Value>>
    >
    using TF_FlatMap = boost::container::flat_map<
        __F_Key,
        __F_Value,
        __F_Compare,
        __F_Allocator
    >;
    /**
     * @brief Flat multimap template using boost::container::flat_multimap
     * @tparam __F_Key Key type
     * @tparam __F_Value Value type
     * @tparam __F_Compare Key comparison function type
     * @tparam __F_Allocator Allocator type
     */
    template<
        typename __F_Key,
        typename __F_Value,
        typename __F_Compare = std::less<__F_Key>,
        typename __F_Allocator = boost::default_allocator<std::pair<const __F_Key, __F_Value>>
    >
    using TF_FlatMultiMap = boost::container::flat_multimap<
        __F_Key,
        __F_Value,
        __F_Compare,
        __F_Allocator
    >;

    /**
     * @brief Variant template using boost::variant2::variant
     * @tparam __Fs Data types
     */
    template<typename... __Fs>
    using TF_Variant = boost::variant2::variant<__Fs...>;
}