#pragma once

#include "Python.prerequisites.pch.hpp"
#include "SharedPyObject.hpp"
#include "Abytek/PyInt.hpp"


namespace Abytek
{
    class ABYTEK_BASE_PLATFORM_API F_PyList : public F_SharedPyObject
    {
        ABYTEK_DERIVED_SHARED_PY_OBJECT_BODY(F_PyList);

    public:
        F_PyList(const F_PyInt& Size);

        F_PyInt GetSize() const;

        F_SharedPyObject GetElement(const F_PyInt& Index) const;
        void SetElement(const F_PyInt& Index, const F_SharedPyObject& Element) const;

        ABYTEK_FORCE_INLINE F_SharedPyObject operator [] (const F_PyInt& Index) const
        {
            return GetElement(Index);
        }
    };
}
