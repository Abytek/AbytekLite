#include "Abytek/Development/CoreCookGraph/AssetAnalyzeCookRange.hpp"
#include "Abytek/Development/Cook/CookUtilities.hpp"
#include "Abytek/EngineRuntime.hpp"
#include "Abytek/ModuleList.hpp"
#include "Abytek/World/World.hpp"
#include "Abytek/World/WorldContext.hpp"
#include "Abytek/DevelopmentData.hpp"
#include "Abytek/Development/Cook/CookProfile.hpp"
#include "Abytek/Development/Cook/CookUtilities.hpp"
#include "Abytek/Development/CoreCookGraph/HighLevelCookRange.hpp"


#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
namespace Abytek
{
    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(F_AssetAnalyzeCookRange);
    
    F_Name F_AssetAnalyzeCookRange::GetBeginFunctionName()
    {
        return ABYTEK_NAME("Abytek::AssetAnalyzeCookRange::Begin");
    }
    F_Name F_AssetAnalyzeCookRange::GetEndFunctionName()
    {
        return ABYTEK_NAME("Abytek::AssetAnalyzeCookRange::End");
    }
    F_Name F_AssetAnalyzeCookRange::GetTaskTag()
    {
        return ABYTEK_NAME("Abytek::AssetAnalyzeCookRange");
    }

    F_AssetAnalyzeCookRange::F_AssetAnalyzeCookRange()
    {
        ABYTEK_BIND_OBJECT_SINGLETON_CRT();
        {
            auto CookFunction = H_CookUtilities::RegisterFunction(
                [this]
                {
                    Begin();
                    for (const auto& Command : _Queue.PopAll())
                    {
                        Command();
                    }
                },
                GetBeginFunctionName()
            );
        }
        {
            auto CookFunction = H_CookUtilities::RegisterFunction(
                [this]
                {
                    End();
                },
                GetEndFunctionName()
            );
            CookFunction->AddDependency(
                GetBeginFunctionName()
            );
            CookFunction->AddReverseDependency(
                F_HighLevelCookRange::GetBeginFunctionName()  
            );
        }
    }
    F_AssetAnalyzeCookRange::~F_AssetAnalyzeCookRange()
    {
        ABYTEK_ENGINE_CORE_ASSERT(_Queue.GetSize() == 0);
        H_CookUtilities::UnregisterFunction(GetEndFunctionName());
        H_CookUtilities::UnregisterFunction(GetBeginFunctionName());
    }

    void F_AssetAnalyzeCookRange::Begin()
    {
        H_TaskUtilities::AddTag(GetTaskTag());
        _SynchronizationSection.Begin();
        _Analyze();
    }
    void F_AssetAnalyzeCookRange::End()
    {
        _SynchronizationSection.End();
        H_TaskUtilities::RemoveTag(GetTaskTag());
    }

    void F_AssetAnalyzeCookRange::EnqueueCommand(TF_Function<void()>&& Command)
    {
        GetInstance()->_Queue.Push(ABYTEK_MOVE(Command));
    }

    void F_AssetAnalyzeCookRange::_Analyze()
    {
        auto CookProfile = F_CookProfile::GetMain();
        auto World = F_World::GetMain();
        auto MainDevelopmentData = F_EngineRuntime::GetInstance()->GetMainDevelopmentData();
        for (const auto& ModuleName : MainDevelopmentData->GetAccessibleModuleNames(CookProfile->GetModuleName()))
        {
            F_Text AssetsDirectory;
            ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                H_WorldContext::ResolveAbsolutePath(
                    World,
                    ABYTEK_TEXT("@") + *ModuleName + ABYTEK_TEXT("::Assets:/"),
                    AssetsDirectory
                )
            );
            
            F_Text IntermediateAssetsDirectory;
            ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                H_WorldContext::ResolveAbsolutePath(
                    World,
                    ABYTEK_TEXT("@") + *ModuleName + ABYTEK_TEXT("::Intermediate::Assets:/"),
                    IntermediateAssetsDirectory
                )
            );
            
            TF_Vector<F_Name> AssetPackageNames;   
            {
                TF_Vector<F_Text> FileAbsolutePaths;
                if (H_FSUtilities::Exists(AssetsDirectory, E_FSEntryType::DIRECTORY))
                {
                    TF_Vector<F_Text> FileRelativePaths;
                    ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                        H_FSUtilities::ListEntries<E_FSEntryType::FILE>(
                            AssetsDirectory,
                            FileRelativePaths,
                            true
                        )
                    );
                    for (const auto& FileRelativePath : FileRelativePaths)
                    {
                        auto FileAbsolutePath = AssetsDirectory + FileRelativePath;
                        if (H_Path::GetExtension(FileAbsolutePath) != *F_SerializablePackage::GetFileExtension())
                        {
                            continue;
                        }
                        AssetPackageNames.push_back(
                            ABYTEK_TEXT("@") + *ModuleName + ABYTEK_TEXT("::Assets:/") + H_Path::RemoveExtension(FileRelativePath)
                        );
                    }
                }
                if (H_FSUtilities::Exists(IntermediateAssetsDirectory, E_FSEntryType::DIRECTORY))
                {
                    TF_Vector<F_Text> FileRelativePaths;
                    ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                        H_FSUtilities::ListEntries<E_FSEntryType::FILE>(
                            IntermediateAssetsDirectory,
                            FileRelativePaths,
                            true
                        )
                    );
                    for (const auto& FileRelativePath : FileRelativePaths)
                    {
                        auto FileAbsolutePath = IntermediateAssetsDirectory + FileRelativePath;
                        if (H_Path::GetExtension(FileAbsolutePath) != *F_SerializablePackage::GetFileExtension())
                        {
                            continue;
                        }
                        AssetPackageNames.push_back(
                            ABYTEK_TEXT("@") + *ModuleName + ABYTEK_TEXT("::Intermediate::Assets:/") + H_Path::RemoveExtension(FileRelativePath)
                        );
                    }
                }
            }
            
            for (const auto& AssetPackageName : AssetPackageNames)
            {
                auto Package = H_WorldContext::EnsurePackage(World, AssetPackageName);
                
                F_AssetAnalyzedPackageInfo PackageInfo;
                PackageInfo.Name = AssetPackageName;
                for (const auto& [ ObjectName, Object ] : Package->GetObjects())
                {
                    auto& ObjectInfo = PackageInfo.Objects[ObjectName];   
                    ObjectInfo.Name = ObjectName;
                    ObjectInfo.Type = Object->GetType();
                }
                for (const auto& LastObjectHeader : Package->GetLastObjectHeaders())
                {
                    F_Name ObjectName;
                    F_Name PackageName;
                    ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                        F_SerializableEnvironment::ParseObjectPath(LastObjectHeader.Path, ObjectName, PackageName)  
                    );
                    
                    if (PackageInfo.Objects.find(ObjectName) != PackageInfo.Objects.end())
                    {
                        continue;
                    }
                    
                    auto& ObjectInfo = PackageInfo.Objects[ObjectName];   
                    ObjectInfo.Name = ObjectName;
                    ObjectInfo.Type = LastObjectHeader.Type;
                }
                
                _PackageInfos[AssetPackageName] = PackageInfo;
                ABYTEK_LOG_INFO() << "Analyzed for cooking, package: " << PackageInfo.Name;
            }
        }
    }
}
#endif