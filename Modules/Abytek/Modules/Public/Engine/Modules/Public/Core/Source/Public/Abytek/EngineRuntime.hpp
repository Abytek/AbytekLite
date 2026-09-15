#pragma once

#include "Abytek/Engine.Core.prerequisites.hpp"
#include "Abytek/LLEvents/LLEvents.hpp"


namespace Abytek
{
    class F_DevelopmentData;
    class F_ModuleList;
    
    class A_LLEvents;
    
    class A_ApplicationCore;
    
    struct F_EngineRuntimeBuildParams
    {
        F_Text ProjectDirectoryPath;
        
        TF_Function<TU<A_ApplicationCore>()> ApplicationCreator;
    };
    
    class ABYTEK_ENGINE_CORE_API F_EngineRuntime : public A_Runtime
    {
        ABYTEK_DECLARE_OBJECT_SINGLETON_CRT(F_EngineRuntime);

        ABYTEK_BEGIN_REFLECTOR()
        ABYTEK_END_REFLECTOR(F_EngineRuntime)
        
    public:
        static F_Text GetConfigFileName();
        void GatherConfigFilePaths(TF_Vector<F_Text>& FilePaths) override;
        
        static F_Name GetMainTaskTag();

    private:
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        F_Text _ProjectDirectoryPath;
#endif
    
        TF_Function<TU<A_ApplicationCore>()> _ApplicationCreator;
        
        TU<F_ModuleList> _ModuleList;
        
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        TS<F_DevelopmentData> _MainDevelopmentData;
#endif
        
        TU<A_LLEvents> _LLEvents;
        LLEvents::F_OnTick::F_ListenerHandle _TickHandle;
        
        TU<A_ApplicationCore> _Application;
        B8 _IsFirstTick = true;

    public:
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        ABYTEK_FORCE_INLINE const auto& GetProjectDirectoryPath() const noexcept
        {
            return _ProjectDirectoryPath;
        }
#endif
        
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        ABYTEK_FORCE_INLINE const auto& GetMainDevelopmentData() const noexcept
        {
            return _MainDevelopmentData;
        }
#endif

    public:
        F_EngineRuntime(const F_EngineRuntimeBuildParams& BuildParams);
        ~F_EngineRuntime() override;

    protected:
        void Reflect() override;
        void PostCreateConsole() override;
        void PostCreateSerializableEnvironment() override;
        void Main() override;
        void PreDestroySerializableEnvironment() override;
        void PreDestroyConsole() override;

    private:
        void _Init();
        void _Tick();
        void _Release();
    };
}
