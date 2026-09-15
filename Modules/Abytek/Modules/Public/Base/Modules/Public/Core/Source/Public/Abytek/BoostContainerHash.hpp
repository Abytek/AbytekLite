#pragma once

#include "Abytek/HashBase.hpp"


namespace boost
{
    template<typename __F_Item, Sz __Size>
    struct hash<array<__F_Item, __Size>>
    {
        size_t operator()(const array<__F_Item, __Size>& X) const
        {
            size_t Result = 0;
            hash_range(Result, X.begin(), X.end());
            return Result;
        }
    };
    template<typename __F_Item>
    struct hash<span<__F_Item>>
    {
        size_t operator()(const span<__F_Item>& X) const
        {
            size_t Result = 0;
            hash_range(Result, X.begin(), X.end());
            return Result;
        }
    };
    template<typename __F>
    struct hash<optional<__F>>
    {
        size_t operator()(const optional<__F>& X) const
        {
            size_t Result = 0;
            if (X.has_value())
            {
                hash_combine(Result, 1U);
                hash_combine(Result, X.value());
            }
            else
            {
                hash_combine(Result, 0U);
            }
            return Result;
        }
    };
    
    template<typename __F_Item, typename __F_Allocator>
    struct hash<container::vector<__F_Item, __F_Allocator>>
    {
        size_t operator()(const container::vector<__F_Item, __F_Allocator>& X) const
        {
            size_t Result = 0;
            hash_range(Result, X.begin(), X.end());
            return Result;
        }
    };
    template<typename __F_Item, typename __F_Allocator>
    struct hash<container::devector<__F_Item, __F_Allocator>>
    {
        size_t operator()(const container::devector<__F_Item, __F_Allocator>& X) const
        {
            size_t Result = 0;
            hash_range(Result, X.begin(), X.end());
            return Result;
        }
    };
    template<typename __F_Item, Sz __StaticSize, typename __F_Allocator>
    struct hash<container::small_vector<__F_Item, __StaticSize, __F_Allocator>>
    {
        size_t operator()(const container::small_vector<__F_Item, __StaticSize, __F_Allocator>& X) const
        {
            size_t Result = 0;
            hash_range(Result, X.begin(), X.end());
            return Result;
        }
    };
    template<typename __F_Item, typename __F_Allocator>
    struct hash<container::stable_vector<__F_Item, __F_Allocator>>
    {
        size_t operator()(const container::stable_vector<__F_Item, __F_Allocator>& X) const
        {
            size_t Result = 0;
            hash_range(Result, X.begin(), X.end());
            return Result;
        }
    };
    template<typename __F_Item, Sz __StaticSize, typename __F_Options>
    struct hash<container::static_vector<__F_Item, __StaticSize, __F_Options>>
    {
        size_t operator()(const container::static_vector<__F_Item, __StaticSize, __F_Options>& X) const
        {
            size_t Result = 0;
            hash_range(Result, X.begin(), X.end());
            return Result;
        }
    };
    template<typename __F_Item, typename __F_Allocator>
    struct hash<container::deque<__F_Item, __F_Allocator>>
    {
        size_t operator()(const container::deque<__F_Item, __F_Allocator>& X) const
        {
            size_t Result = 0;
            hash_range(Result, X.begin(), X.end());
            return Result;
        }
    };
    template<typename __F_Item, typename __F_Allocator>
    struct hash<container::list<__F_Item, __F_Allocator>>
    {
        size_t operator()(const container::list<__F_Item, __F_Allocator>& X) const
        {
            size_t Result = 0;
            hash_range(Result, X.begin(), X.end());
            return Result;
        }
    };
    template<typename __F_Item, typename __F_Allocator>
    struct hash<container::slist<__F_Item, __F_Allocator>>
    {
        size_t operator()(const container::slist<__F_Item, __F_Allocator>& X) const
        {
            size_t Result = 0;
            hash_range(Result, X.begin(), X.end());
            return Result;
        }
    };
    template<typename __F_Item, typename __F_Compare, typename __F_Allocator>
    struct hash<container::set<__F_Item, __F_Compare, __F_Allocator>>
    {
        size_t operator()(const container::set<__F_Item, __F_Compare, __F_Allocator>& X) const
        {
            size_t Result = 0;
            hash_range(Result, X.begin(), X.end());
            return Result;
        }
    };
    template<typename __F_Key, typename __F_Value, typename __F_Compare, typename __F_Allocator>
    struct hash<container::map<__F_Key, __F_Value, __F_Compare, __F_Allocator>>
    {
        size_t operator()(const container::map<__F_Key, __F_Value, __F_Compare, __F_Allocator>& X) const
        {
            size_t Result = 0;
            hash_range(Result, X.begin(), X.end());
            return Result;
        }
    };
    template<typename __F_Key, typename __F_Value, typename __F_Compare, typename __F_Allocator>
    struct hash<container::multimap<__F_Key, __F_Value, __F_Compare, __F_Allocator>>
    {
        size_t operator()(const container::multimap<__F_Key, __F_Value, __F_Compare, __F_Allocator>& X) const
        {
            size_t Result = 0;
            hash_range(Result, X.begin(), X.end());
            return Result;
        }
    };
    template<typename __F_Item, typename __F_Compare, typename __F_Allocator>
    struct hash<container::flat_set<__F_Item, __F_Compare, __F_Allocator>>
    {
        size_t operator()(const container::flat_set<__F_Item, __F_Compare, __F_Allocator>& X) const
        {
            size_t Result = 0;
            hash_range(Result, X.begin(), X.end());
            return Result;
        }
    };
    template<typename __F_Key, typename __F_Value, typename __F_Compare, typename __F_Allocator>
    struct hash<container::flat_map<__F_Key, __F_Value, __F_Compare, __F_Allocator>>
    {
        size_t operator()(const container::flat_map<__F_Key, __F_Value, __F_Compare, __F_Allocator>& X) const
        {
            size_t Result = 0;
            hash_range(Result, X.begin(), X.end());
            return Result;
        }
    };
    template<typename __F_Key, typename __F_Value, typename __F_Compare, typename __F_Allocator>
    struct hash<container::flat_multimap<__F_Key, __F_Value, __F_Compare, __F_Allocator>>
    {
        size_t operator()(const container::flat_multimap<__F_Key, __F_Value, __F_Compare, __F_Allocator>& X) const
        {
            size_t Result = 0;
            hash_range(Result, X.begin(), X.end());
            return Result;
        }
    };
}