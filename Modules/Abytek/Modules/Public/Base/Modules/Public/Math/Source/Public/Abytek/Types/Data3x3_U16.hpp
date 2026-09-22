/**
 * @file Data3x3_U16.hpp
 * @brief Defines a 3x3 matrix with 32-bit unsigned integer elements
 */
#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/DataTemplate.hpp"
#include "Abytek/Types/Data3_U16.hpp"
#include "Abytek/Types/Data3x3_U16.hpp"


namespace Abytek
{
    /**
     * @brief A 3x3 matrix with U16 (unsigned int) elements
     * 
     * Matrix is stored in column-major order with columns labeled A, B, and C.
     * The matrix can be accessed by column or by individual elements.
     * 
     * @tparam __F_Config Configuration type for the matrix
     */
    template<class __F_Config>
    struct TF_Data<3, 3, U16, __F_Config> : __F_Config::template TA_DefaultData<3, 3, U16, __F_Config>
    {
        using F_This = TF_Data;
        ABYTEK_BEGIN_REFLECTOR(I_Serializable)
        ABYTEK_END_REFLECTOR(F_This);
        
        static constexpr U16 RowCount = 3;
        static constexpr U16 ColumnCount = 3;

        using F_Element = U16;
        using F_Config = __F_Config;

        using F_PassedArgument = const TF_Data&;

        using F_Column = TF_Data<RowCount, 1, F_Element, F_Config>;
        using PA_Column = typename F_Column::F_PassedArgument;
        
        template<U32 __RowCount>
        using TF_RebindRowCount = TF_Data<__RowCount, ColumnCount, F_Element, F_Config>;
        
        template<U32 __RowCount>
        using TPA_RebindRowCount = TPA_Data<__RowCount, ColumnCount, F_Element, F_Config>;
        
        template<U32 __ColumnCount>
        using TF_RebindColumnCount = TF_Data<RowCount, __ColumnCount, F_Element, F_Config>;
        
        template<U32 __ColumnCount>
        using TPA_RebindColumnCount = TPA_Data<RowCount, __ColumnCount, F_Element, F_Config>;
        
        template<U32 __RowCount, U32 __ColumnCount>
        using TF_RebindRowColumnCount = TF_Data<__RowCount, __ColumnCount, F_Element, F_Config>;
        
        template<U32 __RowCount, U32 __ColumnCount>
        using TPA_RebindRowColumnCount = TPA_Data<__RowCount, __ColumnCount, F_Element, F_Config>;
        
        template<typename __F_OtherElement>
        using TF_RebindElement = TF_Data<RowCount, ColumnCount, __F_OtherElement, F_Config>;
        
        template<class __F_Config2>
        using TF_RebindConfig = TF_Data<RowCount, ColumnCount, F_Element, __F_Config2>;

        using F_SmallerRectangle = TF_RebindRowColumnCount<RowCount - 1, ColumnCount - 1>;
        using PA_SmallerRectangle = typename F_SmallerRectangle::F_PassedArgument;
        
        /**
         * @brief Matrix data storage
         * 
         * Accessible as columns A, B, and C, array of columns, or flat array of elements
         */
        union
        {
            struct
            {
                F_Column A;
                F_Column B;
                F_Column C;
            };
            F_Column Columns[
                3 // payload
                + 1 // padding
            ];
            F_Element Elements[
                (
                    3 // payload
                    + 1 // padding
                )
                * (
                    3 // payload
                    + 1 // padding
                )
            ];
        };

        /**
         * @brief Default constructor
         */
        constexpr TF_Data() noexcept = default;
        
        /**
         * @brief Construct a matrix with uniform value in all elements
         * @param Uniform Value to use for all elements
         */
        ABYTEK_FORCE_INLINE TF_Data(F_Element Uniform) noexcept :
            A(Uniform),
            B(Uniform),
            C(Uniform)
        {}
        
        /**
         * @brief Construct a matrix from three column vectors
         * @param InA First column
         * @param InB Second column
         * @param InC Third column
         */
        ABYTEK_FORCE_INLINE TF_Data(PA_Column InA, PA_Column InB, PA_Column InC) noexcept :
            A(InA),
            B(InB),
            C(InC)
        {}
        
        /**
         * @brief Copy constructor
         * @param Other Matrix to copy
         */
        ABYTEK_FORCE_INLINE TF_Data(const TF_Data& Other) noexcept :
            A(Other.A),
            B(Other.B),
            C(Other.C)
        {}
        
        /**
         * @brief Assignment operator
         * @param Other Matrix to copy
         * @return Reference to this matrix
         */
        ABYTEK_FORCE_INLINE TF_Data& operator = (F_PassedArgument Other) noexcept
        {
            A = Other.A;
            B = Other.B;
            C = Other.C;
            return *this;
        }
        
        explicit TF_Data(typename TF_RebindElement<I32>::F_PassedArgument Other) noexcept;
        TF_Data& operator = (typename TF_RebindElement<I32>::F_PassedArgument Other) noexcept;
        
        explicit TF_Data(typename TF_RebindElement<F32>::F_PassedArgument Other) noexcept;
        TF_Data& operator = (typename TF_RebindElement<F32>::F_PassedArgument Other) noexcept;
        
        explicit TF_Data(typename TF_RebindElement<U32>::F_PassedArgument Other) noexcept;
        TF_Data& operator = (typename TF_RebindElement<U32>::F_PassedArgument Other) noexcept;
        
        explicit TF_Data(typename TF_RebindElement<U8>::F_PassedArgument Other) noexcept;
        TF_Data& operator = (typename TF_RebindElement<U8>::F_PassedArgument Other) noexcept;
        
        explicit TF_Data(typename TF_RebindElement<I8>::F_PassedArgument Other) noexcept;
        TF_Data& operator = (typename TF_RebindElement<I8>::F_PassedArgument Other) noexcept;
        
        explicit TF_Data(typename TF_RebindElement<I16>::F_PassedArgument Other) noexcept;
        TF_Data& operator = (typename TF_RebindElement<I16>::F_PassedArgument Other) noexcept;
        
        explicit TF_Data(typename TF_RebindElement<I64>::F_PassedArgument Other) noexcept;
        TF_Data& operator = (typename TF_RebindElement<I64>::F_PassedArgument Other) noexcept;
        
        explicit TF_Data(typename TF_RebindElement<U64>::F_PassedArgument Other) noexcept;
        TF_Data& operator = (typename TF_RebindElement<U64>::F_PassedArgument Other) noexcept;

        /**
         * @brief Access column by index (lvalue reference)
         * @param Idx Column index (0-2)
         * @return Reference to the column
         */
        ABYTEK_FORCE_INLINE F_Column& operator [] (U16 Idx) & noexcept
        {
            ABYTEK_BASE_MATH_ASSERT(Idx < ColumnCount) << "Idx out of bounds";
            return Columns[Idx];
        }
        
        /**
         * @brief Access column by index (rvalue reference)
         * @param Idx Column index (0-2)
         * @return Rvalue reference to the column
         */
        ABYTEK_FORCE_INLINE F_Column&& operator [] (U16 Idx) && noexcept
        {
            ABYTEK_BASE_MATH_ASSERT(Idx < ColumnCount) << "Idx out of bounds";
            return boost::move(Columns[Idx]);
        }
        
        /**
         * @brief Access column by index (const lvalue reference)
         * @param Idx Column index (0-2)
         * @return Const reference to the column
         */
        ABYTEK_FORCE_INLINE PA_Column operator [] (U16 Idx) const & noexcept
        {
            ABYTEK_BASE_MATH_ASSERT(Idx < ColumnCount) << "Idx out of bounds";
            return Columns[Idx];
        }
        
        /**
         * @brief Access column by index (const rvalue reference)
         * @param Idx Column index (0-2)
         * @return Const reference to the column
         */
        ABYTEK_FORCE_INLINE PA_Column operator [] (U16 Idx) const && noexcept
        {
            ABYTEK_BASE_MATH_ASSERT(Idx < ColumnCount) << "Idx out of bounds";
            return Columns[Idx];
        }

        /**
         * @brief Equality comparison
         * @param A Left-hand matrix
         * @param B Right-hand matrix
         * @return True if all elements are equal
         */
        friend ABYTEK_FORCE_INLINE B8 operator == (F_PassedArgument A, F_PassedArgument B) noexcept
        {
            return (
                (A.A == B.A)
                && (A.B == B.B)
                && (A.C == B.C)
            );
        }
        
        /**
         * @brief Inequality comparison
         * @param A Left-hand matrix
         * @param B Right-hand matrix
         * @return True if any elements differ
         */
        friend ABYTEK_FORCE_INLINE B8 operator != (F_PassedArgument A, F_PassedArgument B) noexcept
        {
            return (
                (A.A != B.A)
                || (A.B != B.B)
                || (A.C != B.C)
            );
        }

        /**
         * @brief Reinterpret as a matrix with different configuration
         * @tparam __F_Config2 Target configuration type
         * @return Reference to reinterpreted matrix
         */
        template<typename __F_Config2>
        ABYTEK_FORCE_INLINE TF_RebindConfig<__F_Config2>& RebindConfig() & noexcept
        {
            return (TF_RebindConfig<__F_Config2>&)*this;
        }
        
        /**
         * @brief Reinterpret as a matrix with different configuration (const version)
         * @tparam __F_Config2 Target configuration type
         * @return Const reference to reinterpreted matrix
         */
        template<typename __F_Config2>
        ABYTEK_FORCE_INLINE TF_RebindConfig<__F_Config2> const & RebindConfig() const & noexcept
        {
            return (TF_RebindConfig<__F_Config2> const &)*this;
        }
        
        /**
         * @brief Reinterpret as a matrix with different configuration (rvalue version)
         * @tparam __F_Config2 Target configuration type
         * @return Rvalue reference to reinterpreted matrix
         */
        template<typename __F_Config2>
        ABYTEK_FORCE_INLINE TF_RebindConfig<__F_Config2>&& RebindConfig() && noexcept
        {
            return (TF_RebindConfig<__F_Config2>&&)*this;
        }
        
        /**
         * @brief Reinterpret as a matrix with different configuration (const rvalue version)
         * @tparam __F_Config2 Target configuration type
         * @return Const rvalue reference to reinterpreted matrix
         */
        template<typename __F_Config2>
        ABYTEK_FORCE_INLINE TF_RebindConfig<__F_Config2> const && RebindConfig() const && noexcept
        {
            return (TF_RebindConfig<__F_Config2> const &&)*this;
        }

        /**
         * @brief Create a new matrix by permuting columns
         * @tparam __ColumnIndices Indices of columns to include
         * @return New matrix with permuted columns
         */
        template<U32... __ColumnIndices>
        ABYTEK_FORCE_INLINE TF_RebindColumnCount<sizeof...(__ColumnIndices)> StaticPermute() const noexcept
        {
            return { Columns[__ColumnIndices]... };
        }
        
        /**
         * @brief Get the top-left 2x2 submatrix
         * @return 2x2 matrix from the top-left corner
         */
        ABYTEK_FORCE_INLINE F_SmallerRectangle GetSmallerTL() const noexcept
        {
            return {
                Columns[0].template StaticPermute<0, 1>(),
                Columns[1].template StaticPermute<0, 1>()
            };
        }
        
        /**
         * @brief Create a matrix from a 2x2 submatrix in the top-left corner
         * @param Smaller 2x2 matrix to place in the top-left corner
         * @return New 3x3 matrix with the specified submatrix in the top-left corner
         */
        static ABYTEK_FORCE_INLINE TF_Data FromSmallerTL(PA_SmallerRectangle Smaller) noexcept
        {
            TF_Data Result;
            Result.Columns[0] = F_Column { Smaller.Columns[0], 0 };
            Result.Columns[1] = F_Column { Smaller.Columns[1], 0 };
            return Result;
        }
        
        /**
         * @brief Get the top-right 2x2 submatrix
         * @return 2x2 matrix from the top-right corner
         */
        ABYTEK_FORCE_INLINE F_SmallerRectangle GetSmallerTR() const noexcept
        {
            return {
                Columns[1].template StaticPermute<0, 1>(),
                Columns[2].template StaticPermute<0, 1>()
            };
        }
        
        /**
         * @brief Create a matrix from a 2x2 submatrix in the top-right corner
         * @param Smaller 2x2 matrix to place in the top-right corner
         * @return New 3x3 matrix with the specified submatrix in the top-right corner
         */
        static ABYTEK_FORCE_INLINE TF_Data FromSmallerTR(PA_SmallerRectangle Smaller) noexcept
        {
            TF_Data Result;
            Result.Columns[1] = F_Column { Smaller.Columns[0], 0 };
            Result.Columns[2] = F_Column { Smaller.Columns[1], 0 };
            return Result;
        }
        
        /**
         * @brief Get the bottom-left 2x2 submatrix
         * @return 2x2 matrix from the bottom-left corner
         */
        ABYTEK_FORCE_INLINE F_SmallerRectangle GetSmallerBL() const noexcept
        {
            return {
                Columns[0].template StaticPermute<1, 2>(),
                Columns[1].template StaticPermute<1, 2>()
            };
        }
        
        /**
         * @brief Create a matrix from a 2x2 submatrix in the bottom-left corner
         * @param Smaller 2x2 matrix to place in the bottom-left corner
         * @return New 3x3 matrix with the specified submatrix in the bottom-left corner
         */
        static ABYTEK_FORCE_INLINE TF_Data FromSmallerBL(PA_SmallerRectangle Smaller) noexcept
        {
            TF_Data Result;
            Result.Columns[0] = F_Column { 0, Smaller.Columns[0] };
            Result.Columns[1] = F_Column { 0, Smaller.Columns[1] };
            return Result;
        }
        
        /**
         * @brief Get the bottom-right 2x2 submatrix
         * @return 2x2 matrix from the bottom-right corner
         */
        ABYTEK_FORCE_INLINE F_SmallerRectangle GetSmallerBR() const noexcept
        {
            return {
                Columns[1].template StaticPermute<1, 2>(),
                Columns[2].template StaticPermute<1, 2>()
            };
        }
        
        /**
         * @brief Create a matrix from a 2x2 submatrix in the bottom-right corner
         * @param Smaller 2x2 matrix to place in the bottom-right corner
         * @return New 3x3 matrix with the specified submatrix in the bottom-right corner
         */
        static ABYTEK_FORCE_INLINE TF_Data FromSmallerBR(PA_SmallerRectangle Smaller) noexcept
        {
            TF_Data Result;
            Result.Columns[1] = F_Column { 0, Smaller.Columns[0] };
            Result.Columns[2] = F_Column { 0, Smaller.Columns[1] };
            return Result;
        }
        
        /**
         * @brief Create a matrix filled with zeros
         * @return Matrix with all elements set to 0
         */
        static ABYTEK_FORCE_INLINE TF_Data Zero() noexcept
        {
            return {
                0
            };
        }
        
        /**
         * @brief Create a matrix filled with ones
         * @return Matrix with all elements set to 1
         */
        static ABYTEK_FORCE_INLINE TF_Data One() noexcept
        {
            return {
                1
            };
        }
    };
    template<class __F_Config>
    ABYTEK_REFLECT(TF_Data<3, 3, U16, __F_Config>)
    {
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE_ADVANCED(A, F_Column, sizeof(F_Column) * 0);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE_ADVANCED(B, F_Column, sizeof(F_Column) * 1);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE_ADVANCED(C, F_Column, sizeof(F_Column) * 2);
    }
}