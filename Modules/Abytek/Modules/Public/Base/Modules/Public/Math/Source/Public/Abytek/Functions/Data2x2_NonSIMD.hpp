#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Functions/Data2.hpp"
#include "Abytek/Types/Data2x2.hpp"


namespace Abytek
{
    /**
     * @brief Helper struct providing non-SIMD implementations of common operations for 2x2 matrices
     * 
     * This struct contains template methods for performing matrix operations on 2x2 matrices.
     * These implementations are not using SIMD optimizations and serve as fallbacks
     * or reference implementations.
     */
    struct H_Data2x2
    {
        /**
         * @brief Negates all components of a 2x2 matrix
         * @param A Input matrix
         * @return A new matrix with all components negated
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE TF_Data<2, 2, __F_Element, __F_Config> Minus(
            TPA_Data<2, 2, __F_Element, __F_Config> A
        ) noexcept
        {
            return {
                H_Data2::template Minus<__F_Element, __F_Config>(A.A),
                H_Data2::template Minus<__F_Element, __F_Config>(A.B)
            };
        }
        
        /**
         * @brief Adds two 2x2 matrices component-wise
         * @param A First matrix
         * @param B Second matrix
         * @return The component-wise sum of the matrices
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE TF_Data<2, 2, __F_Element, __F_Config> Add(
            TPA_Data<2, 2, __F_Element, __F_Config> A,
            TPA_Data<2, 2, __F_Element, __F_Config> B
        ) noexcept
        {
            return {
                H_Data2::template Add<__F_Element, __F_Config>(A.A, B.A),
                H_Data2::template Add<__F_Element, __F_Config>(A.B, B.B)
            };
        }
        
        /**
         * @brief Subtracts one 2x2 matrix from another component-wise
         * @param A First matrix (minuend)
         * @param B Second matrix (subtrahend)
         * @return The component-wise difference of the matrices
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE TF_Data<2, 2, __F_Element, __F_Config> Subtract(
            TPA_Data<2, 2, __F_Element, __F_Config> A,
            TPA_Data<2, 2, __F_Element, __F_Config> B
        ) noexcept
        {
            return {
                H_Data2::template Subtract<__F_Element, __F_Config>(A.A, B.A),
                H_Data2::template Subtract<__F_Element, __F_Config>(A.B, B.B)
            };
        }
        
        /**
         * @brief Multiplies two 2x2 matrices component-wise (Hadamard product)
         * @param A First matrix
         * @param B Second matrix
         * @return The component-wise product of the matrices
         * @note This is NOT matrix multiplication but element-wise multiplication
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE TF_Data<2, 2, __F_Element, __F_Config> Multiply(
            TPA_Data<2, 2, __F_Element, __F_Config> A,
            TPA_Data<2, 2, __F_Element, __F_Config> B
        ) noexcept
        {
            return {
                H_Data2::template Multiply<__F_Element, __F_Config>(A.A, B.A),
                H_Data2::template Multiply<__F_Element, __F_Config>(A.B, B.B)
            };
        }
        
        /**
         * @brief Divides one 2x2 matrix by another component-wise
         * @param A First matrix (dividend)
         * @param B Second matrix (divisor)
         * @return The component-wise quotient of the matrices
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE TF_Data<2, 2, __F_Element, __F_Config> Divide(
            TPA_Data<2, 2, __F_Element, __F_Config> A,
            TPA_Data<2, 2, __F_Element, __F_Config> B
        ) noexcept
        {
            return {
                H_Data2::template Divide<__F_Element, __F_Config>(A.A, B.A),
                H_Data2::template Divide<__F_Element, __F_Config>(A.B, B.B)
            };
        }
        
        /**
         * @brief Checks if two 2x2 matrices are equal
         * @param A First matrix
         * @param B Second matrix
         * @return True if all corresponding components are equal, false otherwise
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE B8 Equal(
            TPA_Data<2, 2, __F_Element, __F_Config> A,
            TPA_Data<2, 2, __F_Element, __F_Config> B
        ) noexcept
        {
            return (
                H_Data2::template Equal<__F_Element, __F_Config>(A.A, B.A)
                && H_Data2::template Equal<__F_Element, __F_Config>(A.B, B.B)
            );
        }
        
        /**
         * @brief Checks if two 2x2 matrices are not equal
         * @param A First matrix
         * @param B Second matrix
         * @return True if any corresponding components are not equal, false otherwise
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE B8 NotEqual(
            TPA_Data<2, 2, __F_Element, __F_Config> A,
            TPA_Data<2, 2, __F_Element, __F_Config> B
        ) noexcept
        {
            return (
                H_Data2::template NotEqual<__F_Element, __F_Config>(A.A, B.A)
                || H_Data2::template NotEqual<__F_Element, __F_Config>(A.B, B.B)
            );
        }
        
        /**
         * @brief Adds a scalar to all components of a 2x2 matrix
         * @param A Matrix
         * @param B Scalar value to add
         * @return A new matrix with the scalar added to each component
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE TF_Data<2, 2, __F_Element, __F_Config> Add(
            TPA_Data<2, 2, __F_Element, __F_Config> A,
            __F_Element B
        ) noexcept
        {
            return {
                H_Data2::template Add<__F_Element, __F_Config>(A.A, B),
                H_Data2::template Add<__F_Element, __F_Config>(A.B, B)
            };
        }
        
        /**
         * @brief Subtracts a scalar from all components of a 2x2 matrix
         * @param A Matrix
         * @param B Scalar value to subtract
         * @return A new matrix with the scalar subtracted from each component
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE TF_Data<2, 2, __F_Element, __F_Config> Subtract(
            TPA_Data<2, 2, __F_Element, __F_Config> A,
            __F_Element B
        ) noexcept
        {
            return {
                H_Data2::template Subtract<__F_Element, __F_Config>(A.A, B),
                H_Data2::template Subtract<__F_Element, __F_Config>(A.B, B)
            };
        }
        
        /**
         * @brief Multiplies all components of a 2x2 matrix by a scalar
         * @param A Matrix
         * @param B Scalar value to multiply by
         * @return A new matrix with each component multiplied by the scalar
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE TF_Data<2, 2, __F_Element, __F_Config> Multiply(
            TPA_Data<2, 2, __F_Element, __F_Config> A,
            __F_Element B
        ) noexcept
        {
            return {
                H_Data2::template Multiply<__F_Element, __F_Config>(A.A, B),
                H_Data2::template Multiply<__F_Element, __F_Config>(A.B, B)
            };
        }
        
        /**
         * @brief Divides all components of a 2x2 matrix by a scalar
         * @param A Matrix
         * @param B Scalar value to divide by
         * @return A new matrix with each component divided by the scalar
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE TF_Data<2, 2, __F_Element, __F_Config> Divide(
            TPA_Data<2, 2, __F_Element, __F_Config> A,
            __F_Element B
        ) noexcept
        {
            return {
                H_Data2::template Divide<__F_Element, __F_Config>(A.A, B),
                H_Data2::template Divide<__F_Element, __F_Config>(A.B, B)
            };
        }
        
        /**
         * @brief Checks if all components of a 2x2 matrix are equal to a scalar
         * @param A Matrix
         * @param B Scalar value to compare against
         * @return True if all components are equal to the scalar, false otherwise
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE B8 Equal(
            TPA_Data<2, 2, __F_Element, __F_Config> A,
            __F_Element B
        ) noexcept
        {
            return (
                H_Data2::template Equal<__F_Element, __F_Config>(A.A, B)
                && H_Data2::template Equal<__F_Element, __F_Config>(A.B, B)
            );
        }
        
        /**
         * @brief Checks if any component of a 2x2 matrix is not equal to a scalar
         * @param A Matrix
         * @param B Scalar value to compare against
         * @return True if any component is not equal to the scalar, false otherwise
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE B8 NotEqual(
            TPA_Data<2, 2, __F_Element, __F_Config> A,
            __F_Element B
        ) noexcept
        {
            return (
                H_Data2::template NotEqual<__F_Element, __F_Config>(A.A, B)
                || H_Data2::template NotEqual<__F_Element, __F_Config>(A.B, B)
            );
        }
    };
}