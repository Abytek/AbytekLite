#pragma once

#include "Abytek/ApplicationSubsystem.hpp"
#include "Abytek/Development/EBT/EBTToolchainUtilities.hpp"


#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
namespace Abytek
{
    class F_EBTCookSetting;
    class F_DevelopmentData;
    
    class ABYTEK_ENGINE_CORE_API F_EBTCookManager final : public A_ApplicationSubsystem
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationSubsystem)
        ABYTEK_END_REFLECTOR(F_EBTCookManager);

    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_SUBSYSTEM(F_EBTCookManager);
        
    public:
        static F_Name GetCreateDevelopmentDataCookFunctionName();
        static F_Name GetBuildApplicationCookFunctionName();
        static void GlobalInit();
        static void GlobalRelease();
        
    private:
        TS<F_DevelopmentData> _DevelopmentData;
        F_Text _CookDataDirectoryPath;
        
        F_Name _CookedExecutableTargetName;
        F_Text _CookedExecutableTargetFilePath;
        F_Text _CookedExecutableTargetFileDirectoryPath;
        
        F_EBTApplicationBuildResult _ApplicationBuildResult;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetDevelopmentData() const noexcept
        {
            return _DevelopmentData;
        }
        ABYTEK_FORCE_INLINE const auto& GetCookDataDirectoryPath() const noexcept
        {
            return _CookDataDirectoryPath;
        }

        ABYTEK_FORCE_INLINE const auto& GetCookedExecutableTargetName() const noexcept
        {
            return _CookedExecutableTargetName;
        }
        ABYTEK_FORCE_INLINE const auto& GetCookedExecutableTargetFilePath() const noexcept
        {
            return _CookedExecutableTargetFilePath;
        }
        ABYTEK_FORCE_INLINE const auto& GetCookedExecutableTargetFileDirectoryPath() const noexcept
        {
            return _CookedExecutableTargetFileDirectoryPath;
        }
    
        ABYTEK_FORCE_INLINE const auto& GetApplicationBuildResult() const noexcept
        {
            return _ApplicationBuildResult;
        }
        
    public:
        F_EBTCookManager(const F_ProgramUnitBuildParams& BuildParams);
        ~F_EBTCookManager() override;
        
    private:
        void _CreateDevelopmentData();
        void _BuildApplication();
    };
}
#endif