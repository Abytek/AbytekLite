#pragma once

#include "Abytek/ModuleContainer.hpp"


namespace Abytek
{
    class A_ApplicationCore;
    
    class ABYTEK_ENGINE_CORE_API F_ApplicationModuleContainer : public F_ModuleContainer
    {
    public:
        friend class A_ApplicationModule;
        
    public:
        ABYTEK_BEGIN_REFLECTOR(F_ModuleContainer)
        ABYTEK_END_REFLECTOR(F_ApplicationModuleContainer);
        
    public:
        ABYTEK_DECLARE_OBJECT_SINGLETON_CRT(F_ApplicationModuleContainer);
        
    private:
        
    public:
        
    public:
        F_ApplicationModuleContainer(const F_ProgramUnitContainerBuildParams& BuildParams);
        ~F_ApplicationModuleContainer() override;
        
    protected:
        F_FeedbackStatus ValidateUnit(const TW_Valid<F_ProgramUnit>& Unit) override;
        
    private:
        void _StartupUnits();
        void _PostStartupUnits();
        void _TickUnits();
        void _PostTickUnits();
        void _PreShutdownUnits();
        void _ShutdownUnits();
    };
}
