#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/DataTemplate.hpp"


namespace Abytek
{
    /**
     * @brief Template specialization for 2D vector with unsigned integer (U16) elements
     * 
     * Implements a 2D vector with X and Y components of type U16 (32-bit unsigned integer).
     * Provides various utility methods for vector operations and predefined constants.
     * 
     * @tparam __F_Config Configuration type for specialized behavior
     */
    template<class __F_Config>
    struct TF_Data<2, 1, U16, __F_Config> : __F_Config::template TA_DefaultData<2, 1, U16, __F_Config>
    {
        using F_This = TF_Data;
        ABYTEK_BEGIN_REFLECTOR(I_Serializable)
        ABYTEK_END_REFLECTOR(F_This);
        
        static constexpr U16 RowCount = 2;
        static constexpr U16 ColumnCount = 1;

        using F_Element = U16;
        using F_Config = __F_Config;

        using F_PassedArgument = const TF_Data&;
        
        /**
         * @brief Type alias for rebinding to different row count
         * @tparam __RowCount New row count
         */
        template<U32 __RowCount>
        using TF_RebindRowCount = TF_Data<__RowCount, ColumnCount, F_Element, F_Config>;
        
        /**
         * @brief Type alias for rebinding to different row count (passed argument version)
         * @tparam __RowCount New row count
         */
        template<U32 __RowCount>
        using TPA_RebindRowCount = TPA_Data<__RowCount, ColumnCount, F_Element, F_Config>;
        
        /**
         * @brief Type alias for rebinding to different column count
         * @tparam __ColumnCount New column count
         */
        template<U32 __ColumnCount>
        using TF_RebindColumnCount = TF_Data<RowCount, __ColumnCount, F_Element, F_Config>;
        
        /**
         * @brief Type alias for rebinding to different column count (passed argument version)
         * @tparam __ColumnCount New column count
         */
        template<U32 __ColumnCount>
        using TPA_RebindColumnCount = TPA_Data<RowCount, __ColumnCount, F_Element, F_Config>;
        
        /**
         * @brief Type alias for rebinding to different row and column count
         * @tparam __RowCount New row count
         * @tparam __ColumnCount New column count
         */
        template<U32 __RowCount, U32 __ColumnCount>
        using TF_RebindRowColumnCount = TF_Data<__RowCount, __ColumnCount, F_Element, F_Config>;
        
        /**
         * @brief Type alias for rebinding to different row and column count (passed argument version)
         * @tparam __RowCount New row count
         * @tparam __ColumnCount New column count
         */
        template<U32 __RowCount, U32 __ColumnCount>
        using TPA_RebindRowColumnCount = TPA_Data<__RowCount, __ColumnCount, F_Element, F_Config>;
        
        /**
         * @brief Type alias for rebinding to different element type
         * @tparam __F_OtherElement New element type
         */
        template<typename __F_OtherElement>
        using TF_RebindElement = TF_Data<RowCount, ColumnCount, __F_OtherElement, F_Config>;
        
        /**
         * @brief Type alias for rebinding to different configuration
         * @tparam __F_Config2 New configuration type
         */
        template<class __F_Config2>
        using TF_RebindConfig = TF_Data<RowCount, ColumnCount, F_Element, __F_Config2>;
        
        /**
         * @brief Union for accessing vector components by name or by array index
         */
        union
        {
            struct
            {
                F_Element X;  /**< X component of the vector */
                F_Element Y;  /**< Y component of the vector */
            };
            F_Element Elements[2];  /**< Array access to vector elements */
        };

        /**
         * @brief Default constructor
         */
        constexpr TF_Data() noexcept = default;
        
        /**
         * @brief Constructor setting all elements to the same value
         * @param Uniform Value to set for all elements
         */
        ABYTEK_FORCE_INLINE TF_Data(F_Element Uniform) noexcept :
            X(Uniform),
            Y(Uniform)
        {}
        
        /**
         * @brief Constructor with individual component values
         * @param InX Value for X component
         * @param InY Value for Y component
         */
        ABYTEK_FORCE_INLINE TF_Data(F_Element InX, F_Element InY) noexcept :
            X(InX),
            Y(InY)
        {}
        
        /**
         * @brief Copy constructor
         * @param Other Vector to copy
         */
        ABYTEK_FORCE_INLINE TF_Data(const TF_Data& Other) noexcept :
            X(Other.X),
            Y(Other.Y)
        {}
        
        /**
         * @brief Assignment operator
         * @param Other Vector to copy from
         * @return Reference to this vector
         */
        ABYTEK_FORCE_INLINE TF_Data& operator = (F_PassedArgument Other) noexcept
        {
            X = Other.X;
            Y = Other.Y;
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
         * @brief Index access operator (lvalue reference)
         * @param Idx Index of the element to access
         * @return Reference to the element at the specified index
         */
        ABYTEK_FORCE_INLINE F_Element& operator [] (I32 Idx) & noexcept
        {
            ABYTEK_BASE_MATH_ASSERT(Idx < RowCount) << "Idx out of bounds";
            return Elements[Idx];
        }
        
        /**
         * @brief Index access operator (rvalue reference)
         * @param Idx Index of the element to access
         * @return Rvalue reference to the element at the specified index
         */
        ABYTEK_FORCE_INLINE F_Element&& operator [] (I32 Idx) && noexcept
        {
            ABYTEK_BASE_MATH_ASSERT(Idx < RowCount) << "Idx out of bounds";
            return boost::move(Elements[Idx]);
        }
        
        /**
         * @brief Index access operator (const lvalue reference)
         * @param Idx Index of the element to access
         * @return Value of the element at the specified index
         */
        ABYTEK_FORCE_INLINE F_Element operator [] (I32 Idx) const & noexcept
        {
            ABYTEK_BASE_MATH_ASSERT(Idx < RowCount) << "Idx out of bounds";
            return Elements[Idx];
        }
        
        /**
         * @brief Index access operator (const rvalue reference)
         * @param Idx Index of the element to access
         * @return Value of the element at the specified index
         */
        ABYTEK_FORCE_INLINE F_Element operator [] (I32 Idx) const && noexcept
        {
            ABYTEK_BASE_MATH_ASSERT(Idx < RowCount) << "Idx out of bounds";
            return Elements[Idx];
        }

        /**
         * @brief Equality comparison operator
         * @param A First vector
         * @param B Second vector
         * @return True if vectors are equal, false otherwise
         */
        friend ABYTEK_FORCE_INLINE B8 operator == (F_PassedArgument A, F_PassedArgument B) noexcept
        {
            return (
                (A.X == B.X)
                && (A.Y == B.Y)
            );
        }
        
        /**
         * @brief Inequality comparison operator
         * @param A First vector
         * @param B Second vector
         * @return True if vectors are not equal, false otherwise
         */
        friend ABYTEK_FORCE_INLINE B8 operator != (F_PassedArgument A, F_PassedArgument B) noexcept
        {
            return (
                (A.X != B.X)
                || (A.Y != B.Y)
            );
        }

        /**
         * @brief Rebind configuration (lvalue reference)
         * @tparam __F_Config2 Target configuration type
         * @return Reference with rebound configuration
         */
        template<typename __F_Config2>
        ABYTEK_FORCE_INLINE TF_RebindConfig<__F_Config2>& RebindConfig() & noexcept
        {
            return (TF_RebindConfig<__F_Config2>&)*this;
        }
        
        /**
         * @brief Rebind configuration (const lvalue reference)
         * @tparam __F_Config2 Target configuration type
         * @return Const reference with rebound configuration
         */
        template<typename __F_Config2>
        ABYTEK_FORCE_INLINE TF_RebindConfig<__F_Config2> const & RebindConfig() const & noexcept
        {
            return (TF_RebindConfig<__F_Config2> const &)*this;
        }
        
        /**
         * @brief Rebind configuration (rvalue reference)
         * @tparam __F_Config2 Target configuration type
         * @return Rvalue reference with rebound configuration
         */
        template<typename __F_Config2>
        ABYTEK_FORCE_INLINE TF_RebindConfig<__F_Config2>&& RebindConfig() && noexcept
        {
            return (TF_RebindConfig<__F_Config2>&&)*this;
        }
        
        /**
         * @brief Rebind configuration (const rvalue reference)
         * @tparam __F_Config2 Target configuration type
         * @return Const rvalue reference with rebound configuration
         */
        template<typename __F_Config2>
        ABYTEK_FORCE_INLINE TF_RebindConfig<__F_Config2> const && RebindConfig() const && noexcept
        {
            return (TF_RebindConfig<__F_Config2> const &&)*this;
        }

        /**
         * @brief Create a permutation of the vector's elements
         * @tparam __ElementIndices Indices specifying the order of elements in the result
         * @return New vector with permuted elements
         */
        template<U32... __ElementIndices>
        ABYTEK_FORCE_INLINE TF_RebindRowCount<sizeof...(__ElementIndices)> StaticPermute() const noexcept
        {
            return { Elements[__ElementIndices]... };
        }
        
        /**
         * @brief Create a zero vector (0,0)
         * @return Zero initialized vector
         */
        static ABYTEK_FORCE_INLINE TF_Data Zero() noexcept
        {
            return {
                0
            };
        }
        
        /**
         * @brief Create a vector with all ones (1,1)
         * @return Vector with all elements set to 1
         */
        static ABYTEK_FORCE_INLINE TF_Data One() noexcept
        {
            return {
                1
            };
        }
        
        /**
         * @brief Create a unit vector pointing right (1,0)
         * @return Right-pointing unit vector
         */
        static ABYTEK_FORCE_INLINE TF_Data Right() noexcept
        {
            return {
                1,
                0
            };
        }
        
        /**
         * @brief Create a unit vector pointing up (0,1)
         * @return Up-pointing unit vector
         */
        static ABYTEK_FORCE_INLINE TF_Data Up() noexcept
        {
            return {
                0,
                1
            };
        }
        
        /**
         * @brief Create a unit vector pointing left (-1,0)
         * @return Left-pointing unit vector
         */
        static ABYTEK_FORCE_INLINE TF_Data Left() noexcept
        {
            return {
                -1,
                0
            };
        }
        
        /**
         * @brief Create a unit vector pointing down (0,-1)
         * @return Down-pointing unit vector
         */
        static ABYTEK_FORCE_INLINE TF_Data Down() noexcept
        {
            return {
                0,
                -1
            };
        }
    };
    template<class __F_Config>
    ABYTEK_REFLECT(TF_Data<2, 1, U16, __F_Config>)
    {
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE_ADVANCED(X, F_Element, sizeof(F_Element) * 0);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE_ADVANCED(Y, F_Element, sizeof(F_Element) * 1);
    }
}