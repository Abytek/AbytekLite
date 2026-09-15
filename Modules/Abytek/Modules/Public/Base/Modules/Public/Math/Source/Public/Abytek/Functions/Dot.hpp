/**
 * @file Dot.hpp
 * @brief Dot product operations for various vector types
 * 
 * Provides dot product functions for Vector2, Vector3, Vector4 and Quaternion
 * types with different element types (F32, I32, U32).
 */
#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Vecma.hpp"
#include "Abytek/Types/Quaternion.hpp"
#include "Abytek/Functions/Data2_NonSIMD.hpp"
#include "Abytek/Functions/Data3_NonSIMD.hpp"
#include "Abytek/Functions/Data4_NonSIMD.hpp"


namespace Abytek
{
    /**
     * @brief Calculates the dot product of two 2D floating-point vectors
     * 
     * @param A First vector
     * @param B Second vector
     * @return Dot product as F32
     */
    ABYTEK_FORCE_INLINE F32 Dot(PA_Vector2_F32 A, PA_Vector2_F32 B) noexcept
    {
        return H_Data2::template Dot<F32, F_VecmaConfig>(A, B);
    }
    
    /**
     * @brief Calculates the dot product of two 2D signed integer vectors
     * 
     * @param A First vector
     * @param B Second vector
     * @return Dot product as I32
     */
    ABYTEK_FORCE_INLINE I32 Dot(PA_Vector2_I32 A, PA_Vector2_I32 B) noexcept
    {
        return H_Data2::template Dot<I32, F_VecmaConfig>(A, B);
    }
    
    /**
     * @brief Calculates the dot product of two 2D unsigned integer vectors
     * 
     * @param A First vector
     * @param B Second vector
     * @return Dot product as U32
     */
    ABYTEK_FORCE_INLINE U32 Dot(PA_Vector2_U32 A, PA_Vector2_U32 B) noexcept
    {
        return H_Data2::template Dot<U32, F_VecmaConfig>(A, B);
    }
    
    /**
     * @brief Calculates the dot product of two 2D unsigned integer vectors
     * 
     * @param A First vector
     * @param B Second vector
     * @return Dot product as U16
     */
    ABYTEK_FORCE_INLINE U16 Dot(PA_Vector2_U16 A, PA_Vector2_U16 B) noexcept
    {
        return H_Data2::template Dot<U16, F_VecmaConfig>(A, B);
    }
    
    /**
     * @brief Calculates the dot product of two 2D unsigned integer vectors
     * 
     * @param A First vector
     * @param B Second vector
     * @return Dot product as U8
     */
    ABYTEK_FORCE_INLINE U8 Dot(PA_Vector2_U8 A, PA_Vector2_U8 B) noexcept
    {
        return H_Data2::template Dot<U8, F_VecmaConfig>(A, B);
    }
    
    /**
     * @brief Calculates the dot product of two 2D unsigned integer vectors
     * 
     * @param A First vector
     * @param B Second vector
     * @return Dot product as I64
     */
    ABYTEK_FORCE_INLINE I64 Dot(PA_Vector2_I64 A, PA_Vector2_I64 B) noexcept
    {
        return H_Data2::template Dot<I64, F_VecmaConfig>(A, B);
    }
    
    /**
     * @brief Calculates the dot product of two 2D unsigned integer vectors
     * 
     * @param A First vector
     * @param B Second vector
     * @return Dot product as U64
     */
    ABYTEK_FORCE_INLINE U64 Dot(PA_Vector2_U64 A, PA_Vector2_U64 B) noexcept
    {
        return H_Data2::template Dot<U64, F_VecmaConfig>(A, B);
    }
    
    /**
     * @brief Calculates the dot product of two 2D unsigned integer vectors
     * 
     * @param A First vector
     * @param B Second vector
     * @return Dot product as I8
     */
    ABYTEK_FORCE_INLINE I8 Dot(PA_Vector2_I8 A, PA_Vector2_I8 B) noexcept
    {
        return H_Data2::template Dot<I8, F_VecmaConfig>(A, B);
    }
    
    /**
     * @brief Calculates the dot product of two 2D unsigned integer vectors
     * 
     * @param A First vector
     * @param B Second vector
     * @return Dot product as I16
     */
    ABYTEK_FORCE_INLINE I16 Dot(PA_Vector2_I16 A, PA_Vector2_I16 B) noexcept
    {
        return H_Data2::template Dot<I16, F_VecmaConfig>(A, B);
    }
    
    /**
     * @brief Calculates the dot product of two 3D floating-point vectors
     * 
     * @param A First vector
     * @param B Second vector
     * @return Dot product as F32
     */
    ABYTEK_FORCE_INLINE F32 Dot(PA_Vector3_F32 A, PA_Vector3_F32 B) noexcept
    {
        return H_Data3::template Dot<F32, F_VecmaConfig>(A, B);
    }
    
    /**
     * @brief Calculates the dot product of two 3D signed integer vectors
     * 
     * @param A First vector
     * @param B Second vector
     * @return Dot product as I32
     */
    ABYTEK_FORCE_INLINE I32 Dot(PA_Vector3_I32 A, PA_Vector3_I32 B) noexcept
    {
        return H_Data3::template Dot<I32, F_VecmaConfig>(A, B);
    }
    
    /**
     * @brief Calculates the dot product of two 3D unsigned integer vectors
     * 
     * @param A First vector
     * @param B Second vector
     * @return Dot product as U32
     */
    ABYTEK_FORCE_INLINE U32 Dot(PA_Vector3_U32 A, PA_Vector3_U32 B) noexcept
    {
        return H_Data3::template Dot<U32, F_VecmaConfig>(A, B);
    }
    
    /**
     * @brief Calculates the dot product of two 3D unsigned integer vectors
     * 
     * @param A First vector
     * @param B Second vector
     * @return Dot product as U16
     */
    ABYTEK_FORCE_INLINE U16 Dot(PA_Vector3_U16 A, PA_Vector3_U16 B) noexcept
    {
        return H_Data3::template Dot<U16, F_VecmaConfig>(A, B);
    }
    
    /**
     * @brief Calculates the dot product of two 3D unsigned integer vectors
     * 
     * @param A First vector
     * @param B Second vector
     * @return Dot product as U8
     */
    ABYTEK_FORCE_INLINE U8 Dot(PA_Vector3_U8 A, PA_Vector3_U8 B) noexcept
    {
        return H_Data3::template Dot<U8, F_VecmaConfig>(A, B);
    }
    
    /**
     * @brief Calculates the dot product of two 3D unsigned integer vectors
     * 
     * @param A First vector
     * @param B Second vector
     * @return Dot product as I64
     */
    ABYTEK_FORCE_INLINE I64 Dot(PA_Vector3_I64 A, PA_Vector3_I64 B) noexcept
    {
        return H_Data3::template Dot<I64, F_VecmaConfig>(A, B);
    }
    
    /**
     * @brief Calculates the dot product of two 3D unsigned integer vectors
     * 
     * @param A First vector
     * @param B Second vector
     * @return Dot product as U64
     */
    ABYTEK_FORCE_INLINE U64 Dot(PA_Vector3_U64 A, PA_Vector3_U64 B) noexcept
    {
        return H_Data3::template Dot<U64, F_VecmaConfig>(A, B);
    }
    
    /**
     * @brief Calculates the dot product of two 3D unsigned integer vectors
     * 
     * @param A First vector
     * @param B Second vector
     * @return Dot product as I8
     */
    ABYTEK_FORCE_INLINE I8 Dot(PA_Vector3_I8 A, PA_Vector3_I8 B) noexcept
    {
        return H_Data3::template Dot<I8, F_VecmaConfig>(A, B);
    }
    
    /**
     * @brief Calculates the dot product of two 3D unsigned integer vectors
     * 
     * @param A First vector
     * @param B Second vector
     * @return Dot product as I16
     */
    ABYTEK_FORCE_INLINE I16 Dot(PA_Vector3_I16 A, PA_Vector3_I16 B) noexcept
    {
        return H_Data3::template Dot<I16, F_VecmaConfig>(A, B);
    }
    
    /**
     * @brief Calculates the dot product of two 4D floating-point vectors
     * 
     * @param A First vector
     * @param B Second vector
     * @return Dot product as F32
     */
    ABYTEK_FORCE_INLINE F32 Dot(PA_Vector4_F32 A, PA_Vector4_F32 B) noexcept
    {
        return H_Data4::template Dot<F32, F_VecmaConfig>(A, B);
    }
    
    /**
     * @brief Calculates the dot product of two 4D signed integer vectors
     * 
     * @param A First vector
     * @param B Second vector
     * @return Dot product as I32
     */
    ABYTEK_FORCE_INLINE I32 Dot(PA_Vector4_I32 A, PA_Vector4_I32 B) noexcept
    {
        return H_Data4::template Dot<I32, F_VecmaConfig>(A, B);
    }
    
    /**
     * @brief Calculates the dot product of two 4D unsigned integer vectors
     * 
     * @param A First vector
     * @param B Second vector
     * @return Dot product as U32
     */
    ABYTEK_FORCE_INLINE U32 Dot(PA_Vector4_U32 A, PA_Vector4_U32 B) noexcept
    {
        return H_Data4::template Dot<U32, F_VecmaConfig>(A, B);
    }
    
    /**
     * @brief Calculates the dot product of two 4D unsigned integer vectors
     * 
     * @param A First vector
     * @param B Second vector
     * @return Dot product as U16
     */
    ABYTEK_FORCE_INLINE U16 Dot(PA_Vector4_U16 A, PA_Vector4_U16 B) noexcept
    {
        return H_Data4::template Dot<U16, F_VecmaConfig>(A, B);
    }
    
    /**
     * @brief Calculates the dot product of two 4D unsigned integer vectors
     * 
     * @param A First vector
     * @param B Second vector
     * @return Dot product as U8
     */
    ABYTEK_FORCE_INLINE U8 Dot(PA_Vector4_U8 A, PA_Vector4_U8 B) noexcept
    {
        return H_Data4::template Dot<U8, F_VecmaConfig>(A, B);
    }
    
    /**
     * @brief Calculates the dot product of two 4D unsigned integer vectors
     * 
     * @param A First vector
     * @param B Second vector
     * @return Dot product as I64
     */
    ABYTEK_FORCE_INLINE I64 Dot(PA_Vector4_I64 A, PA_Vector4_I64 B) noexcept
    {
        return H_Data4::template Dot<I64, F_VecmaConfig>(A, B);
    }
    
    /**
     * @brief Calculates the dot product of two 4D unsigned integer vectors
     * 
     * @param A First vector
     * @param B Second vector
     * @return Dot product as U64
     */
    ABYTEK_FORCE_INLINE U64 Dot(PA_Vector4_U64 A, PA_Vector4_U64 B) noexcept
    {
        return H_Data4::template Dot<U64, F_VecmaConfig>(A, B);
    }
    
    /**
     * @brief Calculates the dot product of two 4D unsigned integer vectors
     * 
     * @param A First vector
     * @param B Second vector
     * @return Dot product as I8
     */
    ABYTEK_FORCE_INLINE I8 Dot(PA_Vector4_I8 A, PA_Vector4_I8 B) noexcept
    {
        return H_Data4::template Dot<I8, F_VecmaConfig>(A, B);
    }
    
    /**
     * @brief Calculates the dot product of two 4D unsigned integer vectors
     * 
     * @param A First vector
     * @param B Second vector
     * @return Dot product as I16
     */
    ABYTEK_FORCE_INLINE I16 Dot(PA_Vector4_I16 A, PA_Vector4_I16 B) noexcept
    {
        return H_Data4::template Dot<I16, F_VecmaConfig>(A, B);
    }
    
    /**
     * @brief Calculates the dot product of two quaternions
     * 
     * @param A First quaternion
     * @param B Second quaternion
     * @return Dot product as F32
     */
    ABYTEK_FORCE_INLINE F32 Dot(PA_Quaternion_F32 A, PA_Quaternion_F32 B) noexcept
    {
        return H_Data4::template Dot<F32, F_QuaternionConfig>(A, B);
    }
}