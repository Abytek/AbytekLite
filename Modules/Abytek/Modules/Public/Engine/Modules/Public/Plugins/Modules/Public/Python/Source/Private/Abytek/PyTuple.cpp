#include "Abytek/PyTuple.hpp"


namespace Abytek
{
    F_PyTuple::F_PyTuple(I32 Size) :
        F_SharedPyObject(
            PyTuple_New(Size)
        )
    {
    }
    F_PyTuple::F_PyTuple(const TF_Span<F_SharedPyObject>& Objects) :
        F_PyTuple(static_cast<I32>(Objects.size()))
    {
        I32 Size = static_cast<I32>(Objects.size());
        for (I32 Idx = 0; Idx < Size; ++Idx)
        {
            SetElement(Idx, Objects[Idx]);
        }
    }

    I32 F_PyTuple::GetSize() const
    {
        ABYTEK_ENGINE_PLUGINS_PYTHON_ASSERT(*this) << "invalid py object";

        return static_cast<I32>(PyTuple_Size(_Handle));
    }

    F_SharedPyObject F_PyTuple::GetElement(I32 Index) const
    {
        ABYTEK_ENGINE_PLUGINS_PYTHON_ASSERT(*this) << "invalid py object";

        return F_SharedPyObject(
            PyTuple_GetItem(_Handle, Index)
        );
    }
    void F_PyTuple::SetElement(I32 Index, const F_SharedPyObject& Element) const
    {
        ABYTEK_ENGINE_PLUGINS_PYTHON_ASSERT(*this) << "invalid py object";

        PyTuple_SetItem(_Handle, Index, Element.GetHandle());
    }
}
