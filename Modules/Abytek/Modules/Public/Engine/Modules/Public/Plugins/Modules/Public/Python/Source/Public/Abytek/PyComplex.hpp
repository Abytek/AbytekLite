#pragma once

#include "Python.prerequisites.pch.hpp"
#include "SharedPyObject.hpp"


namespace Abytek
{
    class ABYTEK_BASE_PLATFORM_API F_PyComplex : public F_SharedPyObject
    {
        ABYTEK_DERIVED_SHARED_PY_OBJECT_BODY(F_PyComplex);

    public:
        F_PyComplex(PA_Vector2 Value);

    public:
        F_PyComplex Clone() const;

        F_Vector2 GetValue() const;
        ABYTEK_FORCE_INLINE F_Vector2 operator * () const
        {
            return GetValue();
        }
    };
}