#pragma once

#include "Abytek/TypeVersion.hpp"


namespace Abytek
{
    template<typename __F_HashCode, typename __F_Index, typename __F_Allocator>
    struct TF_TypeVersionHashCode<ContainerTemplates::TF_HashTable<__F_HashCode, __F_Index, __F_Allocator>>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            Sz Result = 0;
            Result = HashCombineU64(Result, HashStringView("Abytek::ContainerTemplates::TF_HashTable"));
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F_HashCode>());
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F_Index>());
            return Result;
        }
    };
    template<typename __F_Item, typename __F_Allocator>
    struct TF_TypeVersionHashCode<ContainerTemplates::TF_RingBuffer<__F_Item, __F_Allocator>>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            Sz Result = 0;
            Result = HashCombineU64(Result, HashStringView("Abytek::ContainerTemplates::TF_RingBuffer"));
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F_Item>());
            return Result;
        }
    };
    template<typename __F_Item, typename __F_Allocator>
    struct TF_TypeVersionHashCode<ContainerTemplates::TF_ConcurrentRingBuffer<__F_Item, __F_Allocator>>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            Sz Result = 0;
            Result = HashCombineU64(Result, HashStringView("Abytek::ContainerTemplates::TF_ConcurrentRingBuffer"));
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F_Item>());
            return Result;
        }
    };
    template<typename __F_Item, typename __F_Allocator>
    struct TF_TypeVersionHashCode<ContainerTemplates::TF_FixedPOQueue<__F_Item, __F_Allocator>>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            Sz Result = 0;
            Result = HashCombineU64(Result, HashStringView("Abytek::ContainerTemplates::TF_FixedPOQueue"));
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F_Item>());
            return Result;
        }
    };
    template<typename __F_Item, typename __F_Allocator>
    struct TF_TypeVersionHashCode<ContainerTemplates::TF_ConcurrentFixedPOQueue<__F_Item, __F_Allocator>>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            Sz Result = 0;
            Result = HashCombineU64(Result, HashStringView("Abytek::ContainerTemplates::TF_ConcurrentFixedPOQueue"));
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F_Item>());
            return Result;
        }
    };
    template<typename __F_Item, typename __F_Allocator>
    struct TF_TypeVersionHashCode<ContainerTemplates::TF_FixedPOStack<__F_Item, __F_Allocator>>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            Sz Result = 0;
            Result = HashCombineU64(Result, HashStringView("Abytek::ContainerTemplates::TF_FixedPOStack"));
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F_Item>());
            return Result;
        }
    };
    template<typename __F_Item, typename __F_Allocator>
    struct TF_TypeVersionHashCode<ContainerTemplates::TF_ConcurrentFixedPOStack<__F_Item, __F_Allocator>>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            Sz Result = 0;
            Result = HashCombineU64(Result, HashStringView("Abytek::ContainerTemplates::TF_ConcurrentFixedPOStack"));
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F_Item>());
            return Result;
        }
    };
    template<typename __F_Allocator, typename __F_PayloadIndex>
    struct TF_TypeVersionHashCode<ContainerTemplates::TF_FlatIndirectMap<__F_Allocator, __F_PayloadIndex>>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            Sz Result = 0;
            Result = HashCombineU64(Result, HashStringView("Abytek::ContainerTemplates::TF_FlatIndirectMap"));
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F_PayloadIndex>());
            return Result;
        }
    };
    template<class __F_StringDatabase>
    struct TF_TypeVersionHashCode<TF_StringId<__F_StringDatabase>>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            Sz Result = 0;
            Result = HashCombineU64(Result, HashStringView("Abytek::TF_StringId"));
            Result = HashCombineU64(Result, GetTypeVersionHashCode<typename __F_StringDatabase::F_Char>());
            return Result;
        }
    };
    template<typename __F_Item, class __F_Container>
    struct TF_TypeVersionHashCode<ContainerTemplates::TF_Queue<__F_Item, __F_Container>>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            Sz Result = 0;
            Result = HashCombineU64(Result, HashStringView("Abytek::ContainerTemplates::TF_Queue"));
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F_Item>());
            return Result;
        }
    };
    template<typename __F_Item, class __F_Container>
    struct TF_TypeVersionHashCode<ContainerTemplates::TF_Stack<__F_Item, __F_Container>>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            Sz Result = 0;
            Result = HashCombineU64(Result, HashStringView("Abytek::ContainerTemplates::TF_Stack"));
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F_Item>());
            return Result;
        }
    };
    template<typename __F_Item, class __F_Container>
    struct TF_TypeVersionHashCode<ContainerTemplates::TF_ConcurrentQueue<__F_Item, __F_Container>>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            Sz Result = 0;
            Result = HashCombineU64(Result, HashStringView("Abytek::ContainerTemplates::TF_ConcurrentQueue"));
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F_Item>());
            return Result;
        }
    };
    template<typename __F_Item, class __F_Container>
    struct TF_TypeVersionHashCode<ContainerTemplates::TF_ConcurrentStack<__F_Item, __F_Container>>
    {
        static constexpr F_TypeVersionHashCode Get() noexcept
        {
            Sz Result = 0;
            Result = HashCombineU64(Result, HashStringView("Abytek::ContainerTemplates::TF_ConcurrentStack"));
            Result = HashCombineU64(Result, GetTypeVersionHashCode<__F_Item>());
            return Result;
        }
    };
}