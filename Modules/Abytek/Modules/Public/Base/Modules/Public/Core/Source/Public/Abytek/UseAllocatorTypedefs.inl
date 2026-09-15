/**
 * @file UseAllocatorTypedefs.inl
 * @brief Type definitions for containers using custom allocator
 * 
 * This file defines memory allocation functions using a custom allocator
 * and includes ContainerTypedefs.inl to create container type definitions 
 * that use the specified allocator.
 */

#include "Abytek/Base.Core.prerequisites.pch.hpp"

#ifndef ABYTEK_USE_ALLOCATOR_TYPEDEFS_NAMESPACE
#define ABYTEK_USE_ALLOCATOR_TYPEDEFS_NAMESPACE Abytek
#endif

namespace ABYTEK_USE_ALLOCATOR_TYPEDEFS_NAMESPACE
{
    /**
     * @brief Allocates memory for a single object
     * @tparam __F Type to allocate
     * @return Pointer to allocated memory
     */
    template<typename __F>
    __F* Allocate()
    {
        return TF_DefaultAllocator<__F>().Allocate(1);
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
        return TF_DefaultAllocator<__F>().Allocate(Count);
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
        return TF_DefaultAllocator<__F>().Allocate(__Count);
    }
    
    /**
     * @brief Deallocates memory for a single object
     * @tparam __F Type to deallocate
     * @param Pointer Pointer to memory to deallocate
     */
    template<typename __F>
    void Deallocate(__F* Pointer)
    {
        TF_DefaultAllocator<__F>().Deallocate(Pointer);
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
}

#define ABYTEK_CONTAINER_TYPEDEFS_NAMESPACE ABYTEK_USE_ALLOCATOR_TYPEDEFS_NAMESPACE
#include "Abytek/ContainerTypedefs.inl"

#undef ABYTEK_USE_ALLOCATOR_TYPEDEFS_NAMESPACE