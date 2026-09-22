#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API F_RHIModule final : public A_ApplicationModule
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationModule)
        ABYTEK_END_REFLECTOR(F_RHIModule);
        
    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_MODULE(F_RHIModule);
        
    private:
#ifdef ABYTEK_ENGINE_RHI_ENABLE_PIX
        TW<TF_ConsoleVariable<B8>> _ConsoleVariable_ShouldEnablePIX;
        B8 _ShouldEnablePIX = false;
#endif
        
    public:
#ifdef ABYTEK_ENGINE_RHI_ENABLE_PIX
        ABYTEK_FORCE_INLINE auto GetShouldEnablePIX() const noexcept
        {
            return _ShouldEnablePIX;
        }     
#endif
        
    public:
        F_RHIModule(const F_ProgramUnitBuildParams& BuildParams);
        ~F_RHIModule() override;

    protected:
        void OnReflect() override;
        void OnConfig() override;
        void OnPostConfig() override;
    };
}
