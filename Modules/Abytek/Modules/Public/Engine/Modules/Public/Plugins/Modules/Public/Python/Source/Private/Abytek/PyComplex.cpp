#include "Abytek/PyComplex.hpp"


namespace Abytek
{
    F_PyComplex::F_PyComplex(PA_Vector2 Value) :
        F_SharedPyObject(
            PyComplex_FromDoubles(Value.X, Value.Y)
        )
    {
    }

    F_PyComplex F_PyComplex::Clone() const
    {
                ABYTEK_ENGINE_PLUGINS_PYTHON_ASSERT(*this) << "invalid py object";
        return F_PyComplex(
            GetValue()
        );
    }

    F_Vector2 F_PyComplex::GetValue() const
    {
                ABYTEK_ENGINE_PLUGINS_PYTHON_ASSERT(*this) << "invalid py object";
        return {
            static_cast<F32>(PyComplex_RealAsDouble(_Handle)),
            static_cast<F32>(PyComplex_ImagAsDouble(_Handle))
        };
    }
}
