#pragma once

#include "Abytek/Engine.Core.prerequisites.hpp"
#include "Abytek/Module.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_CORE_API A_ApplicationModule : public F_Module
    {
    public:
        friend class A_ApplicationCore;
        friend class F_ApplicationModuleContainer;
        
    public:
        ABYTEK_BEGIN_REFLECTOR(F_Module)
        ABYTEK_END_REFLECTOR(A_ApplicationModule);
        
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
        A_ApplicationModule(const F_ProgramUnitBuildParams& BuildParams);
        
    public:
        ~A_ApplicationModule() override;
        
    public:
        TW<F_Console> GetConsole() const noexcept;
        
    public:
        virtual void OnStartup();
        virtual void OnPostStartup();
        virtual void OnPrepareTick();
        virtual void OnTick();
        virtual void OnPostTick();
        virtual void OnPreShutdown();
        virtual void OnShutdown();
    };
}

#define ABYTEK_DECLARE_STATIC_APPLICATION_MODULE(...) \
            ABYTEK_DECLARE_STATIC_MODULE(__VA_ARGS__) \
            ABYTEK_DECLARE_OBJECT_SINGLETON_CRT(__VA_ARGS__)

#define ABYTEK_DEFINE_STATIC_APPLICATION_MODULE(...) \
            ABYTEK_DEFINE_STATIC_MODULE(__VA_ARGS__) \
            ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(__VA_ARGS__)

#define ABYTEK_BIND_STATIC_APPLICATION_MODULE() \
            ABYTEK_BIND_OBJECT_SINGLETON_CRT()
