#pragma once

#include "Abytek/Engine.RenderCore.prerequisites.hpp"
#include "Abytek/RenderCoreCommon.hpp"


namespace Abytek
{
    class F_RenderRegistryRuntime;

    enum class E_GlobalRenderCoreTemplateFlag
    {
        NONE = 0x0,
        DELAYED_ACTIVE = 0x1,
        DEFAULT = NONE
    };
    ABYTEK_DEFINE_FLAG_OPERATORS(E_GlobalRenderCoreTemplateFlag);
    
    class ABYTEK_ENGINE_RENDER_CORE_API F_RenderRegistry final : public A_Object
    {
    public:
        friend class F_RenderRegistryRuntime;
        
    public:
        ABYTEK_BEGIN_REFLECTOR()
        ABYTEK_END_REFLECTOR(F_RenderRegistry)
        
    public:
        static F_Name GetSerializableEnvironmentMetadataElementName_Registry();
        static TS<F_RenderRegistry> GetSerializableEnvironmentMetadataElement_Registry(
            const TW_Valid<F_SerializableEnvironment>& SerializableEnvironment    
        );
        static void SetSerializableEnvironmentMetadataElement_Registry(
            const TW_Valid<F_SerializableEnvironment>& SerializableEnvironment,
            const TS<F_RenderRegistry>& Value
        );
        static void UnsetSerializableEnvironmentMetadataElement_Registry(
            const TW_Valid<F_SerializableEnvironment>& SerializableEnvironment
        );
        
    private:
        F_RenderCoreRHIConfig _RHIConfig;
        TF_Vector<TS<F_RenderRegistry>> _Dependencies;
        
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        TS<A_RHICompiler> _Compiler;
#endif
        TS<A_RHITemplateDatabase> _TemplateDatabase;
        TS<A_RHITemplateSerializer> _TemplateSerializer;
        
        TF_Map<TS<A_RHIContext>, TW<F_RenderRegistryRuntime>> _Runtimes;
        mutable F_SpinLock _RuntimeLock;

    public:
        ABYTEK_FORCE_INLINE const auto& GetRHIConfig() const noexcept
        {
            return _RHIConfig;
        }
        ABYTEK_FORCE_INLINE const auto& GetDependencies() const noexcept
        {
            return _Dependencies;
        }
        
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        ABYTEK_FORCE_INLINE const auto& GetCompiler() const noexcept
        {
            return _Compiler;
        }
#endif
        ABYTEK_FORCE_INLINE const auto& GetTemplateDatabase() const noexcept
        {
            return _TemplateDatabase;
        }
        ABYTEK_FORCE_INLINE const auto& GetTemplateSerializer() const noexcept
        {
            return _TemplateSerializer;
        }
        
    public:
        F_RenderRegistry(const F_RenderRegistryBuildParams& BuildParams);
        ~F_RenderRegistry() override;

    public:
        TS_Valid<F_RenderRegistryRuntime> GetOrActiveRuntime(const TS_Valid<A_RHIContext>& Context);
        TS<A_RHITemplate> QueryTemplate(F_RHITemplateHashCode HashCode);
        
    private:
        void _TrackRuntime(const TW_Valid<F_RenderRegistryRuntime>& Runtime);
        void _UntrackRuntime(const TW_Valid<F_RenderRegistryRuntime>& Runtime);
    };
}
