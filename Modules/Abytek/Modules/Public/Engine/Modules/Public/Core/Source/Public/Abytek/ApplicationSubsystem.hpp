#pragma once

#include "Abytek/Engine.Core.prerequisites.hpp"
#include "Abytek/Subsystem.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_CORE_API A_ApplicationSubsystem : public F_Subsystem
    {
    public:
        friend class A_ApplicationCore;
        friend class F_ApplicationSubsystemContainer;
        
    public:
        ABYTEK_BEGIN_REFLECTOR(F_Subsystem)
        ABYTEK_END_REFLECTOR(A_ApplicationSubsystem);
        
    public:
        static F_Name GetStartupUpdateFunctionName();
        static F_Name GetPostStartupUpdateFunctionName();
        static F_Name GetTickUpdateFunctionName();
        static F_Name GetPostTickUpdateFunctionName();
        static F_Name GetPreShutdownUpdateFunctionName();
        static F_Name GetShutdownUpdateFunctionName();
        
    public:
        static void GlobalInit();
        static void GlobalRelease();
        
    private:
        
    public:
    
    protected:
        A_ApplicationSubsystem(const F_ProgramUnitBuildParams& BuildParams);
        
    public:
        ~A_ApplicationSubsystem() override;
        
    public:
        TW<F_Console> GetConsole() const noexcept;
        
    protected:
        virtual void OnTick();
        virtual void OnPostTick();
        
    protected:
        virtual void OnStartup();
        virtual void OnPostStartup();
        virtual void OnPrepareTick();
        virtual void OnPreShutdown();
        virtual void OnShutdown();
    };
}

#define ABYTEK_DECLARE_STATIC_APPLICATION_SUBSYSTEM(...) \
            ABYTEK_DECLARE_STATIC_SUBSYSTEM(__VA_ARGS__) \
            ABYTEK_DECLARE_OBJECT_SINGLETON_CRT(__VA_ARGS__)

#define ABYTEK_DEFINE_STATIC_APPLICATION_SUBSYSTEM(...) \
            ABYTEK_DEFINE_STATIC_SUBSYSTEM(__VA_ARGS__) \
            ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(__VA_ARGS__)

#define ABYTEK_BIND_STATIC_APPLICATION_SUBSYSTEM() \
            ABYTEK_BIND_OBJECT_SINGLETON_CRT()