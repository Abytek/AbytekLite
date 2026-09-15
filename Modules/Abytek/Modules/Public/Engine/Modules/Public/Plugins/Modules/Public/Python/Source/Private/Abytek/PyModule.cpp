#include "Abytek/PyModule.hpp"
#include "Abytek/PythonBinder.hpp"


namespace Abytek
{
    F_PyModule F_PyModule::Import(const F_PyString& Path)
    {
        PyObject* Handle = PyImport_Import(Path.GetHandle());
        if (!Handle)
        {
            PyErr_Print();
            return {};
        }
        return F_PyModule { Handle };
    }
    F_PyModule F_PyModule::Search(const F_PyString& Path)
    {
        PyObject* Handle = PyImport_GetModule(Path.GetHandle());
        if (!Handle)
        {
            PyErr_Print();
            return {};
        }
        return F_PyModule { Handle };
    }
    void F_PyModule::Make(const F_PyModuleDesc& Desc)
    {
        F_PythonBinder& PythonBinder = F_PythonBinder::GetInstance();

        PythonBinder.EnqueueModuleDesc(Desc);
    }
    F_PyModule F_PyModule::GetMain()
    {
        return Search({ ABYTEK_TEXT("__main__") });
    }

    F_PyModule F_PyModule::Reload() const
    {
        ABYTEK_ENGINE_PLUGINS_PYTHON_ASSERT(*this) << "invalid py object";

        return F_PyModule(
            PyImport_ReloadModule(_Handle)
        );
    }
}
