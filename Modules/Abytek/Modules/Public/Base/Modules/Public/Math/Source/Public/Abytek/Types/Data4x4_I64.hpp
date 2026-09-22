#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/DataTemplate.hpp"
#include "Abytek/Types/Data4_I64.hpp"
#include "Abytek/Types/Data3x3_I64.hpp"


namespace Abytek
{
    /**
     * @brief 4x4 Matrix of 32-bit unsigned integer values
     * 
     * Specialized implementation of a 4x4 matrix using I64 (unsigned integer) elements.
     * Provides storage and operations for 4x4 matrices with unsigned integer elements.
     */
    template<class __F_Config>
    struct TF_Data<4, 4, I64, __F_Config> : __F_Config::template TA_DefaultData<4, 4, I64, __F_Config>
    {
        using F_This = TF_Data;
        ABYTEK_BEGIN_REFLECTOR(I_Serializable)
        ABYTEK_END_REFLECTOR(F_This);
        
        static constexpr I64 RowCount = 4;
        static constexpr I64 ColumnCount = 4;

        using F_Element = I64;
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
        
        union
        {
            struct
            {
                F_Column A;
                F_Column B;
                F_Column C;
                F_Column D;
            };
            F_Column Columns[4];
            F_Element Elements[4 * 4];
        };

        /// @brief Default constructor, creates an uninitialized matrix
        constexpr TF_Data() noexcept = default;
        
        /// @brief Creates a matrix with all elements set to the specified uniform value
        /// @param Uniform Value to set for all elements
        ABYTEK_FORCE_INLINE TF_Data(F_Element Uniform) noexcept :
            A(Uniform),
            B(Uniform),
            C(Uniform),
            D(Uniform)
        {}
        
        /// @brief Creates a matrix from four column vectors
        /// @param InA First column vector
        /// @param InB Second column vector
        /// @param InC Third column vector
        /// @param InD Fourth column vector
        ABYTEK_FORCE_INLINE TF_Data(PA_Column InA, PA_Column InB, PA_Column InC, PA_Column InD) noexcept :
            A(InA),
            B(InB),
            C(InC),
            D(InD)
        {}
        
        /// @brief Copy constructor
        /// @param Other Matrix to copy
        ABYTEK_FORCE_INLINE TF_Data(const TF_Data& Other) noexcept :
            A(Other.A),
            B(Other.B),
            C(Other.C),
            D(Other.D)
        {}
        
        /// @brief Assignment operator
        /// @param Other Matrix to copy
        /// @return Reference to this matrix
        ABYTEK_FORCE_INLINE TF_Data& operator = (F_PassedArgument Other) noexcept
        {
            A = Other.A;
            B = Other.B;
            C = Other.C;
            D = Other.D;
            return *this;
        }
        
        explicit TF_Data(typename TF_RebindElement<I32>::F_PassedArgument Other) noexcept;
        TF_Data& operator = (typename TF_RebindElement<I32>::F_PassedArgument Other) noexcept;
        
        explicit TF_Data(typename TF_RebindElement<F32>::F_PassedArgument Other) noexcept;
        TF_Data& operator = (typename TF_RebindElement<F32>::F_PassedArgument Other) noexcept;
        
        explicit TF_Data(typename TF_RebindElement<U32>::F_PassedArgument Other) noexcept;
        TF_Data& operator = (typename TF_RebindElement<U32>::F_PassedArgument Other) noexcept;
        
        explicit TF_Data(typename TF_RebindElement<U16>::F_PassedArgument Other) noexcept;
        TF_Data& operator = (typename TF_RebindElement<U16>::F_PassedArgument Other) noexcept;
        
        explicit TF_Data(typename TF_RebindElement<U8>::F_PassedArgument Other) noexcept;
        TF_Data& operator = (typename TF_RebindElement<U8>::F_PassedArgument Other) noexcept;
        
        explicit TF_Data(typename TF_RebindElement<I16>::F_PassedArgument Other) noexcept;
        TF_Data& operator = (typename TF_RebindElement<I16>::F_PassedArgument Other) noexcept;
        
        explicit TF_Data(typename TF_RebindElement<I8>::F_PassedArgument Other) noexcept;
        TF_Data& operator = (typename TF_RebindElement<I8>::F_PassedArgument Other) noexcept;
        
        explicit TF_Data(typename TF_RebindElement<U64>::F_PassedArgument Other) noexcept;
        TF_Data& operator = (typename TF_RebindElement<U64>::F_PassedArgument Other) noexcept;

        /// @brief Access column by index (non-const lvalue reference)
        /// @param Idx Column index (0-3)
        /// @return Reference to the specified column
        ABYTEK_FORCE_INLINE F_Column& operator [] (I32 Idx) & noexcept
        {
                        ABYTEK_BASE_MATH_ASSERT(Idx < ColumnCount) << "Idx out of bounds";
            return Columns[Idx];
        }
        
        /// @brief Access column by index (rvalue reference)
        /// @param Idx Column index (0-3)
        /// @return Rvalue reference to the specified column
        ABYTEK_FORCE_INLINE F_Column&& operator [] (I32 Idx) && noexcept
        {
                        ABYTEK_BASE_MATH_ASSERT(Idx < ColumnCount) << "Idx out of bounds";
            return boost::move(Columns[Idx]);
        }
        
        /// @brief Access column by index (const lvalue reference)
        /// @param Idx Column index (0-3)
        /// @return Const reference to the specified column
        ABYTEK_FORCE_INLINE PA_Column operator [] (I32 Idx) const & noexcept
        {
                        ABYTEK_BASE_MATH_ASSERT(Idx < ColumnCount) << "Idx out of bounds";
            return Columns[Idx];
        }
        
        /// @brief Access column by index (const rvalue reference)
        /// @param Idx Column index (0-3)
        /// @return Const rvalue reference to the specified column
        ABYTEK_FORCE_INLINE PA_Column operator [] (I32 Idx) const && noexcept
        {
                        ABYTEK_BASE_MATH_ASSERT(Idx < ColumnCount) << "Idx out of bounds";
            return Columns[Idx];
        }

        /// @brief Equality comparison operator
        /// @param A First matrix
        /// @param B Second matrix
        /// @return True if matrices are equal, false otherwise
        friend ABYTEK_FORCE_INLINE B8 operator == (F_PassedArgument A, F_PassedArgument B) noexcept
        {
            return (
                (A.A == B.A)
                && (A.B == B.B)
                && (A.C == B.C)
                && (A.D == B.D)
            );
        }
        
        /// @brief Inequality comparison operator
        /// @param A First matrix
        /// @param B Second matrix
        /// @return True if matrices are not equal, false otherwise
        friend ABYTEK_FORCE_INLINE B8 operator != (F_PassedArgument A, F_PassedArgument B) noexcept
        {
            return (
                (A.A != B.A)
                || (A.B != B.B)
                || (A.C != B.C)
                || (A.D != B.D)
            );
        }

        /// @brief Rebind configuration type (non-const lvalue reference)
        /// @return Reference with rebound configuration type
        template<typename __F_Config2>
        ABYTEK_FORCE_INLINE TF_RebindConfig<__F_Config2>& RebindConfig() & noexcept
        {
            return (TF_RebindConfig<__F_Config2>&)*this;
        }
        
        /// @brief Rebind configuration type (const lvalue reference)
        /// @return Const reference with rebound configuration type
        template<typename __F_Config2>
        ABYTEK_FORCE_INLINE TF_RebindConfig<__F_Config2> const & RebindConfig() const & noexcept
        {
            return (TF_RebindConfig<__F_Config2> const &)*this;
        }
        
        /// @brief Rebind configuration type (rvalue reference)
        /// @return Rvalue reference with rebound configuration type
        template<typename __F_Config2>
        ABYTEK_FORCE_INLINE TF_RebindConfig<__F_Config2>&& RebindConfig() && noexcept
        {
            return (TF_RebindConfig<__F_Config2>&&)*this;
        }
        
        /// @brief Rebind configuration type (const rvalue reference)
        /// @return Const rvalue reference with rebound configuration type
        template<typename __F_Config2>
        ABYTEK_FORCE_INLINE TF_RebindConfig<__F_Config2> const && RebindConfig() const && noexcept
        {
            return (TF_RebindConfig<__F_Config2> const &&)*this;
        }

        /// @brief Create new matrix by permuting columns
        /// @return Matrix with permuted columns
        template<U32... __ColumnIndices>
        ABYTEK_FORCE_INLINE TF_RebindColumnCount<sizeof...(__ColumnIndices)> StaticPermute() const noexcept
        {
            return { Columns[__ColumnIndices]... };
        }
        
        /// @brief Get the top-left 3x3 submatrix
        /// @return 3x3 matrix from the top-left of this matrix
        ABYTEK_FORCE_INLINE F_SmallerRectangle GetSmallerTL() const noexcept
        {
            return {
                Columns[0].template StaticPermute<0, 1, 2>(),
                Columns[1].template StaticPermute<0, 1, 2>(),
                Columns[2].template StaticPermute<0, 1, 2>()
            };
        }
        
        /// @brief Create a 4x4 matrix from a 3x3 matrix, placing it in the top-left corner
        /// @param Smaller 3x3 matrix to use
        /// @return 4x4 matrix with the 3x3 matrix in the top-left corner
        static ABYTEK_FORCE_INLINE TF_Data FromSmallerTL(PA_SmallerRectangle Smaller) noexcept
        {
            TF_Data Result;
            Result.Columns[0] = F_Column { Smaller.Columns[0], 0 };
            Result.Columns[1] = F_Column { Smaller.Columns[1], 0 };
            Result.Columns[2] = F_Column { Smaller.Columns[2], 0 };
            return Result;
        }
        
        /// @brief Get the top-right 3x3 submatrix
        /// @return 3x3 matrix from the top-right of this matrix
        ABYTEK_FORCE_INLINE F_SmallerRectangle GetSmallerTR() const noexcept
        {
            return {
                Columns[1].template StaticPermute<0, 1, 2>(),
                Columns[2].template StaticPermute<0, 1, 2>(),
                Columns[3].template StaticPermute<0, 1, 2>()
            };
        }
        
        /// @brief Create a 4x4 matrix from a 3x3 matrix, placing it in the top-right corner
        /// @param Smaller 3x3 matrix to use
        /// @return 4x4 matrix with the 3x3 matrix in the top-right corner
        static ABYTEK_FORCE_INLINE TF_Data FromSmallerTR(PA_SmallerRectangle Smaller) noexcept
        {
            TF_Data Result;
            Result.Columns[1] = F_Column { Smaller.Columns[0], 0 };
            Result.Columns[2] = F_Column { Smaller.Columns[1], 0 };
            Result.Columns[3] = F_Column { Smaller.Columns[2], 0 };
            return Result;
        }
        
        /// @brief Get the bottom-left 3x3 submatrix
        /// @return 3x3 matrix from the bottom-left of this matrix
        ABYTEK_FORCE_INLINE F_SmallerRectangle GetSmallerBL() const noexcept
        {
            return {
                Columns[0].template StaticPermute<1, 2, 3>(),
                Columns[1].template StaticPermute<1, 2, 3>(),
                Columns[2].template StaticPermute<1, 2, 3>()
            };
        }
        
        /// @brief Create a 4x4 matrix from a 3x3 matrix, placing it in the bottom-left corner
        /// @param Smaller 3x3 matrix to use
        /// @return 4x4 matrix with the 3x3 matrix in the bottom-left corner
        static ABYTEK_FORCE_INLINE TF_Data FromSmallerBL(PA_SmallerRectangle Smaller) noexcept
        {
            TF_Data Result;
            Result.Columns[0] = F_Column { 0, Smaller.Columns[0] };
            Result.Columns[1] = F_Column { 0, Smaller.Columns[1] };
            Result.Columns[2] = F_Column { 0, Smaller.Columns[2] };
            return Result;
        }
        
        /// @brief Get the bottom-right 3x3 submatrix
        /// @return 3x3 matrix from the bottom-right of this matrix
        ABYTEK_FORCE_INLINE F_SmallerRectangle GetSmallerBR() const noexcept
        {
            return {
                Columns[1].template StaticPermute<1, 2, 3>(),
                Columns[2].template StaticPermute<1, 2, 3>(),
                Columns[3].template StaticPermute<1, 2, 3>()
            };
        }
        
        /// @brief Create a 4x4 matrix from a 3x3 matrix, placing it in the bottom-right corner
        /// @param Smaller 3x3 matrix to use
        /// @return 4x4 matrix with the 3x3 matrix in the bottom-right corner
        static ABYTEK_FORCE_INLINE TF_Data FromSmallerBR(PA_SmallerRectangle Smaller) noexcept
        {
            TF_Data Result;
            Result.Columns[1] = F_Column { 0, Smaller.Columns[0] };
            Result.Columns[2] = F_Column { 0, Smaller.Columns[1] };
            Result.Columns[3] = F_Column { 0, Smaller.Columns[2] };
            return Result;
        }
        
        /// @brief Create a matrix with all elements set to zero
        /// @return Zero matrix
        static ABYTEK_FORCE_INLINE TF_Data Zero() noexcept
        {
            return {
                0
            };
        }
        
        /// @brief Create a matrix with all elements set to one
        /// @return Matrix with all ones
        static ABYTEK_FORCE_INLINE TF_Data One() noexcept
        {
            return {
                1
            };
        }
    };
    template<class __F_Config>
    ABYTEK_REFLECT(TF_Data<4, 4, I64, __F_Config>)
    {
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE_ADVANCED(A, F_Column, sizeof(F_Column) * 0);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE_ADVANCED(B, F_Column, sizeof(F_Column) * 1);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE_ADVANCED(C, F_Column, sizeof(F_Column) * 2);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE_ADVANCED(D, F_Column, sizeof(F_Column) * 3);
    }
}