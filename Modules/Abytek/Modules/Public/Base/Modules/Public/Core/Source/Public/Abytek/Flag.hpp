#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"


namespace Abytek
{
    /**
     * @brief Type mapping template to select an appropriate unsigned integer type based on size
     * 
     * @tparam __F_Flag The flag type to map to an unsigned integer
     */
    template<typename __F_Flag>
    using TF_FlagToUInt = TF_NthTemplateTArg<
        sizeof(__F_Flag),
        void,
        U8,
        U16,
        void,
        U32,
        void,
        void,
        void,
        U64
    >;

    /**
     * @brief Combines two flag values using bitwise OR
     * 
     * @tparam __F_A1 Type of the first flag
     * @tparam __F_A2 Type of the second flag
     * @param A1 First flag value
     * @param A2 Second flag value
     * @return Combined flags result
     */
    template<typename __F_A1, typename __F_A2>
    ABYTEK_FORCE_INLINE constexpr auto FlagCombine(__F_A1 A1, __F_A2 A2) noexcept
    {
        using F_uint = TF_FlagToUInt<std::remove_const_t<decltype(A1)>>;

        return std::remove_const_t<decltype(A1)>(
            F_uint(A1)
            | F_uint(A2)
        );
    }
    
    /**
     * @brief Combines three or more flag values using bitwise OR
     * 
     * @tparam __F_A1 Type of the first flag
     * @tparam __F_A2 Type of the second flag
     * @tparam __F_A3 Type of the third flag
     * @tparam __Fs_Rest Types of additional flags
     * @param A1 First flag value
     * @param A2 Second flag value
     * @param A3 Third flag value
     * @param Rest Additional flag values
     * @return Combined flags result
     */
    template<typename __F_A1, typename __F_A2, typename __F_A3, typename... __Fs_Rest>
    ABYTEK_FORCE_INLINE constexpr auto FlagCombine(__F_A1 A1, __F_A2 A2, __F_A3 A3, __Fs_Rest... Rest) noexcept
    {
        return FlagCombine(
            FlagCombine(A1, A2),
            A3,
            Rest...
        );
    }
    
    /**
     * @brief Removes one flag value from another using bitwise operations
     * 
     * @tparam __F_A1 Type of the base flag
     * @tparam __F_A2 Type of the flag to remove
     * @param A1 Base flag value
     * @param A2 Flag value to remove
     * @return Result after removing the flag
     */
    template<typename __F_A1, typename __F_A2>
    ABYTEK_FORCE_INLINE constexpr auto FlagRemove(__F_A1 A1, __F_A2 A2) noexcept
    {
        using F_uint = TF_FlagToUInt<std::remove_const_t<decltype(A1)>>;

        return std::remove_const_t<decltype(A1)>(
            F_uint(A1)
            & (~F_uint(A2))
        );
    }
    
    /**
     * @brief Removes multiple flag values from a base flag
     * 
     * @tparam __F_A1 Type of the base flag
     * @tparam __F_A2 Type of the first flag to remove
     * @tparam __F_A3 Type of the second flag to remove
     * @tparam __Fs_Rest Types of additional flags to remove
     * @param A1 Base flag value
     * @param A2 First flag value to remove
     * @param A3 Second flag value to remove
     * @param Rest Additional flag values to remove
     * @return Result after removing all flags
     */
    template<typename __F_A1, typename __F_A2, typename __F_A3, typename... __Fs_Rest>
    ABYTEK_FORCE_INLINE constexpr auto FlagRemove(__F_A1 A1, __F_A2 A2, __F_A3 A3, __Fs_Rest... Rest) noexcept
    {
        return FlagRemove(
            FlagRemove(A1, A2),
            A3,
            Rest...
        );
    }
    
    /**
     * @brief Checks if a flag contains another flag (all bits set)
     * 
     * @tparam __F_A1 Type of the container flag
     * @tparam __F_A2 Type of the flag to check for
     * @param A1 Container flag value
     * @param A2 Flag value to check for
     * @return True if all bits of A2 are set in A1, false otherwise
     */
    template<typename __F_A1, typename __F_A2>
    ABYTEK_FORCE_INLINE constexpr B8 FlagHas(__F_A1 A1, __F_A2 A2) noexcept
    {
        using F_uint = TF_FlagToUInt<std::remove_const_t<decltype(A1)>>;

        return (
            F_uint(A2) == (
                F_uint(A1)
                & F_uint(A2)
            )
        );
    }
    
    /**
     * @brief Checks if a flag contains multiple other flags (all bits set)
     * 
     * @tparam __F_A1 Type of the container flag
     * @tparam __F_A2 Type of the first flag to check for
     * @tparam __F_A3 Type of the second flag to check for
     * @tparam __Fs_Rest Types of additional flags to check for
     * @param A1 Container flag value
     * @param A2 First flag value to check for
     * @param A3 Second flag value to check for
     * @param Rest Additional flag values to check for
     * @return True if all bits of all flags are set in A1, false otherwise
     */
    template<typename __F_A1, typename __F_A2, typename __F_A3, typename... __Fs_Rest>
    ABYTEK_FORCE_INLINE constexpr B8 FlagHas(__F_A1 A1, __F_A2 A2, __F_A3 A3, __Fs_Rest... Rest) noexcept
    {
        return (
            FlagHas(A1, A2)
            && FlagHas(A1, A3, Rest...)
        );
    }
    
    /**
     * @brief Checks if a flag contains any bits of another flag (any bits set)
     * 
     * @tparam __F_A1 Type of the container flag
     * @tparam __F_A2 Type of the flag to check for any bits
     * @param A1 Container flag value
     * @param A2 Flag value to check for any bits
     * @return True if any bits of A2 are set in A1, false otherwise
     */
    template<typename __F_A1, typename __F_A2>
    ABYTEK_FORCE_INLINE constexpr B8 FlagHasAny(__F_A1 A1, __F_A2 A2) noexcept
    {
        using F_uint = TF_FlagToUInt<std::remove_const_t<decltype(A1)>>;

        return (
            0 != (
                F_uint(A1)
                & F_uint(A2)
            )
        );
    }
    
    /**
     * @brief Checks if a flag contains any bits of multiple other flags
     * 
     * @tparam __F_A1 Type of the container flag
     * @tparam __F_A2 Type of the first flag to check for any bits
     * @tparam __F_A3 Type of the second flag to check for any bits
     * @tparam __Fs_Rest Types of additional flags to check for any bits
     * @param A1 Container flag value
     * @param A2 First flag value to check for any bits
     * @param A3 Second flag value to check for any bits
     * @param Rest Additional flag values to check for any bits
     * @return True if any bits of any flags are set in A1, false otherwise
     */
    template<typename __F_A1, typename __F_A2, typename __F_A3, typename... __Fs_Rest>
    ABYTEK_FORCE_INLINE constexpr B8 FlagHasAny(__F_A1 A1, __F_A2 A2, __F_A3 A3, __Fs_Rest... Rest) noexcept
    {
        return (
            FlagHasAny(A1, A2)
            || FlagHasAny(A1, A3, Rest...)
        );
    }
}