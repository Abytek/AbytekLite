#pragma once

#include "Python.prerequisites.pch.hpp"
#include "Abytek/PyModule.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_PLUGINS_PYTHON_API F_PythonBinder
    {
    private:
        static F_PythonBinder* Instance;
        static WithRawAllocator::TF_Vector<F_PyModuleDesc> _ModuleDescs;

    public:
        static ABYTEK_FORCE_INLINE F_PythonBinder& GetInstance() noexcept { return *Instance; }
        static void EnqueueModuleDesc(const F_PyModuleDesc& ModuleDesc);

    private:
        F_PyModuleDesc _CurrentModuleDesc;
        WithCrtAllocator::TF_Vector<PyMethodDef*> _MethodDefPs;
        WithCrtAllocator::TF_Vector<PyModuleDef*> _ModuleDefPs;

    public:
        F_PythonBinder();
        ~F_PythonBinder();

    private:
        static PyObject* _Init();
        void _CreateModules();
        void _ReleaseInitTemp();
    };
}