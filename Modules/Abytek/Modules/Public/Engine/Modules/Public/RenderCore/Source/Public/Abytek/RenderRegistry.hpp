#pragma once

#include "Abytek/Engine.RenderCore.prerequisites.hpp"
#include "Abytek/World/WorldContext.hpp"
#include "Abytek/RenderCoreCommon.hpp"


namespace Abytek
{
    class F_RenderPack;
    class F_RenderRegistry;
    class F_RenderRegistryRuntime;
    class A_RenderRegistryPortData;
    class A_RenderRegistryPort;
    class F_MainRenderRegistryPortData;
    class F_MainRenderRegistryPort;

    class ABYTEK_ENGINE_RENDER_CORE_API A_RenderRegistryPortData : public A_Object
    {
    private:
        TW<A_RenderRegistryPort> _Port;
        F_RHIFeatureSupports _RHIFeatureSupports;
        TS<F_RenderRegistryRuntime> _RegistryRuntime;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetPort() const noexcept
        {
            return _Port;
        }
        ABYTEK_FORCE_INLINE const auto& GetRHIFeatureSupports() const noexcept
        {
            return _RHIFeatureSupports;
        }
        ABYTEK_FORCE_INLINE const auto& GetRegistryRuntime() const noexcept
        {
            return _RegistryRuntime;
        }
        
    protected:
        A_RenderRegistryPortData(const TW_Valid<A_RenderRegistryPort>& Port);
        
    public:
        ~A_RenderRegistryPortData() override;
        
    protected:
        virtual void OnInit();
        virtual void OnRelease();
        
    public:
        void Init();
        void Release();
        
    protected:
        virtual TS<F_RenderRegistryRuntime> CreateRegistryRuntime() = 0;
        
    public:
        virtual TS<A_RHISubmissionItemContainer> GetSubmissionItemContainer() = 0;
    };

    class ABYTEK_ENGINE_RENDER_CORE_API A_RenderRegistryPort : public A_Object
    {
    private:
        TW<F_RenderRegistry> _Registry;
        TS<A_RenderRegistryPortData> _Data;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetRegistry() const noexcept
        {
            return _Registry;
        }
        ABYTEK_FORCE_INLINE const auto& GetData() const noexcept
        {
            return _Data;
        }
        
    protected:
        A_RenderRegistryPort(const TW_Valid<F_RenderRegistry>& Registry);
        
    public:
        ~A_RenderRegistryPort() override;
        
    protected:
        virtual void OnInit();
        virtual void OnRelease();
        
    public:
        void Init();
        void Release();
        
    protected:
        virtual TS<A_RenderRegistryPortData> CreateData() = 0;
        
    public:
        virtual void EnqueueCommand(TF_Function<void()>&& Command);
    };

    class ABYTEK_ENGINE_RENDER_CORE_API F_MainRenderRegistryPortData final : public A_RenderRegistryPortData
    {
    private:
        
    public:
        
    public:
        F_MainRenderRegistryPortData(const TW_Valid<F_MainRenderRegistryPort>& Port);
        ~F_MainRenderRegistryPortData() override;
        
    protected:
        TS<F_RenderRegistryRuntime> CreateRegistryRuntime() override;
        
    public:
        TS<A_RHISubmissionItemContainer> GetSubmissionItemContainer() override;
    };

    class ABYTEK_ENGINE_RENDER_CORE_API F_MainRenderRegistryPort : public A_RenderRegistryPort
    {
    private:
        
    public:
        
    public:
        F_MainRenderRegistryPort(const TW_Valid<F_RenderRegistry>& Registry);
        ~F_MainRenderRegistryPort() override;
        
    protected:
        void OnInit() override;
        void OnRelease() override;
        
    protected:
        TS<A_RenderRegistryPortData> CreateData() override;
        
    public:
        void EnqueueCommand(TF_Function<void()>&& Command) override;
    };

    class ABYTEK_ENGINE_RENDER_CORE_API F_RenderRegistry final : public A_Object, public I_GetWorld
    {
    public:
        friend class A_RenderRegistryPort;
        friend class F_RenderRegistryRuntime;
        
    public:
        ABYTEK_BEGIN_REFLECTOR()
        ABYTEK_END_REFLECTOR(F_RenderRegistry);
        
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
        TW<F_World> _World;
        TW<F_SerializableEnvironment> _SerializableEnvironment;
        F_RenderCoreRHIConfig _RHIConfig;
        B8 _DebugGeneratedShaders = false;
        TF_Vector<TS<F_RenderRegistry>> _Dependencies;
        
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        TS<A_RHICompiler> _Compiler;
#endif
        TS<A_RHITemplateDatabase> _TemplateDatabase;
        TS<A_RHITemplateSerializer> _TemplateSerializer;
        
        TF_Vector<TW<A_RenderRegistryPort>> _Ports;
        TS<F_MainRenderRegistryPort> _MainPort;
        
        TF_Set<TW<F_RenderPack>> _Packs;

    public:
        TW_Valid<F_World> GetWorld() const override
        {
            return _World;
        }
        ABYTEK_FORCE_INLINE const auto& GetSerializableEnvironment() const noexcept
        {
            return _SerializableEnvironment;
        }
        ABYTEK_FORCE_INLINE const auto& GetRHIConfig() const noexcept
        {
            return _RHIConfig;
        }
        ABYTEK_FORCE_INLINE auto GetDebugGeneratedShaders() const noexcept
        {
            return _DebugGeneratedShaders;
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
        
        ABYTEK_FORCE_INLINE const auto& GetPorts() const noexcept
        {
            return _Ports;
        }
        ABYTEK_FORCE_INLINE const auto& GetMainPort() const noexcept
        {
            return _MainPort;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetPacks() const noexcept
        {
            return _Packs;
        }
        
    public:
        F_RenderRegistry(const F_RenderRegistryBuildParams& BuildParams);
        ~F_RenderRegistry() override;

    public:
        TS<A_RHITemplate> QueryTemplate(F_RHITemplateHashCode HashCode);
        
    public:
        void _RegistryPort(const TW_Valid<A_RenderRegistryPort>& Port);
        void _UnregistryPort(const TW_Valid<A_RenderRegistryPort>& Port);
        
    public:
        void _RegistryPack(const TW_Valid<F_RenderPack>& Pack);
        void _UnregistryPack(const TW_Valid<F_RenderPack>& Pack);
    };
}
