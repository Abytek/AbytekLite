#include "Abytek/PyFloat.hpp"
#include "Abytek/PyInt.hpp"
#include "Abytek/PyBool.hpp"


namespace Abytek
{
    F_PyFloat::F_PyFloat(F64 Value) :
        F_SharedPyObject(
            PyFloat_FromDouble(Value)
        )
    {
    }
    F_PyFloat::F_PyFloat(const F_PyInt& Int) :
        F_PyFloat(static_cast<F64>(*Int))
    {
    }
    F_PyFloat::F_PyFloat(const F_PyBool& Bool) :
        F_PyFloat(static_cast<F64>(*Bool))
    {
    }

    F_PyFloat F_PyFloat::Clone() const
    {
        ABYTEK_ENGINE_PLUGINS_PYTHON_ASSERT(*this) << "invalid py object";
        return F_PyFloat(
            GetValue()
        );
    }

    F64 F_PyFloat::GetValue() const
    {
        ABYTEK_ENGINE_PLUGINS_PYTHON_ASSERT(*this) << "invalid py object";
        return PyFloat_AsDouble(_Handle);
    }
}
