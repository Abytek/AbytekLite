#pragma once

#include "Python.prerequisites.pch.hpp"
#include "SharedPyObject.hpp"
#include "PyString.hpp"


namespace Abytek
{
    enum class E_PyMethodFlag
    {
        None = 0x0,
        VarArgs = 0x1
    };
    struct F_PyMethodDesc
    {
        WithCrtAllocator::F_String Name;
        PyObject* (*Function)(PyObject* Self, PyObject* Args) = 0;
        E_PyMethodFlag Flags = E_PyMethodFlag::VarArgs;
        WithCrtAllocator::F_String Description;
    };
    struct F_PyModuleDesc
    {
        WithCrtAllocator::F_String Name;
        WithCrtAllocator::TF_Vector<F_PyMethodDesc> Methods;
        WithCrtAllocator::F_String Description;
    };

    class ABYTEK_BASE_PLATFORM_API F_PyModule : public F_SharedPyObject
    {
        ABYTEK_DERIVED_SHARED_PY_OBJECT_BODY(F_PyModule);

    public:
        static F_PyModule Import(const F_PyString& Path);
        static F_PyModule Search(const F_PyString& Path);
        static void Make(const F_PyModuleDesc& Desc);
        static F_PyModule GetMain();

    public:
        F_PyModule Reload() const;
    };
}


#define ABYTEK_BEGIN_PY_METHOD(...) \
            struct __VA_ARGS__ \
            { \
                static PyObject* Invoke(PyObject* Self, PyObject* Args) \
                { \
                    Abytek::F_SharedPyObject Result = HighLevelInvoke( \
                        Abytek::F_SharedPyObject(Self), \
                        Abytek::F_SharedPyObject(Args) \
                    ); \
                    PyObject* ResultHandle = Result.GetHandle(); \
                    Py_INCREF(ResultHandle); \
                    return ResultHandle; \
                } \
                static Abytek::F_SharedPyObject HighLevelInvoke(const Abytek::F_SharedPyObject& Self, const Abytek::F_SharedPyObject& Args) \
                {

#define ABYTEK_END_PY_METHOD() \
                } \
            };