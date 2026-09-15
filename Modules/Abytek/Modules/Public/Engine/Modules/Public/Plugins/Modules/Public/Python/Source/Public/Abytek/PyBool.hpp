#pragma once

#include "Python.prerequisites.pch.hpp"
#include "SharedPyObject.hpp"


namespace Abytek
{
    class F_PyInt;

    class ABYTEK_BASE_PLATFORM_API F_PyBool : public F_SharedPyObject
    {
        ABYTEK_DERIVED_SHARED_PY_OBJECT_BODY(F_PyBool);

    public:
        F_PyBool(B8 Value);
        F_PyBool(const F_PyInt& Int);

    public:
        F_PyBool Clone() const;

        B8 GetValue() const;
        ABYTEK_FORCE_INLINE B8 operator * () const
        {
            return GetValue();
        }

        friend F_PyBool operator || (const F_PyBool& A, const F_PyBool& B)
        {
            return A.GetValue() || B.GetValue();
        }
        friend F_PyBool operator && (const F_PyBool& A, const F_PyBool& B)
        {
            return A.GetValue() && B.GetValue();
        }
    };
}