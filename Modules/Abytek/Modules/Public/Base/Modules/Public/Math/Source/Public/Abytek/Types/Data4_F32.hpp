#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/DataTemplate.hpp"

/// @file Data4_F32.hpp
/// @brief Contains the specialized template definition for 4-dimensional float data

namespace Abytek
{
    /// @brief Specialized template for 4-dimensional float vectors
    /// @tparam __F_Config Configuration structure for the data type
    template<class __F_Config>
    struct TF_Data<4, 1, F32, __F_Config> : __F_Config::template TA_DefaultData<4, 1, F32, __F_Config>
    {
        using F_This = TF_Data;
        ABYTEK_BEGIN_REFLECTOR(I_Serializable)
        ABYTEK_END_REFLECTOR(F_This)
        
        static constexpr U32 RowCount = 4;
        static constexpr U32 ColumnCount = 1;

        using F_Element = F32;
        using F_Config = __F_Config;

#ifdef ABYTEK_ENABLE_SSE
        using F_PassedArgument = TF_Data;
#else
        using F_PassedArgument = const TF_Data&;
#endif
        
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
        
        /// @brief Union containing different ways to access the data
        union
        {
            /// @brief Access elements by coordinate names
            struct
            {
                F_Element X;  ///< X coordinate
                F_Element Y;  ///< Y coordinate
                F_Element Z;  ///< Z coordinate
                F_Element W;  ///< W coordinate
            };
            /// @brief Access elements by array index
            F_Element Elements[4];

#ifdef ABYTEK_ENABLE_SSE
            /// @brief SSE-optimized storage
            __m128 XYZW_SSE;
#endif
        };

        /// @brief Default constructor
        constexpr TF_Data() noexcept = default;
        
        /// @brief Construct with uniform value across all elements
        /// @param Uniform The value to set for all elements
        ABYTEK_FORCE_INLINE TF_Data(F_Element Uniform) noexcept :
            X(Uniform),
            Y(Uniform),
            Z(Uniform),
            W(Uniform)
        {}
        
        /// @brief Construct with individual values for each element
        /// @param InX X component value
        /// @param InY Y component value
        /// @param InZ Z component value
        /// @param InW W component value
        ABYTEK_FORCE_INLINE TF_Data(F_Element InX, F_Element InY, F_Element InZ, F_Element InW) noexcept :
            X(InX),
            Y(InY),
            Z(InZ),
            W(InW)
        {}
        
        /// @brief Construct with a 2D vector for XY and separate values for Z and W
        /// @param InXY Vector for X and Y components
        /// @param InZ Z component value
        /// @param InW W component value
        ABYTEK_FORCE_INLINE TF_Data(TPA_RebindRowCount<RowCount - 2> InXY, F_Element InZ, F_Element InW) noexcept :
            X(InXY.X),
            Y(InXY.Y),
            Z(InZ),
            W(InW)
        {}
        
        /// @brief Construct with separate values for X and Y and a 2D vector for ZW
        /// @param InX X component value
        /// @param InY Y component value
        /// @param InZW Vector for Z and W components
        ABYTEK_FORCE_INLINE TF_Data(F_Element InX, F_Element InY, TPA_RebindRowCount<RowCount - 2> InZW) noexcept :
            X(InX),
            Y(InY),
            Z(InZW.X),
            W(InZW.Y)
        {}
        
        /// @brief Construct with two 2D vectors for XY and ZW
        /// @param InXY Vector for X and Y components
        /// @param InZW Vector for Z and W components
        ABYTEK_FORCE_INLINE TF_Data(TPA_RebindRowCount<RowCount - 2> InXY, TPA_RebindRowCount<RowCount - 2> InZW) noexcept :
            X(InXY.X),
            Y(InXY.Y),
            Z(InZW.X),
            W(InZW.Y)
        {}
        
        /// @brief Construct with a 3D vector for XYZ and a separate value for W
        /// @param InXYZ Vector for X, Y, and Z components
        /// @param InW W component value
        ABYTEK_FORCE_INLINE TF_Data(TPA_RebindRowCount<RowCount - 1> InXYZ, F_Element InW) noexcept :
            X(InXYZ.X),
            Y(InXYZ.Y),
            Z(InXYZ.Z),
            W(InW)
        {}
        
        /// @brief Construct with a separate value for X and a 3D vector for YZW
        /// @param InX X component value
        /// @param InYZW Vector for Y, Z, and W components
        ABYTEK_FORCE_INLINE TF_Data(F_Element InX, TPA_RebindRowCount<RowCount - 1> InYZW) noexcept :
            X(InX),
            Y(InYZW.X),
            Z(InYZW.Y),
            W(InYZW.Z)
        {}
        
        /// @brief Copy constructor
        /// @param Other The vector to copy from
        ABYTEK_FORCE_INLINE TF_Data(const TF_Data& Other) noexcept :
            X(Other.X),
            Y(Other.Y),
            Z(Other.Z),
            W(Other.W)
        {}
        
        /// @brief Assignment operator
        /// @param Other The vector to copy from
        /// @return Reference to this vector
        ABYTEK_FORCE_INLINE TF_Data& operator = (F_PassedArgument Other) noexcept
        {
            X = Other.X;
            Y = Other.Y;
            Z = Other.Z;
            W = Other.W;
            return *this;
        }
        
        explicit TF_Data(typename TF_RebindElement<I32>::F_PassedArgument Other) noexcept;
        TF_Data& operator = (typename TF_RebindElement<I32>::F_PassedArgument Other) noexcept;
        
        explicit TF_Data(typename TF_RebindElement<U32>::F_PassedArgument Other) noexcept;
        TF_Data& operator = (typename TF_RebindElement<U32>::F_PassedArgument Other) noexcept;
        
        explicit TF_Data(typename TF_RebindElement<U16>::F_PassedArgument Other) noexcept;
        TF_Data& operator = (typename TF_RebindElement<U16>::F_PassedArgument Other) noexcept;
        
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

#ifdef ABYTEK_ENABLE_SSE
        /// @brief Construct directly from SSE data
        /// @param InXYZW_SSE SSE register containing vector data
        explicit ABYTEK_FORCE_INLINE TF_Data(__m128 InXYZW_SSE) noexcept :
            XYZW_SSE(InXYZW_SSE)
        {}
        
        /// @brief Assignment from SSE data
        /// @param InXYZW_SSE SSE register containing vector data
        /// @return Reference to this vector
        ABYTEK_FORCE_INLINE TF_Data& operator = (__m128 InXYZW_SSE) noexcept
        {
            XYZW_SSE = InXYZW_SSE;
            return *this;
        }
#endif

        /// @brief Access element by index (lvalue reference)
        /// @param Idx Index of the element to access
        /// @return Reference to the element
        ABYTEK_FORCE_INLINE F_Element& operator [] (I32 Idx) & noexcept
        {
                        ABYTEK_BASE_MATH_ASSERT(Idx < RowCount) << "Idx out of bounds";
            return Elements[Idx];
        }
        
        /// @brief Access element by index (rvalue reference)
        /// @param Idx Index of the element to access
        /// @return Rvalue reference to the element
        ABYTEK_FORCE_INLINE F_Element&& operator [] (I32 Idx) && noexcept
        {
                        ABYTEK_BASE_MATH_ASSERT(Idx < RowCount) << "Idx out of bounds";
            return boost::move(Elements[Idx]);
        }
        
        /// @brief Access element by index (const lvalue reference)
        /// @param Idx Index of the element to access
        /// @return Value of the element
        ABYTEK_FORCE_INLINE F_Element operator [] (I32 Idx) const & noexcept
        {
                        ABYTEK_BASE_MATH_ASSERT(Idx < RowCount) << "Idx out of bounds";
            return Elements[Idx];
        }
        
        /// @brief Access element by index (const rvalue reference)
        /// @param Idx Index of the element to access
        /// @return Value of the element
        ABYTEK_FORCE_INLINE F_Element operator [] (I32 Idx) const && noexcept
        {
                        ABYTEK_BASE_MATH_ASSERT(Idx < RowCount) << "Idx out of bounds";
            return Elements[Idx];
        }

        /// @brief Equality comparison operator
        /// @param A First vector
        /// @param B Second vector
        /// @return True if all elements are equal
        friend ABYTEK_FORCE_INLINE B8 operator == (F_PassedArgument A, F_PassedArgument B) noexcept
        {
            return (
                (A.X == B.X)
                && (A.Y == B.Y)
                && (A.Z == B.Z)
                && (A.W == B.W)
            );
        }
        
        /// @brief Inequality comparison operator
        /// @param A First vector
        /// @param B Second vector
        /// @return True if any elements differ
        friend ABYTEK_FORCE_INLINE B8 operator != (F_PassedArgument A, F_PassedArgument B) noexcept
        {
            return (
                (A.X != B.X)
                || (A.Y != B.Y)
                || (A.Z != B.Z)
                || (A.W != B.W)
            );
        }

        /// @brief Rebind configuration type (lvalue reference)
        /// @tparam __F_Config2 New configuration type
        /// @return Reference with rebound configuration
        template<typename __F_Config2>
        ABYTEK_FORCE_INLINE TF_RebindConfig<__F_Config2>& RebindConfig() & noexcept
        {
            return (TF_RebindConfig<__F_Config2>&)*this;
        }
        
        /// @brief Rebind configuration type (const lvalue reference)
        /// @tparam __F_Config2 New configuration type
        /// @return Const reference with rebound configuration
        template<typename __F_Config2>
        ABYTEK_FORCE_INLINE TF_RebindConfig<__F_Config2> const & RebindConfig() const & noexcept
        {
            return (TF_RebindConfig<__F_Config2> const &)*this;
        }
        
        /// @brief Rebind configuration type (rvalue reference)
        /// @tparam __F_Config2 New configuration type
        /// @return Rvalue reference with rebound configuration
        template<typename __F_Config2>
        ABYTEK_FORCE_INLINE TF_RebindConfig<__F_Config2>&& RebindConfig() && noexcept
        {
            return (TF_RebindConfig<__F_Config2>&&)*this;
        }
        
        /// @brief Rebind configuration type (const rvalue reference)
        /// @tparam __F_Config2 New configuration type
        /// @return Const rvalue reference with rebound configuration
        template<typename __F_Config2>
        ABYTEK_FORCE_INLINE TF_RebindConfig<__F_Config2> const && RebindConfig() const && noexcept
        {
            return (TF_RebindConfig<__F_Config2> const &&)*this;
        }

        /// @brief Create a new vector by permuting elements of this vector
        /// @tparam __ElementIndices Indices of elements to use in the new vector
        /// @return Permuted vector with elements in the specified order
        template<U32... __ElementIndices>
        ABYTEK_FORCE_INLINE TF_RebindRowCount<sizeof...(__ElementIndices)> StaticPermute() const noexcept
        {
            return { Elements[__ElementIndices]... };
        }
        
        /// @brief Create a vector with all elements set to zero
        /// @return Vector with all zeros
        static ABYTEK_FORCE_INLINE TF_Data Zero() noexcept
        {
            return {
                ABYTEK_F32_ZERO
            };
        }
        
        /// @brief Create a vector with all elements set to one
        /// @return Vector with all ones
        static ABYTEK_FORCE_INLINE TF_Data One() noexcept
        {
            return {
                ABYTEK_F32_ONE
            };
        }
        
        /// @brief Create a vector with all elements set to 0.5
        /// @return Vector with all elements set to 0.5
		static ABYTEK_FORCE_INLINE TF_Data Half() noexcept
        {
			return {
				ABYTEK_F32_HALF
			};
		}
		
		/// @brief Create a vector with all elements set to -0.5
        /// @return Vector with all elements set to -0.5
		static ABYTEK_FORCE_INLINE TF_Data NegativeHalf() noexcept
        {
			return {
				ABYTEK_F32_NEGATIVE_HALF
			};
		}
		
		/// @brief Create a vector with all elements set to 2.0
        /// @return Vector with all elements set to 2.0
		static ABYTEK_FORCE_INLINE TF_Data InverseHalf() noexcept
        {
			return {
				ABYTEK_F32_INV_HALF
			};
		}
		
		/// @brief Create a vector with all elements set to -2.0
        /// @return Vector with all elements set to -2.0
		static ABYTEK_FORCE_INLINE TF_Data InverseNegativeHalf() noexcept
        {
			return {
				ABYTEK_F32_INV_NEGATIVE_HALF
			};
		}
        
        /// @brief Create a vector with all elements set to infinity
        /// @return Vector with all infinity
        static ABYTEK_FORCE_INLINE TF_Data Infinity() noexcept
        {
            return {
                ABYTEK_F32_INFINITY
            };
        }
        
        /// @brief Create a vector with all elements set to negative infinity
        /// @return Vector with all negative infinity
        static ABYTEK_FORCE_INLINE TF_Data NegativeInfinity() noexcept
        {
            return {
                ABYTEK_F32_NEGATIVE_INFINITY
            };
        }
        
        /// @brief Create a vector with all elements set to PI
        /// @return Vector with all PI
        static ABYTEK_FORCE_INLINE TF_Data PI() noexcept
        {
            return {
                ABYTEK_F32_PI
            };
        }
        
        /// @brief Create a vector with all elements set to negative PI
        /// @return Vector with all negative PI
        static ABYTEK_FORCE_INLINE TF_Data NegativePI() noexcept
        {
            return {
                ABYTEK_F32_NEGATIVE_PI
            };
        }
        
        /// @brief Create a vector with all elements set to PI/2
        /// @return Vector with all PI/2
        static ABYTEK_FORCE_INLINE TF_Data HalfPI() noexcept
        {
            return {
                ABYTEK_F32_PI
            };
        }
        
        /// @brief Create a vector with all elements set to -PI/2
        /// @return Vector with all -PI/2
        static ABYTEK_FORCE_INLINE TF_Data NegativeHalfPI() noexcept
        {
            return {
                ABYTEK_F32_NEGATIVE_HALF_PI
            };
        }
        
        /// @brief Create a vector with all elements set to 2*PI
        /// @return Vector with all 2*PI
        static ABYTEK_FORCE_INLINE TF_Data TwoPI() noexcept
        {
            return {
                ABYTEK_F32_TWO_PI
            };
        }
        
        /// @brief Create a vector with all elements set to -2*PI
        /// @return Vector with all -2*PI
        static ABYTEK_FORCE_INLINE TF_Data NegativeTwoPI() noexcept
        {
            return {
                ABYTEK_F32_NEGATIVE_TWO_PI
            };
        }
        
        /// @brief Create a vector with all elements set to 1/PI
        /// @return Vector with all 1/PI
        static ABYTEK_FORCE_INLINE TF_Data InversePI() noexcept
        {
            return {
                ABYTEK_F32_INV_PI
            };
        }
        
        /// @brief Create a vector with all elements set to -1/PI
        /// @return Vector with all -1/PI
        static ABYTEK_FORCE_INLINE TF_Data InverseNegativePI() noexcept
        {
            return {
                ABYTEK_F32_INV_NEGATIVE_PI
            };
        }
        
        /// @brief Create a vector with all elements set to 1/(PI/2)
        /// @return Vector with all 1/(PI/2)
        static ABYTEK_FORCE_INLINE TF_Data InverseHalfPI() noexcept
        {
            return {
                ABYTEK_F32_INV_PI
            };
        }
        
        /// @brief Create a vector with all elements set to -1/(PI/2)
        /// @return Vector with all -1/(PI/2)
        static ABYTEK_FORCE_INLINE TF_Data InverseNegativeHalfPI() noexcept
        {
            return {
                ABYTEK_F32_INV_NEGATIVE_HALF_PI
            };
        }
        
        /// @brief Create a vector with all elements set to 1/(2*PI)
        /// @return Vector with all 1/(2*PI)
        static ABYTEK_FORCE_INLINE TF_Data InverseTwoPI() noexcept
        {
            return {
                ABYTEK_F32_TWO_PI
            };
        }
        
        /// @brief Create a vector with all elements set to -1/(2*PI)
        /// @return Vector with all -1/(2*PI)
        static ABYTEK_FORCE_INLINE TF_Data InverseNegativeTwoPI() noexcept
        {
            return {
                ABYTEK_F32_INV_NEGATIVE_TWO_PI
            };
        }
        
        /// @brief Create a unit vector pointing along the X axis (1,0,0,0)
        /// @return Unit X vector
        static ABYTEK_FORCE_INLINE TF_Data Right() noexcept
        {
            return {
                ABYTEK_F32_ONE,
                ABYTEK_F32_ZERO,
                ABYTEK_F32_ZERO,
                ABYTEK_F32_ZERO
            };
        }
        
        /// @brief Create a unit vector pointing along the Y axis (0,1,0,0)
        /// @return Unit Y vector
        static ABYTEK_FORCE_INLINE TF_Data Up() noexcept
        {
            return {
                ABYTEK_F32_ZERO,
                ABYTEK_F32_ONE,
                ABYTEK_F32_ZERO,
                ABYTEK_F32_ZERO
            };
        }
        
        /// @brief Create a unit vector pointing along the Z axis (0,0,1,0)
        /// @return Unit Z vector
        static ABYTEK_FORCE_INLINE TF_Data Forward() noexcept
        {
            return {
                ABYTEK_F32_ZERO,
                ABYTEK_F32_ZERO,
                ABYTEK_F32_ONE,
                ABYTEK_F32_ZERO
            };
        }
        
        /// @brief Create a unit vector pointing along the W axis (0,0,0,1)
        /// @return Unit W vector
        static ABYTEK_FORCE_INLINE TF_Data Future() noexcept
        {
            return {
                ABYTEK_F32_ZERO,
                ABYTEK_F32_ZERO,
                ABYTEK_F32_ZERO,
                ABYTEK_F32_ONE
            };
        }
        
        /// @brief Create a unit vector pointing along the negative X axis (-1,0,0,0)
        /// @return Negative unit X vector
        static ABYTEK_FORCE_INLINE TF_Data Left() noexcept
        {
            return {
                -ABYTEK_F32_ONE,
                ABYTEK_F32_ZERO,
                ABYTEK_F32_ZERO,
                ABYTEK_F32_ZERO
            };
        }
        
        /// @brief Create a unit vector pointing along the negative Y axis (0,-1,0,0)
        /// @return Negative unit Y vector
        static ABYTEK_FORCE_INLINE TF_Data Down() noexcept
        {
            return {
                ABYTEK_F32_ZERO,
                -ABYTEK_F32_ONE,
                ABYTEK_F32_ZERO,
                ABYTEK_F32_ZERO
            };
        }
        
        /// @brief Create a unit vector pointing along the negative Z axis (0,0,-1,0)
        /// @return Negative unit Z vector
        static ABYTEK_FORCE_INLINE TF_Data Backward() noexcept
        {
            return {
                ABYTEK_F32_ZERO,
                ABYTEK_F32_ZERO,
                -ABYTEK_F32_ONE,
                ABYTEK_F32_ZERO
            };
        }
        
        /// @brief Create a unit vector pointing along the negative W axis (0,0,0,-1)
        /// @return Negative unit W vector
        static ABYTEK_FORCE_INLINE TF_Data Past() noexcept
        {
            return {
                ABYTEK_F32_ZERO,
                ABYTEK_F32_ZERO,
                ABYTEK_F32_ZERO,
                -ABYTEK_F32_ONE
            };
        }
    };
    template<class __F_Config>
    ABYTEK_REFLECT(TF_Data<4, 1, F32, __F_Config>)
    {
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE_ADVANCED(X, F_Element, sizeof(F_Element) * 0);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE_ADVANCED(Y, F_Element, sizeof(F_Element) * 1);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE_ADVANCED(Z, F_Element, sizeof(F_Element) * 2);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE_ADVANCED(W, F_Element, sizeof(F_Element) * 3);
    }
}