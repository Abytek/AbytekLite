#pragma once

#include "Abytek/Templates.Demo.prerequisites.pch.hpp"


namespace Abytek
{
    class F_DemoFrameProcessor : public A_FrameProcessor
    {
        ABYTEK_BEGIN_REFLECTOR(A_FrameProcessor)
        ABYTEK_END_REFLECTOR(F_DemoFrameProcessor)

    public:
        ABYTEK_DECLARE_OBJECT_SINGLETON_CRT(F_DemoFrameProcessor);
        
    private:
        TS<F_MathDebugger2D> _MathDebugger;
        
    public:
        ABYTEK_FORCE_INLINE auto GetMathDebugger() const noexcept
        {
            return _MathDebugger.Weak();
        }

    public:
        F_DemoFrameProcessor();
        virtual ~F_DemoFrameProcessor() override;
        
    public: 
        virtual void Init_Main(const TS_Valid<F_Frame>& CurrentFrame) override;
        virtual void Tick_Main(const TS_Valid<F_Frame>& CurrentFrame) override;
        virtual void Release_Main(const TS_Valid<F_Frame>& CurrentFrame) override;
    };
    
    class F_DemoApplication : public F_Application
    {
        ABYTEK_BEGIN_REFLECTOR(F_Application)
        ABYTEK_END_REFLECTOR(F_DemoApplication)

    public:
        ABYTEK_DECLARE_OBJECT_SINGLETON_CRT(F_DemoApplication);
        
    private:
        TU<F_DemoFrameProcessor> _DemoFrameProcessor;

    public:
        F_DemoApplication(const F_ApplicationConfig& Config = {
            ABYTEK_TEXT("DemoApplication")
        });
        virtual ~F_DemoApplication() override;

    protected:
        virtual B8 OnAddProgramModules() override;

    protected:
        virtual B8 PostInit() override;
        virtual B8 PreRelease() override;
    };
}
