#pragma once

#include "Python.prerequisites.pch.hpp"
#include "SharedPyObject.hpp"


namespace Abytek
{
    class ABYTEK_BASE_PLATFORM_API F_PyTuple : public F_SharedPyObject
    {
        ABYTEK_DERIVED_SHARED_PY_OBJECT_BODY(F_PyTuple);

    public:
        F_PyTuple(I32 Size);
        F_PyTuple(const TF_Span<F_SharedPyObject>& Objects);

        I32 GetSize() const;

        F_SharedPyObject GetElement(I32 Index) const;
        void SetElement(I32 Index, const F_SharedPyObject& Element) const;

        ABYTEK_FORCE_INLINE F_SharedPyObject operator [] (I32 Index) const
        {
            return GetElement(Index);
        }
    };
}
