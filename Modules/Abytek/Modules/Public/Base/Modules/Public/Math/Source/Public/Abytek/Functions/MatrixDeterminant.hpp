#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data.hpp"
#include "Abytek/Types/Vecma.hpp"


namespace Abytek
{
    /**
     * Calculates the determinant of a 2x2 matrix.
     * 
     * @param A The input 2x2 matrix
     * @return The determinant value
     */
    ABYTEK_FORCE_INLINE F32 Determinant(PA_Matrix2x2_F32 A) noexcept
    {
        return {
            (A.A.X * A.B.Y)
            - (A.B.X * A.A.Y)
        };
    }
    
    /**
     * Calculates the determinant of a 3x3 matrix using cofactor expansion.
     * 
     * @param A The input 3x3 matrix
     * @return The determinant value
     */
    ABYTEK_FORCE_INLINE F32 Determinant(PA_Matrix3x3_F32 A) noexcept
    {
        F_Matrix2x2_F32 aa = {
            A.B.StaticPermute<1, 2>(),
            A.C.StaticPermute<1, 2>()
        };
        F_Matrix2x2_F32 ab = {
            A.A.StaticPermute<1, 2>(),
            A.C.StaticPermute<1, 2>()
        };
        F_Matrix2x2_F32 ac = {
            A.A.StaticPermute<1, 2>(),
            A.B.StaticPermute<1, 2>()
        };

        return (
            A.A.X * Determinant(aa)
            - A.B.X * Determinant(ab)
            + A.C.X * Determinant(ac)
        );
    }
    
    /**
     * Calculates the determinant of a 4x4 matrix using cofactor expansion.
     * 
     * @param A The input 4x4 matrix
     * @return The determinant value
     */
    ABYTEK_FORCE_INLINE F32 Determinant(PA_Matrix4x4_F32 A) noexcept
    {
        F_Matrix3x3_F32 aa = {
            A.B.StaticPermute<1, 2, 3>(),
            A.C.StaticPermute<1, 2, 3>(),
            A.D.StaticPermute<1, 2, 3>()
        };
        F_Matrix3x3_F32 ab = {
            A.A.StaticPermute<1, 2, 3>(),
            A.C.StaticPermute<1, 2, 3>(),
            A.D.StaticPermute<1, 2, 3>()
        };
        F_Matrix3x3_F32 ac = {
            A.A.StaticPermute<1, 2, 3>(),
            A.B.StaticPermute<1, 2, 3>(),
            A.D.StaticPermute<1, 2, 3>()
        };
        F_Matrix3x3_F32 ad = {
            A.A.StaticPermute<1, 2, 3>(),
            A.B.StaticPermute<1, 2, 3>(),
            A.C.StaticPermute<1, 2, 3>()
        };

        return (
            A.A.X * Determinant(aa)
            - A.B.X * Determinant(ab)
            + A.C.X * Determinant(ac)
            - A.D.X * Determinant(ad)
        );
    }
}