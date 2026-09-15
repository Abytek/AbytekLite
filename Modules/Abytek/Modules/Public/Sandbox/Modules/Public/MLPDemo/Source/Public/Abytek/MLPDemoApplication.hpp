#pragma once

#include "Abytek/Sandbox.MLPDemo.prerequisites.pch.hpp"
#include "Abytek/MathDebugger2D.hpp"


namespace Abytek
{
    class F_MLPDemoApplication : public F_MinimalApplication
    {
        ABYTEK_BEGIN_REFLECTOR(F_MinimalApplication)
        ABYTEK_END_REFLECTOR(F_MLPDemoApplication)

    public:
        ABYTEK_DECLARE_OBJECT_SINGLETON_CRT(F_MLPDemoApplication);
        
    private:
        TS<F_MathDebugger2D> _MathDebugger;
        
    public:
        ABYTEK_FORCE_INLINE auto GetMathDebugger() const noexcept
        {
            return _MathDebugger.Weak();
        }

    public:
        F_MLPDemoApplication(const F_ApplicationBuildParams& BuildParams);
        ~F_MLPDemoApplication() override;
        
    protected:
        void Init() override;
        void PostStartup() override;
        void Tick() override;
        void PreShutdown() override;
    };
}
