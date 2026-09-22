#pragma once

#include "Abytek/Engine.Core.prerequisites.hpp"
#include "Abytek/ModuleContainer.hpp"
#include "Abytek/SubsystemContainer.hpp"


namespace Abytek
{
    class F_ModuleContainer;
    class F_Module;
    class F_SubsystemContainer;
    class F_Subsystem;
    
    class ABYTEK_ENGINE_CORE_API A_Program : public A_Object
    {
    public:
        ABYTEK_BEGIN_REFLECTOR()
        ABYTEK_END_REFLECTOR(A_Program);
        
    public:
        virtual void GatherConfigFilePaths(TF_Vector<F_Text>& FilePaths);
        
    private:
        TW<F_ReflectionSession> _ReflectionSession;
        TU<F_Console> _Console;
        TU<F_ModuleContainer> _ModuleContainer;
        TU<F_SubsystemContainer> _SubsystemContainer;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetReflectionSession() const
        {
            return _ReflectionSession;
        }
        ABYTEK_FORCE_INLINE auto GetConsole() const
        {
            return _Console.Weak();
        }
        ABYTEK_FORCE_INLINE auto GetModuleContainer() const
        {
            return _ModuleContainer.Weak();
        }
        ABYTEK_FORCE_INLINE auto GetSubsystemContainer() const
        {
            return _SubsystemContainer.Weak();
        }
        
    protected:
        A_Program();
        
    public:
        ~A_Program() override;
        
    private:
        void _RegisterSubsystems();
        
    protected:
        virtual TU<F_ModuleContainer> CreateModuleContainer();
        virtual TU<F_SubsystemContainer> CreateSubsystemContainer();
        
    protected:
        virtual void Reflect();
        virtual void PostReflectModules();
        virtual void PostCreateConsole();
        virtual void Init();
        virtual void PostInitModules();
        virtual void PostInitSubsystems();
        virtual void PostInit();
        virtual void PreRelease();
        virtual void PreReleaseSubsystems();
        virtual void PreReleaseModules();
        virtual void Release();
        virtual void PreDestroyConsole();
        virtual void OnPreCleanUpModuleReflectionSessions();
        
    public:
        virtual TF_ReflectionTypeHandle<F_Subsystem> GetBaseSubsystemType();
        
    public:
        template<typename __F_Module = F_Module>
        TW_Valid<__F_Module> RegisterModule(const F_Name& Name = __F_Module::GetStaticName())
        {
            return _ModuleContainer->EnsureUnit<__F_Module>(Name);
        }
        template<typename __F_Module = F_Module>
        TW_Valid<__F_Module> EnsureUnit(const TF_ReflectionTypeHandle<__F_Module>& Type)
        {
            return _ModuleContainer->EnsureUnit<__F_Module>(Type);
        }
        
    public:
        void Start();
        void Stop();
    };
}