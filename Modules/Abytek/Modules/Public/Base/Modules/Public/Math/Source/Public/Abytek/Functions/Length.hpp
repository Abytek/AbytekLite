/**
 * @file Length.hpp
 * @brief Vector and quaternion length calculation functions
 * 
 * Provides functions to calculate the length (magnitude) and squared length
 * of vectors and quaternions.
 */
#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Functions/Dot.hpp"
#include "Abytek/Functions/SquareRoot.hpp"


namespace Abytek
{
    /**
     * @brief Calculates the squared length of a 2D floating-point vector
     * 
     * @param A Input vector
     * @return Squared length (magnitude) of the vector
     */
    ABYTEK_FORCE_INLINE F32 LengthSquare(PA_Vector2_F32 A) noexcept
    {
        return Dot(A, A);
    }
    
    /**
     * @brief Calculates the squared length of a 2D signed integer vector
     * 
     * @param A Input vector
     * @return Squared length (magnitude) of the vector
     */
    ABYTEK_FORCE_INLINE I32 LengthSquare(PA_Vector2_I32 A) noexcept
    {
        return Dot(A, A);
    }
    
    /**
     * @brief Calculates the squared length of a 2D unsigned integer vector
     * 
     * @param A Input vector
     * @return Squared length (magnitude) of the vector
     */
    ABYTEK_FORCE_INLINE U32 LengthSquare(PA_Vector2_U32 A) noexcept
    {
        return Dot(A, A);
    }
    
    /**
     * @brief Calculates the squared length of a 2D unsigned integer vector
     * 
     * @param A Input vector
     * @return Squared length (magnitude) of the vector
     */
    ABYTEK_FORCE_INLINE U16 LengthSquare(PA_Vector2_U16 A) noexcept
    {
        return Dot(A, A);
    }
    
    /**
     * @brief Calculates the squared length of a 2D unsigned integer vector
     * 
     * @param A Input vector
     * @return Squared length (magnitude) of the vector
     */
    ABYTEK_FORCE_INLINE U8 LengthSquare(PA_Vector2_U8 A) noexcept
    {
        return Dot(A, A);
    }
    
    /**
     * @brief Calculates the squared length of a 2D unsigned integer vector
     * 
     * @param A Input vector
     * @return Squared length (magnitude) of the vector
     */
    ABYTEK_FORCE_INLINE I64 LengthSquare(PA_Vector2_I64 A) noexcept
    {
        return Dot(A, A);
    }
    
    /**
     * @brief Calculates the squared length of a 2D unsigned integer vector
     * 
     * @param A Input vector
     * @return Squared length (magnitude) of the vector
     */
    ABYTEK_FORCE_INLINE U64 LengthSquare(PA_Vector2_U64 A) noexcept
    {
        return Dot(A, A);
    }
    
    /**
     * @brief Calculates the squared length of a 2D unsigned integer vector
     * 
     * @param A Input vector
     * @return Squared length (magnitude) of the vector
     */
    ABYTEK_FORCE_INLINE I8 LengthSquare(PA_Vector2_I8 A) noexcept
    {
        return Dot(A, A);
    }
    
    /**
     * @brief Calculates the squared length of a 2D unsigned integer vector
     * 
     * @param A Input vector
     * @return Squared length (magnitude) of the vector
     */
    ABYTEK_FORCE_INLINE I16 LengthSquare(PA_Vector2_I16 A) noexcept
    {
        return Dot(A, A);
    }
    
    /**
     * @brief Calculates the squared length of a 3D floating-point vector
     * 
     * @param A Input vector
     * @return Squared length (magnitude) of the vector
     */
    ABYTEK_FORCE_INLINE F32 LengthSquare(PA_Vector3_F32 A) noexcept
    {
        return Dot(A, A);
    }
    
    /**
     * @brief Calculates the squared length of a 3D signed integer vector
     * 
     * @param A Input vector
     * @return Squared length (magnitude) of the vector
     */
    ABYTEK_FORCE_INLINE I32 LengthSquare(PA_Vector3_I32 A) noexcept
    {
        return Dot(A, A);
    }
    
    /**
     * @brief Calculates the squared length of a 3D unsigned integer vector
     * 
     * @param A Input vector
     * @return Squared length (magnitude) of the vector
     */
    ABYTEK_FORCE_INLINE U32 LengthSquare(PA_Vector3_U32 A) noexcept
    {
        return Dot(A, A);
    }
    
    /**
     * @brief Calculates the squared length of a 3D unsigned integer vector
     * 
     * @param A Input vector
     * @return Squared length (magnitude) of the vector
     */
    ABYTEK_FORCE_INLINE U16 LengthSquare(PA_Vector3_U16 A) noexcept
    {
        return Dot(A, A);
    }
    
    /**
     * @brief Calculates the squared length of a 3D unsigned integer vector
     * 
     * @param A Input vector
     * @return Squared length (magnitude) of the vector
     */
    ABYTEK_FORCE_INLINE U8 LengthSquare(PA_Vector3_U8 A) noexcept
    {
        return Dot(A, A);
    }
    
    /**
     * @brief Calculates the squared length of a 3D unsigned integer vector
     * 
     * @param A Input vector
     * @return Squared length (magnitude) of the vector
     */
    ABYTEK_FORCE_INLINE I64 LengthSquare(PA_Vector3_I64 A) noexcept
    {
        return Dot(A, A);
    }
    
    /**
     * @brief Calculates the squared length of a 3D unsigned integer vector
     * 
     * @param A Input vector
     * @return Squared length (magnitude) of the vector
     */
    ABYTEK_FORCE_INLINE U64 LengthSquare(PA_Vector3_U64 A) noexcept
    {
        return Dot(A, A);
    }
    
    /**
     * @brief Calculates the squared length of a 3D unsigned integer vector
     * 
     * @param A Input vector
     * @return Squared length (magnitude) of the vector
     */
    ABYTEK_FORCE_INLINE I8 LengthSquare(PA_Vector3_I8 A) noexcept
    {
        return Dot(A, A);
    }
    
    /**
     * @brief Calculates the squared length of a 3D unsigned integer vector
     * 
     * @param A Input vector
     * @return Squared length (magnitude) of the vector
     */
    ABYTEK_FORCE_INLINE I16 LengthSquare(PA_Vector3_I16 A) noexcept
    {
        return Dot(A, A);
    }
    
    /**
     * @brief Calculates the squared length of a 4D floating-point vector
     * 
     * @param A Input vector
     * @return Squared length (magnitude) of the vector
     */
    ABYTEK_FORCE_INLINE F32 LengthSquare(PA_Vector4_F32 A) noexcept
    {
        return Dot(A, A);
    }
    
    /**
     * @brief Calculates the squared length of a 4D signed integer vector
     * 
     * @param A Input vector
     * @return Squared length (magnitude) of the vector
     */
    ABYTEK_FORCE_INLINE I32 LengthSquare(PA_Vector4_I32 A) noexcept
    {
        return Dot(A, A);
    }
    
    /**
     * @brief Calculates the squared length of a 4D unsigned integer vector
     * 
     * @param A Input vector
     * @return Squared length (magnitude) of the vector
     */
    ABYTEK_FORCE_INLINE U32 LengthSquare(PA_Vector4_U32 A) noexcept
    {
        return Dot(A, A);
    }
    
    /**
     * @brief Calculates the squared length of a 4D unsigned integer vector
     * 
     * @param A Input vector
     * @return Squared length (magnitude) of the vector
     */
    ABYTEK_FORCE_INLINE U16 LengthSquare(PA_Vector4_U16 A) noexcept
    {
        return Dot(A, A);
    }
    
    /**
     * @brief Calculates the squared length of a 4D unsigned integer vector
     * 
     * @param A Input vector
     * @return Squared length (magnitude) of the vector
     */
    ABYTEK_FORCE_INLINE U8 LengthSquare(PA_Vector4_U8 A) noexcept
    {
        return Dot(A, A);
    }
    
    /**
     * @brief Calculates the squared length of a 4D unsigned integer vector
     * 
     * @param A Input vector
     * @return Squared length (magnitude) of the vector
     */
    ABYTEK_FORCE_INLINE I64 LengthSquare(PA_Vector4_I64 A) noexcept
    {
        return Dot(A, A);
    }
    
    /**
     * @brief Calculates the squared length of a 4D unsigned integer vector
     * 
     * @param A Input vector
     * @return Squared length (magnitude) of the vector
     */
    ABYTEK_FORCE_INLINE U64 LengthSquare(PA_Vector4_U64 A) noexcept
    {
        return Dot(A, A);
    }
    
    /**
     * @brief Calculates the squared length of a 4D unsigned integer vector
     * 
     * @param A Input vector
     * @return Squared length (magnitude) of the vector
     */
    ABYTEK_FORCE_INLINE I8 LengthSquare(PA_Vector4_I8 A) noexcept
    {
        return Dot(A, A);
    }
    
    /**
     * @brief Calculates the squared length of a 4D unsigned integer vector
     * 
     * @param A Input vector
     * @return Squared length (magnitude) of the vector
     */
    ABYTEK_FORCE_INLINE I16 LengthSquare(PA_Vector4_I16 A) noexcept
    {
        return Dot(A, A);
    }
    
    /**
     * @brief Calculates the squared length of a quaternion
     * 
     * @param A Input quaternion
     * @return Squared length (magnitude) of the quaternion
     */
    ABYTEK_FORCE_INLINE F32 LengthSquare(PA_Quaternion_F32 A) noexcept
    {
        return Dot(A, A);
    }
    
    /**
     * @brief Calculates the length of a 2D floating-point vector
     * 
     * @param A Input vector
     * @return Length (magnitude) of the vector
     */
    ABYTEK_FORCE_INLINE F32 Length(PA_Vector2_F32 A) noexcept
    {
        return SquareRoot(LengthSquare(A));
    }
    
    /**
     * @brief Calculates the length of a 3D floating-point vector
     * 
     * @param A Input vector
     * @return Length (magnitude) of the vector
     */
    ABYTEK_FORCE_INLINE F32 Length(PA_Vector3_F32 A) noexcept
    {
        return SquareRoot(LengthSquare(A));
    }
    
    /**
     * @brief Calculates the length of a 4D floating-point vector
     * 
     * @param A Input vector
     * @return Length (magnitude) of the vector
     */
    ABYTEK_FORCE_INLINE F32 Length(PA_Vector4_F32 A) noexcept
    {
        return SquareRoot(LengthSquare(A));
    }
    
    /**
     * @brief Calculates the length of a quaternion
     * 
     * @param A Input quaternion
     * @return Length (magnitude) of the quaternion
     */
    ABYTEK_FORCE_INLINE F32 Length(PA_Quaternion_F32 A) noexcept
    {
        return SquareRoot(LengthSquare(A));
    }
}