#include "Abytek/Development/Cook/CookProfile.hpp"
#include "Abytek/ApplicationModuleContainer.hpp"
#include "Abytek/ApplicationMountHelper.hpp"
#include "Abytek/ConfigFileUtilities.hpp"
#include "Abytek/Module.hpp"
#include "Abytek/Development/Cook/CookSetting.hpp"
#include "Abytek/Development/Cook/CookSettingContainer.hpp"
#include "Abytek/World/World.hpp"
#include "Abytek/World/WorldContextHelper.hpp"


#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
namespace Abytek
{
    ABYTEK_REFLECT(F_CookProfile)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_CookProfile"));
    }
    
    TW<F_CookProfile> F_CookProfile::_Main;

    F_CookProfile::F_CookProfile(const F_Name& Name, const F_Name& ModuleName) :
        _Name(Name),
        _ModuleName(ModuleName)
    {
        auto World = F_World::GetMain();
        
        ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
            H_WorldContext::ResolveAbsolutePath(
                World,
                ABYTEK_TEXT("@") + *ModuleName + ABYTEK_TEXT("::Intermediate::CookData:/") + *Name,
                _IntermediateDataDirectoryPath
            )
        );
        if (_IntermediateDataDirectoryPath.back() == ABYTEK_TEXT('/'))
        {
            _IntermediateDataDirectoryPath.resize(_IntermediateDataDirectoryPath.size() - 1);
        }
        ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
            H_FSUtilities::EnsureDirectory(_IntermediateDataDirectoryPath)
        );
        {
            _IntermediateConfigsDirectoryPath = H_Path::Normalize(_IntermediateDataDirectoryPath + ABYTEK_TEXT("/Configs"));
            if (H_FSUtilities::Exists(_IntermediateConfigsDirectoryPath, E_FSEntryType::DIRECTORY))
            {
                ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                    H_FSUtilities::DeleteDirectory_(_IntermediateConfigsDirectoryPath)
                );
            }
            ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                H_FSUtilities::EnsureDirectory(_IntermediateConfigsDirectoryPath)
            );
        }
        {
            _IntermediateModulesDirectoryPath = H_Path::Normalize(_IntermediateDataDirectoryPath + ABYTEK_TEXT("/Modules"));
            /*if (H_FSUtilities::Exists(_IntermediateModulesDirectoryPath, E_FSEntryType::DIRECTORY))
            {
                ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                    H_FSUtilities::DeleteDirectory_(_IntermediateModulesDirectoryPath)
                );
            }*/
            ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                H_FSUtilities::EnsureDirectory(_IntermediateModulesDirectoryPath)
            );
        }
        
        F_SerializableEnvironmentBuildParams SerializableEnvironmentBuildParams;
        SerializableEnvironmentBuildParams.HasDevelopmentBuild = false;
        _SerializableEnvironment = TS<F_SerializableEnvironment>()(SerializableEnvironmentBuildParams);
        H_ApplicationMount::Export(_SerializableEnvironment.Weak(), _IntermediateModulesDirectoryPath);
        
        _Console = TU<F_Console>()();
        _SetupConsole(_Name);
        
        F_ProgramUnitContainerBuildParams ComponentContainerBuildParams;
        ComponentContainerBuildParams.Console = _Console.Weak();
        _SettingContainer = TU<F_CookSettingContainer>()(ComponentContainerBuildParams);
        _SettingContainer->_Profile = ABYTEK_WTHIS();
        {
            auto BaseSubsystemType = TF_ReflectionTypeHandle<A_CookSetting>(F_ReflectionContext::GetGlobal());
            ABYTEK_ENGINE_NFC_ASSERT(BaseSubsystemType);
        
            F_ApplicationModuleContainer::GetInstance()->ForEachUnit(
                [&BaseSubsystemType, this](const TW_Valid<F_ProgramUnit>& Unit)
                {
                    auto Module = Unit.FastCast<F_Module>();
                    auto ReflectionSession = Module->GetReflectionSession();
                    ReflectionSession->ForEachTypeDerivedFrom(
                        BaseSubsystemType,
                        [this](const TW_Valid<F_ReflectionType>& Type)
                        {
                            _CookSettingTypes.push_back(Type);
                            return true;
                        }
                    );
                    return true;
                }
            );
        }
        for (const auto& CookSettingType : _CookSettingTypes)
        {
            _SettingContainer->EnsureUnit(CookSettingType);
        }
        _SettingContainer->Update();
    }
    F_CookProfile::~F_CookProfile()
    {
        _SettingContainer->EnqueueRemoveAllUnits();
        _SettingContainer->Update();
        _SettingContainer = {};
        _Console = {};
        _SerializableEnvironment = {};
    }

    void F_CookProfile::_SetupConsole(const F_Name& Name)
    {
        if (_ParsedNames.find(Name) != _ParsedNames.end())
        {
            return;
        }
        _ParsedNames.insert(Name);
        
        TF_Vector<F_Text> ConfigFiles;
        H_ConfigFileUtilities::Gather(
            ConfigFiles,
            ABYTEK_TEXT("CookProfiles/") + *Name + ABYTEK_TEXT(".toml"),
            _ModuleName
        );
        for (const auto& ConfigFile : ConfigFiles)
        {
            if (!_Console->HasConfigFile(ConfigFile))
            {
                _Console->AddConfigFile(ConfigFile);
            }
        }
        _Console->Update();
        
        // Parse extends
        TF_Vector<F_Name> Extends;
        TF_Vector<F_Text> Elements;
        boost::split(Elements, ABYTEK_TEXT("Abytek.Extends"), boost::is_any_of(ABYTEK_TEXT(".")));
        for (const auto& RootConfigParsedResult : _Console->GetData().Config.ParsedResults)
        {
            const toml::node* Node = &RootConfigParsedResult;
            for (U32 Idx = 0; Idx < Elements.size(); ++Idx)
            {
                if (!(Node->is_table()))
                {
                    Node = nullptr;
                    break;
                }
                
                auto Table = Node->as_table();
                auto It = Table->find(ToString(Elements[Idx]));
                if (It == Table->end())
                {
                    Node = nullptr;
                    break;
                }
                
                Node = &It->second;
            }
            if (!Node)
            {
                continue;
            }
            if (!Node->is_array())
            {
                continue;
            }
            for (const auto& ItemNode : *Node->as_array())
            {
                if  (!ItemNode.is<std::string>())
                {
                    ABYTEK_LOG_FATAL() << "Cook profile extends must be string";
                }
                F_Name Extend = ToText(ItemNode.value<std::string>()->data());
                ABYTEK_ENGINE_CORE_ASSERT(Extend) << "Invalid cook profile extend";
                Extends.push_back(Extend);
            }
        }
        for (const auto& Extend : Extends)
        {
            _SetupConsole(Extend);
        }
    }

    void F_CookProfile::AddPackageToExport(const F_Name& PackageName, E_CookPackageExportFlag Flags)
    {
        ABYTEK_ENGINE_CORE_ASSERT(_PackagesToExport.find(PackageName) == _PackagesToExport.end());
        F_CookPackageExport PackageExport;
        PackageExport.Name = PackageName;
        PackageExport.Flags = Flags;
        _PackagesToExport.insert({ PackageName, PackageExport });
    }
    void F_CookProfile::ExportPackages(const F_Text& DstDirectoryPath, E_CookPackageExportFlag Flags)
    {
        H_FSUtilities::EnsureDirectory(DstDirectoryPath);
        
        for (const auto& [PackageName, PackageExport] : _PackagesToExport)
        {
            if (!FlagHas(PackageExport.Flags, Flags))
            {
                continue;
            }
            
            F_Text PackagePath;
            ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                F_SerializablePackage::ResolveAbsolutePath(
                    _SerializableEnvironment.Weak(),
                    PackageName,
                    PackagePath
                )   
            );
            
            F_Text RelativePath;
            ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                H_Path::ParseRelativePath(_IntermediateModulesDirectoryPath, PackagePath, RelativePath)    
            );
            
            F_Text DstPackagePath = DstDirectoryPath + ABYTEK_TEXT("/") + RelativePath;
            ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                H_FSUtilities::EnsureDirectory(
                    H_Path::GetBaseName(DstPackagePath)
                )
            );
            
            ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                H_FSUtilities::CopyFile_(
                    DstPackagePath,
                    PackagePath
                )
            );
            
            ABYTEK_LOG_INFO() << "Exported package \"" << PackageExport.Name << "\" from \"" << PackagePath << "\" to \"" << DstPackagePath << "\"";
        }
    }

    void F_CookProfile::ExportConfigs(const F_Text& DstDirectoryPath)
    {
        H_FSUtilities::EnsureDirectory(DstDirectoryPath);
        
        ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
            H_FSUtilities::CopyDirectory_(
                DstDirectoryPath,
                _IntermediateConfigsDirectoryPath
            )  
        );
            
        ABYTEK_LOG_INFO() << "Exported configs from \"" << _IntermediateConfigsDirectoryPath << "\" to \"" << DstDirectoryPath << "\"";
    }
}
#endif