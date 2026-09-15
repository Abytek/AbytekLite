#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data.hpp"
#include "Abytek/Types/Vecma.hpp"
#include "Abytek/Types/Quaternion.hpp"


namespace Abytek
{
    /**
     * @brief Returns the sign of each component in a 2D float vector
     * 
     * @param A Input 2D float vector
     * @return F_Vector2_F32 Vector with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Vector2_F32 Sign(PA_Vector2_F32 A) noexcept
    {
        return {
            Sign(A.X),
            Sign(A.Y)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 2D signed integer vector
     * 
     * @param A Input 2D signed integer vector
     * @return F_Vector2_I32 Vector with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Vector2_I32 Sign(PA_Vector2_I32 A) noexcept
    {
        return {
            Sign(A.X),
            Sign(A.Y)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 2D unsigned integer vector
     * 
     * @param A Input 2D unsigned integer vector
     * @return F_Vector2_U32 Vector with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Vector2_U32 Sign(PA_Vector2_U32 A) noexcept
    {
        return {
            Sign(A.X),
            Sign(A.Y)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 2D unsigned integer vector
     * 
     * @param A Input 2D unsigned integer vector
     * @return F_Vector2_U16 Vector with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Vector2_U16 Sign(PA_Vector2_U16 A) noexcept
    {
        return {
            Sign(A.X),
            Sign(A.Y)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 2D unsigned integer vector
     * 
     * @param A Input 2D unsigned integer vector
     * @return F_Vector2_U8 Vector with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Vector2_U8 Sign(PA_Vector2_U8 A) noexcept
    {
        return {
            Sign(A.X),
            Sign(A.Y)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 2D unsigned integer vector
     * 
     * @param A Input 2D unsigned integer vector
     * @return F_Vector2_I64 Vector with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Vector2_I64 Sign(PA_Vector2_I64 A) noexcept
    {
        return {
            Sign(A.X),
            Sign(A.Y)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 2D unsigned integer vector
     * 
     * @param A Input 2D unsigned integer vector
     * @return F_Vector2_U64 Vector with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Vector2_U64 Sign(PA_Vector2_U64 A) noexcept
    {
        return {
            Sign(A.X),
            Sign(A.Y)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 2D unsigned integer vector
     * 
     * @param A Input 2D unsigned integer vector
     * @return F_Vector2_I8 Vector with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Vector2_I8 Sign(PA_Vector2_I8 A) noexcept
    {
        return {
            Sign(A.X),
            Sign(A.Y)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 2D unsigned integer vector
     * 
     * @param A Input 2D unsigned integer vector
     * @return F_Vector2_I16 Vector with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Vector2_I16 Sign(PA_Vector2_I16 A) noexcept
    {
        return {
            Sign(A.X),
            Sign(A.Y)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 3D float vector
     * 
     * @param A Input 3D float vector
     * @return F_Vector3_F32 Vector with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Vector3_F32 Sign(PA_Vector3_F32 A) noexcept
    {
        return {
            Sign(A.X),
            Sign(A.Y),
            Sign(A.Z)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 3D signed integer vector
     * 
     * @param A Input 3D signed integer vector
     * @return F_Vector3_I32 Vector with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Vector3_I32 Sign(PA_Vector3_I32 A) noexcept
    {
        return {
            Sign(A.X),
            Sign(A.Y),
            Sign(A.Z)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 3D unsigned integer vector
     * 
     * @param A Input 3D unsigned integer vector
     * @return F_Vector3_U32 Vector with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Vector3_U32 Sign(PA_Vector3_U32 A) noexcept
    {
        return {
            Sign(A.X),
            Sign(A.Y),
            Sign(A.Z)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 3D unsigned integer vector
     * 
     * @param A Input 3D unsigned integer vector
     * @return F_Vector3_U16 Vector with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Vector3_U16 Sign(PA_Vector3_U16 A) noexcept
    {
        return {
            Sign(A.X),
            Sign(A.Y),
            Sign(A.Z)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 3D unsigned integer vector
     * 
     * @param A Input 3D unsigned integer vector
     * @return F_Vector3_U8 Vector with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Vector3_U8 Sign(PA_Vector3_U8 A) noexcept
    {
        return {
            Sign(A.X),
            Sign(A.Y),
            Sign(A.Z)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 3D unsigned integer vector
     * 
     * @param A Input 3D unsigned integer vector
     * @return F_Vector3_I64 Vector with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Vector3_I64 Sign(PA_Vector3_I64 A) noexcept
    {
        return {
            Sign(A.X),
            Sign(A.Y),
            Sign(A.Z)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 3D unsigned integer vector
     * 
     * @param A Input 3D unsigned integer vector
     * @return F_Vector3_U64 Vector with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Vector3_U64 Sign(PA_Vector3_U64 A) noexcept
    {
        return {
            Sign(A.X),
            Sign(A.Y),
            Sign(A.Z)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 3D unsigned integer vector
     * 
     * @param A Input 3D unsigned integer vector
     * @return F_Vector3_I8 Vector with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Vector3_I8 Sign(PA_Vector3_I8 A) noexcept
    {
        return {
            Sign(A.X),
            Sign(A.Y),
            Sign(A.Z)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 3D unsigned integer vector
     * 
     * @param A Input 3D unsigned integer vector
     * @return F_Vector3_I16 Vector with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Vector3_I16 Sign(PA_Vector3_I16 A) noexcept
    {
        return {
            Sign(A.X),
            Sign(A.Y),
            Sign(A.Z)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 4D float vector
     * 
     * @param A Input 4D float vector
     * @return F_Vector4_F32 Vector with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Vector4_F32 Sign(PA_Vector4_F32 A) noexcept
    {
        return {
            Sign(A.X),
            Sign(A.Y),
            Sign(A.Z),
            Sign(A.W)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 4D signed integer vector
     * 
     * @param A Input 4D signed integer vector
     * @return F_Vector4_I32 Vector with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Vector4_I32 Sign(PA_Vector4_I32 A) noexcept
    {
        return {
            Sign(A.X),
            Sign(A.Y),
            Sign(A.Z),
            Sign(A.W)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 4D unsigned integer vector
     * 
     * @param A Input 4D unsigned integer vector
     * @return F_Vector4_U32 Vector with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Vector4_U32 Sign(PA_Vector4_U32 A) noexcept
    {
        return {
            Sign(A.X),
            Sign(A.Y),
            Sign(A.Z),
            Sign(A.W)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 4D unsigned integer vector
     * 
     * @param A Input 4D unsigned integer vector
     * @return F_Vector4_U16 Vector with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Vector4_U16 Sign(PA_Vector4_U16 A) noexcept
    {
        return {
            Sign(A.X),
            Sign(A.Y),
            Sign(A.Z),
            Sign(A.W)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 4D unsigned integer vector
     * 
     * @param A Input 4D unsigned integer vector
     * @return F_Vector4_U8 Vector with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Vector4_U8 Sign(PA_Vector4_U8 A) noexcept
    {
        return {
            Sign(A.X),
            Sign(A.Y),
            Sign(A.Z),
            Sign(A.W)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 4D unsigned integer vector
     * 
     * @param A Input 4D unsigned integer vector
     * @return F_Vector4_I64 Vector with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Vector4_I64 Sign(PA_Vector4_I64 A) noexcept
    {
        return {
            Sign(A.X),
            Sign(A.Y),
            Sign(A.Z),
            Sign(A.W)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 4D unsigned integer vector
     * 
     * @param A Input 4D unsigned integer vector
     * @return F_Vector4_U64 Vector with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Vector4_U64 Sign(PA_Vector4_U64 A) noexcept
    {
        return {
            Sign(A.X),
            Sign(A.Y),
            Sign(A.Z),
            Sign(A.W)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 4D unsigned integer vector
     * 
     * @param A Input 4D unsigned integer vector
     * @return F_Vector4_I8 Vector with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Vector4_I8 Sign(PA_Vector4_I8 A) noexcept
    {
        return {
            Sign(A.X),
            Sign(A.Y),
            Sign(A.Z),
            Sign(A.W)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 4D unsigned integer vector
     * 
     * @param A Input 4D unsigned integer vector
     * @return F_Vector4_I16 Vector with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Vector4_I16 Sign(PA_Vector4_I16 A) noexcept
    {
        return {
            Sign(A.X),
            Sign(A.Y),
            Sign(A.Z),
            Sign(A.W)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 2x2 float matrix
     * 
     * @param A Input 2x2 float matrix
     * @return F_Matrix2x2_F32 Matrix with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_F32 Sign(PA_Matrix2x2_F32 A) noexcept
    {
        return {
            Sign(A.A),
            Sign(A.B)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 2x2 signed integer matrix
     * 
     * @param A Input 2x2 signed integer matrix
     * @return F_Matrix2x2_I32 Matrix with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_I32 Sign(PA_Matrix2x2_I32 A) noexcept
    {
        return {
            Sign(A.A),
            Sign(A.B)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 2x2 unsigned integer matrix
     * 
     * @param A Input 2x2 unsigned integer matrix
     * @return F_Matrix2x2_U32 Matrix with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_U32 Sign(PA_Matrix2x2_U32 A) noexcept
    {
        return {
            Sign(A.A),
            Sign(A.B)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 2x2 unsigned integer matrix
     * 
     * @param A Input 2x2 unsigned integer matrix
     * @return F_Matrix2x2_U16 Matrix with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_U16 Sign(PA_Matrix2x2_U16 A) noexcept
    {
        return {
            Sign(A.A),
            Sign(A.B)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 2x2 unsigned integer matrix
     * 
     * @param A Input 2x2 unsigned integer matrix
     * @return F_Matrix2x2_U8 Matrix with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_U8 Sign(PA_Matrix2x2_U8 A) noexcept
    {
        return {
            Sign(A.A),
            Sign(A.B)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 2x2 unsigned integer matrix
     * 
     * @param A Input 2x2 unsigned integer matrix
     * @return F_Matrix2x2_I64 Matrix with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_I64 Sign(PA_Matrix2x2_I64 A) noexcept
    {
        return {
            Sign(A.A),
            Sign(A.B)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 2x2 unsigned integer matrix
     * 
     * @param A Input 2x2 unsigned integer matrix
     * @return F_Matrix2x2_U64 Matrix with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_U64 Sign(PA_Matrix2x2_U64 A) noexcept
    {
        return {
            Sign(A.A),
            Sign(A.B)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 2x2 unsigned integer matrix
     * 
     * @param A Input 2x2 unsigned integer matrix
     * @return F_Matrix2x2_I8 Matrix with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_I8 Sign(PA_Matrix2x2_I8 A) noexcept
    {
        return {
            Sign(A.A),
            Sign(A.B)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 2x2 unsigned integer matrix
     * 
     * @param A Input 2x2 unsigned integer matrix
     * @return F_Matrix2x2_I16 Matrix with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_I16 Sign(PA_Matrix2x2_I16 A) noexcept
    {
        return {
            Sign(A.A),
            Sign(A.B)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 3x3 float matrix
     * 
     * @param A Input 3x3 float matrix
     * @return F_Matrix3x3_F32 Matrix with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_F32 Sign(PA_Matrix3x3_F32 A) noexcept
    {
        return {
            Sign(A.A),
            Sign(A.B),
            Sign(A.C)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 3x3 signed integer matrix
     * 
     * @param A Input 3x3 signed integer matrix
     * @return F_Matrix3x3_I32 Matrix with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_I32 Sign(PA_Matrix3x3_I32 A) noexcept
    {
        return {
            Sign(A.A),
            Sign(A.B),
            Sign(A.C)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 3x3 unsigned integer matrix
     * 
     * @param A Input 3x3 unsigned integer matrix
     * @return F_Matrix3x3_U32 Matrix with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_U32 Sign(PA_Matrix3x3_U32 A) noexcept
    {
        return {
            Sign(A.A),
            Sign(A.B),
            Sign(A.C)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 3x3 unsigned integer matrix
     * 
     * @param A Input 3x3 unsigned integer matrix
     * @return F_Matrix3x3_U16 Matrix with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_U16 Sign(PA_Matrix3x3_U16 A) noexcept
    {
        return {
            Sign(A.A),
            Sign(A.B),
            Sign(A.C)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 3x3 unsigned integer matrix
     * 
     * @param A Input 3x3 unsigned integer matrix
     * @return F_Matrix3x3_U8 Matrix with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_U8 Sign(PA_Matrix3x3_U8 A) noexcept
    {
        return {
            Sign(A.A),
            Sign(A.B),
            Sign(A.C)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 3x3 unsigned integer matrix
     * 
     * @param A Input 3x3 unsigned integer matrix
     * @return F_Matrix3x3_I64 Matrix with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_I64 Sign(PA_Matrix3x3_I64 A) noexcept
    {
        return {
            Sign(A.A),
            Sign(A.B),
            Sign(A.C)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 3x3 unsigned integer matrix
     * 
     * @param A Input 3x3 unsigned integer matrix
     * @return F_Matrix3x3_U64 Matrix with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_U64 Sign(PA_Matrix3x3_U64 A) noexcept
    {
        return {
            Sign(A.A),
            Sign(A.B),
            Sign(A.C)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 3x3 unsigned integer matrix
     * 
     * @param A Input 3x3 unsigned integer matrix
     * @return F_Matrix3x3_I8 Matrix with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_I8 Sign(PA_Matrix3x3_I8 A) noexcept
    {
        return {
            Sign(A.A),
            Sign(A.B),
            Sign(A.C)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 3x3 unsigned integer matrix
     * 
     * @param A Input 3x3 unsigned integer matrix
     * @return F_Matrix3x3_I16 Matrix with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_I16 Sign(PA_Matrix3x3_I16 A) noexcept
    {
        return {
            Sign(A.A),
            Sign(A.B),
            Sign(A.C)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 4x4 float matrix
     * 
     * @param A Input 4x4 float matrix
     * @return F_Matrix4x4_F32 Matrix with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_F32 Sign(PA_Matrix4x4_F32 A) noexcept
    {
        return {
            Sign(A.A),
            Sign(A.B),
            Sign(A.C),
            Sign(A.D)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 4x4 signed integer matrix
     * 
     * @param A Input 4x4 signed integer matrix
     * @return F_Matrix4x4_I32 Matrix with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_I32 Sign(PA_Matrix4x4_I32 A) noexcept
    {
        return {
            Sign(A.A),
            Sign(A.B),
            Sign(A.C),
            Sign(A.D)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 4x4 unsigned integer matrix
     * 
     * @param A Input 4x4 unsigned integer matrix
     * @return F_Matrix4x4_U32 Matrix with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_U32 Sign(PA_Matrix4x4_U32 A) noexcept
    {
        return {
            Sign(A.A),
            Sign(A.B),
            Sign(A.C),
            Sign(A.D)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 4x4 unsigned integer matrix
     * 
     * @param A Input 4x4 unsigned integer matrix
     * @return F_Matrix4x4_U16 Matrix with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_U16 Sign(PA_Matrix4x4_U16 A) noexcept
    {
        return {
            Sign(A.A),
            Sign(A.B),
            Sign(A.C),
            Sign(A.D)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 4x4 unsigned integer matrix
     * 
     * @param A Input 4x4 unsigned integer matrix
     * @return F_Matrix4x4_U8 Matrix with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_U8 Sign(PA_Matrix4x4_U8 A) noexcept
    {
        return {
            Sign(A.A),
            Sign(A.B),
            Sign(A.C),
            Sign(A.D)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 4x4 unsigned integer matrix
     * 
     * @param A Input 4x4 unsigned integer matrix
     * @return F_Matrix4x4_I64 Matrix with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_I64 Sign(PA_Matrix4x4_I64 A) noexcept
    {
        return {
            Sign(A.A),
            Sign(A.B),
            Sign(A.C),
            Sign(A.D)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 4x4 unsigned integer matrix
     * 
     * @param A Input 4x4 unsigned integer matrix
     * @return F_Matrix4x4_U64 Matrix with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_U64 Sign(PA_Matrix4x4_U64 A) noexcept
    {
        return {
            Sign(A.A),
            Sign(A.B),
            Sign(A.C),
            Sign(A.D)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 4x4 unsigned integer matrix
     * 
     * @param A Input 4x4 unsigned integer matrix
     * @return F_Matrix4x4_I8 Matrix with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_I8 Sign(PA_Matrix4x4_I8 A) noexcept
    {
        return {
            Sign(A.A),
            Sign(A.B),
            Sign(A.C),
            Sign(A.D)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a 4x4 unsigned integer matrix
     * 
     * @param A Input 4x4 unsigned integer matrix
     * @return F_Matrix4x4_I16 Matrix with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_I16 Sign(PA_Matrix4x4_I16 A) noexcept
    {
        return {
            Sign(A.A),
            Sign(A.B),
            Sign(A.C),
            Sign(A.D)
        };
    }
    
    /**
     * @brief Returns the sign of each component in a quaternion
     * 
     * @param A Input quaternion
     * @return F_Quaternion_F32 Quaternion with sign of each component
     */
    ABYTEK_FORCE_INLINE F_Quaternion_F32 Sign(PA_Quaternion_F32 A) noexcept
    {
        return {
            Sign(A.X),
            Sign(A.Y),
            Sign(A.Z),
            Sign(A.W)
        };
    }
}