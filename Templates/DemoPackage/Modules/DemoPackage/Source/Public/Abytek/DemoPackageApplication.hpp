#pragma once

#include "Abytek/Templates.DemoPackage.prerequisites.pch.hpp"


namespace Abytek
{
    class F_DemoPackageFrameProcessor : public A_FrameProcessor
    {
        ABYTEK_BEGIN_REFLECTOR(A_FrameProcessor)
        ABYTEK_END_REFLECTOR(F_DemoPackageFrameProcessor)

    public:
        ABYTEK_DECLARE_OBJECT_SINGLETON_CRT(F_DemoPackageFrameProcessor);
        
    private:
        TS<F_MathDebugger2D> _MathDebugger;
        
    public:
        ABYTEK_FORCE_INLINE auto GetMathDebugger() const noexcept
        {
            return _MathDebugger.Weak();
        }

    public:
        F_DemoPackageFrameProcessor();
        virtual ~F_DemoPackageFrameProcessor() override;
        
    public: 
        virtual void Init_Main(const TS_Valid<F_Frame>& CurrentFrame) override;
        virtual void Tick_Main(const TS_Valid<F_Frame>& CurrentFrame) override;
        virtual void Release_Main(const TS_Valid<F_Frame>& CurrentFrame) override;
    };
    
    class F_DemoPackageApplication : public F_Application
    {
        ABYTEK_BEGIN_REFLECTOR(F_Application)
        ABYTEK_END_REFLECTOR(F_DemoPackageApplication)

    public:
        ABYTEK_DECLARE_OBJECT_SINGLETON_CRT(F_DemoPackageApplication);
        
    private:
        TU<F_DemoPackageFrameProcessor> _DemoPackageFrameProcessor;

    public:
        F_DemoPackageApplication(const F_ApplicationConfig& Config = {
            ABYTEK_TEXT("DemoPackageApplication")
        });
        virtual ~F_DemoPackageApplication() override;

    protected:
        virtual B8 OnAddProgramModules() override;

    protected:
        virtual B8 PostInit() override;
        virtual B8 PreRelease() override;
    };
}
