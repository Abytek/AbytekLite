#include "Abytek/PyInt.hpp"
#include "Abytek/PyFloat.hpp"
#include "Abytek/PyBool.hpp"


namespace Abytek
{
    F_PyInt::F_PyInt(I64 Value) :
        F_SharedPyObject(
            PyLong_FromLongLong(Value)
        )
    {
    }
    F_PyInt::F_PyInt(const F_PyFloat& Float) :
        F_PyInt(static_cast<I64>(*Float))
    {
    }
    F_PyInt::F_PyInt(const F_PyBool& Bool) :
        F_PyInt(static_cast<I64>(*Bool))
    {
    }

    F_PyInt F_PyInt::Clone() const
    {
        ABYTEK_ENGINE_PLUGINS_PYTHON_ASSERT(*this) << "invalid py object";
        return F_PyInt(
            GetValue()
        );
    }

    I64 F_PyInt::GetValue() const
    {
        ABYTEK_ENGINE_PLUGINS_PYTHON_ASSERT(*this) << "invalid py object";
        return PyLong_AsLong(_Handle);
    }
}
