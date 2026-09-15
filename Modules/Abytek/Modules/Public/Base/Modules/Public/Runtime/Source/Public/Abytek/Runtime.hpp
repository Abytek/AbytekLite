#pragma once

#include "Abytek/Base.Runtime.prerequisites.pch.hpp"
#include "Abytek/Console.hpp"
#include "Abytek/TaskManager.hpp"


namespace Abytek
{
    class F_RuntimeLogMonitor;
    
    class ABYTEK_BASE_RUNTIME_API A_Runtime : public A_Object
    {
        ABYTEK_DECLARE_OBJECT_SINGLETON_CRT(A_Runtime);

        ABYTEK_BEGIN_REFLECTOR()
        ABYTEK_END_REFLECTOR(A_Runtime)
        
    public:
        virtual void GatherConfigFilePaths(TF_Vector<F_Text>& FilePaths);

    private:
        TU<F_RuntimeLogMonitor> _LogMonitor;
        
        TS<F_ReflectionContext> _ReflectionContext;
        TW<F_ReflectionSession> _ReflectionSession;
        
        TU<F_Console> _Console;
        TW<TF_ConsoleVariable<B8>> _ConsoleVariable_EnableMemoryLeakDebugging;
        TW<TF_ConsoleVariable<B8>> _ConsoleVariable_EnableProfiler;
        TW<TF_ConsoleVariable<B8>> _ConsoleVariable_EnableObjectTracking;
        TW<TF_ConsoleVariable<B8>> _ConsoleVariable_EnableRAObjectPooling;
        
        TS<F_TaskManager> _TaskManager;

        TS<F_SerializableEnvironment> _SerializableEnvironment;

    public:
        ABYTEK_FORCE_INLINE auto GetReflectionContext() const noexcept
        {
            return _ReflectionContext.Weak();
        }
        ABYTEK_FORCE_INLINE auto GetReflectionSession() const noexcept
        {
            return _ReflectionSession.Weak();
        }
        
        ABYTEK_FORCE_INLINE const auto& GetConsole() const noexcept
        {
            return _Console;
        }
        ABYTEK_FORCE_INLINE const auto& GetConsoleVariable_EnableMemoryLeakDebugging() const noexcept
        {
            return _ConsoleVariable_EnableMemoryLeakDebugging;
        }
        ABYTEK_FORCE_INLINE const auto& GetConsoleVariable_EnableProfiler() const noexcept
        {
            return _ConsoleVariable_EnableProfiler;
        }
        ABYTEK_FORCE_INLINE const auto& GetConsoleVariable_EnableObjectTracking() const noexcept
        {
            return _ConsoleVariable_EnableObjectTracking;
        }
        ABYTEK_FORCE_INLINE const auto& GetConsoleVariable_EnableRAObjectPooling() const noexcept
        {
            return _ConsoleVariable_EnableRAObjectPooling;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetSerializableEnvironment() const noexcept
        {
            return _SerializableEnvironment;
        }
        
    protected:
        A_Runtime();
        
    public:
        ~A_Runtime() override;

    public:
        void Start();

    private:
        void _RegisterLLConsoleVariables();
        void _ApplyLLConsoleVariables();
        void _UnregisterLLConsoleVariables();
        
    public:
        virtual void GatherReflectionSessions(TF_Vector<TW<F_ReflectionSession>>& Out) const; 
        
    protected:
        virtual void Reflect();
        virtual void PostCreateConsole();
        virtual void PostCreateSerializableEnvironment();
        virtual void Main();
        virtual void PreDestroySerializableEnvironment();
        virtual void PreDestroyConsole();
    };
}
