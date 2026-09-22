#pragma once

#include "Abytek/SubsystemContainer.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_CORE_API F_ApplicationSubsystemContainer : public F_SubsystemContainer
    {
    public:
        friend class A_ApplicationSubsystem;
        
    public:
        ABYTEK_BEGIN_REFLECTOR(F_SubsystemContainer)
        ABYTEK_END_REFLECTOR(F_ApplicationSubsystemContainer);
        
    public:
        ABYTEK_DECLARE_OBJECT_SINGLETON_CRT(F_ApplicationSubsystemContainer);
        
    private:
        
    public:
        
    public:
        F_ApplicationSubsystemContainer(const F_ProgramUnitContainerBuildParams& BuildParams);
        ~F_ApplicationSubsystemContainer() override;
        
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
