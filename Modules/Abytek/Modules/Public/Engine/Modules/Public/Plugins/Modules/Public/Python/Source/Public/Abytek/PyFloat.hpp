#pragma once

#include "Python.prerequisites.pch.hpp"
#include "SharedPyObject.hpp"


namespace Abytek
{
    class F_PyInt;
    class F_PyBool;

    class ABYTEK_BASE_PLATFORM_API F_PyFloat : public F_SharedPyObject
    {
        ABYTEK_DERIVED_SHARED_PY_OBJECT_BODY(F_PyFloat);

    public:
        F_PyFloat(F64 Value);
        F_PyFloat(const F_PyInt& Int);
        F_PyFloat(const F_PyBool& Bool);

    public:
        F_PyFloat Clone() const;

        F64 GetValue() const;
        ABYTEK_FORCE_INLINE F64 operator * () const
        {
            return GetValue();
        }

        friend F_PyFloat operator + (const F_PyFloat& A, const F_PyFloat& B)
        {
            return A.GetValue() + B.GetValue();
        }
        F_PyFloat& operator += (const F_PyFloat& B)
        {
            *this = F_PyFloat { GetValue() + B.GetValue() };
            return *this;
        }
        friend F_PyFloat operator - (const F_PyFloat& A, const F_PyFloat& B)
        {
            return A.GetValue() + B.GetValue();
        }
        F_PyFloat& operator -= (const F_PyFloat& B)
        {
            *this = F_PyFloat { GetValue() - B.GetValue() };
            return *this;
        }
        F_PyFloat& operator + ()
        {
            return *this;
        }
        F_PyFloat& operator - ()
        {
            *this = F_PyFloat { -GetValue() };
            return *this;
        }

        friend F_PyFloat operator * (const F_PyFloat& A, const F_PyFloat& B)
        {
            return A.GetValue() * B.GetValue();
        }
        F_PyFloat& operator *= (const F_PyFloat& B)
        {
            *this = F_PyFloat { GetValue() * B.GetValue() };
            return *this;
        }
        friend F_PyFloat operator / (const F_PyFloat& A, const F_PyFloat& B)
        {
            return A.GetValue() / B.GetValue();
        }
        F_PyFloat& operator /= (const F_PyFloat& B)
        {
            *this = F_PyFloat { GetValue() / B.GetValue() };
            return *this;
        }
    };
}