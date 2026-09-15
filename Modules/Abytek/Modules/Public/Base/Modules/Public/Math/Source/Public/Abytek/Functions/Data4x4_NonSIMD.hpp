/**
 * @file Data4x4_NonSIMD.hpp
 * @brief Non-SIMD implementations of 4x4 matrix operations
 * 
 * This file contains helper functions for operations on 4x4 matrices
 * implemented without SIMD optimizations for portability.
 */
#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Functions/Data4.hpp"
#include "Abytek/Types/Data4x4.hpp"


namespace Abytek
{
    /**
     * @brief Helper struct containing non-SIMD implementations of 4x4 matrix operations
     */
    struct H_Data4x4
    {
        /**
         * @brief Negates all components of a 4x4 matrix
         * 
         * @tparam __F_Element Element type
         * @tparam __F_Config Configuration type
         * @param A Input matrix
         * @return Negated matrix
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE TF_Data<4, 4, __F_Element, __F_Config> Minus(
            TPA_Data<4, 4, __F_Element, __F_Config> A
        ) noexcept
        {
            return {
                H_Data4::template Minus<__F_Element, __F_Config>(A.A),
                H_Data4::template Minus<__F_Element, __F_Config>(A.B),
                H_Data4::template Minus<__F_Element, __F_Config>(A.C),
                H_Data4::template Minus<__F_Element, __F_Config>(A.D)
            };
        }
        
        /**
         * @brief Component-wise addition of two 4x4 matrices
         * 
         * @tparam __F_Element Element type
         * @tparam __F_Config Configuration type
         * @param A First matrix
         * @param B Second matrix
         * @return Component-wise sum
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE TF_Data<4, 4, __F_Element, __F_Config> Add(
            TPA_Data<4, 4, __F_Element, __F_Config> A,
            TPA_Data<4, 4, __F_Element, __F_Config> B
        ) noexcept
        {
            return {
                H_Data4::template Add<__F_Element, __F_Config>(A.A, B.A),
                H_Data4::template Add<__F_Element, __F_Config>(A.B, B.B),
                H_Data4::template Add<__F_Element, __F_Config>(A.C, B.C),
                H_Data4::template Add<__F_Element, __F_Config>(A.D, B.D)
            };
        }
        
        /**
         * @brief Component-wise subtraction of two 4x4 matrices
         * 
         * @tparam __F_Element Element type
         * @tparam __F_Config Configuration type
         * @param A First matrix
         * @param B Second matrix
         * @return Component-wise difference
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE TF_Data<4, 4, __F_Element, __F_Config> Subtract(
            TPA_Data<4, 4, __F_Element, __F_Config> A,
            TPA_Data<4, 4, __F_Element, __F_Config> B
        ) noexcept
        {
            return {
                H_Data4::template Subtract<__F_Element, __F_Config>(A.A, B.A),
                H_Data4::template Subtract<__F_Element, __F_Config>(A.B, B.B),
                H_Data4::template Subtract<__F_Element, __F_Config>(A.C, B.C),
                H_Data4::template Subtract<__F_Element, __F_Config>(A.D, B.D)
            };
        }
        
        /**
         * @brief Component-wise multiplication of two 4x4 matrices
         * 
         * Note: This is not standard matrix multiplication but element-wise multiplication
         * 
         * @tparam __F_Element Element type
         * @tparam __F_Config Configuration type
         * @param A First matrix
         * @param B Second matrix
         * @return Component-wise product
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE TF_Data<4, 4, __F_Element, __F_Config> Multiply(
            TPA_Data<4, 4, __F_Element, __F_Config> A,
            TPA_Data<4, 4, __F_Element, __F_Config> B
        ) noexcept
        {
            return {
                H_Data4::template Multiply<__F_Element, __F_Config>(A.A, B.A),
                H_Data4::template Multiply<__F_Element, __F_Config>(A.B, B.B),
                H_Data4::template Multiply<__F_Element, __F_Config>(A.C, B.C),
                H_Data4::template Multiply<__F_Element, __F_Config>(A.D, B.D)
            };
        }
        
        /**
         * @brief Component-wise division of two 4x4 matrices
         * 
         * @tparam __F_Element Element type
         * @tparam __F_Config Configuration type
         * @param A Numerator matrix
         * @param B Denominator matrix
         * @return Component-wise quotient
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE TF_Data<4, 4, __F_Element, __F_Config> Divide(
            TPA_Data<4, 4, __F_Element, __F_Config> A,
            TPA_Data<4, 4, __F_Element, __F_Config> B
        ) noexcept
        {
            return {
                H_Data4::template Divide<__F_Element, __F_Config>(A.A, B.A),
                H_Data4::template Divide<__F_Element, __F_Config>(A.B, B.B),
                H_Data4::template Divide<__F_Element, __F_Config>(A.C, B.C),
                H_Data4::template Divide<__F_Element, __F_Config>(A.D, B.D)
            };
        }
        
        /**
         * @brief Component-wise equality check for two 4x4 matrices
         * 
         * @tparam __F_Element Element type
         * @tparam __F_Config Configuration type
         * @param A First matrix
         * @param B Second matrix
         * @return True if all components are equal, false otherwise
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE B8 Equal(
            TPA_Data<4, 4, __F_Element, __F_Config> A,
            TPA_Data<4, 4, __F_Element, __F_Config> B
        ) noexcept
        {
            return (
                H_Data4::template Equal<__F_Element, __F_Config>(A.A, B.A)
                && H_Data4::template Equal<__F_Element, __F_Config>(A.B, B.B)
                && H_Data4::template Equal<__F_Element, __F_Config>(A.C, B.C)
                && H_Data4::template Equal<__F_Element, __F_Config>(A.D, B.D)
            );
        }
        
        /**
         * @brief Component-wise inequality check for two 4x4 matrices
         * 
         * @tparam __F_Element Element type
         * @tparam __F_Config Configuration type
         * @param A First matrix
         * @param B Second matrix
         * @return True if any component is not equal, false if all are equal
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE B8 NotEqual(
            TPA_Data<4, 4, __F_Element, __F_Config> A,
            TPA_Data<4, 4, __F_Element, __F_Config> B
        ) noexcept
        {
            return (
                H_Data4::template NotEqual<__F_Element, __F_Config>(A.A, B.A)
                || H_Data4::template NotEqual<__F_Element, __F_Config>(A.B, B.B)
                || H_Data4::template NotEqual<__F_Element, __F_Config>(A.C, B.C)
                || H_Data4::template NotEqual<__F_Element, __F_Config>(A.D, B.D)
            );
        }
        
        /**
         * @brief Adds a scalar to each component of a 4x4 matrix
         * 
         * @tparam __F_Element Element type
         * @tparam __F_Config Configuration type
         * @param A Matrix
         * @param B Scalar to add
         * @return Result of component-wise addition
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE TF_Data<4, 4, __F_Element, __F_Config> Add(
            TPA_Data<4, 4, __F_Element, __F_Config> A,
            __F_Element B
        ) noexcept
        {
            return {
                H_Data4::template Add<__F_Element, __F_Config>(A.A, B),
                H_Data4::template Add<__F_Element, __F_Config>(A.B, B),
                H_Data4::template Add<__F_Element, __F_Config>(A.C, B),
                H_Data4::template Add<__F_Element, __F_Config>(A.D, B)
            };
        }
        
        /**
         * @brief Subtracts a scalar from each component of a 4x4 matrix
         * 
         * @tparam __F_Element Element type
         * @tparam __F_Config Configuration type
         * @param A Matrix
         * @param B Scalar to subtract
         * @return Result of component-wise subtraction
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE TF_Data<4, 4, __F_Element, __F_Config> Subtract(
            TPA_Data<4, 4, __F_Element, __F_Config> A,
            __F_Element B
        ) noexcept
        {
            return {
                H_Data4::template Subtract<__F_Element, __F_Config>(A.A, B),
                H_Data4::template Subtract<__F_Element, __F_Config>(A.B, B),
                H_Data4::template Subtract<__F_Element, __F_Config>(A.C, B),
                H_Data4::template Subtract<__F_Element, __F_Config>(A.D, B)
            };
        }
        
        /**
         * @brief Multiplies each component of a 4x4 matrix by a scalar
         * 
         * @tparam __F_Element Element type
         * @tparam __F_Config Configuration type
         * @param A Matrix
         * @param B Scalar to multiply by
         * @return Result of component-wise multiplication
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE TF_Data<4, 4, __F_Element, __F_Config> Multiply(
            TPA_Data<4, 4, __F_Element, __F_Config> A,
            __F_Element B
        ) noexcept
        {
            return {
                H_Data4::template Multiply<__F_Element, __F_Config>(A.A, B),
                H_Data4::template Multiply<__F_Element, __F_Config>(A.B, B),
                H_Data4::template Multiply<__F_Element, __F_Config>(A.C, B),
                H_Data4::template Multiply<__F_Element, __F_Config>(A.D, B)
            };
        }
        
        /**
         * @brief Divides each component of a 4x4 matrix by a scalar
         * 
         * @tparam __F_Element Element type
         * @tparam __F_Config Configuration type
         * @param A Matrix
         * @param B Scalar divisor
         * @return Result of component-wise division
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE TF_Data<4, 4, __F_Element, __F_Config> Divide(
            TPA_Data<4, 4, __F_Element, __F_Config> A,
            __F_Element B
        ) noexcept
        {
            return {
                H_Data4::template Divide<__F_Element, __F_Config>(A.A, B),
                H_Data4::template Divide<__F_Element, __F_Config>(A.B, B),
                H_Data4::template Divide<__F_Element, __F_Config>(A.C, B),
                H_Data4::template Divide<__F_Element, __F_Config>(A.D, B)
            };
        }
        
        /**
         * @brief Checks if all components of a matrix equal a scalar value
         * 
         * @tparam __F_Element Element type
         * @tparam __F_Config Configuration type
         * @param A Matrix to check
         * @param B Scalar to compare against
         * @return True if all components equal the scalar, false otherwise
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE B8 Equal(
            TPA_Data<4, 4, __F_Element, __F_Config> A,
            __F_Element B
        ) noexcept
        {
            return (
                H_Data4::template Equal<__F_Element, __F_Config>(A.A, B)
                && H_Data4::template Equal<__F_Element, __F_Config>(A.B, B)
                && H_Data4::template Equal<__F_Element, __F_Config>(A.C, B)
                && H_Data4::template Equal<__F_Element, __F_Config>(A.D, B)
            );
        }
        
        /**
         * @brief Checks if any component of a matrix is not equal to a scalar value
         * 
         * @tparam __F_Element Element type
         * @tparam __F_Config Configuration type
         * @param A Matrix to check
         * @param B Scalar to compare against
         * @return True if any component is not equal to the scalar, false otherwise
         */
        template<typename __F_Element, class __F_Config>
        static ABYTEK_FORCE_INLINE B8 NotEqual(
            TPA_Data<4, 4, __F_Element, __F_Config> A,
            __F_Element B
        ) noexcept
        {
            return (
                H_Data4::template NotEqual<__F_Element, __F_Config>(A.A, B)
                || H_Data4::template NotEqual<__F_Element, __F_Config>(A.B, B)
                || H_Data4::template NotEqual<__F_Element, __F_Config>(A.C, B)
                || H_Data4::template NotEqual<__F_Element, __F_Config>(A.D, B)
            );
        }
    };
}