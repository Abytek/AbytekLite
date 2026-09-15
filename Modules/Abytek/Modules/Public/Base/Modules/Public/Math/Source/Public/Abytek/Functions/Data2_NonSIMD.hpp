#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data2.hpp"


namespace Abytek
{
    /**
     * @brief Helper struct providing non-SIMD implementations of common operations for 2D data types
     * 
     * This struct contains template methods for performing vector operations on 2-component data types,
     * such as 2D vectors. These implementations are not using SIMD optimizations and serve as fallbacks
     * or reference implementations.
     */
    struct H_Data2
    {
        /**
         * @brief Negates all components of a 2D vector
         * @param A Input vector
         * @return A new vector with all components negated
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE TF_Data<2, 1, __F_Element, __F_Config> Minus(
            TPA_Data<2, 1, __F_Element, __F_Config> A
        ) noexcept
        {
ABYTEK_DISABLE_ALL_WARNINGS_PUSH
            return {
                static_cast<__F_Element>(-A.X),
                static_cast<__F_Element>(-A.Y)
            };
ABYTEK_DISABLE_ALL_WARNINGS_POP
        }
        
        /**
         * @brief Computes the dot product of two 2D vectors
         * @param A First vector
         * @param B Second vector
         * @return The dot product (scalar) of the two vectors
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE __F_Element Dot(
            TPA_Data<2, 1, __F_Element, __F_Config> A,
            TPA_Data<2, 1, __F_Element, __F_Config> B
        ) noexcept
        {
            return static_cast<__F_Element>(
                A.X * B.X
                + A.Y * B.Y
            );
        }
        
        /**
         * @brief Adds two 2D vectors component-wise
         * @param A First vector
         * @param B Second vector
         * @return The component-wise sum of the vectors
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE TF_Data<2, 1, __F_Element, __F_Config> Add(
            TPA_Data<2, 1, __F_Element, __F_Config> A,
            TPA_Data<2, 1, __F_Element, __F_Config> B
        ) noexcept
        {
            return {
                static_cast<__F_Element>(A.X + B.X),
                static_cast<__F_Element>(A.Y + B.Y)
            };
        }
        
        /**
         * @brief Subtracts one 2D vector from another component-wise
         * @param A First vector (minuend)
         * @param B Second vector (subtrahend)
         * @return The component-wise difference of the vectors
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE TF_Data<2, 1, __F_Element, __F_Config> Subtract(
            TPA_Data<2, 1, __F_Element, __F_Config> A,
            TPA_Data<2, 1, __F_Element, __F_Config> B
        ) noexcept
        {
            return {
                static_cast<__F_Element>(A.X - B.X),
                static_cast<__F_Element>(A.Y - B.Y)
            };
        }
        
        /**
         * @brief Multiplies two 2D vectors component-wise
         * @param A First vector
         * @param B Second vector
         * @return The component-wise product of the vectors
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE TF_Data<2, 1, __F_Element, __F_Config> Multiply(
            TPA_Data<2, 1, __F_Element, __F_Config> A,
            TPA_Data<2, 1, __F_Element, __F_Config> B
        ) noexcept
        {
            return {
                static_cast<__F_Element>(A.X * B.X),
                static_cast<__F_Element>(A.Y * B.Y)
            };
        }
        
        /**
         * @brief Divides one 2D vector by another component-wise
         * @param A First vector (dividend)
         * @param B Second vector (divisor)
         * @return The component-wise quotient of the vectors
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE TF_Data<2, 1, __F_Element, __F_Config> Divide(
            TPA_Data<2, 1, __F_Element, __F_Config> A,
            TPA_Data<2, 1, __F_Element, __F_Config> B
        ) noexcept
        {
            return {
                static_cast<__F_Element>(A.X / B.X),
                static_cast<__F_Element>(A.Y / B.Y)
            };
        }
        
        /**
         * @brief Checks if two 2D vectors are equal
         * @param A First vector
         * @param B Second vector
         * @return True if all corresponding components are equal, false otherwise
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE B8 Equal(
            TPA_Data<2, 1, __F_Element, __F_Config> A,
            TPA_Data<2, 1, __F_Element, __F_Config> B
        ) noexcept
        {
            return (
                (A.X == B.X)
                && (A.Y == B.Y)
            );
        }
        
        /**
         * @brief Checks if two 2D vectors are not equal
         * @param A First vector
         * @param B Second vector
         * @return True if any corresponding components are not equal, false otherwise
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE B8 NotEqual(
            TPA_Data<2, 1, __F_Element, __F_Config> A,
            TPA_Data<2, 1, __F_Element, __F_Config> B
        ) noexcept
        {
            return (
                (A.X != B.X)
                || (A.Y != B.Y)
            );
        }
        
        /**
         * @brief Adds a scalar to all components of a 2D vector
         * @param A Vector
         * @param B Scalar value to add
         * @return A new vector with the scalar added to each component
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE TF_Data<2, 1, __F_Element, __F_Config> Add(
            TPA_Data<2, 1, __F_Element, __F_Config> A,
            __F_Element B
        ) noexcept
        {
            return {
                static_cast<__F_Element>(A.X + B),
                static_cast<__F_Element>(A.Y + B)
            };
        }
        
        /**
         * @brief Subtracts a scalar from all components of a 2D vector
         * @param A Vector
         * @param B Scalar value to subtract
         * @return A new vector with the scalar subtracted from each component
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE TF_Data<2, 1, __F_Element, __F_Config> Subtract(
            TPA_Data<2, 1, __F_Element, __F_Config> A,
            __F_Element B
        ) noexcept
        {
            return {
                static_cast<__F_Element>(A.X - B),
                static_cast<__F_Element>(A.Y - B)
            };
        }
        
        /**
         * @brief Multiplies all components of a 2D vector by a scalar
         * @param A Vector
         * @param B Scalar value to multiply by
         * @return A new vector with each component multiplied by the scalar
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE TF_Data<2, 1, __F_Element, __F_Config> Multiply(
            TPA_Data<2, 1, __F_Element, __F_Config> A,
            __F_Element B
        ) noexcept
        {
            return {
                static_cast<__F_Element>(A.X * B),
                static_cast<__F_Element>(A.Y * B)
            };
        }
        
        /**
         * @brief Divides all components of a 2D vector by a scalar
         * @param A Vector
         * @param B Scalar value to divide by
         * @return A new vector with each component divided by the scalar
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE TF_Data<2, 1, __F_Element, __F_Config> Divide(
            TPA_Data<2, 1, __F_Element, __F_Config> A,
            __F_Element B
        ) noexcept
        {
            return {
                static_cast<__F_Element>(A.X / B),
                static_cast<__F_Element>(A.Y / B)
            };
        }
        
        /**
         * @brief Checks if all components of a 2D vector are equal to a scalar
         * @param A Vector
         * @param B Scalar value to compare against
         * @return True if all components are equal to the scalar, false otherwise
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE B8 Equal(
            TPA_Data<2, 1, __F_Element, __F_Config> A,
            __F_Element B
        ) noexcept
        {
            return (
                (A.X == B)
                && (A.Y == B)
            );
        }
        
        /**
         * @brief Checks if any component of a 2D vector is not equal to a scalar
         * @param A Vector
         * @param B Scalar value to compare against
         * @return True if any component is not equal to the scalar, false otherwise
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE B8 NotEqual(
            TPA_Data<2, 1, __F_Element, __F_Config> A,
            __F_Element B
        ) noexcept
        {
            return (
                (A.X != B)
                || (A.Y != B)
            );
        }
    };
}