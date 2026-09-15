#define _CRT_SECURE_NO_WARNINGS

#include "Abytek/PyRun.hpp"


namespace Abytek
{
    B8 H_PyRun::ExecuteFile(const F_PyString& Path)
    {
        
        FILE* CFile = fopen(Path.GetValue().c_str(), "r");
        if(CFile == NULL)
        {
            ABYTEK_LOG_WARNING() << "Cannot execute file " << Path.GetValue();
            return false;
        }

        if(PyRun_SimpleFile(CFile, Path.GetValue().c_str()) != 0)
        {
            PyErr_Print();
            return false;
        }
        return true;
    }
    B8 H_PyRun::ExecuteText(const F_PyString& Text)
    {
        if (PyRun_SimpleString(Text.GetValue().c_str()) != 0)
        {
            PyErr_Print();
            return false;
        }
        return true;
    }
}