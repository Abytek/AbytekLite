#pragma once

#include "Python.prerequisites.pch.hpp"
#include "SharedPyObject.hpp"


namespace Abytek
{
    class F_PyFloat;
    class F_PyBool;

    class ABYTEK_BASE_PLATFORM_API F_PyInt : public F_SharedPyObject
    {
        ABYTEK_DERIVED_SHARED_PY_OBJECT_BODY(F_PyInt);

    public:
        F_PyInt(I64 Value);
        F_PyInt(const F_PyFloat& Float);
        F_PyInt(const F_PyBool& Bool);

    public:
        F_PyInt Clone() const;

        I64 GetValue() const;
        ABYTEK_FORCE_INLINE I64 operator * () const
        {
            return GetValue();
        }

        friend F_PyInt operator + (const F_PyInt& A, const F_PyInt& B)
        {
            return A.GetValue() + B.GetValue();
        }
        F_PyInt& operator += (const F_PyInt& B)
        {
            *this = F_PyInt { GetValue() + B.GetValue() };
            return *this;
        }
        friend F_PyInt operator - (const F_PyInt& A, const F_PyInt& B)
        {
            return A.GetValue() + B.GetValue();
        }
        F_PyInt& operator -= (const F_PyInt& B)
        {
            *this = F_PyInt { GetValue() - B.GetValue() };
            return *this;
        }
        F_PyInt& operator + ()
        {
            return *this;
        }
        F_PyInt& operator - ()
        {
            *this = F_PyInt { -GetValue() };
            return *this;
        }

        friend F_PyInt operator * (const F_PyInt& A, const F_PyInt& B)
        {
            return A.GetValue() * B.GetValue();
        }
        F_PyInt& operator *= (const F_PyInt& B)
        {
            *this = F_PyInt { GetValue() * B.GetValue() };
            return *this;
        }
        friend F_PyInt operator / (const F_PyInt& A, const F_PyInt& B)
        {
            return A.GetValue() / B.GetValue();
        }
        F_PyInt& operator /= (const F_PyInt& B)
        {
            *this = F_PyInt { GetValue() / B.GetValue() };
            return *this;
        }

        friend F_PyInt operator | (const F_PyInt& A, const F_PyInt& B)
        {
            return A.GetValue() | B.GetValue();
        }
        F_PyInt& operator |= (const F_PyInt& B)
        {
            *this = F_PyInt { GetValue() | B.GetValue() };
            return *this;
        }
        friend F_PyInt operator & (const F_PyInt& A, const F_PyInt& B)
        {
            return A.GetValue() & B.GetValue();
        }
        F_PyInt& operator &= (const F_PyInt& B)
        {
            *this = F_PyInt { GetValue() & B.GetValue() };
            return *this;
        }

        friend F_PyInt operator || (const F_PyInt& A, const F_PyInt& B)
        {
            return A.GetValue() || B.GetValue();
        }
        friend F_PyInt operator && (const F_PyInt& A, const F_PyInt& B)
        {
            return A.GetValue() && B.GetValue();
        }

        friend F_PyInt operator >> (const F_PyInt& A, const F_PyInt& B)
        {
            return A.GetValue() >> B.GetValue();
        }
        F_PyInt& operator >>= (const F_PyInt& B)
        {
            *this = F_PyInt { GetValue() >> B.GetValue() };
            return *this;
        }
        friend F_PyInt operator << (const F_PyInt& A, const F_PyInt& B)
        {
            return A.GetValue() << B.GetValue();
        }
        F_PyInt& operator <<= (const F_PyInt& B)
        {
            *this = F_PyInt { GetValue() << B.GetValue() };
            return *this;
        }

        F_PyInt operator ~ ()
        {
            return F_PyInt { ~GetValue() };
        }

        friend F_PyInt operator % (const F_PyInt& A, const F_PyInt& B)
        {
            return A.GetValue() % B.GetValue();
        }
        F_PyInt& operator %= (const F_PyInt& B)
        {
            *this = F_PyInt { GetValue() % B.GetValue() };
            return *this;
        }
        friend F_PyInt operator ^ (const F_PyInt& A, const F_PyInt& B)
        {
            return A.GetValue() ^ B.GetValue();
        }
        F_PyInt& operator ^= (const F_PyInt& B)
        {
            *this = F_PyInt { GetValue() ^ B.GetValue() };
            return *this;
        }
    };
}