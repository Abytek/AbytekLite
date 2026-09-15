/**
 * @file Data4_NonSIMD.hpp
 * @brief Non-SIMD implementations of 4-component data vector operations
 * 
 * This file contains helper functions for operations on 4-component vectors
 * implemented without SIMD optimizations for portability.
 */
#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data4.hpp"


namespace Abytek
{
    /**
     * @brief Helper struct containing non-SIMD implementations of 4-component data operations
     */
    struct H_Data4
    {
        /**
         * @brief Negates all components of a 4-component vector
         * 
         * @tparam __F_Element Element type
         * @tparam __F_Config Configuration type
         * @param A Input vector
         * @return Negated vector
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE TF_Data<4, 1, __F_Element, __F_Config> Minus(
            TPA_Data<4, 1, __F_Element, __F_Config> A
        ) noexcept
        {
ABYTEK_DISABLE_ALL_WARNINGS_PUSH
            return {
                static_cast<__F_Element>(-A.X),
                static_cast<__F_Element>(-A.Y),
                static_cast<__F_Element>(-A.Z),
                static_cast<__F_Element>(-A.W)
            };
ABYTEK_DISABLE_ALL_WARNINGS_POP
        }
        
        /**
         * @brief Calculates the dot product of two 4-component vectors
         * 
         * @tparam __F_Element Element type
         * @tparam __F_Config Configuration type
         * @param A First vector
         * @param B Second vector
         * @return Dot product value
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE __F_Element Dot(
            TPA_Data<4, 1, __F_Element, __F_Config> A,
            TPA_Data<4, 1, __F_Element, __F_Config> B
        ) noexcept
        {
            return static_cast<__F_Element>(
                A.X * B.X
                + A.Y * B.Y
                + A.Z * B.Z
                + A.W * B.W
            );
        }
        
        /**
         * @brief Component-wise addition of two 4-component vectors
         * 
         * @tparam __F_Element Element type
         * @tparam __F_Config Configuration type
         * @param A First vector
         * @param B Second vector
         * @return Component-wise sum
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE TF_Data<4, 1, __F_Element, __F_Config> Add(
            TPA_Data<4, 1, __F_Element, __F_Config> A,
            TPA_Data<4, 1, __F_Element, __F_Config> B
        ) noexcept
        {
            return {
                static_cast<__F_Element>(A.X + B.X),
                static_cast<__F_Element>(A.Y + B.Y),
                static_cast<__F_Element>(A.Z + B.Z),
                static_cast<__F_Element>(A.W + B.W)
            };
        }
        
        /**
         * @brief Component-wise subtraction of two 4-component vectors
         * 
         * @tparam __F_Element Element type
         * @tparam __F_Config Configuration type
         * @param A First vector
         * @param B Second vector
         * @return Component-wise difference
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE TF_Data<4, 1, __F_Element, __F_Config> Subtract(
            TPA_Data<4, 1, __F_Element, __F_Config> A,
            TPA_Data<4, 1, __F_Element, __F_Config> B
        ) noexcept
        {
            return {
                static_cast<__F_Element>(A.X - B.X),
                static_cast<__F_Element>(A.Y - B.Y),
                static_cast<__F_Element>(A.Z - B.Z),
                static_cast<__F_Element>(A.W - B.W)
            };
        }
        
        /**
         * @brief Component-wise multiplication of two 4-component vectors
         * 
         * @tparam __F_Element Element type
         * @tparam __F_Config Configuration type
         * @param A First vector
         * @param B Second vector
         * @return Component-wise product
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE TF_Data<4, 1, __F_Element, __F_Config> Multiply(
            TPA_Data<4, 1, __F_Element, __F_Config> A,
            TPA_Data<4, 1, __F_Element, __F_Config> B
        ) noexcept
        {
            return {
                static_cast<__F_Element>(A.X * B.X),
                static_cast<__F_Element>(A.Y * B.Y),
                static_cast<__F_Element>(A.Z * B.Z),
                static_cast<__F_Element>(A.W * B.W)
            };
        }
        
        /**
         * @brief Component-wise division of two 4-component vectors
         * 
         * @tparam __F_Element Element type
         * @tparam __F_Config Configuration type
         * @param A Numerator vector
         * @param B Denominator vector
         * @return Component-wise quotient
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE TF_Data<4, 1, __F_Element, __F_Config> Divide(
            TPA_Data<4, 1, __F_Element, __F_Config> A,
            TPA_Data<4, 1, __F_Element, __F_Config> B
        ) noexcept
        {
            return {
                static_cast<__F_Element>(A.X / B.X),
                static_cast<__F_Element>(A.Y / B.Y),
                static_cast<__F_Element>(A.Z / B.Z),
                static_cast<__F_Element>(A.W / B.W)
            };
        }
        
        /**
         * @brief Component-wise equality check for two 4-component vectors
         * 
         * @tparam __F_Element Element type
         * @tparam __F_Config Configuration type
         * @param A First vector
         * @param B Second vector
         * @return True if all components are equal, false otherwise
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE B8 Equal(
            TPA_Data<4, 1, __F_Element, __F_Config> A,
            TPA_Data<4, 1, __F_Element, __F_Config> B
        ) noexcept
        {
            return (
                (A.X == B.X)
                && (A.Y == B.Y)
                && (A.Z == B.Z)
                && (A.W == B.W)
            );
        }
        
        /**
         * @brief Component-wise inequality check for two 4-component vectors
         * 
         * @tparam __F_Element Element type
         * @tparam __F_Config Configuration type
         * @param A First vector
         * @param B Second vector
         * @return True if any component is not equal, false if all are equal
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE B8 NotEqual(
            TPA_Data<4, 1, __F_Element, __F_Config> A,
            TPA_Data<4, 1, __F_Element, __F_Config> B
        ) noexcept
        {
            return (
                (A.X != B.X)
                || (A.Y != B.Y)
                || (A.Z != B.Z)
                || (A.W != B.W)
            );
        }
        
        /**
         * @brief Adds a scalar to each component of a 4-component vector
         * 
         * @tparam __F_Element Element type
         * @tparam __F_Config Configuration type
         * @param A Vector
         * @param B Scalar to add
         * @return Result of component-wise addition
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE TF_Data<4, 1, __F_Element, __F_Config> Add(
            TPA_Data<4, 1, __F_Element, __F_Config> A,
            __F_Element B
        ) noexcept
        {
            return {
                static_cast<__F_Element>(A.X + B),
                static_cast<__F_Element>(A.Y + B),
                static_cast<__F_Element>(A.Z + B),
                static_cast<__F_Element>(A.W + B)
            };
        }
        
        /**
         * @brief Subtracts a scalar from each component of a 4-component vector
         * 
         * @tparam __F_Element Element type
         * @tparam __F_Config Configuration type
         * @param A Vector
         * @param B Scalar to subtract
         * @return Result of component-wise subtraction
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE TF_Data<4, 1, __F_Element, __F_Config> Subtract(
            TPA_Data<4, 1, __F_Element, __F_Config> A,
            __F_Element B
        ) noexcept
        {
            return {
                static_cast<__F_Element>(A.X - B),
                static_cast<__F_Element>(A.Y - B),
                static_cast<__F_Element>(A.Z - B),
                static_cast<__F_Element>(A.W - B)
            };
        }
        
        /**
         * @brief Multiplies each component of a 4-component vector by a scalar
         * 
         * @tparam __F_Element Element type
         * @tparam __F_Config Configuration type
         * @param A Vector
         * @param B Scalar to multiply by
         * @return Result of component-wise multiplication
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE TF_Data<4, 1, __F_Element, __F_Config> Multiply(
            TPA_Data<4, 1, __F_Element, __F_Config> A,
            __F_Element B
        ) noexcept
        {
            return {
                static_cast<__F_Element>(A.X * B),
                static_cast<__F_Element>(A.Y * B),
                static_cast<__F_Element>(A.Z * B),
                static_cast<__F_Element>(A.W * B)
            };
        }
        
        /**
         * @brief Divides each component of a 4-component vector by a scalar
         * 
         * @tparam __F_Element Element type
         * @tparam __F_Config Configuration type
         * @param A Vector
         * @param B Scalar divisor
         * @return Result of component-wise division
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE TF_Data<4, 1, __F_Element, __F_Config> Divide(
            TPA_Data<4, 1, __F_Element, __F_Config> A,
            __F_Element B
        ) noexcept
        {
            return {
                static_cast<__F_Element>(A.X / B),
                static_cast<__F_Element>(A.Y / B),
                static_cast<__F_Element>(A.Z / B),
                static_cast<__F_Element>(A.W / B)
            };
        }
        
        /**
         * @brief Checks if all components of a vector equal a scalar value
         * 
         * @tparam __F_Element Element type
         * @tparam __F_Config Configuration type
         * @param A Vector to check
         * @param B Scalar to compare against
         * @return True if all components equal the scalar, false otherwise
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE B8 Equal(
            TPA_Data<4, 1, __F_Element, __F_Config> A,
            __F_Element B
        ) noexcept
        {
            return (
                (A.X == B)
                && (A.Y == B)
                && (A.Z == B)
                && (A.W == B)
            );
        }
        
        /**
         * @brief Checks if any component of a vector is not equal to a scalar value
         * 
         * @tparam __F_Element Element type
         * @tparam __F_Config Configuration type
         * @param A Vector to check
         * @param B Scalar to compare against
         * @return True if any component is not equal to the scalar, false otherwise
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE B8 NotEqual(
            TPA_Data<4, 1, __F_Element, __F_Config> A,
            __F_Element B
        ) noexcept
        {
            return (
                (A.X != B)
                || (A.Y != B)
                || (A.Z != B)
                || (A.W != B)
            );
        }
        
        /**
         * @brief Performs quaternion multiplication of two 4-component vectors
         *
         * This implements the Hamilton product formula for quaternions.
         * 
         * @tparam __F_Element Element type
         * @tparam __F_Config Configuration type
         * @param A First quaternion
         * @param B Second quaternion
         * @return Result of quaternion multiplication
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE TF_Data<4, 1, __F_Element, __F_Config> ComplexMultiply(
            TPA_Data<4, 1, __F_Element, __F_Config> A,
            TPA_Data<4, 1, __F_Element, __F_Config> B
        ) noexcept
        {
            return {
                static_cast<__F_Element>((A.W * B.X) + (A.X * B.W) + (A.Y * B.Z) - (A.Z * B.Y)),
                static_cast<__F_Element>((A.W * B.Y) - (A.X * B.Z) + (A.Y * B.W) + (A.Z * B.X)),
                static_cast<__F_Element>((A.W * B.Z) + (A.X * B.Y) - (A.Y * B.X) + (A.Z * B.W)),
                static_cast<__F_Element>((A.W * B.W) - (A.X * B.X) - (A.Y * B.Y) - (A.Z * B.Z))
            };
        }
    };
}