#include "Abytek/PyBool.hpp"
#include "Abytek/PyInt.hpp"


namespace Abytek
{
    F_PyBool::F_PyBool(B8 Value) :
        F_SharedPyObject(
            Value ? Py_True : Py_False
        )
    {
        Py_INCREF(_Handle);
    }
    F_PyBool::F_PyBool(const F_PyInt& Int) :
        F_PyBool(*Int)
    {
    }

    F_PyBool F_PyBool::Clone() const
    {
                ABYTEK_ENGINE_PLUGINS_PYTHON_ASSERT(*this) << "invalid py object";
        return F_PyBool(
            GetValue()
        );
    }

    B8 F_PyBool::GetValue() const
    {
                ABYTEK_ENGINE_PLUGINS_PYTHON_ASSERT(*this) << "invalid py object";
        return (_Handle == Py_True);
    }
}
