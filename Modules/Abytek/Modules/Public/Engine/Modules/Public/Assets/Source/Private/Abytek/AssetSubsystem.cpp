#include "Abytek/AssetSubsystem.hpp"
#include "Abytek/Asset.hpp"
#include "Abytek/AssetFactory.hpp"
#include "Abytek/TextAssetFactory.hpp"
#include "Abytek/AssetRedirector.hpp"
#include "Abytek/TaskUtilities.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_AssetSubsystem)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_AssetSubsystem"));
    }

    ABYTEK_DEFINE_STATIC_APPLICATION_SUBSYSTEM(F_AssetSubsystem);

    F_AssetSubsystem::F_AssetSubsystem(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationSubsystem(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_SUBSYSTEM();
    }
    F_AssetSubsystem::~F_AssetSubsystem()
    {
    }

    void F_AssetSubsystem::OnStartup()
    {
        _TaskWorker = F_TaskWorker::Search(E_TaskWorkerFlag::LOW_FREQUENCY);

#ifdef ABYTEK_ENABLE_DEVELOPMENT_DATA
        LoadAssetDirectors();
#endif
        
        {
            RegisterFactory<F_TextAssetFactory>();
        }
    }
    void F_AssetSubsystem::OnShutdown()
    {
        WaitLiveCounter();
    }

#ifdef ABYTEK_ENABLE_DEVELOPMENT_DATA
    void F_AssetSubsystem::LoadAssetDirectors()
    {
        F_Text AssetDirectorsDirectoryPath = F_Text() + ABYTEK_TEXT(ABYTEK_DEVELOPMENT_DATA_DIR) + ABYTEK_TEXT("/AssetDirectors");
        ABYTEK_ENGINE_ASSETS_ASSERT(H_FSUtilities::Exists(AssetDirectorsDirectoryPath, E_FSEntryType::DIRECTORY)) << "Not found asset directors directory";
        
        TF_Vector<F_Text> AssetDirectorFileNames;
        ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
            H_FSUtilities::ListEntries<E_FSEntryType::FILE>(AssetDirectorsDirectoryPath, AssetDirectorFileNames)
        );
        TF_Vector<F_Text> AssetDirectorPaths;
        for (const auto& AssetDirectorFileName : AssetDirectorFileNames)
        {
            AssetDirectorPaths.push_back(AssetDirectorsDirectoryPath + ABYTEK_TEXT("/") + AssetDirectorFileName);
        }

        _AssetDirectors = {};
        for (U32 Idx = 0; Idx < AssetDirectorPaths.size(); ++Idx)
        {
            const auto& AssetDirectorFileName = AssetDirectorFileNames[Idx];
            const auto& AssetDirectorPath = AssetDirectorPaths[Idx];

            auto ModuleName = AssetDirectorFileName.substr(0, AssetDirectorFileName.size() - 5);
            
            F_Text FileContent;
            ABYTEK_FEEDBACK_STATUS_CHECK_HARD(H_FSUtilities::ReadFileText(AssetDirectorPath, FileContent));

            F_String AssetDirectorJSONStr = ToString(FileContent);
            auto JSONObject = boost::json::parse(AssetDirectorJSONStr.c_str());

            F_AssetDirector AssetDirector;
            AssetDirector.AssetsDirectoryPath = ToText(
                JSONObject.at("AssetsDirectory").as_string().c_str()
            );
            AssetDirector.CookedAssetsDirectoryPath = ToText(
                JSONObject.at("CookedAssetsDirectory").as_string().c_str()
            );
            AssetDirector.AssetCacheDirectoryPath = ToText(
                JSONObject.at("AssetCacheDirectory").as_string().c_str()
            );

            _AssetDirectors[F_Name(ModuleName)] = AssetDirector;
            ABYTEK_LOG_INFO()
                << "Loaded asset director for module " << ModuleName << ":" << F_NewLine{}
                << "    Assets directory: " << AssetDirector.AssetsDirectoryPath << F_NewLine{}
                << "    Cooked assets directory: " << AssetDirector.CookedAssetsDirectoryPath << F_NewLine{}
                << "    Asset cache directory: " << AssetDirector.AssetCacheDirectoryPath << F_NewLine{};
        }
    }
#endif

#ifdef ABYTEK_ENABLE_DEVELOPMENT_DATA
    B8 F_AssetSubsystem::CookAssetsForAllModules()
    {
        ABYTEK_LOG_INFO() << "Cooking assets for all modules";
        const auto& MainDevelopmentData = F_EngineRuntime::GetInstance()->GetMainDevelopmentData();
        for (const auto& ModuleName : MainDevelopmentData->GetModuleNames())
        {
            if (!CookAssets(ModuleName))
            {
                ABYTEK_LOG_ERROR() << "Cannot cook module " << ModuleName;
                return false;
            }
        }
        ABYTEK_LOG_INFO() << "Cooked assets for all modules";
        return true;
    }
    B8 F_AssetSubsystem::CookAssets(const F_Name& ModuleName)
    {
        ABYTEK_LOG_INFO() << "Cooking assets for module: " << ModuleName;
        const auto& AssetDirector = _AssetDirectors.find(ModuleName)->second;
        TF_Vector<F_Text> AssetRelativePaths;
        if (!H_FSUtilities::ListEntries<E_FSEntryType::FILE>(AssetDirector.AssetsDirectoryPath, AssetRelativePaths))
        {
            return false;
        }
        for (const auto& AssetRelativePath : AssetRelativePaths)
        {
            ABYTEK_LOG_INFO() << "Found asset: " << AssetRelativePath << " in module: " << ModuleName;
            CookAsset(AssetRelativePath, ModuleName);
        }
        ABYTEK_LOG_INFO() << "Cooked assets for module: " << ModuleName;
        return true;
    }
    B8 F_AssetSubsystem::CookAsset(const F_Text& Path, const F_Name& SpecifiedModuleName)
    {
        ABYTEK_LOG_INFO() << "Cooking asset: " << Path;
        
        TF_Vector<F_Text> ExternalBasePaths = GetDefaultExternalBasePaths();
        if (SpecifiedModuleName)
        {
            const auto& AssetDirector = _AssetDirectors.find(SpecifiedModuleName)->second;
            ExternalBasePaths.push_back(AssetDirector.AssetsDirectoryPath);
        }

        auto Asset = LoadAsset(Path, ExternalBasePaths);
        if (!Asset)
        {
            ABYTEK_LOG_ERROR() << "Cannot load asset for cooking: " << Path;
            return false;
        }

        auto AssetCookerTypeHandle = Asset->GetCookerTypeHandle();
        if (!AssetCookerTypeHandle)
        {
            ABYTEK_LOG_WARNING() << "Skip cooking asset: " << Asset->GetAbsolutePath() << " (asset cooker was not specified)";
            return true;
        }

        ABYTEK_ENGINE_ASSETS_ASSERT(AssetCookerTypeHandle->AdditionalInfo.MakeUniqueDefaultObject);
        auto AssetCooker = AssetCookerTypeHandle->AdditionalInfo.MakeUniqueDefaultObject();
        ABYTEK_ENGINE_ASSETS_ASSERT(AssetCooker);

        auto CastedAssetCooker = AssetCooker.FastCast<A_AssetCooker>();
        if (!CastedAssetCooker)
        {
            ABYTEK_LOG_ERROR() << "Invalid asset cooker of type: " << AssetCookerTypeHandle->Desc.FullName;
            return true;
        }

        if (!CastedAssetCooker->Cook())
        {
            ABYTEK_LOG_ERROR() << "Cooking asset failed: " << Asset->GetAbsolutePath();
            return true;
        }
        
        ABYTEK_LOG_INFO() << "Cooked asset: " << Asset->GetAbsolutePath();
        return true;
    }
#endif

    TW<A_AssetFactory> F_AssetSubsystem::FindAssetFactory(const F_Name& FileExtension)
    {
        auto It = _FileExtensionToFactoryMap.find(FileExtension);
        if (It != _FileExtensionToFactoryMap.end())
        {
            return It->second;
        }
        return {};
    }
    TF_Vector<TW<A_AssetFactory>> F_AssetSubsystem::FindAssetFactories(const F_Name& FileExtension)
    {
        TF_Vector<TW<A_AssetFactory>> Factories;
        auto Range = _FileExtensionToFactoryMap.equal_range(FileExtension);
        for (auto It = Range.first; It != Range.second; ++It)
        {
            Factories.push_back(It->second);
        }
        return ABYTEK_MOVE(Factories);
    }

    TW<A_AssetRedirector> F_AssetSubsystem::FindAssetRedirector(const F_Name& FileExtension)
    {
        auto It = _FileExtensionToRedirectorMap.find(FileExtension);
        if (It != _FileExtensionToRedirectorMap.end())
        {
            return It->second;
        }
        return {};
    }
    TF_Vector<TW<A_AssetRedirector>> F_AssetSubsystem::FindAssetRedirectors(const F_Name& FileExtension)
    {
        TF_Vector<TW<A_AssetRedirector>> Redirectors;
        auto Range = _FileExtensionToRedirectorMap.equal_range(FileExtension);
        for (auto It = Range.first; It != Range.second; ++It)
        {
            Redirectors.push_back(It->second);
        }
        return ABYTEK_MOVE(Redirectors);
    }

    TF_Vector<F_Text> F_AssetSubsystem::GetDefaultExternalBasePaths()
    {
        auto This = GetInstance();
        TF_Vector<F_Text> Result;
        Result.push_back(ABYTEK_TEXT("Assets"));
        Result.push_back(ABYTEK_TEXT("CookedAssets"));
#ifdef ABYTEK_ENABLE_DEVELOPMENT_DATA
        for (const auto& [ModuleName, AssetDirector] : This->_AssetDirectors)
        {
            Result.push_back(AssetDirector.AssetsDirectoryPath + ABYTEK_TEXT("/.."));
            Result.push_back(AssetDirector.CookedAssetsDirectoryPath + ABYTEK_TEXT("/.."));
        }
#endif
        return ABYTEK_MOVE(Result);
    }

    void F_AssetSubsystem::LockAssetLoading(const F_Text& AbsPath)
    {
        _AssetLoadingLock.Lock();
    }
    void F_AssetSubsystem::UnlockAssetLoading(const F_Text& AbsPath)
    {
        _AssetLoadingLock.Unlock();
    }

    TS<A_Asset> F_AssetSubsystem::LoadAsset(
        const F_Text& Path,
        const TF_Vector<F_Text>& ExternalBasePaths
    )
    {
        TS<A_Asset> Result;
        ABYTEK_AWAIT AsyncLoadAsset(
            [&Result](const TS<A_Asset>& Asset)
            {
                Result = Asset;
            },
            Path,
            ExternalBasePaths
        );
        return ABYTEK_MOVE(Result);
    }
    TS<A_Asset> F_AssetSubsystem::LoadAsset(
        const F_Text& Path,
        const F_Text& OverrideFileExtension,
        const TF_Vector<F_Text>& ExternalBasePaths,
        B8 EnableRedirector
    )
    {
        TS<A_Asset> Result;
        ABYTEK_AWAIT AsyncLoadAsset(
            [&Result](const TS<A_Asset>& Asset)
            {
                Result = Asset;
            },
            Path,
            OverrideFileExtension,
            ExternalBasePaths,
            EnableRedirector
        );
        return ABYTEK_MOVE(Result);
    }
    TS<A_Asset> F_AssetSubsystem::LoadAsset(
        const F_Text& Path, 
        const TW_Valid<A_AssetFactory>& Factory,
        const TF_Vector<F_Text>& ExternalBasePaths
    )
    {
        TS<A_Asset> Result;
        ABYTEK_AWAIT AsyncLoadAsset(
            [&Result](const TS<A_Asset>& Asset)
            {
                Result = Asset;
            },
            Path,
            Factory,
            ExternalBasePaths
        );
        return ABYTEK_MOVE(Result);
    }

    TS<A_Asset> F_AssetSubsystem::LoadAssetFromAbsolutePath(const F_Text& AbsolutePath)
    {
        TS<A_Asset> Result;
        ABYTEK_AWAIT AsyncLoadAssetFromAbsolutePath(
            [&Result](const TS<A_Asset>& Asset)
            {
                Result = Asset;
            },
            AbsolutePath
        );
        return ABYTEK_MOVE(Result);
    }
    TS<A_Asset> F_AssetSubsystem::LoadAssetFromAbsolutePath(
        const F_Text& AbsolutePath,
        const F_Name& OverrideFileExtension
    )
    {
        TS<A_Asset> Result;
        ABYTEK_AWAIT AsyncLoadAssetFromAbsolutePath(
            [&Result](const TS<A_Asset>& Asset)
            {
                Result = Asset;
            },
            AbsolutePath,
            OverrideFileExtension
        );
        return ABYTEK_MOVE(Result);
    }
    TS<A_Asset> F_AssetSubsystem::LoadAssetFromAbsolutePath(
        const F_Text& AbsolutePath,
        const TW_Valid<A_AssetFactory>& Factory
    )
    {
        TS<A_Asset> Result;
        ABYTEK_AWAIT AsyncLoadAssetFromAbsolutePath(
            [&Result](const TS<A_Asset>& Asset)
            {
                Result = Asset;
            },
            AbsolutePath,
            Factory
        );
        return ABYTEK_MOVE(Result);
    }

    TS_Unmanaged<F_TaskPromise> F_AssetSubsystem::AsyncLoadAsset(
        F_AssetAsyncLoadingCallback&& Callback, 
        const F_Text& Path,
        const TF_Vector<F_Text>& ExternalBasePaths
    )
    {
        auto FileExtension = H_Path::GetExtension(Path);
        return AsyncLoadAsset(
            ABYTEK_FORWARD(Callback),
            Path,
            FileExtension,
            ExternalBasePaths
        );
    }
    TS_Unmanaged<F_TaskPromise> F_AssetSubsystem::AsyncLoadAsset(
        F_AssetAsyncLoadingCallback&& Callback, 
        const F_Text& Path,
        const F_Text& OverrideFileExtension, 
        const TF_Vector<F_Text>& ExternalBasePaths, 
        B8 EnableRedirector
    )
    {
        F_Text RedirectedPath = Path;
        if (EnableRedirector)
        {
            TF_Vector<TW<A_AssetRedirector>> Redirectors = FindAssetRedirectors(OverrideFileExtension);
            for (const auto& Redirector : Redirectors)
            {
                if (Redirector->TryRedirect(RedirectedPath))
                {
                    ABYTEK_LOG_INFO() << "Redirected asset loading path from " << Path << " to " << RedirectedPath;
                    break;
                }
            }
        }
        
        auto AssetFactory = FindAssetFactory(OverrideFileExtension);
        if (!AssetFactory)
        {
            ABYTEK_LOG_ERROR() << "Asset factory not found for file extension: " << OverrideFileExtension;
            return {};
        }
        return AsyncLoadAsset(
            ABYTEK_FORWARD(Callback),
            RedirectedPath,
            AssetFactory,
            ExternalBasePaths
        );
    }
    TS_Unmanaged<F_TaskPromise> F_AssetSubsystem::AsyncLoadAsset(
        F_AssetAsyncLoadingCallback&& Callback, 
        const F_Text& Path,
        const TW_Valid<A_AssetFactory>& Factory, 
        const TF_Vector<F_Text>& ExternalBasePaths
    )
    {
        ABYTEK_LOG_INFO() << "Loading an asset at relative path: " << Path;
        if(TF_Optional<F_Text> AbsPathOpt = F_AssetPathInfo::FindAbsolutePath(Path, ExternalBasePaths))
        {
            const auto& AbsPath = *AbsPathOpt;
            return AsyncLoadAssetFromAbsolutePath(
                ABYTEK_FORWARD(Callback),
                AbsPath,
                Factory
            );
        }
        ABYTEK_LOG_ERROR() << "Asset not found at relative path: " << Path;
        return {};
    }

    TS_Unmanaged<F_TaskPromise> F_AssetSubsystem::AsyncLoadAssetFromAbsolutePath(
        F_AssetAsyncLoadingCallback&& Callback, 
        const F_Text& AbsolutePath
    )
    {
        auto FileExtension = H_Path::GetExtension(AbsolutePath);
        return AsyncLoadAssetFromAbsolutePath(
            ABYTEK_FORWARD(Callback),
            AbsolutePath,
            FileExtension
        );
    }
    TS_Unmanaged<F_TaskPromise> F_AssetSubsystem::AsyncLoadAssetFromAbsolutePath(
        F_AssetAsyncLoadingCallback&& Callback, 
        const F_Text& AbsolutePath,
        const F_Name& OverrideFileExtension
    )
    {
        auto AssetFactory = FindAssetFactory(OverrideFileExtension);
        if (!AssetFactory)
        {
            ABYTEK_LOG_ERROR() << "Asset factory not found for file extension: " << OverrideFileExtension;
            return {};
        }
        return AsyncLoadAssetFromAbsolutePath(
            ABYTEK_FORWARD(Callback),
            AbsolutePath,
            AssetFactory
        );
    }
    TS_Unmanaged<F_TaskPromise> F_AssetSubsystem::AsyncLoadAssetFromAbsolutePath(
        F_AssetAsyncLoadingCallback&& Callback, 
        const F_Text& AbsolutePath,
        const TW_Valid<A_AssetFactory>& Factory
    )
    {
        ABYTEK_LOG_INFO() << "Loading an asset at absolute path: " << AbsolutePath;
        IncreaseLiveCounter();
        return H_TaskUtilities::Schedule(
            _TaskWorker,
            [this, CachedCallback = ABYTEK_FORWARD(Callback), AbsolutePath, Factory]
            {
                ABYTEK_DEFER(this)
                {
                    DecreaseLiveCounter();
                };
                
                LockAssetLoading(AbsolutePath);
                if (auto It = _Assets.find(AbsolutePath); It != _Assets.end())
                {
                    CachedCallback(It->second);
                    return;
                }
                if(Factory->GetBuildMode() == E_AssetBuildMode::FROM_FILE)
                {
                    auto Asset = Factory->BuildFromFile(AbsolutePath);
                    if (Asset)
                    {
                        _Assets[AbsolutePath] = Asset;
                    }
                    else
                    {
                        ABYTEK_LOG_ERROR() << "Cannot build asset: " << AbsolutePath;
                    }
                    UnlockAssetLoading(AbsolutePath);
                    CachedCallback(Asset);
                    return;
                }
                else
                {
                    TF_Vector<U8> Binary;
                    if(H_FSUtilities::ReadFileBinary(AbsolutePath, Binary))
                    {
                        F_Archive Archive = F_Archive::From(Binary);
                        auto Asset = Factory->BuildFromBinary(
                            AbsolutePath, 
                            F_ArchiveReadOnlyView::From(Archive)
                        );
                        if (Asset)
                        {
                            _Assets[AbsolutePath] = Asset;
                        }
                        else
                        {
                            ABYTEK_LOG_ERROR() << "Cannot build asset: " << AbsolutePath;
                        }
                        UnlockAssetLoading(AbsolutePath);
                        CachedCallback(Asset);
                        return;
                    }
                    ABYTEK_LOG_ERROR() << "Cannot read asset binary: " << AbsolutePath;
                    UnlockAssetLoading(AbsolutePath);
                    CachedCallback({});
                    return;
                }
                ABYTEK_LOG_ERROR() << "Asset not found at absolute path: " << AbsolutePath;
                UnlockAssetLoading(AbsolutePath);
                CachedCallback({});
                return;
            }
        );
    }

    B8 F_AssetSubsystem::RelaxAsset(const TS_Valid<A_Asset>& Asset)
    {
        return RelaxAsset(Asset->GetAbsolutePath());
    }
    B8 F_AssetSubsystem::RelaxAsset(const F_Text& AbsPath)
    {
        ABYTEK_LOG_INFO() << "Relaxing asset: " << AbsPath;
        LockAssetLoading(AbsPath);
        B8 Result = false;
        auto It = _Assets.find(AbsPath);
        if (It != _Assets.end())
        {
            _Assets.erase(It);
            Result = true;
        }
        UnlockAssetLoading(AbsPath);
        if (Result)
        {
            ABYTEK_LOG_INFO() << "Relaxed asset: " << AbsPath;
        }
        else
        {
            ABYTEK_LOG_INFO() << "Cannot relax asset: " << AbsPath;
        }
        return Result;
    }
}
