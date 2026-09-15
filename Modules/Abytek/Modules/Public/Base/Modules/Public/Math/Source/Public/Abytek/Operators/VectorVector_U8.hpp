#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Functions/Data.hpp"
#include "Abytek/Types/Vecma.hpp"


namespace Abytek::Internal::MathOperators
{
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_U8 operator - (Abytek::PA_Vector2_U8 A) noexcept
    {
        return Abytek::H_Data2::Minus<Abytek::U8, Abytek::F_VecmaConfig>(A);
    }

    ABYTEK_FORCE_INLINE Abytek::F_Vector2_U8 operator + (Abytek::PA_Vector2_U8 A, Abytek::PA_Vector2_U8 B) noexcept
    {
        return Abytek::H_Data2::Add<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_U8& operator += (Abytek::F_Vector2_U8& A, Abytek::PA_Vector2_U8 B) noexcept
    {
        A = Abytek::H_Data2::Add<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_U8 operator - (Abytek::PA_Vector2_U8 A, Abytek::PA_Vector2_U8 B) noexcept
    {
        return Abytek::H_Data2::Subtract<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_U8& operator -= (Abytek::F_Vector2_U8& A, Abytek::PA_Vector2_U8 B) noexcept
    {
        A = Abytek::H_Data2::Subtract<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_U8 operator * (Abytek::PA_Vector2_U8 A, Abytek::PA_Vector2_U8 B) noexcept
    {
        return Abytek::H_Data2::Multiply<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_U8& operator *= (Abytek::F_Vector2_U8& A, Abytek::PA_Vector2_U8 B) noexcept
    {
        A = Abytek::H_Data2::Multiply<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_U8 operator / (Abytek::PA_Vector2_U8 A, Abytek::PA_Vector2_U8 B) noexcept
    {
        return Abytek::H_Data2::Divide<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_U8& operator /= (Abytek::F_Vector2_U8& A, Abytek::PA_Vector2_U8 B) noexcept
    {
        A = Abytek::H_Data2::Divide<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }

    ABYTEK_FORCE_INLINE Abytek::F_Vector2_U8 operator + (Abytek::PA_Vector2_U8 A, Abytek::U8 B) noexcept
    {
        return Abytek::H_Data2::Add<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_U8& operator += (Abytek::F_Vector2_U8& A, Abytek::U8 B) noexcept
    {
        A = Abytek::H_Data2::Add<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_U8 operator - (Abytek::PA_Vector2_U8 A, Abytek::U8 B) noexcept
    {
        return Abytek::H_Data2::Subtract<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_U8& operator -= (Abytek::F_Vector2_U8& A, Abytek::U8 B) noexcept
    {
        A = Abytek::H_Data2::Subtract<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_U8 operator * (Abytek::PA_Vector2_U8 A, Abytek::U8 B) noexcept
    {
        return Abytek::H_Data2::Multiply<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_U8& operator *= (Abytek::F_Vector2_U8& A, Abytek::U8 B) noexcept
    {
        A = Abytek::H_Data2::Multiply<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_U8 operator / (Abytek::PA_Vector2_U8 A, Abytek::U8 B) noexcept
    {
        return Abytek::H_Data2::Divide<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector2_U8& operator /= (Abytek::F_Vector2_U8& A, Abytek::U8 B) noexcept
    {
        A = Abytek::H_Data2::Divide<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }

    ABYTEK_FORCE_INLINE Abytek::F_Vector3_U8 operator - (Abytek::PA_Vector3_U8 A) noexcept
    {
        return Abytek::H_Data3::Minus<Abytek::U8, Abytek::F_VecmaConfig>(A);
    }

    ABYTEK_FORCE_INLINE Abytek::F_Vector3_U8 operator + (Abytek::PA_Vector3_U8 A, Abytek::PA_Vector3_U8 B) noexcept
    {
        return Abytek::H_Data3::Add<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_U8& operator += (Abytek::F_Vector3_U8& A, Abytek::PA_Vector3_U8 B) noexcept
    {
        A = Abytek::H_Data3::Add<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_U8 operator - (Abytek::PA_Vector3_U8 A, Abytek::PA_Vector3_U8 B) noexcept
    {
        return Abytek::H_Data3::Subtract<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_U8& operator -= (Abytek::F_Vector3_U8& A, Abytek::PA_Vector3_U8 B) noexcept
    {
        A = Abytek::H_Data3::Subtract<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_U8 operator * (Abytek::PA_Vector3_U8 A, Abytek::PA_Vector3_U8 B) noexcept
    {
        return Abytek::H_Data3::Multiply<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_U8& operator *= (Abytek::F_Vector3_U8& A, Abytek::PA_Vector3_U8 B) noexcept
    {
        A = Abytek::H_Data3::Multiply<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_U8 operator / (Abytek::PA_Vector3_U8 A, Abytek::PA_Vector3_U8 B) noexcept
    {
        return Abytek::H_Data3::Divide<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_U8& operator /= (Abytek::F_Vector3_U8& A, Abytek::PA_Vector3_U8 B) noexcept
    {
        A = Abytek::H_Data3::Divide<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }

    ABYTEK_FORCE_INLINE Abytek::F_Vector3_U8 operator + (Abytek::PA_Vector3_U8 A, Abytek::U8 B) noexcept
    {
        return Abytek::H_Data3::Add<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_U8& operator += (Abytek::F_Vector3_U8& A, Abytek::U8 B) noexcept
    {
        A = Abytek::H_Data3::Add<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_U8 operator - (Abytek::PA_Vector3_U8 A, Abytek::U8 B) noexcept
    {
        return Abytek::H_Data3::Subtract<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_U8& operator -= (Abytek::F_Vector3_U8& A, Abytek::U8 B) noexcept
    {
        A = Abytek::H_Data3::Subtract<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_U8 operator * (Abytek::PA_Vector3_U8 A, Abytek::U8 B) noexcept
    {
        return Abytek::H_Data3::Multiply<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_U8& operator *= (Abytek::F_Vector3_U8& A, Abytek::U8 B) noexcept
    {
        A = Abytek::H_Data3::Multiply<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_U8 operator / (Abytek::PA_Vector3_U8 A, Abytek::U8 B) noexcept
    {
        return Abytek::H_Data3::Divide<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector3_U8& operator /= (Abytek::F_Vector3_U8& A, Abytek::U8 B) noexcept
    {
        A = Abytek::H_Data3::Divide<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }

    ABYTEK_FORCE_INLINE Abytek::F_Vector4_U8 operator - (Abytek::PA_Vector4_U8 A) noexcept
    {
        return Abytek::H_Data4::Minus<Abytek::U8, Abytek::F_VecmaConfig>(A);
    }

    ABYTEK_FORCE_INLINE Abytek::F_Vector4_U8 operator + (Abytek::PA_Vector4_U8 A, Abytek::PA_Vector4_U8 B) noexcept
    {
        return Abytek::H_Data4::Add<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_U8& operator += (Abytek::F_Vector4_U8& A, Abytek::PA_Vector4_U8 B) noexcept
    {
        A = Abytek::H_Data4::Add<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_U8 operator - (Abytek::PA_Vector4_U8 A, Abytek::PA_Vector4_U8 B) noexcept
    {
        return Abytek::H_Data4::Subtract<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_U8& operator -= (Abytek::F_Vector4_U8& A, Abytek::PA_Vector4_U8 B) noexcept
    {
        A = Abytek::H_Data4::Subtract<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_U8 operator * (Abytek::PA_Vector4_U8 A, Abytek::PA_Vector4_U8 B) noexcept
    {
        return Abytek::H_Data4::Multiply<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_U8& operator *= (Abytek::F_Vector4_U8& A, Abytek::PA_Vector4_U8 B) noexcept
    {
        A = Abytek::H_Data4::Multiply<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_U8 operator / (Abytek::PA_Vector4_U8 A, Abytek::PA_Vector4_U8 B) noexcept
    {
        return Abytek::H_Data4::Divide<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_U8& operator /= (Abytek::F_Vector4_U8& A, Abytek::PA_Vector4_U8 B) noexcept
    {
        A = Abytek::H_Data4::Divide<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }

    ABYTEK_FORCE_INLINE Abytek::F_Vector4_U8 operator + (Abytek::PA_Vector4_U8 A, Abytek::U8 B) noexcept
    {
        return Abytek::H_Data4::Add<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_U8& operator += (Abytek::F_Vector4_U8& A, Abytek::U8 B) noexcept
    {
        A = Abytek::H_Data4::Add<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_U8 operator - (Abytek::PA_Vector4_U8 A, Abytek::U8 B) noexcept
    {
        return Abytek::H_Data4::Subtract<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_U8& operator -= (Abytek::F_Vector4_U8& A, Abytek::U8 B) noexcept
    {
        A = Abytek::H_Data4::Subtract<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_U8 operator * (Abytek::PA_Vector4_U8 A, Abytek::U8 B) noexcept
    {
        return Abytek::H_Data4::Multiply<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_U8& operator *= (Abytek::F_Vector4_U8& A, Abytek::U8 B) noexcept
    {
        A = Abytek::H_Data4::Multiply<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_U8 operator / (Abytek::PA_Vector4_U8 A, Abytek::U8 B) noexcept
    {
        return Abytek::H_Data4::Divide<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
    }
    ABYTEK_FORCE_INLINE Abytek::F_Vector4_U8& operator /= (Abytek::F_Vector4_U8& A, Abytek::U8 B) noexcept
    {
        A = Abytek::H_Data4::Divide<Abytek::U8, Abytek::F_VecmaConfig>(A, B);
        return A;
    }
}