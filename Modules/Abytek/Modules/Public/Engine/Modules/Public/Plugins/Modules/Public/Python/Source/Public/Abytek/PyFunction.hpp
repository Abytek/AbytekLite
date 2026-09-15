#pragma once

#include "Python.prerequisites.pch.hpp"
#include "SharedPyObject.hpp"
#include "Abytek/PyTuple.hpp"


namespace Abytek
{
    class ABYTEK_BASE_PLATFORM_API F_PyFunction : public F_SharedPyObject
    {
        ABYTEK_DERIVED_SHARED_PY_OBJECT_BODY(F_PyFunction);

    public:
        F_SharedPyObject Invoke(const F_PyTuple& Args) const;
        template<typename... __F_Args>
        F_SharedPyObject operator () (__F_Args&&... Args) const
        {
            TF_Array<F_SharedPyObject, sizeof...(__F_Args)> ArgArray = { ABYTEK_FORWARD(Args)... };
            return Invoke(
                F_PyTuple(
                    {
                        ArgArray.data(),
                        ArgArray.size()
                    }
                )
            );
        }
    };
}
