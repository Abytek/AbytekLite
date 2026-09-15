#include "Abytek/PythonBinder.hpp"


namespace Abytek
{
    F_PythonBinder* F_PythonBinder::Instance = 0;
    WithRawAllocator::TF_Vector<F_PyModuleDesc> F_PythonBinder::_ModuleDescs;

    void F_PythonBinder::EnqueueModuleDesc(const F_PyModuleDesc &ModuleDesc)
    {
        _ModuleDescs.push_back(boost::move(ModuleDesc));
    }

    F_PythonBinder::F_PythonBinder()
    {
        Instance = this;

        _CreateModules();

        Py_Initialize();

        _ReleaseInitTemp();
    }
    F_PythonBinder::~F_PythonBinder()
    {
        Instance = 0;
    }

    PyObject* F_PythonBinder::_Init()
    {
        F_PythonBinder& PythonBinder = F_PythonBinder::GetInstance();
        F_PyModuleDesc& CurrentModuleDesc = PythonBinder._CurrentModuleDesc;

        WithCrtAllocator::TF_Vector<PyMethodDef> MethodDefs;
        for (auto& MethodDesc : CurrentModuleDesc.Methods)
        {
            ABYTEK_ENGINE_PLUGINS_PYTHON_ASSERT(MethodDesc.Function);
            ABYTEK_ENGINE_PLUGINS_PYTHON_ASSERT(MethodDesc.Flags == E_PyMethodFlag::VarArgs);
            PyMethodDef MethodDef = {
                MethodDesc.Name.c_str(),
                MethodDesc.Function,
                METH_VARARGS,
                MethodDesc.Description.c_str()
            };
            MethodDefs.push_back(MethodDef);
        }
        MethodDefs.push_back({ 0, 0, 0, 0 });

        PyMethodDef* MethodDefP = WithCrtAllocator::AllocateArray<PyMethodDef>(MethodDefs.size());
        memcpy(
            MethodDefP,
            MethodDefs.data(),
            MethodDefs.size() * sizeof(PyMethodDef)
        );

        PyModuleDef* ModuleDefP = WithCrtAllocator::New<PyModuleDef>(
            PyModuleDef {
                PyModuleDef_HEAD_INIT,
                CurrentModuleDesc.Name.c_str(),
                CurrentModuleDesc.Description.c_str(),
                -1,
                MethodDefP,
                0,
                0,
                0,
                0
            }
        );

        PythonBinder._MethodDefPs.push_back(MethodDefP);
        PythonBinder._ModuleDefPs.push_back(ModuleDefP);

        return PyModule_Create(ModuleDefP);
    }
    void F_PythonBinder::_CreateModules()
    {
        I32 ModuleCount = static_cast<I32>(_ModuleDescs.size());
        for (I32 Idx = 0; Idx < ModuleCount; ++Idx)
        {
            _CurrentModuleDesc = boost::move(_ModuleDescs[Idx]);
            if (PyImport_AppendInittab(_CurrentModuleDesc.Name.c_str(), _Init) != 0)
            {
                PyErr_Print();
            }
        }
        _ModuleDescs.clear();
    }
    void F_PythonBinder::_ReleaseInitTemp()
    {
        for (PyMethodDef* MethodDefP : _MethodDefPs)
        {
            WithCrtAllocator::Deallocate(MethodDefP);
        }
        for (PyModuleDef* ModuleDefP : _ModuleDefPs)
        {
            WithCrtAllocator::Delete(ModuleDefP);
        }
    }
}
