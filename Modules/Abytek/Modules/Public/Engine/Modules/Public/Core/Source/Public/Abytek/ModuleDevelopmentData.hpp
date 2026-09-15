#pragma once

#include "Abytek/Engine.Core.prerequisites.hpp"
#include "Abytek/ModuleDevelopmentData.hpp"


#ifdef ABYTEK_ENABLE_DEVELOPMENT_DATA
namespace Abytek
{
    struct F_ModuleDevelopmentData
    {
        B8 EnableUnityBuild;
        F_Text MacroName;
        F_Text Type;
        F_Text Directory;
        F_Text BuildDirectory;
        F_Text ModulesDirectory;
        F_Text ModulesInterfaceDirectory;
        F_Text ModulesPrivateDirectory;
        F_Text ModulesPublicDirectory;
        F_Text SourceDirectory;
        F_Text SourceInterfaceDirectory;
        F_Text SourcePrivateDirectory;
        F_Text SourcePublicDirectory;
        F_Text AutoCopyDirectory;
        F_Text ThirdPartyDirectory;
        F_Text GeneratedSourceDirectory;
        F_Text GeneratedSourceTempDirectory;
        F_Text GeneratedSourceInterfaceDirectory;
        F_Text GeneratedSourcePrivateDirectory;
        F_Text GeneratedSourcePublicDirectory;
        F_Text CMakeDirectory;
        F_Text JSDirectory;
        F_Text GeneratedJSDirectory;
        F_Text JSModuleFile;
        F_Text JSModulesFile;
        F_Text JSPrivateModulesFile;
        F_Text JSPublicModulesFile;
        F_Text JSDependenciesFile;
        F_Text JSPrivateDependenciesFile;
        F_Text JSPublicDependenciesFile;
        TF_Set<F_Name> ModuleNames;
        TF_Set<F_Name> PrivateModuleNames;
        TF_Set<F_Name> PublicModuleNames;
        TF_Set<F_Name> DependencyNames;
        TF_Set<F_Name> PrivateDependencyNames;
        TF_Set<F_Name> PublicDependencyNames;
    };
}
#endif