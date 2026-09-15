#pragma once

#include "Abytek/Development/Cook/CookSetting.hpp"
#include "Abytek/Development/EBT/EBTToolchainUtilities.hpp"


#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
namespace Abytek
{
    class ABYTEK_ENGINE_CORE_API F_EBTCookSetting : public A_CookSetting
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_CookSetting)
        ABYTEK_END_REFLECTOR(F_EBTCookSetting)
        
    public:
        ABYTEK_DECLARE_STATIC_COOK_SETTING(F_EBTCookSetting);

    private:
        TW<TF_ConsoleVariable<B8>> _ConsoleVariable_CookMode;
        TW<TF_ConsoleVariable<F_Name>> _ConsoleVariable_EnvironmentName;
        TW<TF_ConsoleVariable<F_Name>> _ConsoleVariable_PlatformName;
        TW<TF_ConsoleVariable<F_Name>> _ConsoleVariable_CMakeConfigName;
        TW<TF_ConsoleVariable<TF_Vector<F_Name>>> _ConsoleVariable_ProcessorArchNames;
        
        F_EBTProjectInstance _ProjectInstance;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetConsoleVariable_EnvironmentName() const noexcept
        {
            return _ConsoleVariable_EnvironmentName;
        }
        ABYTEK_FORCE_INLINE const auto& GetConsoleVariable_PlatformName() const noexcept
        {
            return _ConsoleVariable_PlatformName;
        }
        ABYTEK_FORCE_INLINE const auto& GetConsoleVariable_CMakeConfigName() const noexcept
        {
            return _ConsoleVariable_CMakeConfigName;
        }
        ABYTEK_FORCE_INLINE const auto& GetConsoleVariable_ProcessorArchNames() const noexcept
        {
            return _ConsoleVariable_ProcessorArchNames;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetProjectInstance() const noexcept
        {
            return _ProjectInstance;
        }
        
    public:
        F_EBTCookSetting(const F_ProgramUnitBuildParams& BuildParams);
        ~F_EBTCookSetting() override;
        
    protected:
        void OnConfig() override;
        void OnPostConfig() override;
    };
}
#endif