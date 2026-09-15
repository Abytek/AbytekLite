#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Functions/Data.hpp"
#include "Abytek/Types/Vecma.hpp"


namespace Abytek::Internal::MathOperators
{
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_I8 operator - (Abytek::PA_Vector2_I8 A) noexcept
    {
        return Abytek::H_Data2::Minus<Abytek::I8, Abytek::F_VecmaConfig>(A);
    }

    ABYTEK_FORCE_INLINE Abytek::F_Vector2_I8 operator + (Abytek::PA_Vector2_I8 A, Abytek::PA_Vector2_I8 B) noexcept
    {
        return Abytek::H_Data2::Add<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_I8& operator += (Abytek::F_Vector2_I8& A, Abytek::PA_Vector2_I8 B) noexcept
    {
        A = Abytek::H_Data2::Add<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_I8 operator - (Abytek::PA_Vector2_I8 A, Abytek::PA_Vector2_I8 B) noexcept
    {
        return Abytek::H_Data2::Subtract<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_I8& operator -= (Abytek::F_Vector2_I8& A, Abytek::PA_Vector2_I8 B) noexcept
    {
        A = Abytek::H_Data2::Subtract<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_I8 operator * (Abytek::PA_Vector2_I8 A, Abytek::PA_Vector2_I8 B) noexcept
    {
        return Abytek::H_Data2::Multiply<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_I8& operator *= (Abytek::F_Vector2_I8& A, Abytek::PA_Vector2_I8 B) noexcept
    {
        A = Abytek::H_Data2::Multiply<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_I8 operator / (Abytek::PA_Vector2_I8 A, Abytek::PA_Vector2_I8 B) noexcept
    {
        return Abytek::H_Data2::Divide<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_I8& operator /= (Abytek::F_Vector2_I8& A, Abytek::PA_Vector2_I8 B) noexcept
    {
        A = Abytek::H_Data2::Divide<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }

    ABYTEK_FORCE_INLINE Abytek::F_Vector2_I8 operator + (Abytek::PA_Vector2_I8 A, Abytek::I8 B) noexcept
    {
        return Abytek::H_Data2::Add<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_I8& operator += (Abytek::F_Vector2_I8& A, Abytek::I8 B) noexcept
    {
        A = Abytek::H_Data2::Add<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_I8 operator - (Abytek::PA_Vector2_I8 A, Abytek::I8 B) noexcept
    {
        return Abytek::H_Data2::Subtract<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_I8& operator -= (Abytek::F_Vector2_I8& A, Abytek::I8 B) noexcept
    {
        A = Abytek::H_Data2::Subtract<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_I8 operator * (Abytek::PA_Vector2_I8 A, Abytek::I8 B) noexcept
    {
        return Abytek::H_Data2::Multiply<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_I8& operator *= (Abytek::F_Vector2_I8& A, Abytek::I8 B) noexcept
    {
        A = Abytek::H_Data2::Multiply<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_I8 operator / (Abytek::PA_Vector2_I8 A, Abytek::I8 B) noexcept
    {
        return Abytek::H_Data2::Divide<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_I8& operator /= (Abytek::F_Vector2_I8& A, Abytek::I8 B) noexcept
    {
        A = Abytek::H_Data2::Divide<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }

    ABYTEK_FORCE_INLINE Abytek::F_Vector3_I8 operator - (Abytek::PA_Vector3_I8 A) noexcept
    {
        return Abytek::H_Data3::Minus<Abytek::I8, Abytek::F_VecmaConfig>(A);
    }

    ABYTEK_FORCE_INLINE Abytek::F_Vector3_I8 operator + (Abytek::PA_Vector3_I8 A, Abytek::PA_Vector3_I8 B) noexcept
    {
        return Abytek::H_Data3::Add<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_I8& operator += (Abytek::F_Vector3_I8& A, Abytek::PA_Vector3_I8 B) noexcept
    {
        A = Abytek::H_Data3::Add<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_I8 operator - (Abytek::PA_Vector3_I8 A, Abytek::PA_Vector3_I8 B) noexcept
    {
        return Abytek::H_Data3::Subtract<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_I8& operator -= (Abytek::F_Vector3_I8& A, Abytek::PA_Vector3_I8 B) noexcept
    {
        A = Abytek::H_Data3::Subtract<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_I8 operator * (Abytek::PA_Vector3_I8 A, Abytek::PA_Vector3_I8 B) noexcept
    {
        return Abytek::H_Data3::Multiply<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_I8& operator *= (Abytek::F_Vector3_I8& A, Abytek::PA_Vector3_I8 B) noexcept
    {
        A = Abytek::H_Data3::Multiply<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_I8 operator / (Abytek::PA_Vector3_I8 A, Abytek::PA_Vector3_I8 B) noexcept
    {
        return Abytek::H_Data3::Divide<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_I8& operator /= (Abytek::F_Vector3_I8& A, Abytek::PA_Vector3_I8 B) noexcept
    {
        A = Abytek::H_Data3::Divide<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }

    ABYTEK_FORCE_INLINE Abytek::F_Vector3_I8 operator + (Abytek::PA_Vector3_I8 A, Abytek::I8 B) noexcept
    {
        return Abytek::H_Data3::Add<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_I8& operator += (Abytek::F_Vector3_I8& A, Abytek::I8 B) noexcept
    {
        A = Abytek::H_Data3::Add<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_I8 operator - (Abytek::PA_Vector3_I8 A, Abytek::I8 B) noexcept
    {
        return Abytek::H_Data3::Subtract<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_I8& operator -= (Abytek::F_Vector3_I8& A, Abytek::I8 B) noexcept
    {
        A = Abytek::H_Data3::Subtract<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_I8 operator * (Abytek::PA_Vector3_I8 A, Abytek::I8 B) noexcept
    {
        return Abytek::H_Data3::Multiply<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_I8& operator *= (Abytek::F_Vector3_I8& A, Abytek::I8 B) noexcept
    {
        A = Abytek::H_Data3::Multiply<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_I8 operator / (Abytek::PA_Vector3_I8 A, Abytek::I8 B) noexcept
    {
        return Abytek::H_Data3::Divide<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_I8& operator /= (Abytek::F_Vector3_I8& A, Abytek::I8 B) noexcept
    {
        A = Abytek::H_Data3::Divide<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }

    ABYTEK_FORCE_INLINE Abytek::F_Vector4_I8 operator - (Abytek::PA_Vector4_I8 A) noexcept
    {
        return Abytek::H_Data4::Minus<Abytek::I8, Abytek::F_VecmaConfig>(A);
    }

    ABYTEK_FORCE_INLINE Abytek::F_Vector4_I8 operator + (Abytek::PA_Vector4_I8 A, Abytek::PA_Vector4_I8 B) noexcept
    {
        return Abytek::H_Data4::Add<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_I8& operator += (Abytek::F_Vector4_I8& A, Abytek::PA_Vector4_I8 B) noexcept
    {
        A = Abytek::H_Data4::Add<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_I8 operator - (Abytek::PA_Vector4_I8 A, Abytek::PA_Vector4_I8 B) noexcept
    {
        return Abytek::H_Data4::Subtract<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_I8& operator -= (Abytek::F_Vector4_I8& A, Abytek::PA_Vector4_I8 B) noexcept
    {
        A = Abytek::H_Data4::Subtract<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_I8 operator * (Abytek::PA_Vector4_I8 A, Abytek::PA_Vector4_I8 B) noexcept
    {
        return Abytek::H_Data4::Multiply<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_I8& operator *= (Abytek::F_Vector4_I8& A, Abytek::PA_Vector4_I8 B) noexcept
    {
        A = Abytek::H_Data4::Multiply<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_I8 operator / (Abytek::PA_Vector4_I8 A, Abytek::PA_Vector4_I8 B) noexcept
    {
        return Abytek::H_Data4::Divide<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_I8& operator /= (Abytek::F_Vector4_I8& A, Abytek::PA_Vector4_I8 B) noexcept
    {
        A = Abytek::H_Data4::Divide<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }

    ABYTEK_FORCE_INLINE Abytek::F_Vector4_I8 operator + (Abytek::PA_Vector4_I8 A, Abytek::I8 B) noexcept
    {
        return Abytek::H_Data4::Add<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_I8& operator += (Abytek::F_Vector4_I8& A, Abytek::I8 B) noexcept
    {
        A = Abytek::H_Data4::Add<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_I8 operator - (Abytek::PA_Vector4_I8 A, Abytek::I8 B) noexcept
    {
        return Abytek::H_Data4::Subtract<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_I8& operator -= (Abytek::F_Vector4_I8& A, Abytek::I8 B) noexcept
    {
        A = Abytek::H_Data4::Subtract<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_I8 operator * (Abytek::PA_Vector4_I8 A, Abytek::I8 B) noexcept
    {
        return Abytek::H_Data4::Multiply<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_I8& operator *= (Abytek::F_Vector4_I8& A, Abytek::I8 B) noexcept
    {
        A = Abytek::H_Data4::Multiply<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_I8 operator / (Abytek::PA_Vector4_I8 A, Abytek::I8 B) noexcept
    {
        return Abytek::H_Data4::Divide<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_I8& operator /= (Abytek::F_Vector4_I8& A, Abytek::I8 B) noexcept
    {
        A = Abytek::H_Data4::Divide<Abytek::I8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
}