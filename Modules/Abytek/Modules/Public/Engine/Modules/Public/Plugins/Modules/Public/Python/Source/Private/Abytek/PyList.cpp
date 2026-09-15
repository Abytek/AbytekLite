#include "Abytek/PyList.hpp"


namespace Abytek
{
    F_PyList::F_PyList(const F_PyInt& Size) :
        F_SharedPyObject(
            PyList_New(Size)
        )
    {
    }

    F_PyInt F_PyList::GetSize() const
    {
        ABYTEK_ENGINE_PLUGINS_PYTHON_ASSERT(*this) << "invalid py object";

        return PyList_Size(_Handle);
    }

    F_SharedPyObject F_PyList::GetElement(const F_PyInt& Index) const
    {
        ABYTEK_ENGINE_PLUGINS_PYTHON_ASSERT(*this) << "invalid py object";
        ABYTEK_ENGINE_PLUGINS_PYTHON_ASSERT(Index) << "invalid py object";

        return F_SharedPyObject(
            PyList_GetItem(_Handle, Index)
        );
    }
    void F_PyList::SetElement(const F_PyInt& Index, const F_SharedPyObject &Element) const
    {
        ABYTEK_ENGINE_PLUGINS_PYTHON_ASSERT(*this) << "invalid py object";
        ABYTEK_ENGINE_PLUGINS_PYTHON_ASSERT(Index) << "invalid py object";

        PyList_SetItem(_Handle, Index, Element.GetHandle());
    }
}
