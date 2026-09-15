#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data.hpp"
#include "Abytek/Types/Vecma.hpp"
#include "Abytek/Types/Quaternion.hpp"


namespace Abytek
{
    /**
     * Returns a 2D vector with the component-wise maximum of two input vectors.
     * 
     * @param A First vector to compare
     * @param B Second vector to compare
     * @return Vector with each component being the maximum of the corresponding components in A and B
     */
    ABYTEK_FORCE_INLINE F_Vector2_F32 Max(PA_Vector2_F32 A, PA_Vector2_F32 B) noexcept
    {
        return {
            Max(A.X, B.X),
            Max(A.Y, B.Y)
        };
    }
    ABYTEK_FORCE_INLINE F_Vector2_I32 Max(PA_Vector2_I32 A, PA_Vector2_I32 B) noexcept
    {
        return {
            Max(A.X, B.X),
            Max(A.Y, B.Y)
        };
    }
    ABYTEK_FORCE_INLINE F_Vector2_U32 Max(PA_Vector2_U32 A, PA_Vector2_U32 B) noexcept
    {
        return {
            Max(A.X, B.X),
            Max(A.Y, B.Y)
        };
    }
    ABYTEK_FORCE_INLINE F_Vector2_U16 Max(PA_Vector2_U16 A, PA_Vector2_U16 B) noexcept
    {
        return {
            Max(A.X, B.X),
            Max(A.Y, B.Y)
        };
    }
    ABYTEK_FORCE_INLINE F_Vector2_U8 Max(PA_Vector2_U8 A, PA_Vector2_U8 B) noexcept
    {
        return {
            Max(A.X, B.X),
            Max(A.Y, B.Y)
        };
    }
    ABYTEK_FORCE_INLINE F_Vector2_I64 Max(PA_Vector2_I64 A, PA_Vector2_I64 B) noexcept
    {
        return {
            Max(A.X, B.X),
            Max(A.Y, B.Y)
        };
    }
    ABYTEK_FORCE_INLINE F_Vector2_U64 Max(PA_Vector2_U64 A, PA_Vector2_U64 B) noexcept
    {
        return {
            Max(A.X, B.X),
            Max(A.Y, B.Y)
        };
    }
    ABYTEK_FORCE_INLINE F_Vector2_I8 Max(PA_Vector2_I8 A, PA_Vector2_I8 B) noexcept
    {
        return {
            Max(A.X, B.X),
            Max(A.Y, B.Y)
        };
    }
    ABYTEK_FORCE_INLINE F_Vector2_I16 Max(PA_Vector2_I16 A, PA_Vector2_I16 B) noexcept
    {
        return {
            Max(A.X, B.X),
            Max(A.Y, B.Y)
        };
    }
    
    /**
     * Returns a 3D vector with the component-wise maximum of two input vectors.
     * 
     * @param A First vector to compare
     * @param B Second vector to compare
     * @return Vector with each component being the maximum of the corresponding components in A and B
     */
    ABYTEK_FORCE_INLINE F_Vector3_F32 Max(PA_Vector3_F32 A, PA_Vector3_F32 B) noexcept
    {
        return {
            Max(A.X, B.X),
            Max(A.Y, B.Y),
            Max(A.Z, B.Z)
        };
    }
    ABYTEK_FORCE_INLINE F_Vector3_I32 Max(PA_Vector3_I32 A, PA_Vector3_I32 B) noexcept
    {
        return {
            Max(A.X, B.X),
            Max(A.Y, B.Y),
            Max(A.Z, B.Z)
        };
    }
    ABYTEK_FORCE_INLINE F_Vector3_U32 Max(PA_Vector3_U32 A, PA_Vector3_U32 B) noexcept
    {
        return {
            Max(A.X, B.X),
            Max(A.Y, B.Y),
            Max(A.Z, B.Z)
        };
    }
    ABYTEK_FORCE_INLINE F_Vector3_U16 Max(PA_Vector3_U16 A, PA_Vector3_U16 B) noexcept
    {
        return {
            Max(A.X, B.X),
            Max(A.Y, B.Y),
            Max(A.Z, B.Z)
        };
    }
    ABYTEK_FORCE_INLINE F_Vector3_U8 Max(PA_Vector3_U8 A, PA_Vector3_U8 B) noexcept
    {
        return {
            Max(A.X, B.X),
            Max(A.Y, B.Y),
            Max(A.Z, B.Z)
        };
    }
    ABYTEK_FORCE_INLINE F_Vector3_I64 Max(PA_Vector3_I64 A, PA_Vector3_I64 B) noexcept
    {
        return {
            Max(A.X, B.X),
            Max(A.Y, B.Y),
            Max(A.Z, B.Z)
        };
    }
    ABYTEK_FORCE_INLINE F_Vector3_U64 Max(PA_Vector3_U64 A, PA_Vector3_U64 B) noexcept
    {
        return {
            Max(A.X, B.X),
            Max(A.Y, B.Y),
            Max(A.Z, B.Z)
        };
    }
    ABYTEK_FORCE_INLINE F_Vector3_I8 Max(PA_Vector3_I8 A, PA_Vector3_I8 B) noexcept
    {
        return {
            Max(A.X, B.X),
            Max(A.Y, B.Y),
            Max(A.Z, B.Z)
        };
    }
    ABYTEK_FORCE_INLINE F_Vector3_I16 Max(PA_Vector3_I16 A, PA_Vector3_I16 B) noexcept
    {
        return {
            Max(A.X, B.X),
            Max(A.Y, B.Y),
            Max(A.Z, B.Z)
        };
    }
    
    /**
     * Returns a 4D vector with the component-wise maximum of two input vectors.
     * 
     * @param A First vector to compare
     * @param B Second vector to compare
     * @return Vector with each component being the maximum of the corresponding components in A and B
     */
    ABYTEK_FORCE_INLINE F_Vector4_F32 Max(PA_Vector4_F32 A, PA_Vector4_F32 B) noexcept
    {
        return {
            Max(A.X, B.X),
            Max(A.Y, B.Y),
            Max(A.Z, B.Z),
            Max(A.W, B.W)
        };
    }
    ABYTEK_FORCE_INLINE F_Vector4_I32 Max(PA_Vector4_I32 A, PA_Vector4_I32 B) noexcept
    {
        return {
            Max(A.X, B.X),
            Max(A.Y, B.Y),
            Max(A.Z, B.Z),
            Max(A.W, B.W)
        };
    }
    ABYTEK_FORCE_INLINE F_Vector4_U32 Max(PA_Vector4_U32 A, PA_Vector4_U32 B) noexcept
    {
        return {
            Max(A.X, B.X),
            Max(A.Y, B.Y),
            Max(A.Z, B.Z),
            Max(A.W, B.W)
        };
    }
    ABYTEK_FORCE_INLINE F_Vector4_U16 Max(PA_Vector4_U16 A, PA_Vector4_U16 B) noexcept
    {
        return {
            Max(A.X, B.X),
            Max(A.Y, B.Y),
            Max(A.Z, B.Z),
            Max(A.W, B.W)
        };
    }
    ABYTEK_FORCE_INLINE F_Vector4_U8 Max(PA_Vector4_U8 A, PA_Vector4_U8 B) noexcept
    {
        return {
            Max(A.X, B.X),
            Max(A.Y, B.Y),
            Max(A.Z, B.Z),
            Max(A.W, B.W)
        };
    }
    ABYTEK_FORCE_INLINE F_Vector4_I64 Max(PA_Vector4_I64 A, PA_Vector4_I64 B) noexcept
    {
        return {
            Max(A.X, B.X),
            Max(A.Y, B.Y),
            Max(A.Z, B.Z),
            Max(A.W, B.W)
        };
    }
    ABYTEK_FORCE_INLINE F_Vector4_U64 Max(PA_Vector4_U64 A, PA_Vector4_U64 B) noexcept
    {
        return {
            Max(A.X, B.X),
            Max(A.Y, B.Y),
            Max(A.Z, B.Z),
            Max(A.W, B.W)
        };
    }
    ABYTEK_FORCE_INLINE F_Vector4_I8 Max(PA_Vector4_I8 A, PA_Vector4_I8 B) noexcept
    {
        return {
            Max(A.X, B.X),
            Max(A.Y, B.Y),
            Max(A.Z, B.Z),
            Max(A.W, B.W)
        };
    }
    ABYTEK_FORCE_INLINE F_Vector4_I16 Max(PA_Vector4_I16 A, PA_Vector4_I16 B) noexcept
    {
        return {
            Max(A.X, B.X),
            Max(A.Y, B.Y),
            Max(A.Z, B.Z),
            Max(A.W, B.W)
        };
    }
    
    /**
     * Returns a 2x2 matrix with the component-wise maximum of two input matrices.
     * 
     * @param A First matrix to compare
     * @param B Second matrix to compare
     * @return Matrix with each component being the maximum of the corresponding components in A and B
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_F32 Max(PA_Matrix2x2_F32 A, PA_Matrix2x2_F32 B) noexcept
    {
        return {
            Max(A.A, B.A),
            Max(A.B, B.B)
        };
    }
    ABYTEK_FORCE_INLINE F_Matrix2x2_I32 Max(PA_Matrix2x2_I32 A, PA_Matrix2x2_I32 B) noexcept
    {
        return {
            Max(A.A, B.A),
            Max(A.B, B.B)
        };
    }
    ABYTEK_FORCE_INLINE F_Matrix2x2_U32 Max(PA_Matrix2x2_U32 A, PA_Matrix2x2_U32 B) noexcept
    {
        return {
            Max(A.A, B.A),
            Max(A.B, B.B)
        };
    }
    ABYTEK_FORCE_INLINE F_Matrix2x2_U16 Max(PA_Matrix2x2_U16 A, PA_Matrix2x2_U16 B) noexcept
    {
        return {
            Max(A.A, B.A),
            Max(A.B, B.B)
        };
    }
    ABYTEK_FORCE_INLINE F_Matrix2x2_U8 Max(PA_Matrix2x2_U8 A, PA_Matrix2x2_U8 B) noexcept
    {
        return {
            Max(A.A, B.A),
            Max(A.B, B.B)
        };
    }
    ABYTEK_FORCE_INLINE F_Matrix2x2_I64 Max(PA_Matrix2x2_I64 A, PA_Matrix2x2_I64 B) noexcept
    {
        return {
            Max(A.A, B.A),
            Max(A.B, B.B)
        };
    }
    ABYTEK_FORCE_INLINE F_Matrix2x2_U64 Max(PA_Matrix2x2_U64 A, PA_Matrix2x2_U64 B) noexcept
    {
        return {
            Max(A.A, B.A),
            Max(A.B, B.B)
        };
    }
    ABYTEK_FORCE_INLINE F_Matrix2x2_I8 Max(PA_Matrix2x2_I8 A, PA_Matrix2x2_I8 B) noexcept
    {
        return {
            Max(A.A, B.A),
            Max(A.B, B.B)
        };
    }
    ABYTEK_FORCE_INLINE F_Matrix2x2_I16 Max(PA_Matrix2x2_I16 A, PA_Matrix2x2_I16 B) noexcept
    {
        return {
            Max(A.A, B.A),
            Max(A.B, B.B)
        };
    }
    
    /**
     * Returns a 3x3 matrix with the component-wise maximum of two input matrices.
     * 
     * @param A First matrix to compare
     * @param B Second matrix to compare
     * @return Matrix with each component being the maximum of the corresponding components in A and B
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_F32 Max(PA_Matrix3x3_F32 A, PA_Matrix3x3_F32 B) noexcept
    {
        return {
            Max(A.A, B.A),
            Max(A.B, B.B),
            Max(A.C, B.C)
        };
    }
    ABYTEK_FORCE_INLINE F_Matrix3x3_I32 Max(PA_Matrix3x3_I32 A, PA_Matrix3x3_I32 B) noexcept
    {
        return {
            Max(A.A, B.A),
            Max(A.B, B.B),
            Max(A.C, B.C)
        };
    }
    ABYTEK_FORCE_INLINE F_Matrix3x3_U32 Max(PA_Matrix3x3_U32 A, PA_Matrix3x3_U32 B) noexcept
    {
        return {
            Max(A.A, B.A),
            Max(A.B, B.B),
            Max(A.C, B.C)
        };
    }
    ABYTEK_FORCE_INLINE F_Matrix3x3_U16 Max(PA_Matrix3x3_U16 A, PA_Matrix3x3_U16 B) noexcept
    {
        return {
            Max(A.A, B.A),
            Max(A.B, B.B),
            Max(A.C, B.C)
        };
    }
    ABYTEK_FORCE_INLINE F_Matrix3x3_U8 Max(PA_Matrix3x3_U8 A, PA_Matrix3x3_U8 B) noexcept
    {
        return {
            Max(A.A, B.A),
            Max(A.B, B.B),
            Max(A.C, B.C)
        };
    }
    ABYTEK_FORCE_INLINE F_Matrix3x3_I64 Max(PA_Matrix3x3_I64 A, PA_Matrix3x3_I64 B) noexcept
    {
        return {
            Max(A.A, B.A),
            Max(A.B, B.B),
            Max(A.C, B.C)
        };
    }
    ABYTEK_FORCE_INLINE F_Matrix3x3_U64 Max(PA_Matrix3x3_U64 A, PA_Matrix3x3_U64 B) noexcept
    {
        return {
            Max(A.A, B.A),
            Max(A.B, B.B),
            Max(A.C, B.C)
        };
    }
    ABYTEK_FORCE_INLINE F_Matrix3x3_I8 Max(PA_Matrix3x3_I8 A, PA_Matrix3x3_I8 B) noexcept
    {
        return {
            Max(A.A, B.A),
            Max(A.B, B.B),
            Max(A.C, B.C)
        };
    }
    ABYTEK_FORCE_INLINE F_Matrix3x3_I16 Max(PA_Matrix3x3_I16 A, PA_Matrix3x3_I16 B) noexcept
    {
        return {
            Max(A.A, B.A),
            Max(A.B, B.B),
            Max(A.C, B.C)
        };
    }
    
    /**
     * Returns a 4x4 matrix with the component-wise maximum of two input matrices.
     * 
     * @param A First matrix to compare
     * @param B Second matrix to compare
     * @return Matrix with each component being the maximum of the corresponding components in A and B
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_F32 Max(PA_Matrix4x4_F32 A, PA_Matrix4x4_F32 B) noexcept
    {
        return {
            Max(A.A, B.A),
            Max(A.B, B.B),
            Max(A.C, B.C),
            Max(A.D, B.D)
        };
    }
    ABYTEK_FORCE_INLINE F_Matrix4x4_I32 Max(PA_Matrix4x4_I32 A, PA_Matrix4x4_I32 B) noexcept
    {
        return {
            Max(A.A, B.A),
            Max(A.B, B.B),
            Max(A.C, B.C),
            Max(A.D, B.D)
        };
    }
    ABYTEK_FORCE_INLINE F_Matrix4x4_U32 Max(PA_Matrix4x4_U32 A, PA_Matrix4x4_U32 B) noexcept
    {
        return {
            Max(A.A, B.A),
            Max(A.B, B.B),
            Max(A.C, B.C),
            Max(A.D, B.D)
        };
    }
    ABYTEK_FORCE_INLINE F_Matrix4x4_U16 Max(PA_Matrix4x4_U16 A, PA_Matrix4x4_U16 B) noexcept
    {
        return {
            Max(A.A, B.A),
            Max(A.B, B.B),
            Max(A.C, B.C),
            Max(A.D, B.D)
        };
    }
    ABYTEK_FORCE_INLINE F_Matrix4x4_U8 Max(PA_Matrix4x4_U8 A, PA_Matrix4x4_U8 B) noexcept
    {
        return {
            Max(A.A, B.A),
            Max(A.B, B.B),
            Max(A.C, B.C),
            Max(A.D, B.D)
        };
    }
    ABYTEK_FORCE_INLINE F_Matrix4x4_I64 Max(PA_Matrix4x4_I64 A, PA_Matrix4x4_I64 B) noexcept
    {
        return {
            Max(A.A, B.A),
            Max(A.B, B.B),
            Max(A.C, B.C),
            Max(A.D, B.D)
        };
    }
    ABYTEK_FORCE_INLINE F_Matrix4x4_U64 Max(PA_Matrix4x4_U64 A, PA_Matrix4x4_U64 B) noexcept
    {
        return {
            Max(A.A, B.A),
            Max(A.B, B.B),
            Max(A.C, B.C),
            Max(A.D, B.D)
        };
    }
    ABYTEK_FORCE_INLINE F_Matrix4x4_I8 Max(PA_Matrix4x4_I8 A, PA_Matrix4x4_I8 B) noexcept
    {
        return {
            Max(A.A, B.A),
            Max(A.B, B.B),
            Max(A.C, B.C),
            Max(A.D, B.D)
        };
    }
    ABYTEK_FORCE_INLINE F_Matrix4x4_I16 Max(PA_Matrix4x4_I16 A, PA_Matrix4x4_I16 B) noexcept
    {
        return {
            Max(A.A, B.A),
            Max(A.B, B.B),
            Max(A.C, B.C),
            Max(A.D, B.D)
        };
    }
    
    /**
     * Returns a quaternion with the component-wise maximum of two input quaternions.
     * 
     * @param A First quaternion to compare
     * @param B Second quaternion to compare
     * @return Quaternion with each component being the maximum of the corresponding components in A and B
     */
    ABYTEK_FORCE_INLINE F_Quaternion_F32 Max(PA_Quaternion_F32 A, PA_Quaternion_F32 B) noexcept
    {
        return {
            Max(A.X, B.X),
            Max(A.Y, B.Y),
            Max(A.Z, B.Z),
            Max(A.W, B.W)
        };
    }
}