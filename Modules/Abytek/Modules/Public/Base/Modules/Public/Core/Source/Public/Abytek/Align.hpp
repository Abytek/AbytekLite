/**
 * @file Align.hpp
 * @brief Provides utility functions for memory alignment operations.
 */
#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"
#include "Abytek/Assert.hpp"


namespace Abytek
{
    /**
     * @brief Aligns a size value to a power-of-2 boundary.
     * @param Size The size value to align.
     * @param Align The alignment boundary, must be a power of 2. Defaults to ABYTEK_MIN_MEMORY_ALIGNMENT.
     * @return The aligned size value.
     */
    ABYTEK_FORCE_INLINE constexpr size_t AlignSize_PO2(size_t Size, size_t Align = ABYTEK_MIN_MEMORY_ALIGNMENT)
    {
        const size_t Mask = Align - 1;
        return (Size + Mask) & ~Mask;
    }
    /**
     * @brief Aligns an address to a power-of-2 boundary.
     * @param Addr The address to align.
     * @param Align The alignment boundary, must be a power of 2. Defaults to ABYTEK_MIN_MEMORY_ALIGNMENT.
     * @return The aligned address.
     */
    ABYTEK_FORCE_INLINE uintptr_t AlignAddress_PO2(uintptr_t Addr, size_t Align = ABYTEK_MIN_MEMORY_ALIGNMENT)
    {
        const size_t Mask = Align - 1;
        ABYTEK_BASE_CORE_ASSERT((Align & Mask) == 0); // pwr of 2
        return (Addr + Mask) & ~Mask;
    }
    ABYTEK_FORCE_INLINE constexpr size_t AlignSize(size_t Size, size_t Align = ABYTEK_MIN_MEMORY_ALIGNMENT)
    {
        return Size + ((Align - (Size % Align)) % Align);
    }
    /**
     * @brief Aligns an address using modulo arithmetic.
     * @param Addr The address to align.
     * @param Align The alignment boundary. Defaults to ABYTEK_MIN_MEMORY_ALIGNMENT.
     * @return The aligned address.
     */
    ABYTEK_FORCE_INLINE constexpr uintptr_t AlignAddress(uintptr_t Addr, size_t Align = ABYTEK_MIN_MEMORY_ALIGNMENT)
    {
        return Addr + ((Align - (Addr % Align)) % Align);
    }
    /**
     * @brief Aligns an address to a power-of-2 boundary and shifts it if already aligned.
     * 
     * If the address is already aligned, it will be shifted by the alignment value.
     * 
     * @param Addr The address to align.
     * @param Align The alignment boundary, must be a power of 2. Defaults to ABYTEK_MIN_MEMORY_ALIGNMENT.
     * @return The aligned and potentially shifted address.
     */
    ABYTEK_FORCE_INLINE uintptr_t AlignAddressShift_PO2(uintptr_t Addr, size_t Align = ABYTEK_MIN_MEMORY_ALIGNMENT)
    {
        const size_t Mask = Align - 1;
        ABYTEK_BASE_CORE_ASSERT((Align & Mask) == 0); // pwr of 2

        uintptr_t result = (Addr + Mask) & ~Mask;

        if (result == Addr)
            return result + Align;

        return result;
    }
    /**
     * @brief Aligns a size value using modulo arithmetic.
     * @param Size The size value to align.
     * @param Align The alignment boundary. Defaults to ABYTEK_MIN_MEMORY_ALIGNMENT.
     * @return The aligned size value.
     */
    ABYTEK_FORCE_INLINE constexpr size_t AlignSizeShift(size_t Size, size_t Align = ABYTEK_MIN_MEMORY_ALIGNMENT)
    {
        return Size + (Align - (Size % Align));
    }
    /**
     * @brief Aligns an address using modulo arithmetic.
     * @param Addr The address to align.
     * @param Align The alignment boundary. Defaults to ABYTEK_MIN_MEMORY_ALIGNMENT.
     * @return The aligned address.
     */
    ABYTEK_FORCE_INLINE constexpr uintptr_t AlignAddressShift(uintptr_t Addr, size_t Align = ABYTEK_MIN_MEMORY_ALIGNMENT)
    {
        return Addr + (Align - (Addr % Align));
    }
}