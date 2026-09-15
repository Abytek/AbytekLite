#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data.hpp"
#include "Abytek/Types/Vecma.hpp"
#include "Abytek/Functions/MatrixDeterminant.hpp"


namespace Abytek
{
    /**
     * Calculates the cofactor matrix for a 2x2 matrix.
     * 
     * @param a The input 2x2 matrix
     * @return The cofactor matrix of the input
     */
    ABYTEK_FORCE_INLINE F_Matrix2x2_F32 CoFactor(PA_Matrix2x2_F32 a) noexcept {

        return {
            F_Vector2_F32(a.B.Y, -a.B.X),
            F_Vector2_F32(-a.A.Y, a.A.X)
        };
    }

    /**
     * Calculates the cofactor matrix for a 3x3 matrix.
     * Each element of the cofactor matrix is the determinant of the
     * minor matrix multiplied by the appropriate sign.
     * 
     * @param a The input 3x3 matrix
     * @return The cofactor matrix of the input
     */
    ABYTEK_FORCE_INLINE F_Matrix3x3_F32 CoFactor(PA_Matrix3x3_F32 a) noexcept {

        F_Matrix2x2_F32 aax = {
            a.B.StaticPermute<1, 2>(),
            a.C.StaticPermute<1, 2>()
        };
        F_Matrix2x2_F32 abx = {
            a.A.StaticPermute<1, 2>(),
            a.C.StaticPermute<1, 2>()
        };
        F_Matrix2x2_F32 acx = {
            a.A.StaticPermute<1, 2>(),
            a.B.StaticPermute<1, 2>()
        };

        F_Matrix2x2_F32 aay = {
            a.B.StaticPermute<0, 2>(),
            a.C.StaticPermute<0, 2>()
        };
        F_Matrix2x2_F32 aby = {
            a.A.StaticPermute<0, 2>(),
            a.C.StaticPermute<0, 2>()
        };
        F_Matrix2x2_F32 acy = {
            a.A.StaticPermute<0, 2>(),
            a.B.StaticPermute<0, 2>()
        };

        F_Matrix2x2_F32 aaz = {
            a.B.StaticPermute<0, 1>(),
            a.C.StaticPermute<0, 1>()
        };
        F_Matrix2x2_F32 abz = {
            a.A.StaticPermute<0, 1>(),
            a.C.StaticPermute<0, 1>()
        };
        F_Matrix2x2_F32 acz = {
            a.A.StaticPermute<0, 1>(),
            a.B.StaticPermute<0, 1>()
        };

        return {
            F_Vector3_F32(
                Determinant(aax),
                - Determinant(aay),
                Determinant(aaz)
            ),
            F_Vector3_F32(
                - Determinant(abx),
                + Determinant(aby),
                - Determinant(abz)
            ),
            F_Vector3_F32(
                Determinant(acx),
                - Determinant(acy),
                Determinant(acz)
            )
        };
    }

    /**
     * Calculates the cofactor matrix for a 4x4 matrix.
     * Each element of the cofactor matrix is the determinant of the
     * minor matrix multiplied by the appropriate sign.
     * 
     * @param a The input 4x4 matrix
     * @return The cofactor matrix of the input
     */
    ABYTEK_FORCE_INLINE F_Matrix4x4_F32 CoFactor(PA_Matrix4x4_F32 a) noexcept {

        F_Matrix3x3_F32 aax = {
            a.B.StaticPermute<1, 2, 3>(),
            a.C.StaticPermute<1, 2, 3>(),
            a.D.StaticPermute<1, 2, 3>()
        };
        F_Matrix3x3_F32 abx = {
            a.A.StaticPermute<1, 2, 3>(),
            a.C.StaticPermute<1, 2, 3>(),
            a.D.StaticPermute<1, 2, 3>()
        };
        F_Matrix3x3_F32 acx = {
            a.A.StaticPermute<1, 2, 3>(),
            a.B.StaticPermute<1, 2, 3>(),
            a.D.StaticPermute<1, 2, 3>()
        };
        F_Matrix3x3_F32 adx = {
            a.A.StaticPermute<1, 2, 3>(),
            a.B.StaticPermute<1, 2, 3>(),
            a.C.StaticPermute<1, 2, 3>()
        };

        F_Matrix3x3_F32 aay = {
            a.B.StaticPermute<0, 2, 3>(),
            a.C.StaticPermute<0, 2, 3>(),
            a.D.StaticPermute<0, 2, 3>()
        };
        F_Matrix3x3_F32 aby = {
            a.A.StaticPermute<0, 2, 3>(),
            a.C.StaticPermute<0, 2, 3>(),
            a.D.StaticPermute<0, 2, 3>()
        };
        F_Matrix3x3_F32 acy = {
            a.A.StaticPermute<0, 2, 3>(),
            a.B.StaticPermute<0, 2, 3>(),
            a.D.StaticPermute<0, 2, 3>()
        };
        F_Matrix3x3_F32 ady = {
            a.A.StaticPermute<0, 2, 3>(),
            a.B.StaticPermute<0, 2, 3>(),
            a.C.StaticPermute<0, 2, 3>()
        };

        F_Matrix3x3_F32 aaz = {
            a.B.StaticPermute<0, 1, 3>(),
            a.C.StaticPermute<0, 1, 3>(),
            a.D.StaticPermute<0, 1, 3>()
        };
        F_Matrix3x3_F32 abz = {
            a.A.StaticPermute<0, 1, 3>(),
            a.C.StaticPermute<0, 1, 3>(),
            a.D.StaticPermute<0, 1, 3>()
        };
        F_Matrix3x3_F32 acz = {
            a.A.StaticPermute<0, 1, 3>(),
            a.B.StaticPermute<0, 1, 3>(),
            a.D.StaticPermute<0, 1, 3>()
        };
        F_Matrix3x3_F32 adz = {
            a.A.StaticPermute<0, 1, 3>(),
            a.B.StaticPermute<0, 1, 3>(),
            a.C.StaticPermute<0, 1, 3>()
        };

        F_Matrix3x3_F32 aaw = {
            a.B.StaticPermute<0, 1, 2>(),
            a.C.StaticPermute<0, 1, 2>(),
            a.D.StaticPermute<0, 1, 2>()
        };
        F_Matrix3x3_F32 abw = {
            a.A.StaticPermute<0, 1, 2>(),
            a.C.StaticPermute<0, 1, 2>(),
            a.D.StaticPermute<0, 1, 2>()
        };
        F_Matrix3x3_F32 acw = {
            a.A.StaticPermute<0, 1, 2>(),
            a.B.StaticPermute<0, 1, 2>(),
            a.D.StaticPermute<0, 1, 2>()
        };
        F_Matrix3x3_F32 adw = {
            a.A.StaticPermute<0, 1, 2>(),
            a.B.StaticPermute<0, 1, 2>(),
            a.C.StaticPermute<0, 1, 2>()
        };

        return {
            F_Vector4_F32(
                Determinant(aax),
                - Determinant(aay),
                Determinant(aaz),
                - Determinant(aaw)
            ),
            F_Vector4_F32(
                - Determinant(abx),
                + Determinant(aby),
                - Determinant(abz),
                + Determinant(abw)
            ),
            F_Vector4_F32(
                Determinant(acx),
                - Determinant(acy),
                Determinant(acz),
                - Determinant(acw)
            ),
            F_Vector4_F32(
                - Determinant(adx),
                + Determinant(ady),
                - Determinant(adz),
                + Determinant(adw)
            )
        };
    }
}