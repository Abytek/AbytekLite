#pragma once

#include "Abytek/Engine.Core.prerequisites.hpp"
#include "Abytek/ApplicationMountManager.hpp"


namespace Abytek
{
    struct ABYTEK_ENGINE_CORE_API H_ApplicationMount
    {
        static void Add(const F_Name& Name, const F_Name& ModuleName, const F_Text& RelativePath, E_ApplicationMountFlag Flags = E_ApplicationMountFlag::DEFAULT);
        static void Export(const TW_Valid<F_SerializableEnvironment>& SerializableEnvironment, const F_Text& BaseDirectoryPath);
    };
}
