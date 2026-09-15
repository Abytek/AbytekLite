/**
 * @file Data3_U64.hpp
 * @brief Defines a 3D vector with 32-bit unsigned integer elements
 */
#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/DataTemplate.hpp"
#include "Abytek/Types/Data2_U64.hpp"


namespace Abytek
{
    /**
     * @brief A 3D vector with U64 (unsigned int) elements
     * 
     * Vector components can be accessed as X, Y, Z or as array elements.
     * SSE2 optimization is used when available.
     * 
     * @tparam __F_Config Configuration type for the vector
     */
    template<class __F_Config>
    struct TF_Data<3, 1, U64, __F_Config> : __F_Config::template TA_DefaultData<3, 1, U64, __F_Config>
    {
        using F_This = TF_Data;
        ABYTEK_BEGIN_REFLECTOR(I_Serializable)
        ABYTEK_END_REFLECTOR(F_This)
        
        static constexpr U64 RowCount = 3;
        static constexpr U64 ColumnCount = 1;

        using F_Element = U64;
        using F_Config = __F_Config;

        using F_PassedArgument = const TF_Data&;
        
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
        
        /**
         * @brief Vector data storage
         * 
         * Accessible as X, Y, Z components or as array elements
         */
        union
        {
            struct
            {
                F_Element X;
                F_Element Y;
                F_Element Z;
            };
            F_Element Elements[
                3 // payload 
                + 1 // padding
            ];
        };

        /**
         * @brief Default constructor
         */
        constexpr TF_Data() noexcept = default;
        
        /**
         * @brief Construct a vector with uniform value for all components
         * @param Uniform Value to use for all components
         */
        ABYTEK_FORCE_INLINE TF_Data(F_Element Uniform) noexcept :
            X(Uniform),
            Y(Uniform),
            Z(Uniform)
        {}
        
        /**
         * @brief Construct a vector from individual components
         * @param InX X component
         * @param InY Y component
         * @param InZ Z component
         */
        ABYTEK_FORCE_INLINE TF_Data(F_Element InX, F_Element InY, F_Element InZ) noexcept :
            X(InX),
            Y(InY),
            Z(InZ)
        {}
        
        /**
         * @brief Construct a vector from a 2D vector and Z component
         * @param InXY 2D vector for X and Y components
         * @param InZ Z component
         */
        ABYTEK_FORCE_INLINE TF_Data(TPA_RebindRowCount<RowCount - 1> InXY, F_Element InZ) noexcept :
            X(InXY.X),
            Y(InXY.Y),
            Z(InZ)
        {}
        
        /**
         * @brief Construct a vector from X component and a 2D vector
         * @param InX X component
         * @param InYZ 2D vector for Y and Z components
         */
        ABYTEK_FORCE_INLINE TF_Data(F_Element InX, TPA_RebindRowCount<RowCount - 1> InYZ) noexcept :
            X(InX),
            Y(InYZ.X),
            Z(InYZ.Y)
        {}
        
        /**
         * @brief Copy constructor
         * @param Other Vector to copy
         */
        ABYTEK_FORCE_INLINE TF_Data(const TF_Data& Other) noexcept :
            X(Other.X),
            Y(Other.Y),
            Z(Other.Z)
        {}
        
        /**
         * @brief Assignment operator
         * @param Other Vector to copy
         * @return Reference to this vector
         */
        ABYTEK_FORCE_INLINE TF_Data& operator = (F_PassedArgument Other) noexcept
        {
            X = Other.X;
            Y = Other.Y;
            Z = Other.Z;
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
        
        explicit TF_Data(typename TF_RebindElement<I8>::F_PassedArgument Other) noexcept;
        TF_Data& operator = (typename TF_RebindElement<I8>::F_PassedArgument Other) noexcept;
        
        explicit TF_Data(typename TF_RebindElement<I16>::F_PassedArgument Other) noexcept;
        TF_Data& operator = (typename TF_RebindElement<I16>::F_PassedArgument Other) noexcept;
        
        explicit TF_Data(typename TF_RebindElement<I64>::F_PassedArgument Other) noexcept;
        TF_Data& operator = (typename TF_RebindElement<I64>::F_PassedArgument Other) noexcept;
        
        explicit TF_Data(typename TF_RebindElement<U8>::F_PassedArgument Other) noexcept;
        TF_Data& operator = (typename TF_RebindElement<U8>::F_PassedArgument Other) noexcept;

        /**
         * @brief Access element by index (lvalue reference)
         * @param Idx Element index (0-2)
         * @return Reference to the element
         */
        ABYTEK_FORCE_INLINE F_Element& operator [] (U64 Idx) & noexcept
        {
            ABYTEK_BASE_MATH_ASSERT(Idx < RowCount) << "Idx out of bounds";
            return Elements[Idx];
        }
        
        /**
         * @brief Access element by index (rvalue reference)
         * @param Idx Element index (0-2)
         * @return Rvalue reference to the element
         */
        ABYTEK_FORCE_INLINE F_Element&& operator [] (U64 Idx) && noexcept
        {
            ABYTEK_BASE_MATH_ASSERT(Idx < RowCount) << "Idx out of bounds";
            return boost::move(Elements[Idx]);
        }
        
        /**
         * @brief Access element by index (const lvalue reference)
         * @param Idx Element index (0-2)
         * @return Value of the element
         */
        ABYTEK_FORCE_INLINE F_Element operator [] (U64 Idx) const & noexcept
        {
            ABYTEK_BASE_MATH_ASSERT(Idx < RowCount) << "Idx out of bounds";
            return Elements[Idx];
        }
        
        /**
         * @brief Access element by index (const rvalue reference)
         * @param Idx Element index (0-2)
         * @return Value of the element
         */
        ABYTEK_FORCE_INLINE F_Element operator [] (U64 Idx) const && noexcept
        {
            ABYTEK_BASE_MATH_ASSERT(Idx < RowCount) << "Idx out of bounds";
            return Elements[Idx];
        }

        /**
         * @brief Equality comparison
         * @param A Left-hand vector
         * @param B Right-hand vector
         * @return True if all components are equal
         */
        friend ABYTEK_FORCE_INLINE B8 operator == (F_PassedArgument A, F_PassedArgument B) noexcept
        {
            return (
                (A.X == B.X)
                && (A.Y == B.Y)
                && (A.Z == B.Z)
            );
        }
        
        /**
         * @brief Inequality comparison
         * @param A Left-hand vector
         * @param B Right-hand vector
         * @return True if any components differ
         */
        friend ABYTEK_FORCE_INLINE B8 operator != (F_PassedArgument A, F_PassedArgument B) noexcept
        {
            return (
                (A.X != B.X)
                || (A.Y != B.Y)
                || (A.Z != B.Z)
            );
        }

        /**
         * @brief Reinterpret as a vector with different configuration
         * @tparam __F_Config2 Target configuration type
         * @return Reference to reinterpreted vector
         */
        template<typename __F_Config2>
        ABYTEK_FORCE_INLINE TF_RebindConfig<__F_Config2>& RebindConfig() & noexcept
        {
            return (TF_RebindConfig<__F_Config2>&)*this;
        }
        
        /**
         * @brief Reinterpret as a vector with different configuration (const version)
         * @tparam __F_Config2 Target configuration type
         * @return Const reference to reinterpreted vector
         */
        template<typename __F_Config2>
        ABYTEK_FORCE_INLINE TF_RebindConfig<__F_Config2> const & RebindConfig() const & noexcept
        {
            return (TF_RebindConfig<__F_Config2> const &)*this;
        }
        
        /**
         * @brief Reinterpret as a vector with different configuration (rvalue version)
         * @tparam __F_Config2 Target configuration type
         * @return Rvalue reference to reinterpreted vector
         */
        template<typename __F_Config2>
        ABYTEK_FORCE_INLINE TF_RebindConfig<__F_Config2>&& RebindConfig() && noexcept
        {
            return (TF_RebindConfig<__F_Config2>&&)*this;
        }
        
        /**
         * @brief Reinterpret as a vector with different configuration (const rvalue version)
         * @tparam __F_Config2 Target configuration type
         * @return Const rvalue reference to reinterpreted vector
         */
        template<typename __F_Config2>
        ABYTEK_FORCE_INLINE TF_RebindConfig<__F_Config2> const && RebindConfig() const && noexcept
        {
            return (TF_RebindConfig<__F_Config2> const &&)*this;
        }

        /**
         * @brief Create a new vector by permuting elements
         * @tparam __ElementIndices Indices of elements to include
         * @return New vector with permuted elements
         */
        template<U32... __ElementIndices>
        ABYTEK_FORCE_INLINE TF_RebindRowCount<sizeof...(__ElementIndices)> StaticPermute() const noexcept
        {
            return { Elements[__ElementIndices]... };
        }
        
        /**
         * @brief Create a vector filled with zeros
         * @return Vector with all components set to 0
         */
        static ABYTEK_FORCE_INLINE TF_Data Zero() noexcept
        {
            return {
                0
            };
        }
        
        /**
         * @brief Create a vector filled with ones
         * @return Vector with all components set to 1
         */
        static ABYTEK_FORCE_INLINE TF_Data One() noexcept
        {
            return {
                1
            };
        }
        
        /**
         * @brief Create a unit vector pointing right (1,0,0)
         * @return Unit vector (1,0,0)
         */
        static ABYTEK_FORCE_INLINE TF_Data Right() noexcept
        {
            return {
                1,
                0,
                0
            };
        }
        
        /**
         * @brief Create a unit vector pointing up (0,1,0)
         * @return Unit vector (0,1,0)
         */
        static ABYTEK_FORCE_INLINE TF_Data Up() noexcept
        {
            return {
                0,
                1,
                0
            };
        }
        
        /**
         * @brief Create a unit vector pointing forward (0,0,1)
         * @return Unit vector (0,0,1)
         */
        static ABYTEK_FORCE_INLINE TF_Data Forward() noexcept
        {
            return {
                0,
                0,
                1
            };
        }
        
        /**
         * @brief Create a unit vector pointing left (-1,0,0)
         * @return Unit vector (-1,0,0)
         */
        static ABYTEK_FORCE_INLINE TF_Data Left() noexcept
        {
            return {
                -1,
                0,
                0
            };
        }
        
        /**
         * @brief Create a unit vector pointing down (0,-1,0)
         * @return Unit vector (0,-1,0)
         */
        static ABYTEK_FORCE_INLINE TF_Data Down() noexcept
        {
            return {
                0,
                -1,
                0
            };
        }
        
        /**
         * @brief Create a unit vector pointing backward (0,0,-1)
         * @return Unit vector (0,0,-1)
         */
        static ABYTEK_FORCE_INLINE TF_Data Backward() noexcept
        {
            return {
                0,
                0,
                -1
            };
        }
    };
    template<class __F_Config>
    ABYTEK_REFLECT(TF_Data<3, 1, U64, __F_Config>)
    {
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE_ADVANCED(X, F_Element, sizeof(F_Element) * 0);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE_ADVANCED(Y, F_Element, sizeof(F_Element) * 1);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE_ADVANCED(Z, F_Element, sizeof(F_Element) * 2);
    }
}