/**
 * @file ContainerTypedefs.inl
 * @brief Type definitions for custom and Boost containers with default allocators
 * 
 * This file defines memory allocation functions and type definitions for various container types
 * including both custom Abytek containers and Boost containers with the default allocator.
 * It also includes BoostContainerTypedefs.inl to provide Boost container type definitions.
 */

#include "Abytek/Base.Core.prerequisites.pch.hpp"
#include "Abytek/BoostContainerTemplates.hpp"
#include "Abytek/StringStream.hpp"
#include "Abytek/HashTable.hpp"
#include "Abytek/RingBuffer.hpp"
#include "Abytek/ConcurrentRingBuffer.hpp"
#include "Abytek/FixedPOQueue.hpp"
#include "Abytek/ConcurrentFixedPOQueue.hpp"
#include "Abytek/FixedPOStack.hpp"
#include "Abytek/ConcurrentFixedPOStack.hpp"
#include "Abytek/FlatIndirectMap.hpp"
#include "Abytek/StringId.hpp"
#include "Abytek/Event.hpp"
#include "Abytek/Queue.hpp"
#include "Abytek/Stack.hpp"
#include "Abytek/ConcurrentQueue.hpp"
#include "Abytek/ConcurrentStack.hpp"
#include "Abytek/LinearAllocationDistributor.hpp"

#ifndef ABYTEK_CONTAINER_TYPEDEFS_NAMESPACE
#define ABYTEK_CONTAINER_TYPEDEFS_NAMESPACE Abytek
#endif

namespace ABYTEK_CONTAINER_TYPEDEFS_NAMESPACE
{
    /**
     * @brief Allocates memory for a single object
     * @tparam __F Type to allocate
     * @return Pointer to allocated memory
     */
    template<typename __F>
    __F* Allocate()
    {
        return TF_DefaultAllocator<__F>().allocate(1);
    }
    
    /**
     * @brief Allocates memory for an array of objects
     * @tparam __F Type to allocate
     * @param Count Number of objects to allocate
     * @return Pointer to allocated memory
     */
    template<typename __F>
    __F* AllocateArray(Sz Count)
    {
        return TF_DefaultAllocator<__F>().allocate(Count);
    }
    
    /**
     * @brief Allocates memory for a fixed-size array of objects
     * @tparam __F Type to allocate
     * @tparam __Count Number of objects to allocate
     * @return Pointer to allocated memory
     */
    template<typename __F, Sz __Count>
    __F* AllocateArray()
    {
        return TF_DefaultAllocator<__F>().allocate(__Count);
    }
    
    /**
     * @brief Deallocates memory for a single object
     * @tparam __F Type to deallocate
     * @param Pointer Pointer to memory to deallocate
     */
    template<typename __F>
    void Deallocate(__F* Pointer)
    {
        TF_DefaultAllocator<__F>().deallocate(Pointer, 1);
    }
    
    /**
     * @brief Deallocates memory for void pointer
     * @param Pointer Pointer to memory to deallocate
     */
    inline void Deallocate(void* Pointer)
    {
        TF_DefaultAllocator<U8>().deallocate((U8*)Pointer, 1);
    }
    
    /**
     * @brief Deallocates memory for const void pointer
     * @param Pointer Pointer to memory to deallocate
     */
    inline void Deallocate(const void* Pointer)
    {
        TF_DefaultAllocator<U8>().deallocate((U8*)Pointer, 1);
    }

    /**
     * @brief Allocates and constructs a new object
     * @tparam __F Type to create
     * @tparam __Fs Constructor argument types
     * @param Args Constructor arguments
     * @return Pointer to new object
     */
    template<typename __F, typename... __Fs>
    __F* New(__Fs&&... Args)
    {
        return new(Allocate<__F>()) __F(ABYTEK_FORWARD(Args)...);
    }
    
    /**
     * @brief Allocates and constructs an array of objects
     * @tparam __F Type to create
     * @param Count Number of objects to create
     * @return Pointer to new array
     */
    template<typename __F>
    __F* NewArray(Sz Count)
    {
        return new(AllocateArray<__F>(Count)) __F[Count];
    }
    
    /**
     * @brief Destroys and deallocates an object
     * @tparam __F Type to destroy
     * @param Pointer Pointer to object to destroy
     */
    template<typename __F>
    void Delete(__F* Pointer)
    {
        Pointer->~__F();
        Deallocate(Pointer);
    }
    
    /**
     * @brief Destroys and deallocates an array of objects
     * @tparam __F Type to destroy
     * @param Pointer Pointer to array to destroy
     * @param Count Number of objects in array
     */
    template<typename __F>
    void DeleteArray(__F* Pointer, Sz Count)
    {
        for(Sz Idx = 0; Idx < Count; ++Idx)
        {
            Pointer[Idx].~__F();
        }
        Deallocate(Pointer);
    }
    
    /**
     * @brief Destroys and deallocates a fixed-size array of objects
     * @tparam __F Type to destroy
     * @tparam __Count Number of objects in array
     * @param Pointer Pointer to array to destroy
     */
    template<typename __F, Sz __Count>
    void DeleteArray(__F* Pointer)
    {
        for(Sz Idx = 0; Idx < __Count; ++Idx)
        {
            Pointer[Idx].~__F();
        }
        Deallocate(Pointer);
    }

    /**
     * @brief Hash table type definition with default allocator
     * @tparam __F_HashCode Hash code type
     * @tparam __F_Index Index type
     */
    template<typename __F_HashCode, typename __F_Index>
    using TF_HashTable = Abytek::ContainerTemplates::TF_HashTable<__F_HashCode, __F_Index, TF_DefaultAllocator<U8>>;
    
    /**
     * @brief Ring buffer type definition with default allocator
     * @tparam __F_Item Item type
     */
    template<typename __F_Item>
    using TF_RingBuffer = Abytek::ContainerTemplates::TF_RingBuffer<__F_Item, TF_DefaultAllocator<__F_Item>>;
    
    /**
     * @brief Concurrent ring buffer type definition with default allocator
     * @tparam __F_Item Item type
     */
    template<typename __F_Item>
    using TF_ConcurrentRingBuffer = Abytek::ContainerTemplates::TF_ConcurrentRingBuffer<__F_Item, TF_DefaultAllocator<__F_Item>>;
    
    /**
     * @brief Fixed partially ordered queue type definition with default allocator
     * @tparam __F_Item Item type
     */
    template<typename __F_Item>
    using TF_FixedPOQueue = Abytek::ContainerTemplates::TF_FixedPOQueue<__F_Item, TF_DefaultAllocator<__F_Item>>;
    
    /**
     * @brief Concurrent fixed partially ordered queue type definition with default allocator
     * @tparam __F_Item Item type
     */
    template<typename __F_Item>
    using TF_ConcurrentFixedPOQueue = Abytek::ContainerTemplates::TF_ConcurrentFixedPOQueue<__F_Item, TF_DefaultAllocator<__F_Item>>;
    
    /**
     * @brief Fixed partially ordered stack type definition with default allocator
     * @tparam __F_Item Item type
     */
    template<typename __F_Item>
    using TF_FixedPOStack = Abytek::ContainerTemplates::TF_FixedPOStack<__F_Item, TF_DefaultAllocator<__F_Item>>;
    
    /**
     * @brief Concurrent fixed partially ordered stack type definition with default allocator
     * @tparam __F_Item Item type
     */
    template<typename __F_Item>
    using TF_ConcurrentFixedPOStack = Abytek::ContainerTemplates::TF_ConcurrentFixedPOStack<__F_Item, TF_DefaultAllocator<__F_Item>>;
    
    /**
     * @brief Flat indirect map type definition with default allocator
     * @tparam __F_PayloadIndex Payload index type
     */
    template<typename __F_PayloadIndex = U32>
    using TF_FlatIndirectMap = Abytek::ContainerTemplates::TF_FlatIndirectMap<TF_DefaultAllocator<__F_PayloadIndex>, __F_PayloadIndex>;
    /// @brief Default flat indirect map type
    using F_FlatIndirectMap = TF_FlatIndirectMap<>;
    
    /**
     * @brief String database type definition with default allocator
     * @tparam __F_StringDatabase String database type
     * @tparam __F_Char Character type
     */
    template<class __F_StringDatabase, typename __F_Char>
    using TA_StringDatabase = Abytek::ContainerTemplates::TA_StringDatabase<__F_StringDatabase, __F_Char, TF_DefaultAllocator<U8>>;
    
    /**
     * @brief Event type definition with default allocator
     * @tparam __F_RawFunction Raw function type
     */
    template<typename __F_RawFunction>
    using TF_Event = Abytek::ContainerTemplates::TF_Event<__F_RawFunction, TF_DefaultAllocator<U8>>;
}

#define ABYTEK_BOOST_CONTAINER_TYPEDEFS_NAMESPACE ABYTEK_CONTAINER_TYPEDEFS_NAMESPACE
#include "Abytek/BoostContainerTypedefs.inl"

namespace ABYTEK_CONTAINER_TYPEDEFS_NAMESPACE
{
    /**
     * @brief Queue type definition using the default container
     * @tparam __F_Item Item type
     * @tparam __F_Container Container type
     */
    template<typename __F_Item, class __F_Container = ABYTEK_CONTAINER_TYPEDEFS_NAMESPACE::TF_Deque<__F_Item>>
    using TF_Queue = Abytek::ContainerTemplates::TF_Queue<__F_Item, __F_Container>;
    
    /**
     * @brief Stack type definition using the default container
     * @tparam __F_Item Item type
     * @tparam __F_Container Container type
     */
    template<typename __F_Item, class __F_Container = ABYTEK_CONTAINER_TYPEDEFS_NAMESPACE::TF_Vector<__F_Item>>
    using TF_Stack = Abytek::ContainerTemplates::TF_Stack<__F_Item, __F_Container>;
    
    /**
     * @brief Concurrent queue type definition using the default container
     * @tparam __F_Item Item type
     * @tparam __F_Container Container type
     */
    template<typename __F_Item, class __F_Container = ABYTEK_CONTAINER_TYPEDEFS_NAMESPACE::TF_Deque<__F_Item>>
    using TF_ConcurrentQueue = Abytek::ContainerTemplates::TF_ConcurrentQueue<__F_Item, __F_Container>;
    
    /**
     * @brief Concurrent stack type definition using the default container
     * @tparam __F_Item Item type
     * @tparam __F_Container Container type
     */
    template<typename __F_Item, class __F_Container = ABYTEK_CONTAINER_TYPEDEFS_NAMESPACE::TF_Vector<__F_Item>>
    using TF_ConcurrentStack = Abytek::ContainerTemplates::TF_ConcurrentStack<__F_Item, __F_Container>;
    
    template<B8 __AutoScale = false>
    using TF_LinearAllocationDistributor = Abytek::ContainerTemplates::TF_LinearAllocationDistributor<__AutoScale, TF_DefaultAllocator<U8>>;
    using F_LinearAllocationDistributor = TF_LinearAllocationDistributor<>;
    using F_AutoScaleLinearAllocationDistributor = TF_LinearAllocationDistributor<true>;
}

#undef ABYTEK_CONTAINER_TYPEDEFS_NAMESPACE