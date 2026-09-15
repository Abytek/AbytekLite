/**
 * @file CrtAllocator.hpp
 * @brief Provides a C Runtime (CRT) based allocator implementation.
 */
#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"
#include "Abytek/ByteAllocator.hpp"


namespace Abytek
{
    /**
     * @brief Options specific to the CRT allocator.
     * 
     * Extends the byte allocator options with CRT-specific settings.
     */
    struct F_CrtAllocatorOptions : F_ByteAllocatorOptions
    {
    };

    /**
     * @brief CRT-based allocator template that uses the C Runtime allocation functions.
     * 
     * This allocator leverages the standard C malloc/free functions for memory management.
     * 
     * @tparam __F_Value The value type this allocator works with.
     */
    template<typename __F_Value>
    struct TF_CrtAllocator :
        TA_ByteAllocator<TF_CrtAllocator<__F_Value>, __F_Value, F_CrtAllocatorOptions>
    {
        /**
         * @brief Rebind mechanism to create an allocator for a different value type.
         */
        ABYTEK_DEFINE_ALLOCATOR_REBIND(
            __F_OtherValue,
            TF_CrtAllocator<__F_OtherValue>
        );

        /**
         * @brief Default constructor.
         */
        constexpr TF_CrtAllocator() noexcept = default;

        /**
         * @brief Copy constructor.
         */
        constexpr TF_CrtAllocator(const TF_CrtAllocator&) noexcept = default;
        
        /**
         * @brief Copy assignment operator.
         */
        constexpr TF_CrtAllocator& operator = (const TF_CrtAllocator&) noexcept = default;

        /**
         * @brief Converting constructor from a different value type.
         * 
         * @tparam __F_OtherValue The other value type.
         */
        template<typename __F_OtherValue>
        constexpr TF_CrtAllocator(const TF_CrtAllocator<__F_OtherValue>&) noexcept {}
        
        /**
         * @brief Converting assignment operator from a different value type.
         * 
         * @tparam __F_OtherValue The other value type.
         */
        template<typename __F_OtherValue>
        constexpr TF_CrtAllocator& operator = (const TF_CrtAllocator<__F_OtherValue>&) noexcept {}
    };
}