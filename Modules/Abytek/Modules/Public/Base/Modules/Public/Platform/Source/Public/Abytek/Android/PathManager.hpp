#pragma once

#include "Abytek/PathManager.hpp"


#ifdef ABYTEK_PLATFORM_ANDROID
namespace Abytek
{
    namespace AndroidJavaItems::PathManager_Bridge
    {
        jclass GetClass();
    }

    class ABYTEK_BASE_PLATFORM_API F_AndroidPathManager : public A_PathManager
    {
    private:
        F_Text _UserDataDir;
        F_Text _CacheDir;
        F_Text _ExternalUserDataDir;
        F_Text _ExternalCacheDir;

    public:
        
    public:
        F_AndroidPathManager();
        virtual ~F_AndroidPathManager() override;
        
    public:
        virtual TF_Optional<F_Text> GetCommonDirectoryPath(E_CommonDirectory CommonDirectory) override;

    private:
        void _QueryDirectories();
    };
}
#endif