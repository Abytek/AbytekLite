#pragma once

#include "Abytek/Engine.Assets.prerequisites.hpp"


namespace Abytek
{
    class A_Asset;
    class A_AssetFactory;
    class A_AssetRedirector;
    
    using F_AssetAsyncLoadingCallback = TF_Function<void(const TS<A_Asset>& Asset)>;
    
#ifdef ABYTEK_ENABLE_DEVELOPMENT_DATA
    struct F_AssetDirector
    {
        F_Text AssetsDirectoryPath;
        F_Text CookedAssetsDirectoryPath;
        F_Text AssetCacheDirectoryPath;
    };
#endif
    
    class ABYTEK_ENGINE_ASSETS_API F_AssetSubsystem final : public A_ApplicationSubsystem
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationSubsystem)
        ABYTEK_END_REFLECTOR(F_AssetSubsystem);

    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_SUBSYSTEM(F_AssetSubsystem);

    private:
#ifdef ABYTEK_ENABLE_DEVELOPMENT_DATA
        TF_Map<F_Name, F_AssetDirector> _AssetDirectors;
#endif
        
        TF_Vector<TU<A_AssetFactory>> _Factories;
        TF_MultiMap<F_Name, TW<A_AssetFactory>> _FileExtensionToFactoryMap;
        
        TF_Vector<TU<A_AssetRedirector>> _Redirectors;
        TF_MultiMap<F_Name, TW<A_AssetRedirector>> _FileExtensionToRedirectorMap;

        F_YieldSpinLock _AssetLoadingLock;
        TF_Map<F_Text, TS<A_Asset>> _Assets;

        TW<F_TaskWorker> _TaskWorker;
        
        AU32 _LiveCounter = 0;
        
    public:
#ifdef ABYTEK_ENABLE_DEVELOPMENT_DATA
        ABYTEK_FORCE_INLINE const auto& GetAssetDirectors() const noexcept
        {
            return _AssetDirectors;
        }
#endif

        ABYTEK_FORCE_INLINE const auto& GetFactories() const noexcept
        {
            return _Factories;
        }
        ABYTEK_FORCE_INLINE const auto& GetFileExtensionToFactoryMap() const noexcept
        {
            return _FileExtensionToFactoryMap;
        }
        ABYTEK_FORCE_INLINE const auto& GetRedirectors() const noexcept
        {
            return _Redirectors;
        }
        ABYTEK_FORCE_INLINE const auto& GetFileExtensionToRedirectorMap() const noexcept
        {
            return _FileExtensionToRedirectorMap;
        }
        ABYTEK_FORCE_INLINE const auto& GetAssets() const noexcept
        {
            return _Assets;
        }

        ABYTEK_FORCE_INLINE const auto& GetTaskWorker() const noexcept
        {
            return _TaskWorker;
        }

        ABYTEK_FORCE_INLINE auto GetLiveCounter() const noexcept
        {
            return _LiveCounter.load(boost::memory_order_acquire);
        }
        ABYTEK_FORCE_INLINE void WaitLiveCounter() const noexcept
        {
            while (GetLiveCounter() > 0)
            {
                H_TaskUtilities::Switch();
            }
        }
        ABYTEK_FORCE_INLINE auto IncreaseLiveCounter() noexcept
        {
            return _LiveCounter.fetch_add(1, boost::memory_order_acq_rel);
        }
        ABYTEK_FORCE_INLINE auto DecreaseLiveCounter() noexcept
        {
            return _LiveCounter.fetch_sub(1, boost::memory_order_acq_rel);
        }

    public:
        F_AssetSubsystem(const F_ProgramUnitBuildParams& BuildParams);
        ~F_AssetSubsystem() override;

    protected:
        void OnStartup() override;
        void OnShutdown() override;

#ifdef ABYTEK_ENABLE_DEVELOPMENT_DATA
    private:
        void LoadAssetDirectors();
#endif

#ifdef ABYTEK_ENABLE_DEVELOPMENT_DATA
    public:
        B8 CookAssetsForAllModules();
        B8 CookAssets(const F_Name& ModuleName);
        B8 CookAsset(const F_Text& Path, const F_Name& SpecifiedModuleName = {});
#endif

    public:
        template<typename __F_AssetFactory, typename... __F_Args>
        TW_Valid<__F_AssetFactory> RegisterFactory(__F_Args&&... Args)
        {
            auto OwnedFactory = TU<__F_AssetFactory>()(NCPP_FORWARD(Args)...);
            auto Result = OwnedFactory.Weak();
            _Factories.push_back(
                std::move(OwnedFactory)
            );
            for(const auto& FileExtension : Result->GetFileExtensions())
                _FileExtensionToFactoryMap.insert({ FileExtension, Result });
            return Result;
        }
        TW<A_AssetFactory> FindAssetFactory(const F_Name& FileExtension);
        TF_Vector<TW<A_AssetFactory>> FindAssetFactories(const F_Name& FileExtension);

    public:
        template<typename __F_AssetRedirector, typename... __F_Args>
        TW_Valid<__F_AssetRedirector> RegisterRedirector(__F_Args&&... Args)
        {
            auto OwnedRedirector = TU<__F_AssetRedirector>()(NCPP_FORWARD(Args)...);
            auto Result = OwnedRedirector.Weak();
            _Redirectors.push_back(
                std::move(OwnedRedirector)
            );
            for(const auto& FileExtension : Result->GetFileExtensions())
                _FileExtensionToRedirectorMap.insert({ FileExtension, Result });
            return Result;
        }
        TW<A_AssetRedirector> FindAssetRedirector(const F_Name& FileExtension);
        TF_Vector<TW<A_AssetRedirector>> FindAssetRedirectors(const F_Name& FileExtension);

    public:
        static TF_Vector<F_Text> GetDefaultExternalBasePaths();

    public:
        void LockAssetLoading(const F_Text& AbsPath);
        void UnlockAssetLoading(const F_Text& AbsPath);

    public:
        TS<A_Asset> LoadAsset(const F_Text& Path, const TF_Vector<F_Text>& ExternalBasePaths = {});
        TS<A_Asset> LoadAsset(const F_Text& Path, const F_Text& OverrideFileExtension, const TF_Vector<F_Text>& ExternalBasePaths = GetDefaultExternalBasePaths(), B8 EnableRedirector = true);
        TS<A_Asset> LoadAsset(const F_Text& Path, const TW_Valid<A_AssetFactory>& Factory, const TF_Vector<F_Text>& ExternalBasePaths = GetDefaultExternalBasePaths());

    public:
        TS<A_Asset> LoadAssetFromAbsolutePath(const F_Text& AbsolutePath);
        TS<A_Asset> LoadAssetFromAbsolutePath(const F_Text& AbsolutePath, const F_Name& OverrideFileExtension);
        TS<A_Asset> LoadAssetFromAbsolutePath(const F_Text& AbsolutePath, const TW_Valid<A_AssetFactory>& Factory);

    public:
        TS_Unmanaged<F_TaskPromise> AsyncLoadAsset(
            F_AssetAsyncLoadingCallback&& Callback, 
            const F_Text& Path, const TF_Vector<F_Text>& 
            ExternalBasePaths = {}
        );
        TS_Unmanaged<F_TaskPromise> AsyncLoadAsset(
            F_AssetAsyncLoadingCallback&& Callback, 
            const F_Text& Path, const F_Text& OverrideFileExtension, 
            const TF_Vector<F_Text>& ExternalBasePaths = GetDefaultExternalBasePaths(), 
            B8 EnableRedirector = true
        );
        TS_Unmanaged<F_TaskPromise> AsyncLoadAsset(
            F_AssetAsyncLoadingCallback&& Callback, 
            const F_Text& Path, const TW_Valid<A_AssetFactory>& Factory, 
            const TF_Vector<F_Text>& ExternalBasePaths = GetDefaultExternalBasePaths()
        );

    public:
        TS_Unmanaged<F_TaskPromise> AsyncLoadAssetFromAbsolutePath(
            F_AssetAsyncLoadingCallback&& Callback, 
            const F_Text& AbsolutePath
        );
        TS_Unmanaged<F_TaskPromise> AsyncLoadAssetFromAbsolutePath(
            F_AssetAsyncLoadingCallback&& Callback, 
            const F_Text& AbsolutePath, 
            const F_Name& OverrideFileExtension
        );
        TS_Unmanaged<F_TaskPromise> AsyncLoadAssetFromAbsolutePath(
            F_AssetAsyncLoadingCallback&& Callback, 
            const F_Text& AbsolutePath, 
            const TW_Valid<A_AssetFactory>& Factory
        );

    public:
        B8 RelaxAsset(const TS_Valid<A_Asset>& Asset);
        B8 RelaxAsset(const F_Text& AbsPath);
    };
}
