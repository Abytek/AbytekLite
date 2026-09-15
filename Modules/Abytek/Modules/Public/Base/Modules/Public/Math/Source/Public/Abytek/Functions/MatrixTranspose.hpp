#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data.hpp"
#include "Abytek/Types/Vecma.hpp"


namespace Abytek
{
    /**
     * Transposes a 2x2 matrix with float values.
     * Swaps rows and columns of the matrix.
     * 
     * @param A Input 2x2 matrix
     * @return Transposed 2x2 matrix
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_F32 Transpose(PA_Matrix2x2_F32 A) noexcept
    {
        return {
            F_Vector2_F32 { A.A.X, A.B.X },
            F_Vector2_F32 { A.A.Y, A.B.Y }
        };
    }
    
    /**
     * Transposes a 2x2 matrix with signed integer values.
     * Swaps rows and columns of the matrix.
     * 
     * @param A Input 2x2 matrix
     * @return Transposed 2x2 matrix
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_I32 Transpose(PA_Matrix2x2_I32 A) noexcept
    {
        return {
            F_Vector2_I32 { A.A.X, A.B.X },
            F_Vector2_I32 { A.A.Y, A.B.Y }
        };
    }
    
    /**
     * Transposes a 2x2 matrix with unsigned integer values.
     * Swaps rows and columns of the matrix.
     * 
     * @param A Input 2x2 matrix
     * @return Transposed 2x2 matrix
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_U32 Transpose(PA_Matrix2x2_U32 A) noexcept
    {
        return {
            F_Vector2_U32 { A.A.X, A.B.X },
            F_Vector2_U32 { A.A.Y, A.B.Y }
        };
    }
    
    /**
     * Transposes a 2x2 matrix with unsigned integer values.
     * Swaps rows and columns of the matrix.
     * 
     * @param A Input 2x2 matrix
     * @return Transposed 2x2 matrix
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_U16 Transpose(PA_Matrix2x2_U16 A) noexcept
    {
        return {
            F_Vector2_U16 { A.A.X, A.B.X },
            F_Vector2_U16 { A.A.Y, A.B.Y }
        };
    }
    
    /**
     * Transposes a 2x2 matrix with unsigned integer values.
     * Swaps rows and columns of the matrix.
     * 
     * @param A Input 2x2 matrix
     * @return Transposed 2x2 matrix
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_U8 Transpose(PA_Matrix2x2_U8 A) noexcept
    {
        return {
            F_Vector2_U8 { A.A.X, A.B.X },
            F_Vector2_U8 { A.A.Y, A.B.Y }
        };
    }
    
    /**
     * Transposes a 2x2 matrix with unsigned integer values.
     * Swaps rows and columns of the matrix.
     * 
     * @param A Input 2x2 matrix
     * @return Transposed 2x2 matrix
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_I64 Transpose(PA_Matrix2x2_I64 A) noexcept
    {
        return {
            F_Vector2_I64 { A.A.X, A.B.X },
            F_Vector2_I64 { A.A.Y, A.B.Y }
        };
    }
    
    /**
     * Transposes a 2x2 matrix with unsigned integer values.
     * Swaps rows and columns of the matrix.
     * 
     * @param A Input 2x2 matrix
     * @return Transposed 2x2 matrix
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_U64 Transpose(PA_Matrix2x2_U64 A) noexcept
    {
        return {
            F_Vector2_U64 { A.A.X, A.B.X },
            F_Vector2_U64 { A.A.Y, A.B.Y }
        };
    }
    
    /**
     * Transposes a 2x2 matrix with unsigned integer values.
     * Swaps rows and columns of the matrix.
     * 
     * @param A Input 2x2 matrix
     * @return Transposed 2x2 matrix
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_I8 Transpose(PA_Matrix2x2_I8 A) noexcept
    {
        return {
            F_Vector2_I8 { A.A.X, A.B.X },
            F_Vector2_I8 { A.A.Y, A.B.Y }
        };
    }
    
    /**
     * Transposes a 2x2 matrix with unsigned integer values.
     * Swaps rows and columns of the matrix.
     * 
     * @param A Input 2x2 matrix
     * @return Transposed 2x2 matrix
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_I16 Transpose(PA_Matrix2x2_I16 A) noexcept
    {
        return {
            F_Vector2_I16 { A.A.X, A.B.X },
            F_Vector2_I16 { A.A.Y, A.B.Y }
        };
    }
    
    /**
     * Transposes a 3x3 matrix with float values.
     * Swaps rows and columns of the matrix.
     * 
     * @param A Input 3x3 matrix
     * @return Transposed 3x3 matrix
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_F32 Transpose(PA_Matrix3x3_F32 A) noexcept
    {
        return {
            F_Vector3_F32 { A.A.X, A.B.X, A.C.X },
            F_Vector3_F32 { A.A.Y, A.B.Y, A.C.Y },
            F_Vector3_F32 { A.A.Z, A.B.Z, A.C.Z }
        };
    }
    
    /**
     * Transposes a 3x3 matrix with signed integer values.
     * Swaps rows and columns of the matrix.
     * 
     * @param A Input 3x3 matrix
     * @return Transposed 3x3 matrix
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_I32 Transpose(PA_Matrix3x3_I32 A) noexcept
    {
        return {
            F_Vector3_I32 { A.A.X, A.B.X, A.C.X },
            F_Vector3_I32 { A.A.Y, A.B.Y, A.C.Y },
            F_Vector3_I32 { A.A.Z, A.B.Z, A.C.Z }
        };
    }
    
    /**
     * Transposes a 3x3 matrix with unsigned integer values.
     * Swaps rows and columns of the matrix.
     * 
     * @param A Input 3x3 matrix
     * @return Transposed 3x3 matrix
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_U32 Transpose(PA_Matrix3x3_U32 A) noexcept
    {
        return {
            F_Vector3_U32 { A.A.X, A.B.X, A.C.X },
            F_Vector3_U32 { A.A.Y, A.B.Y, A.C.Y },
            F_Vector3_U32 { A.A.Z, A.B.Z, A.C.Z }
        };
    }
    
    /**
     * Transposes a 3x3 matrix with unsigned integer values.
     * Swaps rows and columns of the matrix.
     * 
     * @param A Input 3x3 matrix
     * @return Transposed 3x3 matrix
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_U16 Transpose(PA_Matrix3x3_U16 A) noexcept
    {
        return {
            F_Vector3_U16 { A.A.X, A.B.X, A.C.X },
            F_Vector3_U16 { A.A.Y, A.B.Y, A.C.Y },
            F_Vector3_U16 { A.A.Z, A.B.Z, A.C.Z }
        };
    }
    
    /**
     * Transposes a 3x3 matrix with unsigned integer values.
     * Swaps rows and columns of the matrix.
     * 
     * @param A Input 3x3 matrix
     * @return Transposed 3x3 matrix
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_U8 Transpose(PA_Matrix3x3_U8 A) noexcept
    {
        return {
            F_Vector3_U8 { A.A.X, A.B.X, A.C.X },
            F_Vector3_U8 { A.A.Y, A.B.Y, A.C.Y },
            F_Vector3_U8 { A.A.Z, A.B.Z, A.C.Z }
        };
    }
    
    /**
     * Transposes a 3x3 matrix with unsigned integer values.
     * Swaps rows and columns of the matrix.
     * 
     * @param A Input 3x3 matrix
     * @return Transposed 3x3 matrix
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_I64 Transpose(PA_Matrix3x3_I64 A) noexcept
    {
        return {
            F_Vector3_I64 { A.A.X, A.B.X, A.C.X },
            F_Vector3_I64 { A.A.Y, A.B.Y, A.C.Y },
            F_Vector3_I64 { A.A.Z, A.B.Z, A.C.Z }
        };
    }
    
    /**
     * Transposes a 3x3 matrix with unsigned integer values.
     * Swaps rows and columns of the matrix.
     * 
     * @param A Input 3x3 matrix
     * @return Transposed 3x3 matrix
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_U64 Transpose(PA_Matrix3x3_U64 A) noexcept
    {
        return {
            F_Vector3_U64 { A.A.X, A.B.X, A.C.X },
            F_Vector3_U64 { A.A.Y, A.B.Y, A.C.Y },
            F_Vector3_U64 { A.A.Z, A.B.Z, A.C.Z }
        };
    }
    
    /**
     * Transposes a 3x3 matrix with unsigned integer values.
     * Swaps rows and columns of the matrix.
     * 
     * @param A Input 3x3 matrix
     * @return Transposed 3x3 matrix
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_I8 Transpose(PA_Matrix3x3_I8 A) noexcept
    {
        return {
            F_Vector3_I8 { A.A.X, A.B.X, A.C.X },
            F_Vector3_I8 { A.A.Y, A.B.Y, A.C.Y },
            F_Vector3_I8 { A.A.Z, A.B.Z, A.C.Z }
        };
    }
    
    /**
     * Transposes a 3x3 matrix with unsigned integer values.
     * Swaps rows and columns of the matrix.
     * 
     * @param A Input 3x3 matrix
     * @return Transposed 3x3 matrix
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_I16 Transpose(PA_Matrix3x3_I16 A) noexcept
    {
        return {
            F_Vector3_I16 { A.A.X, A.B.X, A.C.X },
            F_Vector3_I16 { A.A.Y, A.B.Y, A.C.Y },
            F_Vector3_I16 { A.A.Z, A.B.Z, A.C.Z }
        };
    }
    
    /**
     * Transposes a 4x4 matrix with float values.
     * Swaps rows and columns of the matrix.
     * 
     * @param A Input 4x4 matrix
     * @return Transposed 4x4 matrix
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_F32 Transpose(PA_Matrix4x4_F32 A) noexcept
    {
        return {
            F_Vector4_F32 { A.A.X, A.B.X, A.C.X, A.D.X },
            F_Vector4_F32 { A.A.Y, A.B.Y, A.C.Y, A.D.Y },
            F_Vector4_F32 { A.A.Z, A.B.Z, A.C.Z, A.D.Z },
            F_Vector4_F32 { A.A.W, A.B.W, A.C.W, A.D.W }
        };
    }
    
    /**
     * Transposes a 4x4 matrix with signed integer values.
     * Swaps rows and columns of the matrix.
     * 
     * @param A Input 4x4 matrix
     * @return Transposed 4x4 matrix
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_I32 Transpose(PA_Matrix4x4_I32 A) noexcept
    {
        return {
            F_Vector4_I32 { A.A.X, A.B.X, A.C.X, A.D.X },
            F_Vector4_I32 { A.A.Y, A.B.Y, A.C.Y, A.D.Y },
            F_Vector4_I32 { A.A.Z, A.B.Z, A.C.Z, A.D.Z },
            F_Vector4_I32 { A.A.W, A.B.W, A.C.W, A.D.W }
        };
    }
    
    /**
     * Transposes a 4x4 matrix with unsigned integer values.
     * Swaps rows and columns of the matrix.
     * 
     * @param A Input 4x4 matrix
     * @return Transposed 4x4 matrix
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_U32 Transpose(PA_Matrix4x4_U32 A) noexcept
    {
        return {
            F_Vector4_U32 { A.A.X, A.B.X, A.C.X, A.D.X },
            F_Vector4_U32 { A.A.Y, A.B.Y, A.C.Y, A.D.Y },
            F_Vector4_U32 { A.A.Z, A.B.Z, A.C.Z, A.D.Z },
            F_Vector4_U32 { A.A.W, A.B.W, A.C.W, A.D.W }
        };
    }
    
    /**
     * Transposes a 4x4 matrix with unsigned integer values.
     * Swaps rows and columns of the matrix.
     * 
     * @param A Input 4x4 matrix
     * @return Transposed 4x4 matrix
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_U16 Transpose(PA_Matrix4x4_U16 A) noexcept
    {
        return {
            F_Vector4_U16 { A.A.X, A.B.X, A.C.X, A.D.X },
            F_Vector4_U16 { A.A.Y, A.B.Y, A.C.Y, A.D.Y },
            F_Vector4_U16 { A.A.Z, A.B.Z, A.C.Z, A.D.Z },
            F_Vector4_U16 { A.A.W, A.B.W, A.C.W, A.D.W }
        };
    }
    
    /**
     * Transposes a 4x4 matrix with unsigned integer values.
     * Swaps rows and columns of the matrix.
     * 
     * @param A Input 4x4 matrix
     * @return Transposed 4x4 matrix
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_U8 Transpose(PA_Matrix4x4_U8 A) noexcept
    {
        return {
            F_Vector4_U8 { A.A.X, A.B.X, A.C.X, A.D.X },
            F_Vector4_U8 { A.A.Y, A.B.Y, A.C.Y, A.D.Y },
            F_Vector4_U8 { A.A.Z, A.B.Z, A.C.Z, A.D.Z },
            F_Vector4_U8 { A.A.W, A.B.W, A.C.W, A.D.W }
        };
    }
    
    /**
     * Transposes a 4x4 matrix with unsigned integer values.
     * Swaps rows and columns of the matrix.
     * 
     * @param A Input 4x4 matrix
     * @return Transposed 4x4 matrix
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_I64 Transpose(PA_Matrix4x4_I64 A) noexcept
    {
        return {
            F_Vector4_I64 { A.A.X, A.B.X, A.C.X, A.D.X },
            F_Vector4_I64 { A.A.Y, A.B.Y, A.C.Y, A.D.Y },
            F_Vector4_I64 { A.A.Z, A.B.Z, A.C.Z, A.D.Z },
            F_Vector4_I64 { A.A.W, A.B.W, A.C.W, A.D.W }
        };
    }
    
    /**
     * Transposes a 4x4 matrix with unsigned integer values.
     * Swaps rows and columns of the matrix.
     * 
     * @param A Input 4x4 matrix
     * @return Transposed 4x4 matrix
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_U64 Transpose(PA_Matrix4x4_U64 A) noexcept
    {
        return {
            F_Vector4_U64 { A.A.X, A.B.X, A.C.X, A.D.X },
            F_Vector4_U64 { A.A.Y, A.B.Y, A.C.Y, A.D.Y },
            F_Vector4_U64 { A.A.Z, A.B.Z, A.C.Z, A.D.Z },
            F_Vector4_U64 { A.A.W, A.B.W, A.C.W, A.D.W }
        };
    }
    
    /**
     * Transposes a 4x4 matrix with unsigned integer values.
     * Swaps rows and columns of the matrix.
     * 
     * @param A Input 4x4 matrix
     * @return Transposed 4x4 matrix
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_I8 Transpose(PA_Matrix4x4_I8 A) noexcept
    {
        return {
            F_Vector4_I8 { A.A.X, A.B.X, A.C.X, A.D.X },
            F_Vector4_I8 { A.A.Y, A.B.Y, A.C.Y, A.D.Y },
            F_Vector4_I8 { A.A.Z, A.B.Z, A.C.Z, A.D.Z },
            F_Vector4_I8 { A.A.W, A.B.W, A.C.W, A.D.W }
        };
    }
    
    /**
     * Transposes a 4x4 matrix with unsigned integer values.
     * Swaps rows and columns of the matrix.
     * 
     * @param A Input 4x4 matrix
     * @return Transposed 4x4 matrix
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_I16 Transpose(PA_Matrix4x4_I16 A) noexcept
    {
        return {
            F_Vector4_I16 { A.A.X, A.B.X, A.C.X, A.D.X },
            F_Vector4_I16 { A.A.Y, A.B.Y, A.C.Y, A.D.Y },
            F_Vector4_I16 { A.A.Z, A.B.Z, A.C.Z, A.D.Z },
            F_Vector4_I16 { A.A.W, A.B.W, A.C.W, A.D.W }
        };
    }
}