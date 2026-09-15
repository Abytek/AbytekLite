#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Functions/Dot.hpp"
#include "Abytek/Functions/Length.hpp"
#include "Abytek/Operators/VectorVector.hpp"
#include "Abytek/Operators/QuaternionScalar.hpp"


namespace Abytek
{
    /**
     * Normalizes a 2D vector to unit length.
     * If the vector length is less than or equal to the tolerance, a zero vector is returned.
     * 
     * @param A Input vector to normalize
     * @param Tolerance Threshold below which the vector is considered too small to normalize safely
     * @return Normalized vector with unit length or zero vector if input length is below tolerance
     */
    ABYTEK_FORCE_INLINE F_Vector2_F32 Normalize(PA_Vector2_F32 A, F32 Tolerance = DEFAULT_TOLERANCE<F32>) noexcept
    {
        F32 ALength = Length(A);
        if(ALength <= Tolerance)
        {
            return F_Vector2_F32::Zero();
        }
        
        return A / ALength;
    }
    
    /**
     * Normalizes a 3D vector to unit length.
     * If the vector length is less than or equal to the tolerance, a zero vector is returned.
     * 
     * @param A Input vector to normalize
     * @param Tolerance Threshold below which the vector is considered too small to normalize safely
     * @return Normalized vector with unit length or zero vector if input length is below tolerance
     */
    ABYTEK_FORCE_INLINE F_Vector3_F32 Normalize(PA_Vector3_F32 A, F32 Tolerance = DEFAULT_TOLERANCE<F32>) noexcept
    {
        F32 ALength = Length(A);
        if(ALength <= Tolerance)
        {
            return F_Vector3_F32::Zero();
        }
        
        return A / ALength;
    }
    
    /**
     * Normalizes a 4D vector to unit length.
     * If the vector length is less than or equal to the tolerance, a zero vector is returned.
     * 
     * @param A Input vector to normalize
     * @param Tolerance Threshold below which the vector is considered too small to normalize safely
     * @return Normalized vector with unit length or zero vector if input length is below tolerance
     */
    ABYTEK_FORCE_INLINE F_Vector4_F32 Normalize(PA_Vector4_F32 A, F32 Tolerance = DEFAULT_TOLERANCE<F32>) noexcept
    {
        F32 ALength = Length(A);
        if(ALength <= Tolerance)
        {
            return F_Vector4_F32::Zero();
        }
        
        return A / ALength;
    }
    
    ABYTEK_FORCE_INLINE F_Quaternion_F32 Normalize(PA_Quaternion_F32 A, F32 Tolerance = DEFAULT_TOLERANCE<F32>) noexcept
    {
        F32 ALength = Length(A);
        if(ALength <= Tolerance)
        {
            return F_Quaternion_F32::Zero();
        }
        
        return A / ALength;
    }
    
    /**
     * Checks if a 2D vector is normalized (has unit length).
     * 
     * @param A Vector to check
     * @param Tolerance Allowed deviation from unit length
     * @return True if the vector's length squared is within tolerance of 1.0
     */
    ABYTEK_FORCE_INLINE B8 IsNormalized(PA_Vector2_F32 A, F32 Tolerance = DEFAULT_TOLERANCE<F32>) noexcept
    {
        return (LengthSquare(A) <= (Tolerance * Tolerance));
    }
    
    /**
     * Checks if a 3D vector is normalized (has unit length).
     * 
     * @param A Vector to check
     * @param Tolerance Allowed deviation from unit length
     * @return True if the vector's length squared is within tolerance of 1.0
     */
    ABYTEK_FORCE_INLINE B8 IsNormalized(PA_Vector3_F32 A, F32 Tolerance = DEFAULT_TOLERANCE<F32>) noexcept
    {
        return (LengthSquare(A) <= (Tolerance * Tolerance));
    }
    
    /**
     * Checks if a 4D vector is normalized (has unit length).
     * 
     * @param A Vector to check
     * @param Tolerance Allowed deviation from unit length
     * @return True if the vector's length squared is within tolerance of 1.0
     */
    ABYTEK_FORCE_INLINE B8 IsNormalized(PA_Vector4_F32 A, F32 Tolerance = DEFAULT_TOLERANCE<F32>) noexcept
    {
        return (LengthSquare(A) <= (Tolerance * Tolerance));
    }
}