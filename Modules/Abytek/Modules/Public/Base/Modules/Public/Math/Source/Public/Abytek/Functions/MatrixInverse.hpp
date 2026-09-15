#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data.hpp"
#include "Abytek/Types/Vecma.hpp"
#include "Abytek/Functions/MatrixTranspose.hpp"
#include "Abytek/Functions/MatrixDeterminant.hpp"
#include "Abytek/Functions/MatrixCoFactor.hpp"


namespace Abytek
{
    /**
     * Calculates the inverse of a 2x2 matrix using the provided determinant.
     * 
     * @param A The input 2x2 matrix
     * @param Determinant The pre-calculated determinant of the matrix
     * @return The inverse of the input matrix
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_F32 Inverse(PA_Matrix2x2_F32 A, F32 Determinant) noexcept
    {
        return H_Data2x2::Divide<F32, F_VecmaConfig>(Transpose(CoFactor(A)), Determinant);
    }    
    
    /**
     * Calculates the inverse of a 3x3 matrix using the provided determinant.
     * 
     * @param A The input 3x3 matrix
     * @param Determinant The pre-calculated determinant of the matrix
     * @return The inverse of the input matrix
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_F32 Inverse(PA_Matrix3x3_F32 A, F32 Determinant) noexcept
    {
        return H_Data3x3::Divide<F32, F_VecmaConfig>(Transpose(CoFactor(A)), Determinant);
    }    
    
    /**
     * Calculates the inverse of a 4x4 matrix using the provided determinant.
     * 
     * @param A The input 4x4 matrix
     * @param Determinant The pre-calculated determinant of the matrix
     * @return The inverse of the input matrix
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_F32 Inverse(PA_Matrix4x4_F32 A, F32 Determinant) noexcept
    {
        return H_Data4x4::Divide<F32, F_VecmaConfig>(Transpose(CoFactor(A)), Determinant);
    }
    
    /**
     * Calculates the inverse of a 2x2 matrix.
     * Computes the determinant internally.
     * 
     * @param A The input 2x2 matrix
     * @return The inverse of the input matrix
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_F32 Inverse(PA_Matrix2x2_F32 A) noexcept
    {
        return Inverse(A, Determinant(A));
    }    
    
    /**
     * Calculates the inverse of a 3x3 matrix.
     * Computes the determinant internally.
     * 
     * @param A The input 3x3 matrix
     * @return The inverse of the input matrix
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_F32 Inverse(PA_Matrix3x3_F32 A) noexcept
    {
        return Inverse(A, Determinant(A));
    }    
    
    /**
     * Calculates the inverse of a 4x4 matrix.
     * Computes the determinant internally.
     * 
     * @param A The input 4x4 matrix
     * @return The inverse of the input matrix
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_F32 Inverse(PA_Matrix4x4_F32 A) noexcept
    {
        return Inverse(A, Determinant(A));
    }
    
}