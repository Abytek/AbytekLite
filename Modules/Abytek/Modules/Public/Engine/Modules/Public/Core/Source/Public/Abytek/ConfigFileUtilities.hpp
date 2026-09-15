#pragma once

#include "Abytek/Engine.Core.prerequisites.hpp"


namespace Abytek
{
    struct ABYTEK_ENGINE_CORE_API H_ConfigFileUtilities
    {
        static void Gather(TF_Vector<F_Text>& OutConfigFiles, const F_Name& FileName, const F_Name& ModuleName, B8 UseDevelopmentData = true);
    };
}