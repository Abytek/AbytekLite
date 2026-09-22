#pragma once

#include "Abytek/Development/Cook/CookSetting.hpp"
#include "Abytek/RenderCoreCommon.hpp"


#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
namespace Abytek
{
    class ABYTEK_ENGINE_CORE_API F_RenderCoreCookSetting : public A_CookSetting
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_CookSetting)
        ABYTEK_END_REFLECTOR(F_RenderCoreCookSetting);
        
    public:
        ABYTEK_DECLARE_STATIC_COOK_SETTING(F_RenderCoreCookSetting);

    private:
        TW<TF_ConsoleVariable<E_RHIAPI>> _ConsoleVariable_RHIAPI;
        TW<TF_ConsoleVariable<E_RHIFeatureModel>> _ConsoleVariable_RHIFeatureModel;
        
        F_RenderCoreRHIConfig _RHIConfig;
        
        TS<F_RenderRegistry> _RenderRegistry;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetConsoleVariable_RHIAPI() const noexcept
        {
            return _ConsoleVariable_RHIAPI;
        }
        ABYTEK_FORCE_INLINE const auto& GetConsoleVariable_RHIFeatureModel() const noexcept
        {
            return _ConsoleVariable_RHIFeatureModel;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetRHIConfig() const noexcept
        {
            return _RHIConfig;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetRenderRegistry() const noexcept
        {
            return _RenderRegistry;
        }
        
    public:
        F_RenderCoreCookSetting(const F_ProgramUnitBuildParams& BuildParams);
        ~F_RenderCoreCookSetting() override;
        
    protected:
        void OnConfig() override;
        void OnPostConfig() override;
        
    protected:
        void OnInit() override;
        void OnRelease() override;
    };
}
#endif