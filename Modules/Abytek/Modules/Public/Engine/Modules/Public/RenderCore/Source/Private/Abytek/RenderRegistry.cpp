#include "Abytek/RenderRegistry.hpp"
#include "Abytek/RenderRegistryRuntime.hpp"
#include "Abytek/Frame/FrameHelper.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_RenderRegistry)
    {
    }

    F_Name F_RenderRegistry::GetSerializableEnvironmentMetadataElementName_Registry()
    {
        return ABYTEK_NAME("RenderRegistry");
    }

    TS<F_RenderRegistry> F_RenderRegistry::GetSerializableEnvironmentMetadataElement_Registry(
        const TW_Valid<F_SerializableEnvironment>& SerializableEnvironment
    )
    {
        const auto& Metadata = SerializableEnvironment->Metadata;
        if (Metadata.find(GetSerializableEnvironmentMetadataElementName_Registry()) == Metadata.end())
        {
            return {};
        }
        return AnyCast<TS<F_RenderRegistry>>(
            Metadata.find(GetSerializableEnvironmentMetadataElementName_Registry())
            ->second
        );
    }
    void F_RenderRegistry::SetSerializableEnvironmentMetadataElement_Registry(
        const TW_Valid<F_SerializableEnvironment>& SerializableEnvironment, 
        const TS<F_RenderRegistry>& Value
    )
    {
        auto& Metadata = SerializableEnvironment->Metadata;
        Metadata[GetSerializableEnvironmentMetadataElementName_Registry()] = Value;
    }
    void F_RenderRegistry::UnsetSerializableEnvironmentMetadataElement_Registry(
        const TW_Valid<F_SerializableEnvironment>& SerializableEnvironment
    )
    {
        auto& Metadata = SerializableEnvironment->Metadata;
        Metadata.erase(Metadata.find(GetSerializableEnvironmentMetadataElementName_Registry()));
    }

    F_RenderRegistry::F_RenderRegistry(const F_RenderRegistryBuildParams& BuildParams) :
        _RHIConfig(BuildParams.RHIConfig),
        _Dependencies(BuildParams.Dependencies)
    {
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        _Compiler = A_RHICompiler::Create(_RHIConfig.API);
#endif
        _TemplateDatabase = A_RHITemplateDatabase::Create(_RHIConfig.API, _RHIConfig.FeatureSupports);
        _TemplateSerializer = A_RHITemplateSerializer::Create(_RHIConfig.API, _RHIConfig.FeatureSupports);
    }
    F_RenderRegistry::~F_RenderRegistry()
    {
    }
 
    TS_Valid<F_RenderRegistryRuntime> F_RenderRegistry::GetOrActiveRuntime(const TS_Valid<A_RHIContext>& Context)
    {
        TS<F_RenderRegistryRuntime> Runtime;
        B8 NeedToActivate;
        {
            TF_ScopeLock<F_SpinLock> _(_RuntimeLock);
            auto It = _Runtimes.find(Context);
            if (It != _Runtimes.end())
            {
                Runtime = ShareObject(It->second);
            }
            NeedToActivate = Runtime.Q_IsNull();
            if (NeedToActivate)
            {
                F_RenderRegistryRuntimeBuildParams BuildParams;
                BuildParams.Registry = ABYTEK_WTHIS();
                BuildParams.Context = Context;
                Runtime = TS<F_RenderRegistryRuntime>()(BuildParams);
            }
        }
        if (NeedToActivate)
        {
            Runtime->FinalizeActivationAndUnlock();
        }
        else
        {
            Runtime->WaitForActivation();
        }
        return ABYTEK_MOVE(Runtime);
    }
    TS<A_RHITemplate> F_RenderRegistry::QueryTemplate(F_RHITemplateHashCode HashCode)
    {
        if (auto Template = _TemplateDatabase->GetTemplate(HashCode))
        {
            return Template;
        }
        for (const auto& Dependency : _Dependencies)
        {
            if (auto Template = Dependency->QueryTemplate(HashCode))
            {
                return Template;
            }
        }
        return {};
    }

    void F_RenderRegistry::_TrackRuntime(const TW_Valid<F_RenderRegistryRuntime>& Runtime)
    {
        // No need to lock because we are always in lock due to GetOrActivateRuntime 
        ABYTEK_ENGINE_RHI_ASSERT(_RuntimeLock.ProducerTryLock() == false) << "Requires to be already in critical session";
        ABYTEK_ENGINE_RHI_ASSERT(_Runtimes.find(Runtime->GetContext()) == _Runtimes.end());
        _Runtimes.insert({ Runtime->GetContext(), Runtime });
    }
    void F_RenderRegistry::_UntrackRuntime(const TW_Valid<F_RenderRegistryRuntime>& Runtime)
    {
        TF_ScopeLock<F_SpinLock> _(_RuntimeLock);
        ABYTEK_ENGINE_RHI_ASSERT(_Runtimes.find(Runtime->GetContext()) != _Runtimes.end());
        _Runtimes.erase(_Runtimes.find(Runtime->GetContext()));
    }
}
