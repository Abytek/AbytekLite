#pragma once

#include "Python.prerequisites.pch.hpp"
#include "Abytek/PyString.hpp"


namespace Abytek
{
    struct ABYTEK_BASE_PLATFORM_API H_PyRun final
    {
        static B8 ExecuteFile(const F_PyString& Path);
        static B8 ExecuteText(const F_PyString& Text);
    };
}