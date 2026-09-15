#include "Abytek/Asset.hpp"
#include "Abytek/EngineRuntime.hpp"
#include "Abytek/DevelopmentData.hpp"


namespace Abytek
{
    TF_Optional<F_Text> GetModuleContentDirectory(const F_Name& ModuleName)
    {
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        const auto& MainDevelopmentData = F_EngineRuntime::GetInstance()->GetMainDevelopmentData();
        if (!MainDevelopmentData->HasModule(ModuleName))
        {
            return {};
        }
        const auto& ModuleDevelopmentData = MainDevelopmentData->GetModuleDevelopmentData(ModuleName);
        return ModuleDevelopmentData.Directory + ABYTEK_TEXT("/Content");
#endif
        return {};    
    }
    
    F_AssetPathInfo F_AssetPathInfo::Parse(const F_Text& Raw)
    {
        F_AssetPathInfo Result{};
        Result.Type = E_AssetPathType::NORMAL;
        Result.Path = Raw;

        if (Raw.empty() || Raw[0] != ABYTEK_TEXT('@'))
        {
            return Result;
        }

        const auto SeparatorPos = Raw.find(ABYTEK_TEXT(":/"));
        if (SeparatorPos == F_Text::npos || SeparatorPos <= 1)
        {
            return Result;
        }

        Result.Type = E_AssetPathType::MODULED;
        Result.ModuleName = F_Name(Raw.substr(1, SeparatorPos - 1));
        Result.Path = Raw.substr(SeparatorPos + 2);

        return Result;
    }
    TF_Optional<F_Text> F_AssetPathInfo::FindAbsolutePath(const F_Text& Raw, const TF_Vector<F_Text>& ExternalBasePaths)
    {
        F_AssetPathInfo Info = Parse(Raw);
        switch (Info.Type)
        {
        case E_AssetPathType::NORMAL:
            return H_Path::FindAbsolutePath(Raw, ExternalBasePaths);
        case E_AssetPathType::MODULED:
            if (auto ModuleContentDirectory = GetModuleContentDirectory(Info.ModuleName))
            {
                return *ModuleContentDirectory + ABYTEK_TEXT("/") + Info.Path;
            }
            break;
        default:
            ABYTEK_LOG_INFO() << "Unknown asset path info type: " << static_cast<U32>(Info.Type);
            break;
        }
        return {};
    }

    ABYTEK_REFLECT(A_Asset)
    {
    }

    A_Asset::A_Asset(const F_Text& AbsolutePath) :
        _AbsolutePath(AbsolutePath)
    {
    }
    A_Asset::~A_Asset()
    {
    }

    TW<F_ReflectionType> A_Asset::GetCookerTypeHandle() const
    {
        return {};
    }
}
