#include "Abytek/PyFunction.hpp"


namespace Abytek
{
    F_SharedPyObject F_PyFunction::Invoke(const F_PyTuple& Args) const
    {
        return F_SharedPyObject(
            PyObject_CallObject(
                _Handle,
                Args.GetHandle()
            )
        );
    }
}
